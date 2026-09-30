#!/usr/bin/env python3
"""
fwtool.py - Unified firmware analysis toolkit.

Builds a SQLite index from decompiled firmware, provides CLI queries,
semantic search via GigaChat embeddings, register analysis, and an
interactive LLM agent.

Commands:
  index         Build or rebuild SQLite index from decompiled firmware
  embed         Generate GigaChat embeddings for all indexed functions
  lookup        Get function metadata and pseudocode by address or name
  search        Full-text search (FTS5) across names and pseudocode
  ssearch       Semantic search using stored embeddings
  grep          Exact substring search in pseudocode
  strings       Search string constants
  callers       Find callers of a function
  callees       Find callees of a function
  list          List functions
  stats         Show database statistics
  reg           Show register info (name, address, type, access, references)
  regs          Search / list registers by name or region
  regrefs       Functions that reference a register
  funcregs      Registers referenced by a function
  chat          Interactive agent (GigaChat function calling)

Requires Python 3.8+ with sqlite3 (FTS5) and requests.
Set GIGACHAT_TOKEN env var for embed, ssearch, and chat.
"""

import argparse
import json
import math
import os
import re
import sqlite3
import struct
import subprocess
import sys
import time
import warnings

warnings.filterwarnings("ignore", message="Unverified HTTPS request")

try:
    import requests
except ImportError:
    requests = None


DEFAULT_DB = os.environ.get("FWDB", "firmware.db")
EMBED_API_URL = "https://gigachat.devices.sberbank.ru/api/v1/embeddings"
CHAT_API_URL = "https://gigachat.devices.sberbank.ru/api/v1/chat/completions"

FUNC_HEADER_RE = re.compile(
    r"/\*\s*(?P<name>\S+)\s*@\s*(?P<address>[0-9a-fA-F]+)\s*\*/"
)


# ---------------------------------------------------------------------------
# Common helpers
# ---------------------------------------------------------------------------

def normalize_addr(addr):
    if not addr:
        return ""
    return str(addr).strip().lower().replace("0x", "").lstrip("0") or "0"


def connect_db(db_path, must_exist=True):
    if must_exist and not os.path.exists(db_path):
        print("ERROR: database not found: %s" % db_path, file=sys.stderr)
        sys.exit(1)
    conn = sqlite3.connect(db_path)
    conn.row_factory = sqlite3.Row
    return conn


def require_requests():
    if requests is None:
        print("ERROR: 'requests' not installed. Run: py -m pip install requests")
        sys.exit(1)


def get_token(args):
    token = getattr(args, "token", None) or os.environ.get("GIGACHAT_TOKEN")
    if not token:
        print("ERROR: set GIGACHAT_TOKEN env var or pass --token")
        sys.exit(1)
    return token


def vector_to_blob(vec):
    return struct.pack("<%df" % len(vec), *vec)


def blob_to_vector(blob):
    n = len(blob) // 4
    return list(struct.unpack("<%df" % n, blob))


def cosine(a, b):
    dot = sum(x * y for x, y in zip(a, b))
    na = math.sqrt(sum(x * x for x in a))
    nb = math.sqrt(sum(x * x for x in b))
    if na == 0.0 or nb == 0.0:
        return 0.0
    return dot / (na * nb)


def check_fts5(conn):
    try:
        conn.execute("CREATE VIRTUAL TABLE temp.t USING fts5(x)")
        conn.execute("DROP TABLE temp.t")
        return True
    except sqlite3.OperationalError:
        return False


# ---------------------------------------------------------------------------
# Index building
# ---------------------------------------------------------------------------

def parse_decompiled_c(path):
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        content = f.read()

    matches = list(FUNC_HEADER_RE.finditer(content))
    if not matches:
        print("WARNING: no function headers found in %s" % path)
        return []

    print("  Found %d function headers" % len(matches))

    functions = []
    for i, m in enumerate(matches):
        body_start = m.end()
        body_end = matches[i + 1].start() if i + 1 < len(matches) else len(content)
        body = content[body_start:body_end].strip()
        functions.append({
            "name": m.group("name").strip(),
            "address": m.group("address").strip(),
            "pseudocode": body,
        })
    return functions


def parse_functions_json(path):
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        data = json.load(f)

    if isinstance(data, dict):
        if "functions" in data and isinstance(data["functions"], list):
            items = data["functions"]
        else:
            items = []
            for k, v in data.items():
                if isinstance(v, dict):
                    v.setdefault("address", k)
                    items.append(v)
    elif isinstance(data, list):
        items = data
    else:
        return {}

    result = {}
    for item in items:
        if not isinstance(item, dict):
            continue
        addr = (item.get("address") or item.get("entry")
                or item.get("entry_point") or item.get("addr"))
        if not addr:
            continue
        addr_norm = normalize_addr(addr)
        result[addr_norm] = {
            "address": str(addr).strip(),
            "address_norm": addr_norm,
            "name": item.get("name") or item.get("symbol") or "",
            "signature": item.get("signature") or item.get("prototype") or "",
            "size": item.get("size") or 0,
            "callees": (item.get("callees") or item.get("calls")
                        or item.get("called_functions") or []),
            "callers": item.get("callers") or item.get("called_by") or [],
        }
    return result


def parse_strings_txt(path):
    if not path or not os.path.exists(path):
        return []
    strings = []
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n\r")
            if not line:
                continue
            m = re.match(r"^(0x[0-9a-fA-F]+|[0-9a-fA-F]{6,})\s*[:\s]\s*(.+)$", line)
            if m:
                strings.append({"address": m.group(1), "value": m.group(2)})
            else:
                strings.append({"address": None, "value": line})
    return strings


def parse_registers_json(path):
    """Parse registers.json produced by Ghidra export scripts.

    Expected top-level: {"program": ..., "registers": [ {...}, ... ]}
    Each register: name, address, size, type, access[], region, references[]
    """
    if not path or not os.path.exists(path):
        return []
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        data = json.load(f)

    if isinstance(data, dict):
        items = data.get("registers") or []
    elif isinstance(data, list):
        items = data
    else:
        return []

    result = []
    for item in items:
        if not isinstance(item, dict):
            continue
        addr = item.get("address")
        if not addr:
            continue
        access = item.get("access") or []
        if isinstance(access, list):
            access_str = ",".join(str(a) for a in access)
        else:
            access_str = str(access)

        refs = item.get("references") or []
        if not isinstance(refs, list):
            refs = []

        result.append({
            "name": item.get("name") or "",
            "address": str(addr).strip(),
            "address_norm": normalize_addr(addr),
            "address_int": item.get("address_int") or 0,
            "size": item.get("size") or 0,
            "type": item.get("type") or "",
            "access": access_str,
            "region": item.get("region") or "",
            "references": [str(r) for r in refs],
            "reference_count": item.get("reference_count") or len(refs),
        })
    return result


def _resolve_name_to_addr(name, name_to_addr, addr_set):
    """Resolve a symbol name to a normalized address using the same rules
    as callee resolution: exact, lowercase, hex-suffix."""
    if not name:
        return None
    if name in name_to_addr:
        return name_to_addr[name]
    if name.lower() in name_to_addr:
        return name_to_addr[name.lower()]
    m = re.search(r"([0-9a-fA-F]{6,8})$", name)
    if m:
        cand = normalize_addr(m.group(1))
        if cand in addr_set:
            return cand
    return None


def build_index(db_path, decompiled_c_path, functions_json_path=None,
                strings_txt_path=None, registers_json_path=None, rebuild=True):
    if rebuild and os.path.exists(db_path):
        os.remove(db_path)

    conn = sqlite3.connect(db_path)
    if not check_fts5(conn):
        print("ERROR: SQLite was built without FTS5 support.")
        sys.exit(1)

    cur = conn.cursor()
    cur.executescript("""
    PRAGMA journal_mode=WAL;

    CREATE TABLE IF NOT EXISTS functions (
        address_norm TEXT PRIMARY KEY,
        address TEXT,
        name TEXT,
        signature TEXT,
        size INTEGER DEFAULT 0,
        pseudocode TEXT
    );

    CREATE VIRTUAL TABLE IF NOT EXISTS functions_fts USING fts5(
        name, signature, pseudocode, address_norm UNINDEXED,
        tokenize='unicode61'
    );

    CREATE TABLE IF NOT EXISTS calls (
        caller_addr TEXT,
        callee_addr TEXT,
        PRIMARY KEY (caller_addr, callee_addr)
    );
    CREATE INDEX IF NOT EXISTS idx_calls_caller ON calls(caller_addr);
    CREATE INDEX IF NOT EXISTS idx_calls_callee ON calls(callee_addr);

    CREATE TABLE IF NOT EXISTS strings (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        address TEXT,
        address_norm TEXT,
        value TEXT
    );
    CREATE INDEX IF NOT EXISTS idx_strings_addr ON strings(address_norm);

    CREATE TABLE IF NOT EXISTS registers (
        address_norm TEXT PRIMARY KEY,
        name TEXT,
        address TEXT,
        address_int INTEGER,
        size INTEGER DEFAULT 0,
        type TEXT,
        access TEXT,
        region TEXT,
        reference_count INTEGER DEFAULT 0
    );
    CREATE INDEX IF NOT EXISTS idx_reg_region ON registers(region);

    CREATE VIRTUAL TABLE IF NOT EXISTS registers_fts USING fts5(
        name, region, type, address_norm UNINDEXED,
        tokenize='unicode61'
    );

    CREATE TABLE IF NOT EXISTS register_refs (
        register_addr TEXT,
        function_addr TEXT,
        raw_ref TEXT,
        PRIMARY KEY (register_addr, function_addr)
    );
    CREATE INDEX IF NOT EXISTS idx_regrefs_reg ON register_refs(register_addr);
    CREATE INDEX IF NOT EXISTS idx_regrefs_func ON register_refs(function_addr);
    """)

    json_funcs = {}
    if functions_json_path and os.path.exists(functions_json_path):
        print("Loading functions.json...")
        json_funcs = parse_functions_json(functions_json_path)
        print("  %d functions in JSON" % len(json_funcs))

    print("Parsing decompiled.c...")
    c_funcs = parse_decompiled_c(decompiled_c_path)
    print("  %d functions in C file" % len(c_funcs))

    inserted = 0
    for fn in c_funcs:
        addr_norm = normalize_addr(fn["address"])
        meta = json_funcs.get(addr_norm, {})
        name = meta.get("name") or fn["name"]
        signature = meta.get("signature", "")
        size = meta.get("size", 0)

        cur.execute(
            "INSERT OR REPLACE INTO functions "
            "(address_norm, address, name, signature, size, pseudocode) "
            "VALUES (?, ?, ?, ?, ?, ?)",
            (addr_norm, fn["address"], name, signature, size, fn["pseudocode"]),
        )
        cur.execute(
            "INSERT INTO functions_fts "
            "(name, signature, pseudocode, address_norm) VALUES (?, ?, ?, ?)",
            (name, signature, fn["pseudocode"], addr_norm),
        )
        inserted += 1

    # Build maps for resolving names -> addresses
    name_to_addr = {}
    addr_set = set()
    for row in cur.execute("SELECT name, address_norm FROM functions"):
        nm, addr = row[0], row[1]
        addr_set.add(addr)
        name_to_addr[nm] = addr
        name_to_addr[nm.lower()] = addr

    def resolve_callee(callee):
        if isinstance(callee, str):
            s = callee.strip()
            if re.match(r"^(0x)?[0-9a-fA-F]{1,8}$", s):
                cand = normalize_addr(s)
                if cand in addr_set:
                    return cand
            return _resolve_name_to_addr(s, name_to_addr, addr_set)
        if isinstance(callee, dict):
            a = callee.get("address") or callee.get("entry")
            if a:
                cand = normalize_addr(a)
                if cand in addr_set:
                    return cand
            nm = callee.get("name") or callee.get("symbol")
            if nm:
                return resolve_callee(nm)
            return None
        return None

    call_count = 0
    unresolved = 0
    for caller_norm, meta in json_funcs.items():
        for callee in meta.get("callees", []):
            callee_norm = resolve_callee(callee)
            if not callee_norm:
                unresolved += 1
                continue
            try:
                cur.execute(
                    "INSERT OR IGNORE INTO calls (caller_addr, callee_addr) "
                    "VALUES (?, ?)", (caller_norm, callee_norm))
                call_count += 1
            except sqlite3.Error:
                pass

    if unresolved:
        print("  %d callee references unresolved" % unresolved)

    string_count = 0
    if strings_txt_path and os.path.exists(strings_txt_path):
        print("Parsing strings.txt...")
        strings = parse_strings_txt(strings_txt_path)
        for s in strings:
            cur.execute(
                "INSERT INTO strings (address, address_norm, value) "
                "VALUES (?, ?, ?)",
                (s["address"], normalize_addr(s["address"]), s["value"]),
            )
            string_count += 1
        print("  %d strings" % string_count)

    reg_count = 0
    reg_ref_count = 0
    reg_unresolved = 0
    if registers_json_path and os.path.exists(registers_json_path):
        print("Parsing registers.json...")
        regs = parse_registers_json(registers_json_path)
        print("  %d registers" % len(regs))

        for r in regs:
            cur.execute(
                "INSERT OR REPLACE INTO registers "
                "(address_norm, name, address, address_int, size, type, "
                " access, region, reference_count) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",
                (r["address_norm"], r["name"], r["address"], r["address_int"],
                 r["size"], r["type"], r["access"], r["region"],
                 r["reference_count"]),
            )
            cur.execute(
                "INSERT INTO registers_fts "
                "(name, region, type, address_norm) VALUES (?, ?, ?, ?)",
                (r["name"], r["region"], r["type"], r["address_norm"]),
            )
            reg_count += 1

            for raw_ref in r["references"]:
                fn_addr = _resolve_name_to_addr(raw_ref, name_to_addr, addr_set)
                if not fn_addr:
                    reg_unresolved += 1
                    continue
                try:
                    cur.execute(
                        "INSERT OR IGNORE INTO register_refs "
                        "(register_addr, function_addr, raw_ref) "
                        "VALUES (?, ?, ?)",
                        (r["address_norm"], fn_addr, raw_ref),
                    )
                    reg_ref_count += 1
                except sqlite3.Error:
                    pass

        if reg_unresolved:
            print("  %d register references unresolved" % reg_unresolved)

    conn.commit()
    conn.close()

    print("\nIndex built: %s" % db_path)
    print("  functions:      %d" % inserted)
    print("  calls:          %d" % call_count)
    print("  strings:        %d" % string_count)
    print("  registers:      %d" % reg_count)
    print("  register_refs:  %d" % reg_ref_count)


# ---------------------------------------------------------------------------
# Query commands
# ---------------------------------------------------------------------------

def resolve_function(conn, key):
    addr_norm = normalize_addr(key)
    cur = conn.cursor()
    row = cur.execute(
        "SELECT * FROM functions WHERE address_norm = ?", (addr_norm,)
    ).fetchone()
    if row:
        return row
    row = cur.execute(
        "SELECT * FROM functions WHERE name = ?", (key,)
    ).fetchone()
    if row:
        return row
    return cur.execute(
        "SELECT * FROM functions WHERE name LIKE ? LIMIT 1", ("%" + key + "%",)
    ).fetchone()


def resolve_register(conn, key):
    addr_norm = normalize_addr(key)
    cur = conn.cursor()
    row = cur.execute(
        "SELECT * FROM registers WHERE address_norm = ?", (addr_norm,)
    ).fetchone()
    if row:
        return row
    row = cur.execute(
        "SELECT * FROM registers WHERE name = ?", (key,)
    ).fetchone()
    if row:
        return row
    return cur.execute(
        "SELECT * FROM registers WHERE name LIKE ? LIMIT 1", ("%" + key + "%",)
    ).fetchone()


def cmd_lookup(conn, args):
    row = resolve_function(conn, args.key)
    if not row:
        print("Not found: %s" % args.key)
        return 1
    print("=" * 60)
    print("Name:      %s" % row["name"])
    print("Address:   %s" % row["address"])
    print("Signature: %s" % (row["signature"] or "(none)"))
    print("Size:      %s bytes" % row["size"])
    print("=" * 60)
    print(row["pseudocode"])
    return 0


def cmd_search(conn, args):
    cur = conn.cursor()
    rows = cur.execute(
        "SELECT f.address, f.name, f.signature, "
        "snippet(functions_fts, 2, '<<', '>>', '...', 20) AS snip "
        "FROM functions_fts "
        "JOIN functions f ON f.address_norm = functions_fts.address_norm "
        "WHERE functions_fts MATCH ? LIMIT ?",
        (args.query, args.limit),
    ).fetchall()
    if not rows:
        print("No matches for: %s" % args.query)
        return 0
    for r in rows:
        print("--- %s @ %s ---" % (r["name"], r["address"]))
        if r["signature"]:
            print("  sig: %s" % r["signature"])
        print("  %s" % r["snip"].replace("\n", " "))
        print()
    return 0


def cmd_grep(conn, args):
    cur = conn.cursor()
    rows = cur.execute(
        "SELECT address, name, pseudocode FROM functions "
        "WHERE pseudocode LIKE ? LIMIT ?",
        ("%" + args.pattern + "%", args.limit),
    ).fetchall()
    if not rows:
        print("No matches for: %s" % args.pattern)
        return 0
    for r in rows:
        print("--- %s @ %s ---" % (r["name"], r["address"]))
        for line in r["pseudocode"].splitlines():
            if args.pattern in line:
                print("  %s" % line.strip())
        print()
    return 0


def cmd_strings(conn, args):
    cur = conn.cursor()
    rows = cur.execute(
        "SELECT address, value FROM strings WHERE value LIKE ? LIMIT ?",
        ("%" + args.pattern + "%", args.limit),
    ).fetchall()
    if not rows:
        print("No strings matching: %s" % args.pattern)
        return 0
    for r in rows:
        print("%s  %s" % (r["address"] or "(no addr)", r["value"]))
    return 0


def _print_neighbors(rows, direction, target_name, target_address):
    if not rows:
        print("No %s known for %s (%s)" % (direction, target_name, target_address))
        return 0
    print("%s of %s (%s):" % (direction.capitalize(), target_name, target_address))
    for r in rows:
        print("  %s @ %s" % (r["name"], r["address"]))
    return 0


def cmd_callers(conn, args):
    row = resolve_function(conn, args.key)
    if not row:
        print("Not found: %s" % args.key)
        return 1
    rows = conn.cursor().execute(
        "SELECT f.address, f.name FROM calls c "
        "JOIN functions f ON f.address_norm = c.caller_addr "
        "WHERE c.callee_addr = ?", (row["address_norm"],),
    ).fetchall()
    return _print_neighbors(rows, "callers", row["name"], row["address"])


def cmd_callees(conn, args):
    row = resolve_function(conn, args.key)
    if not row:
        print("Not found: %s" % args.key)
        return 1
    rows = conn.cursor().execute(
        "SELECT f.address, f.name FROM calls c "
        "JOIN functions f ON f.address_norm = c.callee_addr "
        "WHERE c.caller_addr = ?", (row["address_norm"],),
    ).fetchall()
    return _print_neighbors(rows, "callees", row["name"], row["address"])


def cmd_list(conn, args):
    cur = conn.cursor()
    rows = cur.execute(
        "SELECT address, name, size FROM functions "
        "ORDER BY address_norm LIMIT ? OFFSET ?",
        (args.limit, args.offset),
    ).fetchall()
    for r in rows:
        print("%s  %s  (%s bytes)" % (r["address"], r["name"], r["size"]))
    total = cur.execute("SELECT COUNT(*) FROM functions").fetchone()[0]
    print("\n(%d shown, %d total)" % (len(rows), total))
    return 0


def cmd_stats(conn, args):
    cur = conn.cursor()
    print("functions:     %d" % cur.execute("SELECT COUNT(*) FROM functions").fetchone()[0])
    print("calls:         %d" % cur.execute("SELECT COUNT(*) FROM calls").fetchone()[0])
    print("strings:       %d" % cur.execute("SELECT COUNT(*) FROM strings").fetchone()[0])
    try:
        print("registers:     %d" % cur.execute("SELECT COUNT(*) FROM registers").fetchone()[0])
        print("register_refs: %d" % cur.execute("SELECT COUNT(*) FROM register_refs").fetchone()[0])
    except sqlite3.OperationalError:
        print("registers:     0 (table not created yet)")
        print("register_refs: 0 (table not created yet)")
    try:
        print("embeddings:    %d" % cur.execute("SELECT COUNT(*) FROM embeddings").fetchone()[0])
    except sqlite3.OperationalError:
        print("embeddings:    0 (table not created yet)")
    return 0


# ---------------------------------------------------------------------------
# Register commands
# ---------------------------------------------------------------------------

def cmd_reg(conn, args):
    row = resolve_register(conn, args.key)
    if not row:
        print("Register not found: %s" % args.key)
        return 1
    print("=" * 60)
    print("Name:       %s" % row["name"])
    print("Address:    %s" % row["address"])
    print("AddressInt: %s" % row["address_int"])
    print("Size:       %s bytes" % row["size"])
    print("Type:       %s" % (row["type"] or "(none)"))
    print("Access:     %s" % (row["access"] or "(none)"))
    print("Region:     %s" % (row["region"] or "(none)"))
    print("RefCount:   %s" % row["reference_count"])
    print("=" * 60)

    rows = conn.cursor().execute(
        "SELECT f.address, f.name FROM register_refs rr "
        "JOIN functions f ON f.address_norm = rr.function_addr "
        "WHERE rr.register_addr = ? "
        "ORDER BY f.address_norm", (row["address_norm"],),
    ).fetchall()
    if rows:
        print("Referenced by %d function(s):" % len(rows))
        for r in rows:
            print("  %s @ %s" % (r["name"], r["address"]))
    else:
        print("No function references indexed.")
    return 0


def cmd_regs(conn, args):
    cur = conn.cursor()
    if args.query:
        try:
            rows = cur.execute(
                "SELECT r.address, r.name, r.region, r.type, r.access "
                "FROM registers_fts f JOIN registers r "
                "ON r.address_norm = f.address_norm "
                "WHERE registers_fts MATCH ? LIMIT ?",
                (args.query, args.limit),
            ).fetchall()
        except sqlite3.OperationalError:
            rows = []
    else:
        rows = cur.execute(
            "SELECT address, name, region, type, access FROM registers "
            "ORDER BY address_norm LIMIT ? OFFSET ?",
            (args.limit, args.offset),
        ).fetchall()

    if not rows:
        print("No registers found.")
        return 0
    for r in rows:
        print("%s  %-24s  %-20s  %-8s  %s"
              % (r["address"], r["name"], r["region"] or "-",
                 r["type"] or "-", r["access"] or "-"))
    total = cur.execute("SELECT COUNT(*) FROM registers").fetchone()[0]
    print("\n(%d shown, %d total)" % (len(rows), total))
    return 0


def cmd_regrefs(conn, args):
    row = resolve_register(conn, args.key)
    if not row:
        print("Register not found: %s" % args.key)
        return 1
    rows = conn.cursor().execute(
        "SELECT f.address, f.name FROM register_refs rr "
        "JOIN functions f ON f.address_norm = rr.function_addr "
        "WHERE rr.register_addr = ? "
        "ORDER BY f.address_norm LIMIT ?", (row["address_norm"], args.limit),
    ).fetchall()
    if not rows:
        print("No functions reference %s (%s)" % (row["name"], row["address"]))
        return 0
    print("Functions referencing %s (%s):" % (row["name"], row["address"]))
    for r in rows:
        print("  %s @ %s" % (r["name"], r["address"]))
    return 0


def cmd_funcregs(conn, args):
    row = resolve_function(conn, args.key)
    if not row:
        print("Function not found: %s" % args.key)
        return 1
    rows = conn.cursor().execute(
        "SELECT r.address, r.name, r.region, r.type, r.access "
        "FROM register_refs rr "
        "JOIN registers r ON r.address_norm = rr.register_addr "
        "WHERE rr.function_addr = ? "
        "ORDER BY r.address_norm LIMIT ?", (row["address_norm"], args.limit),
    ).fetchall()
    if not rows:
        print("No registers indexed for %s (%s)" % (row["name"], row["address"]))
        return 0
    print("Registers used by %s (%s):" % (row["name"], row["address"]))
    for r in rows:
        print("  %s  %-24s  %-20s  %s"
              % (r["address"], r["name"], r["region"] or "-",
                 r["access"] or "-"))
    return 0


# ---------------------------------------------------------------------------
# Semantic search
# ---------------------------------------------------------------------------

def cmd_ssearch(conn, args):
    require_requests()
    token = get_token(args)

    try:
        conn.execute("SELECT 1 FROM embeddings LIMIT 1").fetchone()
    except sqlite3.OperationalError:
        print("ERROR: embeddings table does not exist. Run 'embed' first.")
        return 1

    resp = requests.post(
        EMBED_API_URL,
        headers={"Authorization": "Bearer " + token,
                 "Content-Type": "application/json"},
        json={"model": args.model, "input": [args.query]},
        verify=False, timeout=60,
    )
    if resp.status_code != 200:
        print("API error %d: %s" % (resp.status_code, resp.text[:300]))
        return 1
    qvec = resp.json()["data"][0]["embedding"]

    rows = conn.cursor().execute(
        "SELECT e.address_norm, e.vector, f.name, f.address, f.signature "
        "FROM embeddings e JOIN functions f ON f.address_norm = e.address_norm"
    ).fetchall()

    scored = []
    for addr, blob, name, address, sig in rows:
        vec = blob_to_vector(blob)
        if len(vec) != len(qvec):
            continue
        score = cosine(vec, qvec)
        scored.append((score, name, address, sig))

    scored.sort(reverse=True)
    if not scored:
        print("No embeddings available.")
        return 0
    for score, name, address, sig in scored[:args.limit]:
        print("%.4f  %s @ %s" % (score, name, address))
        if sig:
            print("        %s" % sig)
    return 0


# ---------------------------------------------------------------------------
# Embedding generation
# ---------------------------------------------------------------------------

def ensure_embeddings_schema(conn):
    conn.executescript("""
    CREATE TABLE IF NOT EXISTS embeddings (
        address_norm TEXT PRIMARY KEY,
        model TEXT,
        dim INTEGER,
        vector BLOB
    );
    """)


def get_embedding(session, token, model, text):
    resp = session.post(
        EMBED_API_URL,
        headers={"Authorization": "Bearer " + token,
                 "Content-Type": "application/json",
                 "Accept": "application/json"},
        json={"model": model, "input": [text]},
        verify=False, timeout=60,
    )
    if resp.status_code != 200:
        print("API error %d: %s" % (resp.status_code, resp.text[:300]))
        return None
    data = resp.json()
    try:
        return data["data"][0]["embedding"]
    except (KeyError, IndexError):
        print("Unexpected response: %s" % str(data)[:300])
        return None


def cmd_embed(conn, args):
    require_requests()
    token = get_token(args)
    ensure_embeddings_schema(conn)
    cur = conn.cursor()

    rows = cur.execute(
        "SELECT address_norm, name, pseudocode FROM functions "
        "WHERE pseudocode IS NOT NULL AND pseudocode != ''"
    ).fetchall()
    if args.limit:
        rows = rows[:args.limit]

    total = len(rows)
    print("Embedding %d functions with %s" % (total, args.model))

    session = requests.Session()
    done = 0
    skipped = 0

    for i, r in enumerate(rows, 1):
        addr = r["address_norm"]
        text = (r["name"] + "\n" + r["pseudocode"])[:8000]

        existing = cur.execute(
            "SELECT model FROM embeddings WHERE address_norm=?", (addr,)
        ).fetchone()
        if existing and existing[0] == args.model:
            skipped += 1
            continue

        vec = get_embedding(session, token, args.model, text)
        if vec is None:
            time.sleep(2)
            continue

        cur.execute(
            "INSERT OR REPLACE INTO embeddings "
            "(address_norm, model, dim, vector) VALUES (?, ?, ?, ?)",
            (addr, args.model, len(vec), vector_to_blob(vec)),
        )
        done += 1

        if i % 50 == 0:
            conn.commit()
            print("  %d / %d (embedded=%d, skipped=%d)"
                  % (i, total, done, skipped))
            time.sleep(1)

    conn.commit()
    print("\nDone. Embedded: %d, skipped: %d, total rows: %d"
          % (done, skipped, total))
    return 0


# ---------------------------------------------------------------------------
# Interactive agent
# ---------------------------------------------------------------------------

SYSTEM_PROMPT = """You are a firmware reverse-engineering assistant.
You analyze decompiled SuperH firmware via CLI tools.

Strategy:
1. Use `search` or `grep` to find candidate functions.
2. Use `lookup` to read a function's pseudocode.
3. Use `callers` / `callees` to navigate the call graph.
4. Use `register` and `register_refs` to understand hardware interaction.
   Use `function_registers` to see which hardware a function touches.
5. Use `ssearch` for semantic queries when exact words are unknown.
6. Only print what is relevant. Do not dump entire functions unless asked.
7. Reply in the same language the user writes in.
"""


AGENT_TOOLS = [
    {"name": "lookup",
     "description": "Get function metadata and full pseudocode by address or name.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
    {"name": "search",
     "description": "Full-text search across function names and pseudocode.",
     "parameters": {"type": "object",
                    "properties": {"query": {"type": "string"}},
                    "required": ["query"]}},
    {"name": "ssearch",
     "description": "Semantic search using embeddings. Use when exact words unknown.",
     "parameters": {"type": "object",
                    "properties": {"query": {"type": "string"}},
                    "required": ["query"]}},
    {"name": "grep",
     "description": "Exact substring search in pseudocode.",
     "parameters": {"type": "object",
                    "properties": {"pattern": {"type": "string"}},
                    "required": ["pattern"]}},
    {"name": "callers",
     "description": "Find functions that call the given function.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
    {"name": "callees",
     "description": "Find functions called by the given function.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
    {"name": "strings",
     "description": "Search string constants in the firmware.",
     "parameters": {"type": "object",
                    "properties": {"pattern": {"type": "string"}},
                    "required": ["pattern"]}},
    {"name": "register",
     "description": "Show register info (name, address, type, access, region, "
                    "and functions that reference it) by address or name.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
    {"name": "search_registers",
     "description": "Search registers by name, region, or type.",
     "parameters": {"type": "object",
                    "properties": {"query": {"type": "string"}},
                    "required": ["query"]}},
    {"name": "register_refs",
     "description": "List functions that reference a given register.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
    {"name": "function_registers",
     "description": "List registers referenced by a given function.",
     "parameters": {"type": "object",
                    "properties": {"key": {"type": "string"}},
                    "required": ["key"]}},
]


def call_tool(db, name, args):
    tool_path = os.path.abspath(__file__)
    cmd = [sys.executable, tool_path, name, "--db", db]
    if name in ("lookup", "callers", "callees", "register",
                "register_refs", "function_registers"):
        # CLI subcommand names -> args
        cli = {
            "register": "reg",
            "register_refs": "regrefs",
            "function_registers": "funcregs",
        }.get(name, name)
        cmd = [sys.executable, tool_path, cli, "--db", db, args["key"]]
        try:
            out = subprocess.check_output(cmd, stderr=subprocess.STDOUT, timeout=60)
            return out.decode("utf-8", errors="replace")
        except subprocess.CalledProcessError as e:
            return "ERROR: " + e.output.decode("utf-8", errors="replace")
        except subprocess.TimeoutExpired:
            return "ERROR: tool timeout"

    if name in ("search", "ssearch", "grep", "strings"):
        cmd = [sys.executable, tool_path, name, "--db", db,
               args.get("query") or args.get("pattern")]
    elif name == "search_registers":
        cmd = [sys.executable, tool_path, "regs", "--db", db,
               args.get("query") or ""]
    else:
        return "ERROR: unknown tool %s" % name

    try:
        out = subprocess.check_output(cmd, stderr=subprocess.STDOUT, timeout=60)
        return out.decode("utf-8", errors="replace")
    except subprocess.CalledProcessError as e:
        return "ERROR: " + e.output.decode("utf-8", errors="replace")
    except subprocess.TimeoutExpired:
        return "ERROR: tool timeout"


def _normalize_args(raw_args):
    """GigaChat may return arguments as dict or JSON string."""
    if isinstance(raw_args, dict):
        return raw_args
    if isinstance(raw_args, str):
        try:
            parsed = json.loads(raw_args)
            return parsed if isinstance(parsed, dict) else {}
        except json.JSONDecodeError:
            return {}
    return {}


def chat_once(token, model, messages):
    """Send one request to GigaChat. Tries OpenAI-compatible tools format
    first, falls back to legacy functions format if the API rejects it."""
    body_tools = {
        "model": model,
        "messages": messages,
        "temperature": 0.2,
        "tools": [{"type": "function", "function": t} for t in AGENT_TOOLS],
        "tool_choice": "auto",
    }
    resp = requests.post(
        CHAT_API_URL,
        headers={"Authorization": "Bearer " + token,
                 "Content-Type": "application/json"},
        json=body_tools, verify=False, timeout=120,
    )
    if resp.status_code == 200:
        return resp.json(), "tools"

    body_functions = {
        "model": model,
        "messages": messages,
        "temperature": 0.2,
        "functions": AGENT_TOOLS,
        "function_call": "auto",
    }
    resp2 = requests.post(
        CHAT_API_URL,
        headers={"Authorization": "Bearer " + token,
                 "Content-Type": "application/json"},
        json=body_functions, verify=False, timeout=120,
    )
    if resp2.status_code == 200:
        return resp2.json(), "functions"

    print("API error %d (tools): %s" % (resp.status_code, resp.text[:300]))
    print("API error %d (functions): %s" % (resp2.status_code, resp2.text[:300]))
    sys.exit(1)


def cmd_chat(conn, args):
    require_requests()
    token = get_token(args)
    db_path = args.db

    messages = [{"role": "system", "content": SYSTEM_PROMPT}]
    print("Firmware agent ready (model=%s). Type 'exit' to quit." % args.model)

    while True:
        try:
            user = input("\n> ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            break
        if not user or user.lower() in ("exit", "quit"):
            break

        messages.append({"role": "user", "content": user})

        for _ in range(10):
            resp, mode = chat_once(token, args.model, messages)
            msg = resp["choices"][0]["message"]

            tool_calls = msg.get("tool_calls") or []
            if tool_calls:
                tc = tool_calls[0]
                fn = tc["function"]["name"]
                fn_args = _normalize_args(tc["function"].get("arguments"))
                print("  [tool] %s(%s)" % (fn, fn_args))
                result = call_tool(db_path, fn, fn_args)
                messages.append(msg)
                messages.append({
                    "role": "tool",
                    "tool_call_id": tc.get("id", fn),
                    "content": result,
                })
                continue

            if msg.get("function_call"):
                fn = msg["function_call"]["name"]
                fn_args = _normalize_args(msg["function_call"].get("arguments"))
                print("  [tool] %s(%s)" % (fn, fn_args))
                result = call_tool(db_path, fn, fn_args)
                messages.append(msg)
                messages.append({"role": "function", "name": fn,
                                 "content": result})
                continue

            print("\n" + (msg.get("content") or ""))
            messages.append(msg)
            break
    return 0


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def cmd_index(conn, args):
    build_index(
        db_path=args.db,
        decompiled_c_path=args.decompiled,
        functions_json_path=args.functions,
        strings_txt_path=args.strings,
        registers_json_path=args.registers,
        rebuild=not args.no_rebuild,
    )
    return 0


def build_parser():
    db_parent = argparse.ArgumentParser(add_help=False)
    db_parent.add_argument("--db", default=DEFAULT_DB,
                           help="SQLite database path")

    tok_parent = argparse.ArgumentParser(add_help=False)
    tok_parent.add_argument("--token",
                            help="GigaChat access token (or GIGACHAT_TOKEN env)")

    ap = argparse.ArgumentParser(description="Firmware analysis toolkit")
    sub = ap.add_subparsers(dest="cmd")
    sub.required = True

    p = sub.add_parser("index", parents=[db_parent],
                       help="Build SQLite index from decompiled firmware")
    p.add_argument("--decompiled", required=True)
    p.add_argument("--functions")
    p.add_argument("--strings")
    p.add_argument("--registers")
    p.add_argument("--no-rebuild", action="store_true")
    p.set_defaults(func=cmd_index, _needs_db=False)

    p = sub.add_parser("embed", parents=[db_parent, tok_parent])
    p.add_argument("--model", default="Embeddings-2")
    p.add_argument("--limit", type=int, default=0)
    p.set_defaults(func=cmd_embed)

    p = sub.add_parser("lookup", parents=[db_parent])
    p.add_argument("key")
    p.set_defaults(func=cmd_lookup)

    p = sub.add_parser("search", parents=[db_parent])
    p.add_argument("query")
    p.add_argument("--limit", type=int, default=20)
    p.set_defaults(func=cmd_search)

    p = sub.add_parser("ssearch", parents=[db_parent, tok_parent])
    p.add_argument("query")
    p.add_argument("--model", default="Embeddings-2")
    p.add_argument("--limit", type=int, default=10)
    p.set_defaults(func=cmd_ssearch)

    p = sub.add_parser("grep", parents=[db_parent])
    p.add_argument("pattern")
    p.add_argument("--limit", type=int, default=20)
    p.set_defaults(func=cmd_grep)

    p = sub.add_parser("strings", parents=[db_parent])
    p.add_argument("pattern")
    p.add_argument("--limit", type=int, default=50)
    p.set_defaults(func=cmd_strings)

    p = sub.add_parser("callers", parents=[db_parent])
    p.add_argument("key")
    p.set_defaults(func=cmd_callers)

    p = sub.add_parser("callees", parents=[db_parent])
    p.add_argument("key")
    p.set_defaults(func=cmd_callees)

    p = sub.add_parser("list", parents=[db_parent])
    p.add_argument("--limit", type=int, default=50)
    p.add_argument("--offset", type=int, default=0)
    p.set_defaults(func=cmd_list)

    p = sub.add_parser("stats", parents=[db_parent])
    p.set_defaults(func=cmd_stats)

    p = sub.add_parser("reg", parents=[db_parent])
    p.add_argument("key")
    p.set_defaults(func=cmd_reg)

    p = sub.add_parser("regs", parents=[db_parent])
    p.add_argument("query", nargs="?", default=None)
    p.add_argument("--limit", type=int, default=50)
    p.add_argument("--offset", type=int, default=0)
    p.set_defaults(func=cmd_regs)

    p = sub.add_parser("regrefs", parents=[db_parent])
    p.add_argument("key")
    p.add_argument("--limit", type=int, default=50)
    p.set_defaults(func=cmd_regrefs)

    p = sub.add_parser("funcregs", parents=[db_parent])
    p.add_argument("key")
    p.add_argument("--limit", type=int, default=100)
    p.set_defaults(func=cmd_funcregs)

    p = sub.add_parser("chat", parents=[db_parent, tok_parent])
    p.add_argument("--model", default="GigaChat-2-Max")
    p.set_defaults(func=cmd_chat)

    return ap


def main():
    ap = build_parser()
    args = ap.parse_args()

    if args.cmd == "index":
        return args.func(None, args)

    conn = connect_db(args.db)
    try:
        return args.func(conn, args)
    finally:
        conn.close()


if __name__ == "__main__":
    sys.exit(main())
    