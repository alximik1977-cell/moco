// ═══ Путь вызовов от entry point до NORM_START2 ═══
// Всего функций: 6
// Путь: 0000_0_TBR_1_FUN_00005da0 → M_0_FUN_00010a00 → M_1_FUN_00010c2c → M_2_MAIN_APP_CYCLE2_FUN_00011852 → BLOCK_IM_FUN_000151da → NORM_START2_FUN_00015816

// ═══ [1/6] 0000_0_TBR_1_FUN_00005da0 @ 0x00005da0 ═══
// Сигнатура: undefined 0000_0_TBR_1_FUN_00005da0()
// Размер: 72 байт
// Вызывает: FUN_0000112e, O_I_Polling_Timer_Setup_FUN_00005f40, M_0_FUN_00010a00, Watchdog_init_FUN_00000b00, init_hardwareFUN_00005de8, Watchdog_Hardware_Poll_Debounce_FUN_00005f6e, A0_DTC_INIT_FUN_0000151e, init_mem_clearFUN_00005ee6, FUN_00000ba0, M0_Start_FUN_00030a00, UTIL_checksums8_FUN_0000117a, CAN_init_mailboxFUN_00005efc, init_hardware_busFUN_00005e36
// Вызывается из: 0000_0_TBR_0_START_FUN_0000101c
// ⚠️ Тело функции не найдено в C-файле
void 0000_0_TBR_1_FUN_00005da0(void) { /* тело не найдено */ }

// ═══ [2/6] M_0_FUN_00010a00 @ 0x00010a00 ═══
// Сигнатура: undefined M_0_FUN_00010a00()
// Размер: 8 байт
// Вызывает: M_1_FUN_00010c2c
// Вызывается из: 0000_0_TBR_1_FUN_00005da0

void M_0_FUN_00010a00(void)

{
  M_1_FUN_00010c2c();
  return;
}

// ═══ [3/6] M_1_FUN_00010c2c @ 0x00010c2c ═══
// Сигнатура: undefined M_1_FUN_00010c2c()
// Размер: 18 байт
// Вызывает: INIT_RAM_FUN_0001679c, M_2_MAIN_APP_CYCLE2_FUN_00011852
// Вызывается из: M_0_FUN_00010a00

void M_1_FUN_00010c2c(void)

{
  INIT_RAM_FUN_0001679c();
  M_2_MAIN_APP_CYCLE2_FUN_00011852();
  return;
}

// ═══ [4/6] M_2_MAIN_APP_CYCLE2_FUN_00011852 @ 0x00011852 ═══
// Сигнатура: undefined M_2_MAIN_APP_CYCLE2_FUN_00011852()
// Размер: 168 байт
// Вызывает: A2_FUN_00011f00, A2_FUN_00012c2e, A2_FUN_00012d16, A2_FUN_00017f5c, DTC_TRAIN_REMOVE__FUN_00012d52, A2_FUN_00012e24, A2_FUN_00011c9c, CAN_FUN_0001191c, N4_init_Status_Register_FUN_00000baa, CAN_FUN_00011964, BLOCK_IM_FUN_000151da, A2_FUN_000123a6, A2_FUN_00011f76, A2_FUN_00016fa2, NORM_START_FUN_00015800, A2_FUN_00011fcc, CAN_FUN_000119e8, A2_FUN_00012a8c, A2_FUN_000120c4, A2_FUN_00012e64, A2_FUN_00011b92
// Вызывается из: M_1_FUN_00010c2c

void M_2_MAIN_APP_CYCLE2_FUN_00011852(void)

{
  CAN_FUN_0001191c();
  A2_FUN_00017f5c();
  N4_init_Status_Register_FUN_00000baa(0);
  DAT_fff8d800 = (short)(DAT_fffff220 >> 5) + 0x271;
  do {
    DAT_fff88202 = 3;
    CAN_FUN_00011964();
    CAN_FUN_000119e8();
    A2_FUN_00016fa2();
    A2_FUN_000123a6();
    A2_FUN_00012a8c();
    DTC_TRAIN_REMOVE__FUN_00012d52();
    A2_FUN_00012c2e();
    A2_FUN_00011b92();
    A2_FUN_00011c9c();
    A2_FUN_00011f00();
    if ((DAT_fff88344 & 1) == 0) {
      BLOCK_IM_FUN_000151da();
    }
    else {
      NORM_START_FUN_00015800();
    }
    A2_FUN_00011f76();
    A2_FUN_00011fcc();
    A2_FUN_000120c4();
    A2_FUN_00012d16();
    A2_FUN_00012e24();
    A2_FUN_00012e64();
    DAT_fff88344 = DAT_fff88344 + 1;
  } while( true );
}

// ═══ [5/6] BLOCK_IM_FUN_000151da @ 0x000151da ═══
// Сигнатура: undefined BLOCK_IM_FUN_000151da()
// Размер: 1398 байт
// Вызывает: NORM_START2_FUN_00015816, FUN_0001623a, A6_FUN_0001614c, FUN_00015f20, FUN_0001651c, A6_FUN_00016058
// Вызывается из: M_2_MAIN_APP_CYCLE2_FUN_00011852

undefined8 BLOCK_IM_FUN_000151da(void)

{
  bool bVar1;
  bool bVar2;
  short sVar3;
  undefined4 uVar4;
  undefined4 in_r1;
  byte *pbVar5;
  byte *extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  int extraout_r1_03;
  uint extraout_r1_04;
  uint uVar6;
  int extraout_r2;
  uint extraout_r2_00;
  uint extraout_r2_01;
  uint extraout_r2_02;
  uint extraout_r2_03;
  uint uVar7;
  int iVar8;
  uint uVar9;
  ushort uVar11;
  int iVar10;
  byte bVar12;
  byte bVar13;
  code *in_tbr;
  undefined1 uStack_18;
  int iStack_c;
  undefined1 uStack_8;
  byte bStack_4;
  
  uVar9 = DAT_fffff220 >> 5;
  (*in_tbr)();
  if (MASTER_STATE_DAT_fff88eae != 0xffff) {
    DAT_fff88eb2 = 8;
  }
  if (DAT_fff88eb2 != 0) {
    DAT_fff88eb2 = DAT_fff88eb2 + -1;
  }
  sVar3 = DAT_fff88eb8;
  if (DAT_fff88eb2 == 0) {
    SUB_CTRL_FLAGS_DAT_fff88ec2 = SUB_CTRL_FLAGS_DAT_fff88ec2 | 1;
    FUN_0001623a(1);
    sVar3 = DAT_fff88eb8 + 1;
    if ((short)(DAT_fff88eb8 + 1) == 0) {
      sVar3 = DAT_fff88eb8;
    }
  }
  DAT_fff88eb8 = sVar3;
  (*(in_tbr + 0x10))();
  sVar3 = DAT_fff88eb8;
  if ((MASTER_STATE_DAT_fff88eae == 0xffff) && (DAT_fff88e79 != '!')) {
    (*in_tbr)();
    SUB_CTRL_FLAGS_DAT_fff88ec2 = SUB_CTRL_FLAGS_DAT_fff88ec2 | 4;
    (*(in_tbr + 0x10))();
    FUN_0001623a(1);
    sVar3 = DAT_fff88eb8 + 1;
    if ((short)(DAT_fff88eb8 + 1) == 0) {
      sVar3 = DAT_fff88eb8;
    }
  }
  DAT_fff88eb8 = sVar3;
  iVar8 = 0;
  bVar1 = false;
  do {
    if (bVar1) break;
    (*in_tbr)();
    pbVar5 = &IMMO_CAN_FIFO0 + iVar8;
    sVar3 = DAT_fff88eb8;
    if ((&IMMO_CAN_FIFO3)[iVar8] != '\0') {
      bVar12 = (&IMMO_CAN_FIFO3)[iVar8] - 1;
      (&IMMO_CAN_FIFO3)[iVar8] = bVar12;
      if (bVar12 <= DAT_fff88eca) {
        DAT_fff88eca = (ushort)(byte)(&IMMO_CAN_FIFO3)[iVar8];
      }
      if (*pbVar5 == 0) {
        uVar11 = 1;
      }
      else {
        uVar11 = (ushort)(1 << (*pbVar5 & 0x1f));
      }
      DAT_fff88ecc = DAT_fff88ecc | uVar11;
      sVar3 = DAT_fff88eb8;
      if ((&IMMO_CAN_FIFO3)[iVar8] == '\0') {
        SUB_CTRL_FLAGS_DAT_fff88ec2 = SUB_CTRL_FLAGS_DAT_fff88ec2 | 2;
        FUN_0001623a(1);
        pbVar5 = extraout_r1;
        sVar3 = DAT_fff88eb8 + 1;
        if ((short)(DAT_fff88eb8 + 1) == 0) {
          sVar3 = DAT_fff88eb8;
        }
      }
    }
    DAT_fff88eb8 = sVar3;
    if (pbVar5[1] == 0) {
      bVar1 = true;
    }
    (*(in_tbr + 0x10))();
    iVar8 = iVar8 + 4;
  } while (extraout_r2 != 1);
  sVar3 = DAT_fff88eb8;
  if (MASTER_STATE_DAT_fff88eae != 0xffff) {
    if (DAT_fff88eb0 != 0) {
      DAT_fff88eb0 = DAT_fff88eb0 + -1;
    }
    if (DAT_fff88eb0 == 0) {
      DAT_fff88e80 = 0;
      DAT_fff88e81 = 1;
      DAT_fff88ed0 = 0;
      DAT_fff88ed2 = 0;
      DAT_fff88ed4 = 0;
      DAT_fff88ed6 = 0;
      DAT_fff88ed8 = 0;
      DAT_fff88eda = 0;
      (*in_tbr)();
      SUB_CTRL_FLAGS_DAT_fff88ec2 = SUB_CTRL_FLAGS_DAT_fff88ec2 | 8;
      (*(in_tbr + 0x10))();
      FUN_0001623a(1);
      sVar3 = DAT_fff88eb8 + 1;
      if ((short)(DAT_fff88eb8 + 1) == 0) {
        sVar3 = DAT_fff88eb8;
      }
    }
  }
  DAT_fff88eb8 = sVar3;
  if ((MASTER_STATE_DAT_fff88eae == 0) || (MASTER_STATE_DAT_fff88eae == 1)) {
    FUN_0001623a(0);
  }
  else if (3 < MASTER_STATE_DAT_fff88eae) {
    bVar1 = false;
    uVar6 = NORM_START2_FUN_00015816();
    if ((uVar6 & 0xff) == 0) {
      bVar2 = false;
      iStack_c = 0x1e;
      do {
        if (bVar2) break;
        uStack_8 = 0;
        uStack_18 = 0;
        bStack_4 = 0;
        (*in_tbr)();
        bVar12 = *(byte *)(extraout_r1_00 + 1);
        bVar13 = bVar12 & 0xf;
        if (bVar13 == 4) {
          bStack_4 = bVar12 & 0xf0;
          *(byte *)(extraout_r1_00 + 1) = bStack_4 | 5;
          uStack_18 = *(undefined1 *)(extraout_r1_00 + 2);
          uStack_8 = *(undefined1 *)(extraout_r1_00 + 3);
        }
        else if (bVar13 == 3) {
          *(byte *)(extraout_r1_00 + 1) = bVar12 & 0xf0 | 4;
        }
        else if ((bVar13 == 1) || (bVar13 == 2)) {
          bVar1 = true;
        }
        (*(in_tbr + 0x10))();
        if (bStack_4 == 0x10) {
          A6_FUN_0001614c(extraout_r2_00 & 0xff,uStack_18,uStack_8);
          iVar8 = extraout_r1_02;
LAB_0001550c:
          bVar1 = true;
        }
        else {
          iVar8 = extraout_r1_01;
          if (bStack_4 == 0x30) {
            A6_FUN_00016058(extraout_r2_00 & 0xff,uStack_8);
            iVar8 = extraout_r1_03;
            goto LAB_0001550c;
          }
        }
        if (*(char *)(iVar8 + 1) == '\0') {
          bVar2 = true;
        }
        iStack_c = iStack_c + -1;
      } while (iStack_c != 0);
    }
    else if (uVar6 == 2) {
      bVar1 = true;
    }
    if (MASTER_STATE_DAT_fff88eae == 0xffff) {
      FUN_0001623a(0);
      if (DAT_fff88eba == 0) {
        bVar1 = true;
        A6_FUN_0001614c(0,0xf9,0);
      }
      if (DAT_fff88eba < 2) {
        DAT_fff88eba = DAT_fff88eba + 1;
      }
      else {
        DAT_fff88eba = 0;
      }
      if (!bVar1) {
        uVar6 = (uint)(byte)DAT_fff88ea8;
        if (uVar6 == 1) {
          A6_FUN_00016058(1,0);
          uVar6 = extraout_r2_01;
        }
        else {
          A6_FUN_0001614c(uVar6,(&DAT_00013a90)[uVar6],0);
          uVar6 = extraout_r2_02;
        }
        if ((uVar6 & 0xff) == 1) {
          DAT_fff88ea8 = 3;
        }
        else if ((uVar6 & 0xff) < 6) {
          DAT_fff88ea8 = DAT_fff88ea8 + 1;
        }
        else {
          DAT_fff88ea8 = 1;
        }
      }
      (*in_tbr)();
      ERROR_ACK_REG = ERROR_ACK_REG & 0x81;
      (*(in_tbr + 0x10))();
      A6_FUN_0001614c(7,ERROR_ACK_REG,0);
    }
    if (MASTER_STATE_DAT_fff88eae == 0xffff) {
      if ((DAT_fff88eea & 1) == 0) {
        A6_FUN_0001614c(2,1,0);
        (*in_tbr)();
        DAT_fff88eea = DAT_fff88eea | 1;
        if (DAT_fff88ee0 != 0) {
          DAT_fff88ee0 = DAT_fff88ee0 + -1;
        }
        (*(in_tbr + 0x10))();
      }
      FUN_0001651c();
    }
    FUN_00015f20();
  }
  bVar1 = false;
  (*in_tbr)();
  iVar8 = 0;
  uVar6 = extraout_r1_04;
  uVar7 = extraout_r2_03;
  do {
    if (bVar1) break;
    iVar10 = (uVar6 & 0xff) * 4;
    if (((&IMMO_CAN_FIFO1)[iVar10] & 0xf) != 5) {
      uVar6 = uVar6 & 0xff;
      if (uVar6 != (uVar7 & 0xff)) {
        (&IMMO_CAN_FIFO0)[iVar8] = (&IMMO_CAN_FIFO0)[iVar10];
        (&IMMO_CAN_FIFO1)[iVar8] = (&IMMO_CAN_FIFO1)[iVar10];
        (&IMMO_CAN_FIFO2)[iVar8] = (&IMMO_CAN_FIFO2)[iVar10];
        (&IMMO_CAN_FIFO3)[iVar8] = (&IMMO_CAN_FIFO3)[iVar10];
      }
      uVar7 = (uVar7 & 0xff) + 1;
      iVar8 = iVar8 + 4;
    }
    if ((&IMMO_CAN_FIFO1)[iVar10] == '\0') {
      bVar1 = true;
    }
    uVar6 = uVar6 + 1 & 0xff;
  } while (uVar6 < 0x1e);
  while( true ) {
    uVar6 = uVar6 & 0xff;
    uVar7 = uVar7 & 0xff;
    if (uVar6 <= uVar7) break;
    iVar8 = uVar7 * 4;
    (&IMMO_CAN_FIFO0)[iVar8] = 0;
    (&IMMO_CAN_FIFO1)[iVar8] = 0;
    (&IMMO_CAN_FIFO2)[iVar8] = 0;
    (&IMMO_CAN_FIFO3)[iVar8] = 0;
    uVar7 = uVar7 + 1;
  }
  uVar4 = (*(in_tbr + 0x10))();
  DAT_fff8473e = (short)(DAT_fffff220 >> 5);
  DAT_fff84742 = (short)(DAT_fffff220 >> 5) - (short)uVar9;
  DAT_fff84740 = (short)(DAT_fffff220 >> 5) - DAT_fff8473e;
  DAT_fff8473c = (ushort)DAT_fff88e82 * 0x100 + (ushort)DAT_fff88e86;
  DAT_fff8473a = (ushort)DAT_fff88e80 * 0x100 + (ushort)DAT_fff88e81;
  return CONCAT44(in_r1,uVar4);
}

// ═══ [6/6] NORM_START2_FUN_00015816 @ 0x00015816 ═══
// Сигнатура: undefined NORM_START2_FUN_00015816()
// Размер: 272 байт
// Вызывает: FUN_0001a616, A6_FUN_0001614c, A6_FUN_00016058
// Вызывается из: NORM_START_FUN_00015800, BLOCK_IM_FUN_000151da

undefined8 NORM_START2_FUN_00015816(void)

{
  int iVar1;
  ushort uVar2;
  undefined4 in_r1;
  uint extraout_r1;
  uint uVar3;
  uint extraout_r2;
  uint extraout_r2_00;
  uint extraout_r2_01;
  uint extraout_r2_02;
  undefined1 uVar4;
  byte bVar5;
  int iVar6;
  code *in_tbr;
  undefined1 uStack_8;
  undefined1 uStack_4;
  
  (*in_tbr)();
  uVar2 = DAT_fff88ece + 1;
  if ((ushort)(DAT_fff88ece + 1) == 0) {
    uVar2 = DAT_fff88ece;
  }
  DAT_fff88ece = uVar2;
  (*(in_tbr + 0x10))();
  (*in_tbr)();
  if (((DAT_fff88eaa != 0) && (199 < ((DAT_fffff220 >> 5) - (int)DAT_fff88eac & 0xffff))) ||
     (3 < DAT_fff88ece)) {
    DAT_fff88eaa = 0;
  }
  (*(in_tbr + 0x10))();
  uVar3 = extraout_r2;
  if ((extraout_r2 & 0xff) == 1) {
    bVar5 = 0;
    FUN_0001a616();
    DAT_fff88ece = 0;
    uStack_4 = 0;
    uStack_8 = 0;
    uVar4 = 0;
    (*in_tbr)();
    uVar3 = extraout_r1;
    iVar1 = 0;
    do {
      iVar6 = iVar1;
      if (((&IMMO_CAN_FIFO1)[iVar6] & 0xf) != 5) break;
      uVar3 = uVar3 + 1 & 0xff;
      iVar1 = iVar6 + 4;
    } while (uVar3 < 0x1e);
    if ((((&IMMO_CAN_FIFO1)[iVar6] & 0xf) == 3) || (((&IMMO_CAN_FIFO1)[iVar6] & 0xf) == 4)) {
      bVar5 = (&IMMO_CAN_FIFO1)[iVar6];
      (&IMMO_CAN_FIFO1)[iVar6] = bVar5 & 0xf0 | 5;
      uVar4 = (&IMMO_CAN_FIFO0)[iVar6];
      uStack_8 = (&IMMO_CAN_FIFO2)[iVar6];
      uStack_4 = (&IMMO_CAN_FIFO3)[iVar6];
      bVar5 = bVar5 & 0xf0;
    }
    (*(in_tbr + 0x10))();
    if (bVar5 == 0x10) {
      A6_FUN_0001614c(uVar4,uStack_8,uStack_4);
      uVar3 = extraout_r2_01;
    }
    else {
      uVar3 = extraout_r2_00;
      if (bVar5 == 0x30) {
        A6_FUN_00016058(uVar4,uStack_4);
        uVar3 = extraout_r2_02;
      }
    }
  }
  return CONCAT44(in_r1,uVar3);
}
