// ═══════════════════════════════════════════════════════════
// РАСШИРЕННЫЙ КОНТЕКСТ ПУТИ
// Основной путь: 0000_0_TBR_1_FUN_00005da0 → M0_Start_FUN_00030a00 → M1_START_FUN_00030c2c → M2_MAIN_app_FUN_000a9148 → N18_Math_Util_Dtc_set_flags_FUN_000718ea → B_FUN_000800f0 → Immo_StateSyncAndCleanup_FUN_000b4dd8
// Всего функций: 54 (основных: 7, вызываемых: 47)
// Всего регистров: 514
// ═══════════════════════════════════════════════════════════

// ═══════════════════════════════════════════════════════════
// СЕКЦИЯ 1: ОСНОВНОЙ ПУТЬ ВЫЗОВОВ
// ═══════════════════════════════════════════════════════════

// ═══ [1/7] 0000_0_TBR_1_FUN_00005da0 @ 0x00005da0 ═══
// Функция инициализации системы и запуска основного приложения.
// Условия вызова следующей функции неизвестны, так как тело функции отсутствует.
// Использует периферийные регистры для настройки аппаратного обеспечения.

// Сигнатура: undefined 0000_0_TBR_1_FUN_00005da0()
// Вызывает: FUN_0000112e, O_I_Polling_Timer_Setup_FUN_00005f40, M_0_FUN_00010a00, Watchdog_init_FUN_00000b00, init_hardwareFUN_00005de8, Watchdog_Hardware_Poll_Debounce_FUN_00005f6e, A0_DTC_INIT_FUN_0000151e, init_mem_clearFUN_00005ee6, FUN_00000ba0, M0_Start_FUN_00030a00, UTIL_checksums8_FUN_0000117a, CAN_init_mailboxFUN_00005efc, init_hardware_busFUN_00005e36
// ⚠️ Тело не найдено

// ═══ [2/7] M0_Start_FUN_00030a00 @ 0x00030a00 ═══
// Запускает начальную последовательность выполнения программы.
// Всегда вызывает M1_START_FUN_00030c2c без условий.
// Не использует периферийные регистры или другую периферию.

// Сигнатура: undefined M0_Start_FUN_00030a00()
// Вызывает: M1_START_FUN_00030c2c

void M0_Start_FUN_00030a00(void)

{
  M1_START_FUN_00030c2c();
  return;
}

// ═══ [3/7] M1_START_FUN_00030c2c @ 0x00030c2c ═══
// Выполняет очистку глобальных переменных и запускает основное приложение.
// Сначала очищает три глобальные переменные с помощью M2_clear_3_globals_FUN_000becdc,
// затем всегда вызывает M2_MAIN_app_FUN_000a9148.
// Не использует периферийные регистры или другую периферию.

// Сигнатура: undefined M1_START_FUN_00030c2c()
// Вызывает: M2_clear_3_globals_FUN_000becdc, M2_MAIN_app_FUN_000a9148

void M1_START_FUN_00030c2c(void)

{
  M2_clear_3_globals_FUN_000becdc();
  M2_MAIN_app_FUN_000a9148();
  return;
}

// ═══ [4/7] M2_MAIN_app_FUN_000a9148 @ 0x000a9148 ═══
// Основная функция приложения, выполняющая цикл задач и обработку состояний.
// Циклически выполняет различные подзадачи и проверяет состояние флагов.
// Продолжает выполнение бесконечно, пока условие (IMMO_FLAGS_CONF_DAT_fff84540 & 2) == 0 остается истинным.
// Использует множество периферийных регистров для управления состоянием и синхронизации.

// Сигнатура: undefined M2_MAIN_app_FUN_000a9148()
// Вызывает: N8_bits_check_FUN_00094942, N19_Subsystem_StateMachine_Handler_FUN_00089da8, N18_Math_Util_Dtc_set_flags_FUN_000718ea, N17_MAIN_SHED_IMMO_INIT_FUN_000af9b4, N22_DECODE_WRITE_DAT_FUN_000be7ce, N11_State_control_FUN_00091dcc, N1_init_DTC_CALC_STORE_FUN_000949ac, N12_Iz_Immo_Security_Flag_INIT_FUN_000a9718, N6_flags_init_FUN_0006000a, N10_ECU_initialization_sequenceFUN_000abf52, N23_DAT_UTIL_FUN_000bef78, N21_FLAGS_Handler_FUN_000be6a8, N3_init_dataRegisters_FUN_000c3230, N20_DTC_BODY_REMOVE__FUN_000896ca, N15_INIT_routine_FUN_00095788, N5_delay_synchro_FUN_000c3388, N13_INIT_FUN_0008a398, N14_CALL_TBRs_FUN_0008c848, N4_init_Status_Register_FUN_00000baa, N7_copy_init_config_FUN_000a9270, N16_IMMO_FUEL_LAMBDA_FUN_0009c420, N9_other_FUN_0006cb72, N2_init_RAM_FUN_000befc8

void M2_MAIN_app_FUN_000a9148(void)

{
  short sVar1;
  code *in_tbr;
  
  N1_init_DTC_CALC_STORE_FUN_000949ac();
  N2_init_RAM_FUN_000befc8();
  N3_init_dataRegisters_FUN_000c3230();
  N4_init_Status_Register_FUN_00000baa(0);
  do {
    do {
      DAT_fff8dffa = 0xc1b0;
      DAT_fff8dffc = 0;
      DAT_fff8dffe = 0;
      N5_delay_synchro_FUN_000c3388();
      N6_flags_init_FUN_0006000a();
      N7_copy_init_config_FUN_000a9270();
      N8_bits_check_FUN_00094942();
      N9_other_FUN_0006cb72();
      N10_ECU_initialization_sequenceFUN_000abf52();
      N11_State_control_FUN_00091dcc();
      N12_Iz_Immo_Security_Flag_INIT_FUN_000a9718();
      N13_INIT_FUN_0008a398();
      N14_CALL_TBRs_FUN_0008c848();
      N15_INIT_routine_FUN_00095788();
      N16_IMMO_FUEL_LAMBDA_FUN_0009c420();
      N17_MAIN_SHED_IMMO_INIT_FUN_000af9b4();
      N18_Math_Util_Dtc_set_flags_FUN_000718ea();
      N19_Subsystem_StateMachine_Handler_FUN_00089da8();
      N20_DTC_BODY_REMOVE__FUN_000896ca();
      N21_FLAGS_Handler_FUN_000be6a8();
      N22_DECODE_WRITE_DAT_FUN_000be7ce();
      N23_DAT_UTIL_FUN_000bef78();
      sVar1 = (short)(DAT_fffff220 >> 5);
      DAT_fff84524 = sVar1 - DAT_fff84526;
      DAT_fff84526 = sVar1;
    } while ((IMMO_FLAGS_CONF_DAT_fff84540 & 2) == 0);
    (*in_tbr)();
    DAT_fff85218 = DAT_fff85218 | 0x8000;
    (*(in_tbr + 0x10))();
  } while( true );
}

// ═══ [5/7] N18_Math_Util_Dtc_set_flags_FUN_000718ea @ 0x000718ea ═══
// Устанавливает флаги диагностики и состояния системы.
// Последовательно вызывает ряд вспомогательных функций для обработки различных аспектов состояния системы.
// Не имеет явных условий вызова следующей функции, так как все вызовы последовательны.
// Использует несколько периферийных регистров для хранения промежуточных результатов и проверки состояния.

// Сигнатура: undefined N18_Math_Util_Dtc_set_flags_FUN_000718ea()
// Вызывает: B_FUN_000800f0, B_FUN_0008638e, B_FUN_00088c50, MAIN_Sys_Init_Pending_FUN_00071ac0, IMMO_FLAGS_CONF_DAT_FUN_00080324, B_FUN_00062c9c, B_CAN_IMMO_DAT_FUN_000883a0, B_FUN_00088286, B_FUN_00071970, ECU_State_Flags_FUN_00072dae, B_FUN_00088254, DAT_INIT_COPY_FUN_00072ec8, IMMO_FLAGS_decrypt_FUN_00072d2c

void N18_Math_Util_Dtc_set_flags_FUN_000718ea(void)

{
  B_CAN_IMMO_DAT_FUN_000883a0();
  MAIN_Sys_Init_Pending_FUN_00071ac0();
  B_FUN_00062c9c();
  IMMO_FLAGS_decrypt_FUN_00072d2c();
  ECU_State_Flags_FUN_00072dae();
  DAT_INIT_COPY_FUN_00072ec8();
  B_FUN_000800f0();
  IMMO_FLAGS_CONF_DAT_FUN_00080324();
  B_FUN_00088c50();
  B_FUN_0008638e();
  B_FUN_00071970();
  B_FUN_00088254();
  B_FUN_00088286();
  return;
}

// ═══ [6/7] B_FUN_000800f0 @ 0x000800f0 ═══
// Обновляет состояние иммобилайзера и обрабатывает флаги безопасности.
// Проверяет условия для обновления регистра DAT_fff84ed4 и вызывает Immo_StateSyncAndCleanup_FUN_000b4dd8 при необходимости.
// Использует периферийный регистр DAT_fff84ed4 для хранения текущего состояния.

// Сигнатура: undefined B_FUN_000800f0()
// Вызывает: Immo_StateSyncAndCleanup_FUN_000b4dd8

void B_FUN_000800f0(void)

{
  ushort *extraout_r1;
  uint extraout_r2;
  code *in_tbr;
  
  (*in_tbr)();
  if ((DAT_fff84ed4 & 0x40) != 0) {
    if ((((DAT_fff8419c == 0) || ((DAT_fff853b8 & 4) == 0)) ||
        (((uint)(int)DAT_fff8d812 >> 8 & 0x40) == 0)) || ((DAT_fff8d810 & 4) != 0)) {
      DAT_fff84ed4 = DAT_fff84ed4 & 0xffbf;
      DAT_fff84ed8 = DAT_fff84ed8 & 0xeffd;
    }
    else {
      DAT_fff84ed4 = DAT_fff84ed4 | 0x40;
      DAT_fff84ed8 = DAT_fff84ed8 | 0x1000;
    }
    Immo_StateSyncAndCleanup_FUN_000b4dd8();
  }
  (*(in_tbr + 0x10))();
  if (DAT_fff8432a == 0) {
    DAT_fff8432a = 0x28;
  }
  if (((((DAT_fff80720 & *extraout_r1) == 0) && ((DAT_fff80722 & extraout_r1[1]) == 0)) &&
      (((DAT_fff80724 & extraout_r1[2]) == 0 &&
       (((DAT_fff80726 & extraout_r1[3]) == 0 && ((DAT_fff80728 & extraout_r1[4]) == 0)))))) &&
     (((DAT_fff8072a & extraout_r1[5]) == 0 &&
      ((((((DAT_fff8072c & extraout_r1[6]) == 0 && ((DAT_fff8072e & extraout_r1[7]) == 0)) &&
         ((DAT_fff80730 & extraout_r1[8]) == 0)) &&
        (((DAT_fff80732 & extraout_r1[9]) == 0 && ((DAT_fff80734 & extraout_r1[10]) == 0)))) &&
       ((DAT_fff80736 & extraout_r1[0xb]) == 0)))))) {
    Immo_Exch_Flag_DAT_fff84bf6 = Immo_Exch_Flag_DAT_fff84bf6 & 0x7fff;
  }
  else {
    Immo_Exch_Flag_DAT_fff84bf6 = Immo_Exch_Flag_DAT_fff84bf6 | (ushort)extraout_r2;
  }
  if ((DAT_fff84ed4 & 0x40) == 0) {
    if ((extraout_r2 & DAT_fff80784) == 0) {
      if (((extraout_r2 & Immo_Exch_Flag_DAT_fff84bf6) == 0) &&
         (((uint)(int)DAT_fff847ea >> 2 & 0x80) != 0)) goto LAB_000802fa;
    }
    else if (DAT_fff8432a < 0x14) {
LAB_000802fa:
      DAT_fff84532 = DAT_fff84532 & 0xfff7;
      return;
    }
  }
  else if ((DAT_fff84ed8 & 2) == 0) goto LAB_000802fa;
  DAT_fff84532 = DAT_fff84532 | 8;
  return;
}

// ═══ [7/7] Immo_StateSyncAndCleanup_FUN_000b4dd8 @ 0x000b4dd8 ═══
// Синхронизирует состояние иммобилайзера и очищает временные данные.
// Очищает определенные регистры и устанавливает значения по умолчанию после завершения начальной последовательности.
// Использует множество периферийных регистров для очистки данных и сброса состояния.

// Сигнатура: undefined Immo_StateSyncAndCleanup_FUN_000b4dd8()

void Immo_StateSyncAndCleanup_FUN_000b4dd8(void)

{
  code *in_tbr;
  
  (*in_tbr)();
  if (((uint)(int)(short)DAT_fff84ed8 >> 8 & 0x10) == 0) {
    DAT_fff84ed4 = DAT_fff84ed4 & 1;
    DAT_fff84ed6 = DAT_fff84ed6 & 0xffc0;
  }
  else {
    DAT_fff84ee0 = 0x3c;
    if (((DAT_fff84ed4 & 0x50be) == 0) && ((DAT_fff84ed6 & 0x3e) == 0)) {
      DAT_fff84ed8 = DAT_fff84ed8 & 0xdfff;
    }
    else {
      DAT_fff84ed8 = DAT_fff84ed8 | 0x2000;
    }
    if (((uint)(int)(short)DAT_fff84ed4 >> 2 & 0x90) == 0) {
      DAT_fff84ed8 = DAT_fff84ed8 & 0xbfff;
    }
    else {
      DAT_fff84ed8 = DAT_fff84ed8 | 0x4000;
    }
    if ((((uint)(int)(short)DAT_fff84ed4 >> 8 & 0xac) == 0) && ((DAT_fff84ed6 & 1) == 0)) {
      DAT_fff84ed8 = DAT_fff84ed8 & 0x7fff;
    }
    else {
      DAT_fff84ed8 = DAT_fff84ed8 | 0x8000;
    }
    if (((uint)(int)(short)DAT_fff84ed4 >> 1 & 0x80) == 0) {
      DAT_fff84ed8 = DAT_fff84ed8 & 0xf7ff;
    }
    else {
      DAT_fff84ed8 = DAT_fff84ed8 | 0x800;
    }
    if ((((int)(short)DAT_fff84ed4 & 0xfffeU) != 0) || ((DAT_fff84ed6 & 0x3f) != 0))
    goto LAB_000b4f2a;
  }
  DAT_fff84ed8 = DAT_fff84ed8 & 0x400;
  DAT_fff84edc = 0;
  DAT_fff84ede = 0;
  DAT_fff84ee2 = 0;
  DAT_fff84ee4 = 0;
  DAT_fff84ee6 = 0;
  DAT_fff84ee8 = 0;
  DAT_fff84eea = 0;
  DAT_fff84eec = 0;
LAB_000b4f2a:
  (*(in_tbr + 0x10))();
  return;
}

// ═══════════════════════════════════════════════════════════
// СЕКЦИЯ 2: ФУНКЦИИ, ВЫЗЫВАЕМЫЕ ИЗ ОСНОВНОГО ПУТИ
// ═══════════════════════════════════════════════════════════

// ─── FUN_0000112e @ 0x0000112e (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined FUN_0000112e()

uint * FUN_0000112e(void)

{
  return &UINT_00040801;
}

// ─── O_I_Polling_Timer_Setup_FUN_00005f40 @ 0x00005f40 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined O_I_Polling_Timer_Setup_FUN_00005f40()

void O_I_Polling_Timer_Setup_FUN_00005f40(void)

{
  _DAT_fff8df90 = (uint)(ushort)((short)(DAT_fffff220 >> 5) + 0xfa) << 0x10;
  DAT_fffe3882 = DAT_fffe3882 | 0x40;
  DAT_fffe3886._0_1_ = DAT_fffe3886._0_1_ | 0x40;
  return;
}

// ─── M_0_FUN_00010a00 @ 0x00010a00 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined M_0_FUN_00010a00()

void M_0_FUN_00010a00(void)

{
  M_1_FUN_00010c2c();
  return;
}

// ─── Watchdog_init_FUN_00000b00 @ 0x00000b00 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined Watchdog_init_FUN_00000b00()

void Watchdog_init_FUN_00000b00(void)

{
  DAT_fff800cc = 0;
  return;
}

// ─── init_hardwareFUN_00005de8 @ 0x00005de8 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined init_hardwareFUN_00005de8()

void init_hardwareFUN_00005de8(void)

{
  DAT_fffff002 = 0;
  DAT_fffff003 = 0;
  DAT_fffff100 = 4;
  DAT_fffff102 = 0x27;
  DAT_fffff104 = 0x9f;
  DAT_fffff106 = 199;
  DAT_fffff202 = 0;
  DAT_fffff220 = 0xffffffff;
  DAT_fffff000 = 3;
  return;
}

// ─── Watchdog_Hardware_Poll_Debounce_FUN_00005f6e @ 0x00005f6e (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined Watchdog_Hardware_Poll_Debounce_FUN_00005f6e()

void Watchdog_Hardware_Poll_Debounce_FUN_00005f6e(void)

{
  bool bVar1;
  
  if ((((DAT_fffff220 >> 5) - (int)DAT_fff8df90 & 0xffff) >> 8 & 0x80) == 0) {
    DAT_fff8d800 = (short)(DAT_fffff220 >> 5) + 0x2ee;
    DAT_fff8df90 = DAT_fff8d800;
    if ((DAT_ffffc861 & 0x10) == 0) {
      Watchdog_timer_ctrl_FUN_00000b0a();
      bVar1 = (DAT_fffe3882 & 0x40) == 0;
      DAT_fffe3882 = (DAT_fffe3882 | 0x40) * bVar1 + (DAT_fffe3882 & 0xbf) * !bVar1;
      Watchdog_counter_FUN_00000b46();
    }
  }
  return;
}

// ─── A0_DTC_INIT_FUN_0000151e @ 0x0000151e (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined A0_DTC_INIT_FUN_0000151e()

void A0_DTC_INIT_FUN_0000151e(void)

{
  int extraout_r1;
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  uint uVar4;
  uint uVar5;
  
  DAT_fff8d808 = 0;
  CAN_KLINE_INIT_FUN_00008c98();
  FUN_00007a34();
  FUN_00005854();
  FUN_000014f4();
  if (((DAT_fff8001e & 0x80) == 0) && ((DAT_fff8001e & 0x40) != 0)) {
    uVar2 = 0;
    iVar1 = 0;
    uVar5 = 0;
    do {
      *(ushort *)((int)&DAT_fff80068 + iVar1) = (ushort)*(byte *)(extraout_r1 + uVar5);
      uVar4 = uVar5 + 1;
      iVar1 = iVar1 + 2;
      uVar2 = uVar2 + *(byte *)(extraout_r1 + uVar5);
      uVar5 = uVar4;
    } while (uVar4 < 0x11);
    uVar3 = 0;
    DAT_fff800a6 = uVar2 & 0xff;
    iVar1 = 0;
    uVar5 = 0;
    do {
      *(ushort *)((int)&DAT_fff8008a + iVar1) = (ushort)*(byte *)(extraout_r1 + 0x11 + uVar5);
      uVar4 = uVar5 + 1;
      iVar1 = iVar1 + 2;
      uVar3 = uVar3 + *(byte *)(extraout_r1 + 0x11 + uVar5);
      uVar5 = uVar4;
    } while (uVar4 < 10);
    uVar2 = 0;
    DAT_fff800a8 = uVar3 & 0xff;
    iVar1 = 0;
    uVar5 = 0;
    do {
      *(ushort *)((int)&DAT_fff8009e + iVar1) = (ushort)*(byte *)(extraout_r1 + 0x1b + uVar5);
      uVar4 = uVar5 + 1;
      iVar1 = iVar1 + 2;
      uVar2 = uVar2 + *(byte *)(extraout_r1 + 0x1b + uVar5);
      uVar5 = uVar4;
    } while (uVar4 < 4);
    DAT_fff800aa = uVar2 & 0xff;
    DAT_fff8de4c = DAT_fff8de4c | 0x49;
  }
  else {
    DAT_fff8de4c = DAT_fff8de4c & 0xffb6;
  }
  DAT_fff8d804 = 1;
  Sys_Control_Flags = Sys_Control_Flags | 0x41;
  DAT_fff8d810 = DAT_fff8d810 | 2;
  DAT_fff8d924 = &DAT_00030000;
  DAT_fff8d938 = 0;
  DAT_fff8d952 = DAT_fff8d952 & 0xffcf;
  Main_Scheduler_Super_Loop_FUN_00001676();
  return;
}

// ─── init_mem_clearFUN_00005ee6 @ 0x00005ee6 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined init_mem_clearFUN_00005ee6()

void init_mem_clearFUN_00005ee6(void)

{
  undefined4 *puVar1;
  
  for (puVar1 = (undefined4 *)&DAT_fff80000; puVar1 < ARRAY_fff8f008 + 0xff7; puVar1 = puVar1 + 1) {
    *puVar1 = *puVar1;
  }
  return;
}

// ─── FUN_00000ba0 @ 0x00000ba0 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined FUN_00000ba0()

void FUN_00000ba0(void)

{
  DAT_fff800d4 = 0;
  return;
}

// ─── UTIL_checksums8_FUN_0000117a @ 0x0000117a (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined UTIL_checksums8_FUN_0000117a()

undefined4 UTIL_checksums8_FUN_0000117a(void)

{
  Sys_Control_Flags = Sys_Control_Flags & 0xffb8;
  return 1;
}

// ─── CAN_init_mailboxFUN_00005efc @ 0x00005efc (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined CAN_init_mailboxFUN_00005efc()

void CAN_init_mailboxFUN_00005efc(void)

{
  DAT_fffc1400 = 1;
  DAT_fffc1408 = 0xf5;
  return;
}

// ─── init_hardware_busFUN_00005e36 @ 0x00005e36 (вызывается из callee_of_0000_0_TBR_1_FUN_00005da0) ───
// Сигнатура: undefined init_hardware_busFUN_00005e36()

void init_hardware_busFUN_00005e36(void)

{
  DAT_ffff0800 = 0x96ff;
  DAT_ffff0802 = 0x69ff;
  DAT_ffff0804 = 0x7600;
  DAT_ffff0806 = 0;
  DAT_ffff0810 = 0;
  DAT_ffff0812 = 0x7810;
  return;
}

// ─── M2_clear_3_globals_FUN_000becdc @ 0x000becdc (вызывается из callee_of_M1_START_FUN_00030c2c) ───
// Сигнатура: undefined M2_clear_3_globals_FUN_000becdc()

void M2_clear_3_globals_FUN_000becdc(void)

{
  DAT_fff8664c = 0;
  DAT_fff86650 = 0;
  DAT_fff86654 = 0;
  return;
}

// ─── N8_bits_check_FUN_00094942 @ 0x00094942 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N8_bits_check_FUN_00094942()

void N8_bits_check_FUN_00094942(void)

{
  DAT_fff852b4 = DAT_fff852b4 & 0xfffc;
  return;
}

// ─── N19_Subsystem_StateMachine_Handler_FUN_00089da8 @ 0x00089da8 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N19_Subsystem_StateMachine_Handler_FUN_00089da8()

void N19_Subsystem_StateMachine_Handler_FUN_00089da8(void)

{
  short sVar1;
  code *in_tbr;
  
  if ((((uint)(int)(short)DAT_fff852ae >> 8 & 0x80) == 0) &&
     (((uint)(int)(short)DAT_fff852ae >> 8 & 0x40) == 0)) {
    (*in_tbr)();
    DTC_DAT_fff86c4e = DTC_DAT_fff86c4e & 0xfffa;
  }
  else {
    (*in_tbr)();
    if (((uint)(int)(short)DAT_fff852ae >> 8 & 0x40) != 0) {
      DAT_fff852ae = DAT_fff852ae & 0xbfff;
      sVar1 = DAT_fff852b0 + 1;
      if ((short)(DAT_fff852b0 + 1) == 0) {
        sVar1 = DAT_fff852b0;
      }
      DAT_fff852b0 = sVar1;
      DTC_DAT_fff86c4e = DTC_DAT_fff86c4e & 0xfffb;
    }
    if ((DTC_DAT_fff86c4e & 6) == 0) {
      N19N1_J_Watchdog_timer_ctrl_FUN_00000950(0);
      DTC_DAT_fff86c4e = DTC_DAT_fff86c4e | 1;
    }
  }
  (*(in_tbr + 0x10))();
  return;
}

// ─── N17_MAIN_SHED_IMMO_INIT_FUN_000af9b4 @ 0x000af9b4 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N17_MAIN_SHED_IMMO_INIT_FUN_000af9b4()

void N17_MAIN_SHED_IMMO_INIT_FUN_000af9b4(void)

{
  A0_FUN_000afa08();
  BB_FUN_000afbb4();
  B_FUN_000b05c4();
  B_FUN_000b0a50();
  B_FUN_000b0afe();
  B_FUN_000b12e4();
  MAIN_SHED_ROOT_FUN_000b66c2();
  B_FUN_000b6da2();
  return;
}

// ─── N22_DECODE_WRITE_DAT_FUN_000be7ce @ 0x000be7ce (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N22_DECODE_WRITE_DAT_FUN_000be7ce()

void N22_DECODE_WRITE_DAT_FUN_000be7ce(void)

{
  undefined2 uVar1;
  int in_tbr;
  
  uVar1 = (*(code *)(in_tbr + 0x40))(DAT_fff8578e,0x8000,0x100);
  N22N1_FUN_000c5250(2,8000000,uVar1);
  return;
}

// ─── N11_State_control_FUN_00091dcc @ 0x00091dcc (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N11_State_control_FUN_00091dcc()

void N11_State_control_FUN_00091dcc(void)

{
  N11N1_FUN_00091e2e();
  N11N2_FUN_0009201c();
  N11N3_FUN_00092456();
  DAT_fff852b8 = DAT_fff84014;
  N11N4_FUN_00092780();
  DAT_fff852f4 = DAT_fff84016;
  N11N5_FUN_00093384();
  N11N6_FUN_000934e8();
  N11N7_FUN_000924e8();
  N11N8_FUN_000931dc();
  N11N9_FUN_00093584();
  N11N10_FUN_00091ed8();
  return;
}

// ─── N1_init_DTC_CALC_STORE_FUN_000949ac @ 0x000949ac (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N1_init_DTC_CALC_STORE_FUN_000949ac()

void N1_init_DTC_CALC_STORE_FUN_000949ac(void)

{
  int extraout_r1;
  uint uVar1;
  uint uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  
  N1N1_FUN_00060000();
  N1N2_FUN_00089618();
  N1N3_D1_InitiatorFUN_000896a4();
  N1N4_CAN_Hardware_Init_DuplicateFUN_000c4580();
  N4_init_Status_Register_FUN_00000baa(0);
  N1N6_CAN_FUN_000bf540();
  N1N7_CAN_HNDLR_FUN_000bf54e();
  N1N8_FUN_000bc7dc();
  N4_init_Status_Register_FUN_00000baa(0xf);
  N1N10_CAN_Diagnostic_Mailbox_ConfigFUN_000c46b6();
  N1N11_FUN_00094b1e();
  N1N12_FUN_000cab54(1);
  if (((DTC_DAT_fff86c4e & 0x40) != 0) && ((DTC_DAT_fff86c4e & 0x80) == 0)) {
    N1N13__DTC_LoadCalibrationDataFUN_00089e1e();
    DAT_fff803f2 = DAT_fff852a0;
    DAT_fff803f4 = DAT_fff852a2;
    DAT_fff803f6 = DAT_fff852a4;
    DAT_fff803e8 = DAT_fff85298;
  }
  N1N14_MAIN_immo_conf_init_start_FUN_0009530a();
  if (((DAT_fff8001e & 0x80) == 0) && ((DAT_fff8001e & 0x40) != 0)) {
    uVar4 = 0;
    iVar3 = 0;
    uVar2 = 0;
    do {
      *(ushort *)((int)&DAT_fff80068 + iVar3) = (ushort)*(byte *)(extraout_r1 + uVar2);
      uVar1 = uVar2 + 1;
      iVar3 = iVar3 + 2;
      uVar4 = uVar4 + *(byte *)(extraout_r1 + uVar2);
      uVar2 = uVar1;
    } while (uVar1 < 0x11);
    uVar5 = 0;
    DAT_fff800a6 = uVar4 & 0xff;
    iVar3 = 0;
    uVar2 = 0;
    do {
      *(ushort *)((int)&DAT_fff8008a + iVar3) = (ushort)*(byte *)(extraout_r1 + 0x11 + uVar2);
      uVar1 = uVar2 + 1;
      iVar3 = iVar3 + 2;
      uVar5 = uVar5 + *(byte *)(extraout_r1 + 0x11 + uVar2);
      uVar2 = uVar1;
    } while (uVar1 < 10);
    uVar4 = 0;
    DAT_fff800a8 = uVar5 & 0xff;
    iVar3 = 0;
    uVar2 = 0;
    do {
      *(ushort *)((int)&DAT_fff8009e + iVar3) = (ushort)*(byte *)(extraout_r1 + 0x1b + uVar2);
      uVar1 = uVar2 + 1;
      iVar3 = iVar3 + 2;
      uVar4 = uVar4 + *(byte *)(extraout_r1 + 0x1b + uVar2);
      uVar2 = uVar1;
    } while (uVar1 < 4);
    DAT_fff800aa = uVar4 & 0xff;
    DAT_fff8de4c = DAT_fff8de4c | 0x49;
  }
  else {
    DAT_fff8de4c = DAT_fff8de4c & 0xffb6;
  }
  if (DAT_fff841a0 != 0) {
    N1N15_FUN_000bc92c();
  }
  return;
}

// ─── N12_Iz_Immo_Security_Flag_INIT_FUN_000a9718 @ 0x000a9718 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N12_Iz_Immo_Security_Flag_INIT_FUN_000a9718()

void N12_Iz_Immo_Security_Flag_INIT_FUN_000a9718(void)

{
  N12N1_Immo_Security_Flag_double_FUN_000a9790();
  N12N2_ECU_State_Flags_INIT_FUN_000a9a14();
  N12N3_C7_FUN_000aa8a8();
  N12N4_C7_FUN_000aa906();
  N12N5_C7_FUN_000aa976();
  N12N6_C7_FUN_000aaa84();
  N12N7_C7_FUN_000aab60();
  N12N8_C7_FUN_000ab0f4();
  N12N9_C7_FUN_000ab16c();
  N12N10_FUN_0008c786();
  N12N11_FUN_0006aa7c();
  N12N11_FUN_0006a828();
  N12N12_FUN_000ab8ea();
  return;
}

// ─── N6_flags_init_FUN_0006000a @ 0x0006000a (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N6_flags_init_FUN_0006000a()

void N6_flags_init_FUN_0006000a(void)

{
  code *in_tbr;
  
  DAT_fff840a4 = 10;
  N6N1_CAN_FUN_000bdff8();
  (*in_tbr)();
  if ((DAT_fff841c6 == 0) || (DAT_fff8419c == 0)) {
    N6N2_FUN_000c413c_DAT_INIT8();
    N6N3_FUN_000c18d2_DAT_INIT6();
    N6N4_FUN_00063184_DAT_INIT7();
  }
  if (DAT_fff8419c == 0) {
    DAT_fff85526 = 0;
    DAT_fff85528 = 0;
    DAT_fff85506 = 0;
    DAT_fff8561c = 0x78;
    DAT_fff85550 = DAT_fff85550 & 0xc3d0;
    DAT_fff85634 = DAT_fff85634 & 0xfbfe;
    DAT_fff854ec = DAT_fff854ec & 0xfff9;
    DAT_fff840e2 = 0x14;
    DAT_fff843c4 = 200;
    DAT_fff85544 = 0;
    DAT_fff85552 = DAT_fff85552 & 0xd7ff;
  }
  (*(in_tbr + 0x10))();
  if (DAT_fff841c8 == 0) {
    (*in_tbr)();
    N6N5_FUN_000c39f4(7);
    (*(in_tbr + 0x10))();
  }
  return;
}

// ─── N10_ECU_initialization_sequenceFUN_000abf52 @ 0x000abf52 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N10_ECU_initialization_sequenceFUN_000abf52()

void N10_ECU_initialization_sequenceFUN_000abf52(void)

{
  N10N1_FUN_000ac08c();
  N10N2_FUN_000ac126();
  N10N3_CF_FUN_000ac218();
  if ((CAN_IMMO_DAT_fff847ca & 0x3000) == 0x1000) {
    N10N4__Immobilizer_synchro_FUN_00095434();
  }
  N10N5_Stack_set_FUN_000be22a();
  N10N6_FUN_000ac254();
  N10N7_CF_FUN_000ac6e4();
  N10N8_FUN_000ac8d8();
  N10N9_FUN_0006a458();
  N10N10_CF_FUN_0006a420();
  N10N11_FUN_0006a524();
  N10N12_FUN_000acb7a();
  N10N13_FUN_000af600();
  N10N14_UTIL_FUN_000af620();
  N10N15_FUN_000acef0();
  N10N16_FUN_0008ef3c();
  N10N17_FUN_0008f2e4();
  N10N18_CF_FUN_000ad282();
  N10N19_FUN_000ad3d4();
  N10N20_CF_Immobilizer_Security_Init_FUN_000ad524();
  N10N21_IMMO_AUTENIFICATION_FUN_000adabc();
  N10N22_FUN_000ac9f0();
  N10N23_FUN_000acb18();
  N10N24_FUN_000af238();
  N10N25_FUN_000af2ac();
  N10N26_FUN_000adeb0();
  N10N27_FUN_000adebe();
  N10N28_DTC_MANAGER_FUN_000adecc();
  N10N29_FUN_000aee28();
  N10N30_FUN_000aee6a();
  N10N31_init_hardwsre_FUN_000aeedc();
  N10N32_FUN_000af1b8();
  N10N33_DIAG_END_FUN_000af6cc();
  return;
}

// ─── N23_DAT_UTIL_FUN_000bef78 @ 0x000bef78 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N23_DAT_UTIL_FUN_000bef78()

void N23_DAT_UTIL_FUN_000bef78(void)

{
  undefined4 uVar1;
  undefined4 extraout_r1;
  int in_tbr;
  
  if (*DAT_fff86698 == -0x55555556) {
    DAT_fff86698 = DAT_fff86698 + 1;
  }
  else {
    uVar1 = UTIL_Saturating_Sub_FUN_000322be(0xfff90000,DAT_fff86698);
    DAT_fff8669c = (*(code *)(in_tbr + 0x60))(uVar1);
    uVar1 = UTIL_Saturating_Sub_FUN_000322be(extraout_r1,&DAT_fff8f000);
    DAT_fff8669e = (*(code *)(in_tbr + 0x60))(uVar1);
    DAT_fff86698 = &DAT_fff8f000;
  }
  return;
}

// ─── N21_FLAGS_Handler_FUN_000be6a8 @ 0x000be6a8 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N21_FLAGS_Handler_FUN_000be6a8()

void N21_FLAGS_Handler_FUN_000be6a8(void)

{
  byte *extraout_r1;
  int extraout_r1_00;
  byte *extraout_r1_01;
  code *in_tbr;
  
  (*in_tbr)();
  if ((DAT_fff84532 & 0x80) == 0) {
    DAT_ffffc830 = DAT_ffffc830 & 0xf7;
  }
  else {
    DAT_ffffc830 = DAT_ffffc830 | 8;
  }
  (*(in_tbr + 0x10))();
  (*in_tbr)();
  if ((DAT_fff84532 & 1) == 0) {
    *extraout_r1 = *extraout_r1 & 0xfd;
  }
  else {
    *extraout_r1 = *extraout_r1 | 2;
  }
  (*(in_tbr + 0x10))();
  (*in_tbr)();
  if (((uint)(int)(short)DAT_fff84532 >> 1 & 0x80) == 0) {
    DAT_ffffc820 = DAT_ffffc820 & 0xfd;
  }
  else {
    DAT_ffffc820 = DAT_ffffc820 | 2;
  }
  (*(in_tbr + 0x10))();
  (*in_tbr)();
  if (((uint)(int)(short)DAT_fff84532 >> 8 & 0x20) == 0) {
    *(byte *)(extraout_r1_00 + 1) = *(byte *)(extraout_r1_00 + 1) & 0xf7;
  }
  else {
    *(byte *)(extraout_r1_00 + 1) = *(byte *)(extraout_r1_00 + 1) | 8;
  }
  (*(in_tbr + 0x10))();
  (*in_tbr)();
  *extraout_r1_01 = *extraout_r1_01 & 0xfe;
  (*(in_tbr + 0x10))();
  return;
}

// ─── N3_init_dataRegisters_FUN_000c3230 @ 0x000c3230 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N3_init_dataRegisters_FUN_000c3230()

void N3_init_dataRegisters_FUN_000c3230(void)

{
  DAT_fffff000 = 0x3ff;
  DAT_fffec000 = 1;
  N3N1_FUN_000c5bf0();
  N3N2_FUN_000c63e4();
  N3N3_FUN_000c4060();
  return;
}

// ─── N20_DTC_BODY_REMOVE__FUN_000896ca @ 0x000896ca (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N20_DTC_BODY_REMOVE__FUN_000896ca()

void N20_DTC_BODY_REMOVE__FUN_000896ca(void)

{
  short sVar1;
  uint extraout_r1;
  uint uVar2;
  code *in_tbr;
  
  sVar1 = N20N1_J_DIAG_Read_FUN_0000095c();
  if (sVar1 == 0) {
    N20N2_DTC_MAIN_Status_Handler_FUN_000897f2();
    N20N3_J_Watchdog_counter_FUN_00000980();
    N20N4_DTC_Watchdog_FLAGs_SET_FUN_00089bf6();
  }
  (*in_tbr)();
  uVar2 = extraout_r1;
  if ((DAT_fff86c48 & 0x3333) != 0) {
    uVar2 = 1;
  }
  N20N5_FUN_00000998((uVar2 | (int)DTC_DAT_fff86c4e) & 0xffff);
  (*(in_tbr + 0x10))();
  if (((DAT_fff84fba & 0x80) == 0) &&
     (sVar1 = N20N6_Watchdog_counter_J_FUN_000008b4(0xfff0), sVar1 != 0)) {
    N20N7_J_Watchdog_FUN_000008f0();
  }
  return;
}

// ─── N15_INIT_routine_FUN_00095788 @ 0x00095788 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N15_INIT_routine_FUN_00095788()

void N15_INIT_routine_FUN_00095788(void)

{
  ushort extraout_r1;
  undefined2 extraout_r1_00;
  code *in_tbr;
  
  MAIN_INIT_subr_FUN_000958f4();
  FUN_00096330();
  FUN_00096728();
  FUN_0009678c();
  FUN_00096928();
  FUN_0009697a();
  FUN_00096b68();
  FUN_00096c5c();
  FUN_00096f44();
  FUN_00096f4e();
  FUN_00096fd4();
  FUN_00096fe4();
  FUN_00097020();
  FUN_00097036();
  FUN_0009707e();
  FUN_00097218();
  Immo_Configuration_Scaling_InitializationFUN_00097242();
  FUN_00097360();
  FUN_00097480();
  FUN_0009757e();
  FUN_00097684();
  FUN_000976d4();
  FUN_00097a52();
  FUN_0009795e();
  FUN_00097af0();
  FUN_00097b02();
  FUN_00098160();
  FUN_00099c60();
  I3_FUN_000992bc();
  if ((DAT_fff8419c != 0) || ((ECU_State_Flags & 0x10) == 0)) {
    DAT_fff841a0 = 0x168;
  }
  if ((CAN_IMMO_DAT_fff847ca & 0x3000) == 0x1000) {
    DAT_fff847dc = DAT_fff847dc & 0xf7ff;
  }
  else if ((DAT_fff80300 == '\0') && (DAT_fff80301 == '\0')) {
    DAT_fff847dc = DAT_fff847dc | 0x800;
  }
  else if ((ECU_State_Flags & 0x11) == 0) {
    DAT_fff847dc = DAT_fff847dc | 0x800;
  }
  (*in_tbr)();
  DAT_fff86bfc = DAT_fff86bfc & 0xffa7 | extraout_r1 & 0x58;
  (*(in_tbr + 0x10))();
  DAT_fff844d8 = DAT_fff84b1e;
  (*(in_tbr + 0xa0))(&UINT_0004d240);
  (*(in_tbr + 0x90))(&PTR_ARRAY_0001ef05_4347__0004901a);
  (*in_tbr)();
  DAT_fff86b9e = extraout_r1_00;
  (*(in_tbr + 0x10))();
  DAT_fff86b90 = DAT_fff86b8e;
  return;
}

// ─── N5_delay_synchro_FUN_000c3388 @ 0x000c3388 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N5_delay_synchro_FUN_000c3388()

void N5_delay_synchro_FUN_000c3388(void)

{
  code *in_tbr;
  
  N5N1_FUN_000bedca(1);
  N5N2_FUN_000bee3a();
  do {
    N5N1_FUN_000bedca(0);
    N5N3_FUN_000bea40();
    if (DAT_fff8662c == 0) break;
  } while (DAT_fff8662c < 9);
  (*in_tbr)();
  DAT_fff8662c = 8;
  (*(in_tbr + 0x10))();
  return;
}

// ─── N13_INIT_FUN_0008a398 @ 0x0008a398 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N13_INIT_FUN_0008a398()

void N13_INIT_FUN_0008a398(void)

{
  N13N1_MAIN_DISP_FUN_0008a418();
  N13N2_FUN_0008b8fc();
  N13N3_FUN_0008b956();
  N13N4_FUN_0008bb54();
  N13N5_FUN_0008bc0a();
  N13N6_FUN_0008bd84();
  N13N7_FUN_0008bde6();
  N13N8_FUN_0008be16();
  N13N9_FUN_0008be46();
  N13N10__Immobilizer_FuelLimit_Init_FUN_0008c532();
  N13N11_DTC_INIT_FUN_0008c44c();
  return;
}

// ─── N14_CALL_TBRs_FUN_0008c848 @ 0x0008c848 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N14_CALL_TBRs_FUN_0008c848()

void N14_CALL_TBRs_FUN_0008c848(void)

{
  N14N1_FUN_0008c888();
  N14N2_FUN_0008cd68();
  DAT_fff849d6 = N14N3_FUN_0008e134(DAT_fff849ae);
  N14N4_FUN_0008e148();
  N14N5_FUN_0008e17c();
  return;
}

// ─── N4_init_Status_Register_FUN_00000baa @ 0x00000baa (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N4_init_Status_Register_FUN_00000baa()

uint N4_init_Status_Register_FUN_00000baa(uint param_1)

{
  bool bVar1;
  uint in_sr;
  
  bVar1 = param_1 < 0x10;
  if (!bVar1) {
    param_1 = 0xf;
  }
  return in_sr & 0xffffff0e | (uint)bVar1 | (param_1 & 0xf) << 4;
}

// ─── N7_copy_init_config_FUN_000a9270 @ 0x000a9270 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N7_copy_init_config_FUN_000a9270()

void N7_copy_init_config_FUN_000a9270(void)

{
  DAT_fff85318 = 0x1212;
  DAT_fff8531a = 0;
  DAT_fff8531c = 0;
  N7N1_FUN_000c015c(0);
  if (((IMMO_ANOTHER_FLAGS_DAT_fff88f60 & 0x4001) == 0x4001) &&
     (((IMMO_ANOTHER_FLAGS_DAT_fff88f62 & 0x40) != 0 ||
      ((((uint)(int)(short)IMMO_ANOTHER_FLAGS_DAT_fff88f62 >> 2 & 0x80) != 0 &&
       (DAT_fff8477e < 0x66)))))) {
    N7N2_FUN_000c10c8(0);
  }
  else if ((IMMO_ANOTHER_FLAGS_DAT_fff88f60 & 3) == 3) {
    N7N3_FUN_000c0814(0,0);
  }
  else if ((DAT_fff8477e < 0x66) && ((IMMO_ANOTHER_FLAGS_DAT_fff88f60 & 0x21) == 0x21)) {
    N7N4_FUN_000c0b04(0);
  }
  if ((IMMO_ANOTHER_FLAGS_DAT_fff88f60 & 0xe) != 0) {
    DAT_fff86618 = (ushort)DAT_fff88e82;
    DAT_fff8661a = (ushort)DAT_fff88e83;
    DAT_fff8661c = (ushort)DAT_fff88e84;
    DAT_fff8661e = (ushort)DAT_fff88e85;
    DAT_fff86620 = (ushort)DAT_fff88e86;
    DAT_fff86622 = (ushort)DAT_fff88e87;
    DAT_fff8660c = (ushort)DAT_fff88e8e;
    DAT_fff8660e = (ushort)ERROR_ACK_REG;
    DAT_fff86610 = (ushort)DAT_fff88e90;
    DAT_fff86612 = (ushort)DAT_fff88e91;
    DAT_fff86614 = (ushort)DAT_fff88e92;
    DAT_fff86616 = (ushort)DAT_fff88e93;
  }
  return;
}

// ─── N16_IMMO_FUEL_LAMBDA_FUN_0009c420 @ 0x0009c420 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N16_IMMO_FUEL_LAMBDA_FUN_0009c420()

void N16_IMMO_FUEL_LAMBDA_FUN_0009c420(void)

{
  FUN_0009c4e8();
  FUN_0009c51a();
  FUN_0009c56a();
  DIAG_Security_Dispatch_Handler_FUN_0009c59a();
  FUN_000a1fbe();
  FUN_0006bf4c();
  FUN_0006be22();
  FUN_000a2118();
  A8_FUN_000a2940();
  A7_FUN_000a2d70();
  FUN_000a340c();
  FUN_0008f59c();
  FUN_000a5130();
  FUN_000a56e0();
  FUN_000a537c();
  A7_FUN_000a5558();
  C3_FUN_000a779c();
  FUN_000a5790();
  C1_FUN_000a5fd8();
  MAIN_SYNCRO_FUN_000a8188();
  MON_FLAGS_FUN_000a6324();
  return;
}

// ─── N9_other_FUN_0006cb72 @ 0x0006cb72 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N9_other_FUN_0006cb72()

void N9_other_FUN_0006cb72(void)

{
  N9N1_FUN_0006cb8a();
  N9N2_FUN_0006e698();
  N9N3_C6_FUN_0006ccc8();
  return;
}

// ─── N2_init_RAM_FUN_000befc8 @ 0x000befc8 (вызывается из callee_of_M2_MAIN_app_FUN_000a9148) ───
// Сигнатура: undefined N2_init_RAM_FUN_000befc8()

void N2_init_RAM_FUN_000befc8(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int in_tbr;
  
  for (puVar2 = &DAT_fff8f000; puVar2 < ARRAY_fff8f008 + 0xf78; puVar2 = puVar2 + 1) {
    *puVar2 = 0xaaaaaaaa;
  }
  DAT_fff86698 = &DAT_fff8f000;
  DAT_fff8669c = 0;
  uVar1 = UTIL_Saturating_Sub_FUN_000322be(0xfff90000,&DAT_fff8f000);
  DAT_fff8669e = (*(code *)(in_tbr + 0x60))(uVar1);
  return;
}

// ─── B_FUN_0008638e @ 0x0008638e (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_0008638e()

void B_FUN_0008638e(void)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  int extraout_r1;
  int extraout_r1_00;
  int iVar4;
  int extraout_r2;
  uint uVar5;
  
  if ((DAT_fff8588a & 0x40) == 0) {
    DAT_fff8588a = DAT_fff8588a & 0xffdf;
  }
  else {
    DAT_fff8588a = DAT_fff8588a | 0x20;
  }
  if ((DAT_fff854e6 & 8) == 0) {
    DAT_fff8588a = DAT_fff8588a & 0xffbf;
  }
  else {
    DAT_fff8588a = DAT_fff8588a | 0x40;
  }
  uVar1 = DAT_fff85864;
  if ((((uint)(int)(short)DAT_fff85864 >> 2 & 0x80) != 0) && (DAT_fff8419c == 0)) {
    if (DAT_fff85890 != DAT_fff83284) {
      uVar2 = DAT_fff80d0e + 1;
      if ((ushort)(DAT_fff80d0e + 1) == 0) {
        uVar2 = DAT_fff80d0e;
      }
      DAT_fff80d0e = uVar2;
      DAT_fff85890 = DAT_fff83284;
    }
    if ((((DAT_fff82006 & 8) != 0) && ((DAT_fff82006 & 0x10) != 0)) &&
       ((((uint)(int)(short)(DAT_fff85864 & 0xfdff) >> 1 & 0x80) != 0 ||
        (((DAT_fff80d10 & 1) != 0 || (uVar1 = DAT_fff85864 & 0xfdff, 9 < DAT_fff80d0e)))))) {
      DAT_fff85864 = DAT_fff85864 & 0xfcff;
      DAT_fff80d10 = DAT_fff80d10 & 0xfffe;
      if (9 < DAT_fff80d0e) {
        DAT_fff80d0e = 0;
      }
      B_FUN_0008654c();
      DAT_fff8588a = DAT_fff8588a | 1;
      uVar1 = DAT_fff85864;
    }
  }
  DAT_fff85864 = uVar1;
  if (((DAT_fff8588a & 1) != 0) && (iVar3 = FUN_000009ec(), iVar3 != 5)) {
    DAT_fff8588a = DAT_fff8588a & 0xfffe;
  }
  if (199 < DAT_fff84040) {
    DAT_fff84040 = 0;
  }
  FUN_000865b4();
  FUN_00087514();
  B1_FUN_00086b98();
  iVar4 = 0;
  iVar3 = extraout_r1;
  do {
    uVar1 = *(ushort *)((int)&PTR_UINT_0004b678 + iVar4);
    uVar5 = uVar1 & 0xff;
    if ((uVar1 & 0xff) == 0) {
      if ((int)uVar5 < -0x1f) {
        uVar5 = 0;
      }
      else {
        uVar5 = 1 >> -uVar5;
      }
    }
    else {
      uVar5 = 1 << (uVar1 & 0x1f);
    }
    if ((uVar5 & DAT_fff82000) != 0) {
      FUN_000875f0((ushort *)((int)&PTR_UINT_0004b678 + iVar4));
      iVar3 = extraout_r1_00;
      iVar4 = extraout_r2;
    }
    iVar3 = iVar3 + -1;
// ... (обрезано, ещё 12 строк. Используйте: /run python fetch_func.py B_FUN_0008638e)

// ─── B_FUN_00088c50 @ 0x00088c50 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_00088c50()

void B_FUN_00088c50(void)

{
  bool bVar1;
  
  if ((ECU_State_Flags & 0x10) == 0) {
    DAT_fff84e78 = DAT_fff84e78 | 0x4000;
  }
  bVar1 = false;
  if ((((((DAT_fff806e4 & 0xdef8) != 0) || ((DAT_fff806e6 & 0xbd00) != 0)) ||
       ((DAT_fff80714 & 0xdef8) != 0)) ||
      (((DAT_fff80716 & 0xbd00) != 0 || ((DAT_fff806fc & 0xdef8) != 0)))) ||
     ((DAT_fff806fe & 0xbd00) != 0)) {
    bVar1 = true;
  }
  if ((DAT_fff8419c == 0) ||
     ((bVar1 && (((ECU_State_Flags & 0x10) == 0 ||
                 (((uint)(int)(short)DAT_fff84e78 >> 8 & 0x40) != 0)))))) {
    DAT_fff84e78 = DAT_fff84e78 & 0xdfff;
  }
  else {
    DAT_fff84e78 = DAT_fff84e78 | 0x2000;
  }
  DAT_fff8587c = 0xffff;
  return;
}

// ─── MAIN_Sys_Init_Pending_FUN_00071ac0 @ 0x00071ac0 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined MAIN_Sys_Init_Pending_FUN_00071ac0()

void MAIN_Sys_Init_Pending_FUN_00071ac0(void)

{
  ushort extraout_r1;
  ushort extraout_r1_00;
  ushort extraout_r1_01;
  ushort uVar1;
  ushort extraout_r2;
  ushort extraout_r2_00;
  ushort extraout_r2_01;
  code *in_tbr;
  
  (*in_tbr)();
  if ((((Sys_Control_Flags & 0x80) != 0) || ((Ignition_Cycle_State & 0x80) != 0)) ||
     (uVar1 = extraout_r1, (Diagnostic_Session_Flags & 0x40) != 0)) {
    DAT_fff8589a = 0;
    DAT_fff89054 = 0;
    Subsystem_Enable_Registers = Subsystem_Enable_Registers & 0x3fff;
    DAT_fff84ec2 = DAT_fff84ec2 & 0x3fff;
    FUN_0007ff28();
    if ((DAT_fff84fba & 0x80) == 0) {
      DAT_fff844b8 = 10;
      DAT_fff844b6 = 10;
      DAT_fff844b2 = 10;
      DAT_fff844b0 = 10;
      DAT_fff844ae = 10;
      DAT_fff844ac = 10;
      DAT_fff844aa = 10;
      DAT_fff844a8 = 10;
      DAT_fff844a6 = 10;
      DAT_fff844a4 = 10;
      DAT_fff844a2 = 10;
      DAT_fff844bc = 10;
      DAT_fff844ba = 10;
      Param_Default_Array = 10;
      DAT_fff8449e = 10;
      DAT_fff8449c = 10;
      DAT_fff8449a = 10;
      DAT_fff84498 = 10;
      DAT_fff84496 = 10;
      DAT_fff84494 = 10;
      DAT_fff84492 = 10;
      DAT_fff84490 = 10;
      DAT_fff8448e = 10;
      DAT_fff8448c = 10;
      DAT_fff8448a = 10;
      DAT_fff84488 = 10;
      DAT_fff84486 = 10;
      DAT_fff84484 = 10;
      DAT_fff844b4 = 6;
    }
    else {
      DAT_fff844b8 = 2;
      DAT_fff844b6 = 2;
      DAT_fff844b4 = 2;
      DAT_fff844b2 = 2;
      DAT_fff844b0 = 2;
      DAT_fff844ae = 2;
      DAT_fff844ac = 2;
      DAT_fff844aa = 2;
      DAT_fff844a8 = 2;
      DAT_fff844a6 = 2;
      DAT_fff844a4 = 2;
      DAT_fff844a2 = 2;
      DAT_fff844bc = 2;
      DAT_fff844ba = 2;
      Param_Default_Array = 2;
      DAT_fff8449e = 2;
      DAT_fff8449c = 2;
      DAT_fff8449a = 2;
      DAT_fff84498 = 2;
      DAT_fff84496 = 2;
      DAT_fff84494 = 2;
      DAT_fff84492 = 2;
      DAT_fff84490 = 2;
      DAT_fff8448e = 2;
      DAT_fff8448c = 2;
      DAT_fff8448a = 2;
      DAT_fff84488 = 2;
// ... (обрезано, ещё 321 строк. Используйте: /run python fetch_func.py MAIN_Sys_Init_Pending_FUN_00071ac0)

// ─── IMMO_FLAGS_CONF_DAT_FUN_00080324 @ 0x00080324 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined IMMO_FLAGS_CONF_DAT_FUN_00080324()

void IMMO_FLAGS_CONF_DAT_FUN_00080324(void)

{
  if ((((IMMO_FLAGS_CONF_DAT_fff84540 & 2) == 0) && (DAT_fff8419c == 0)) &&
     (((uint)(int)Diagnostic_Session_Flags >> 8 & 0x80) != 0)) {
    B_FUN_00080394();
    B_FUN_00081f38();
    B_FUN_0008309c();
    B_FUN_00083b24();
    B_FUN_00083b9a();
    B_FUN_00083e32();
  }
  B_FUN_00084128();
  return;
}

// ─── B_FUN_00062c9c @ 0x00062c9c (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_00062c9c()

void B_FUN_00062c9c(void)

{
  code *in_tbr;
  
  (*in_tbr)();
  if ((DAT_fff85550 & 0x1001) == 0) {
    DAT_fff847bc = DAT_fff847bc & 0xfffe;
  }
  else {
    DAT_fff847bc = DAT_fff847bc | 1;
  }
  (*(in_tbr + 0x10))();
  return;
}

// ─── B_CAN_IMMO_DAT_FUN_000883a0 @ 0x000883a0 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_CAN_IMMO_DAT_FUN_000883a0()

void B_CAN_IMMO_DAT_FUN_000883a0(void)

{
  if ((Diagnostic_Session_Flags & 0x20) == 0) {
    DAT_fff844c2 = 0x14;
    DAT_fff84ec4 = 5;
    if ((((uint)(int)CAN_IMMO_DAT_fff847ca >> 8 & 0x80) == 0) &&
       (((uint)(int)CAN_IMMO_DAT_fff847ca >> 8 & 0x40) != 0)) {
      DAT_fff84ec4 = 4;
      Diagnostic_Session_Flags = Diagnostic_Session_Flags | 0x20;
    }
  }
  else if (DAT_fff844c2 == 0) {
    DAT_fff844c2 = 0x14;
    DAT_fff84ec4 = 5;
    Diagnostic_Session_Flags = Diagnostic_Session_Flags & 0xffdf;
  }
  else {
    if (((((uint)(int)CAN_IMMO_DAT_fff847ca >> 8 & 0x80) == 0) &&
        (((uint)(int)CAN_IMMO_DAT_fff847ca >> 8 & 0x40) != 0)) && (DAT_fff84ec4 != 0)) {
      DAT_fff84ec4 = DAT_fff84ec4 + -1;
    }
    if (DAT_fff84ec4 == 0) {
      Diagnostic_Session_Flags = Diagnostic_Session_Flags & 0xffdf | 0x40;
    }
  }
  return;
}

// ─── B_FUN_00088286 @ 0x00088286 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_00088286()

undefined8 B_FUN_00088286(void)

{
  int iVar1;
  undefined4 in_r1;
  uint extraout_r2;
  uint extraout_r2_00;
  short sVar2;
  int in_tbr;
  undefined8 uVar3;
  
  (*(code *)(in_tbr + 0x30))(DAT_fff84562,DAT_fff84568);
  if ((extraout_r2 & (int)(short)Diagnostic_Session_Flags) == 0) {
    if (0x6e < DAT_fff84562) {
      Diagnostic_Session_Flags = Diagnostic_Session_Flags | (ushort)extraout_r2;
    }
  }
  else if (DAT_fff84562 < 0x6e) {
    Diagnostic_Session_Flags = Diagnostic_Session_Flags & 0xdfff;
  }
  if (DAT_fff8419c == 0) {
    if (((uint)(int)(short)Diagnostic_Session_Flags >> 8 & 0x80) != 0) {
      FUN_000bb0c0();
    }
    Diagnostic_Session_Flags = Diagnostic_Session_Flags & 0x3fff;
    iVar1 = (int)(short)Diagnostic_Session_Flags;
  }
  else {
    if (((uint)(int)DAT_fff847ea >> 2 & 0x80) != 0) {
      Diagnostic_Session_Flags = Diagnostic_Session_Flags | 0x8000;
    }
    uVar3 = FUN_000ba7c6();
    iVar1 = (int)uVar3;
    if ((0x15 < ((uint)((ulonglong)uVar3 >> 0x20) & 0xffff)) &&
       ((extraout_r2_00 & Diagnostic_Session_Flags) != 0)) {
      if (((uint)(int)(short)Diagnostic_Session_Flags >> 8 & 0x40) == 0) {
        sVar2 = DAT_fff80b36 + 1;
        if (sVar2 == 0) {
          sVar2 = -1;
        }
        DAT_fff80b36 = (*(code *)(in_tbr + 0x50))(sVar2);
      }
      Diagnostic_Session_Flags = Diagnostic_Session_Flags | 0x4000;
      iVar1 = (int)(short)Diagnostic_Session_Flags;
    }
  }
  return CONCAT44(in_r1,iVar1);
}

// ─── B_FUN_00071970 @ 0x00071970 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_00071970()

void B_FUN_00071970(void)

{
  ushort *puVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  ushort uVar7;
  ushort uVar8;
  
  uVar7 = 0;
  iVar4 = 0xc;
  iVar3 = 0;
  do {
    iVar4 = iVar4 + -1;
    puVar1 = (ushort *)((int)&DAT_fff806f0 + iVar3);
    iVar3 = iVar3 + 2;
    uVar7 = uVar7 | *puVar1;
  } while (iVar4 != 0);
  if (uVar7 == 0) {
    DAT_fff8476a = 0;
    DAT_fff8476e = 0;
  }
  else if ((DAT_fff847c2 & 0x10) != 0) {
    do {
      uVar8 = DAT_fff8476e & 0xf;
      uVar7 = DAT_fff8476e >> 4;
      uVar5 = (uint)(byte)uVar8;
      uVar6 = -uVar5;
      if ((int)uVar6 < 1) {
        if ((int)uVar6 < -0x1f) {
          uVar5 = 0;
        }
        else {
          uVar5 = (uint)((ushort)(&DAT_fff806f0)[uVar7] >> uVar5);
        }
      }
      else {
        uVar5 = (uint)(ushort)(&DAT_fff806f0)[uVar7] << (uVar6 & 0x1f);
      }
      uVar2 = DAT_fff8476e + 1;
      if ((ushort)(DAT_fff8476e + 1) == 0) {
        uVar2 = DAT_fff8476e;
      }
      DAT_fff8476e = uVar2;
      if (0xbf < DAT_fff8476e) {
        DAT_fff8476e = 0;
      }
    } while ((uVar5 & 1) == 0);
    DAT_fff8476a = (&DAT_0004b376)[(uint)uVar7 * 0x10 + (uint)uVar8];
  }
  uVar7 = 0;
  iVar4 = 0xc;
  iVar3 = 0;
  do {
    iVar4 = iVar4 + -1;
    puVar1 = (ushort *)((int)&DAT_fff80708 + iVar3);
    iVar3 = iVar3 + 2;
    uVar7 = uVar7 | *puVar1;
  } while (iVar4 != 0);
  if (uVar7 == 0) {
    DAT_fff8476c = 0;
    DAT_fff84770 = 0;
  }
  else if ((DAT_fff847c2 & 0x10) != 0) {
    do {
      uVar8 = DAT_fff84770 & 0xf;
      uVar7 = DAT_fff84770 >> 4;
      uVar5 = (uint)(byte)uVar8;
      uVar6 = -uVar5;
      if ((int)uVar6 < 1) {
        if ((int)uVar6 < -0x1f) {
          uVar5 = 0;
        }
        else {
          uVar5 = (uint)((ushort)(&DAT_fff80708)[uVar7] >> uVar5);
        }
// ... (обрезано, ещё 17 строк. Используйте: /run python fetch_func.py B_FUN_00071970)

// ─── ECU_State_Flags_FUN_00072dae @ 0x00072dae (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined ECU_State_Flags_FUN_00072dae()

void ECU_State_Flags_FUN_00072dae(void)

{
  ushort uVar1;
  
  if (DAT_fff84018 < 0x15) {
    if (((((ECU_State_Flags & 0x10) == 0) || (4 < DAT_fff84556)) || (4 < DAT_fff8456a)) ||
       (uVar1 = DAT_fff84fba, DAT_fff8459c < 0xf6)) {
      uVar1 = DAT_fff84fba & 0xffbf;
    }
  }
  else {
    uVar1 = DAT_fff84fba & 0xffbe;
    if (DAT_fff8459c < 0xb1) {
      uVar1 = DAT_fff84fba & 0xff3e;
    }
  }
  DAT_fff84fba = uVar1 & 0xff7f;
  return;
}

// ─── B_FUN_00088254 @ 0x00088254 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined B_FUN_00088254()

void B_FUN_00088254(void)

{
  if (DAT_fff8419c == 0) {
    DAT_fff8522c = DAT_fff8522c & 0xf87f;
  }
  return;
}

// ─── DAT_INIT_COPY_FUN_00072ec8 @ 0x00072ec8 (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined DAT_INIT_COPY_FUN_00072ec8()

void DAT_INIT_COPY_FUN_00072ec8(void)

{
  if (DAT_fff8419c == 0) {
    DatIntCpy_FUN_00071734();
  }
  DatIntCpy_FUN_000744bc();
  DatIntCpy_FUN_00073290();
  DatIntCpy_FUN_00073374();
  DatIntCpy_FUN_00073458();
  DatIntCpy_FUN_0007345a();
  DatIntCpy_FUN_0007345c();
  DatIntCpy_FUN_00073560();
  DatIntCpy_FUN_0007363e();
  B1_FUN_00073734();
  B1_FUN_0007380c();
  B1_FUN_000738e4();
  B1_FUN_000739c4();
  FUN_00073a9c();
  FUN_00073b68();
  FUN_00073c34();
  FUN_00073d72();
  FUN_00073eaa();
  FUN_00073f82();
  FUN_0007408c();
  FUN_000740e6();
  FUN_00074140();
  FUN_00074234();
  FUN_00074354();
  FUN_000743ae();
  FUN_00074408();
  FUN_00074462();
  FUN_000791a6();
  FUN_00079200();
  FUN_000749a0();
  FUN_00089490();
  FUN_00074afe();
  FUN_00075e92();
  FUN_00076a40();
  FUN_00076b32();
  FUN_00076cb4();
  FUN_00076e20();
  FUN_00076f30();
  FUN_00077138();
  FUN_00077aae();
  FUN_00078014();
  FUN_00078608();
  FUN_00078634();
  FUN_000786d0();
  FUN_0007a4be();
  FUN_0007a5d0();
  FUN_00078738();
  FUN_00078890();
  FUN_00078960();
  FUN_00078a5c();
  FUN_00078b32();
  FUN_00078bee();
  FUN_00078cc2();
  FUN_00078cce();
  FUN_00078e9a();
  FUN_00079034();
  FUN_0007925a();
  FUN_0007a05c();
  FUN_0007a328();
  FUN_0007a3f0();
  FUN_0007a6b2();
  FUN_0007aba2();
  FUN_0007ac5c();
  A93_FUN_0007be54();
  FUN_0007cb98();
  FUN_0007dec0();
  FUN_0007dfe0();
  FUN_0007e0c6();
  FUN_0007e120();
  FUN_0007e1d4();
  FUN_0007e310();
  FUN_0007a6fe();
  FUN_0007a7d4();
  FUN_0007a8ac();
// ... (обрезано, ещё 15 строк. Используйте: /run python fetch_func.py DAT_INIT_COPY_FUN_00072ec8)

// ─── IMMO_FLAGS_decrypt_FUN_00072d2c @ 0x00072d2c (вызывается из callee_of_N18_Math_Util_Dtc_set_flags_FUN_000718ea) ───
// Сигнатура: undefined IMMO_FLAGS_decrypt_FUN_00072d2c()

void IMMO_FLAGS_decrypt_FUN_00072d2c(void)

{
  undefined2 uVar1;
  ushort uVar2;
  uint extraout_r2;
  int in_tbr;
  undefined8 uVar3;
  
  if ((IMMO_FLAGS_CONF_DAT_fff84540 & 2) == 0) {
    DAT_fff845f4 = UTIL_Decrypt_FUN_00031bf6(DAT_fff845c4);
    DAT_fff842bc = 0;
  }
  else {
    if ((DAT_fff847c2 & 1) != 0) {
      uVar3 = UTIL_Decrypt_FUN_00031bf6(DAT_fff845c4);
      DAT_fff845f4 = UTIL_Lerp_U32_Q8_FUN_00031e60((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0x12);
    }
    uVar1 = UTIL_Q16_16_Round_Sat_FUN_00031c18(DAT_fff845f4);
    uVar2 = (*(code *)(in_tbr + 0x30))(uVar1,extraout_r2 & 0xffff);
    if (0xb < uVar2) {
      DAT_fff842bc = 0x14;
    }
  }
  return;
}

// ═══════════════════════════════════════════════════════════
// СЕКЦИЯ 3: РЕГИСТРЫ ДАННЫХ, ИСПОЛЬЗУЕМЫЕ НА ПУТИ
// ═══════════════════════════════════════════════════════════

// DAT_fff8dffa @ 0xfff8dffa | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff8dffa = 0xc1b0;

// DAT_fff8dffc @ 0xfff8dffc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff8dffc = 0;

// DAT_fff8dffe @ 0xfff8dffe | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff8dffe = 0;

// DAT_fffff220 @ 0xfffff220 | 4 байт | uint | [read, unknown, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: sVar1 = (short)(DAT_fffff220 >> 5);

// DAT_fff84524 @ 0xfff84524 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff84524 = sVar1 - DAT_fff84526;

// DAT_fff84526 @ 0xfff84526 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff84524 = sVar1 - DAT_fff84526;
//   Пример: DAT_fff84526 = sVar1;

// DAT_fff85218 @ 0xfff85218 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: M2_MAIN_app_FUN_000a9148
//   Пример: DAT_fff85218 = DAT_fff85218 | 0x8000;

// DAT_fff84ed4 @ 0xfff84ed4 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if ((DAT_fff84ed4 & 0x40) != 0) {
//   Пример: DAT_fff84ed4 = DAT_fff84ed4 & 0xffbf;
//   Пример: DAT_fff84ed4 = DAT_fff84ed4 | 0x40;

// DAT_fff8419c @ 0xfff8419c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if ((((DAT_fff8419c == 0) || ((DAT_fff853b8 & 4) == 0)) ||

// DAT_fff853b8 @ 0xfff853b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if ((((DAT_fff8419c == 0) || ((DAT_fff853b8 & 4) == 0)) ||

// DAT_fff8d812 @ 0xfff8d812 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((uint)(int)DAT_fff8d812 >> 8 & 0x40) == 0)) || ((DAT_fff8d810 & 4) != 0)) {

// DAT_fff8d810 @ 0xfff8d810 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((uint)(int)DAT_fff8d812 >> 8 & 0x40) == 0)) || ((DAT_fff8d810 & 4) != 0)) {

// DAT_fff84ed8 @ 0xfff84ed8 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: DAT_fff84ed8 = DAT_fff84ed8 & 0xeffd;
//   Пример: DAT_fff84ed8 = DAT_fff84ed8 | 0x1000;
//   Пример: else if ((DAT_fff84ed8 & 2) == 0) goto LAB_000802fa;

// DAT_fff8432a @ 0xfff8432a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if (DAT_fff8432a == 0) {
//   Пример: DAT_fff8432a = 0x28;
//   Пример: else if (DAT_fff8432a < 0x14) {

// DAT_fff80720 @ 0xfff80720 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if (((((DAT_fff80720 & *extraout_r1) == 0) && ((DAT_fff80722 & extraout_r1[1]) == 0)) &&

// DAT_fff80722 @ 0xfff80722 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if (((((DAT_fff80720 & *extraout_r1) == 0) && ((DAT_fff80722 & extraout_r1[1]) == 0)) &&

// DAT_fff80724 @ 0xfff80724 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff80724 & extraout_r1[2]) == 0 &&

// DAT_fff80726 @ 0xfff80726 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff80726 & extraout_r1[3]) == 0 && ((DAT_fff80728 & extraout_r1[4]) == 0)))))) &&

// DAT_fff80728 @ 0xfff80728 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff80726 & extraout_r1[3]) == 0 && ((DAT_fff80728 & extraout_r1[4]) == 0)))))) &&

// DAT_fff8072a @ 0xfff8072a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff8072a & extraout_r1[5]) == 0 &&

// DAT_fff8072c @ 0xfff8072c | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: ((((((DAT_fff8072c & extraout_r1[6]) == 0 && ((DAT_fff8072e & extraout_r1[7]) == 0)) &&

// DAT_fff8072e @ 0xfff8072e | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: ((((((DAT_fff8072c & extraout_r1[6]) == 0 && ((DAT_fff8072e & extraout_r1[7]) == 0)) &&

// DAT_fff80730 @ 0xfff80730 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: ((DAT_fff80730 & extraout_r1[8]) == 0)) &&

// DAT_fff80732 @ 0xfff80732 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff80732 & extraout_r1[9]) == 0 && ((DAT_fff80734 & extraout_r1[10]) == 0)))) &&

// DAT_fff80734 @ 0xfff80734 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((DAT_fff80732 & extraout_r1[9]) == 0 && ((DAT_fff80734 & extraout_r1[10]) == 0)))) &&

// DAT_fff80736 @ 0xfff80736 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: ((DAT_fff80736 & extraout_r1[0xb]) == 0)))))) {

// DAT_fff80784 @ 0xfff80784 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: if ((extraout_r2 & DAT_fff80784) == 0) {

// DAT_fff847ea @ 0xfff847ea | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: (((uint)(int)DAT_fff847ea >> 2 & 0x80) != 0)) goto LAB_000802fa;

// DAT_fff84532 @ 0xfff84532 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_000800f0
//   Пример: DAT_fff84532 = DAT_fff84532 & 0xfff7;
//   Пример: DAT_fff84532 = DAT_fff84532 | 8;

// DAT_fff84ed6 @ 0xfff84ed6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ed6 = DAT_fff84ed6 & 0xffc0;
//   Пример: if (((DAT_fff84ed4 & 0x50be) == 0) && ((DAT_fff84ed6 & 0x3e) == 0)) {
//   Пример: if ((((uint)(int)(short)DAT_fff84ed4 >> 8 & 0xac) == 0) && ((DAT_fff84ed6 & 1) == 0)) {

// DAT_fff84ee0 @ 0xfff84ee0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ee0 = 0x3c;

// DAT_fff84edc @ 0xfff84edc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84edc = 0;

// DAT_fff84ede @ 0xfff84ede | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ede = 0;

// DAT_fff84ee2 @ 0xfff84ee2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ee2 = 0;

// DAT_fff84ee4 @ 0xfff84ee4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ee4 = 0;

// DAT_fff84ee6 @ 0xfff84ee6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ee6 = 0;

// DAT_fff84ee8 @ 0xfff84ee8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84ee8 = 0;

// DAT_fff84eea @ 0xfff84eea | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84eea = 0;

// DAT_fff84eec @ 0xfff84eec | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Immo_StateSyncAndCleanup_FUN_000b4dd8
//   Пример: DAT_fff84eec = 0;

// DAT_fffe3882 @ 0xfffe3882 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: O_I_Polling_Timer_Setup_FUN_00005f40
//   Пример: DAT_fffe3882 = DAT_fffe3882 | 0x40;

// DAT_fffe3886 @ 0xfffe3886 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: O_I_Polling_Timer_Setup_FUN_00005f40
//   Пример: DAT_fffe3886._0_1_ = DAT_fffe3886._0_1_ | 0x40;

// DAT_fff800cc @ 0xfff800cc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: Watchdog_init_FUN_00000b00
//   Пример: DAT_fff800cc = 0;

// DAT_fffff002 @ 0xfffff002 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff002 = 0;

// DAT_fffff003 @ 0xfffff003 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff003 = 0;

// DAT_fffff100 @ 0xfffff100 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff100 = 4;

// DAT_fffff102 @ 0xfffff102 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff102 = 0x27;

// DAT_fffff104 @ 0xfffff104 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff104 = 0x9f;

// DAT_fffff106 @ 0xfffff106 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff106 = 199;

// DAT_fffff202 @ 0xfffff202 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff202 = 0;

// DAT_fffff000 @ 0xfffff000 | 1 байт | byte | [read, write] | peripheral_mirror
//   Используется в: init_hardwareFUN_00005de8
//   Пример: DAT_fffff000 = 3;

// DAT_fff8df90 @ 0xfff8df90 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: Watchdog_Hardware_Poll_Debounce_FUN_00005f6e
//   Пример: if ((((DAT_fffff220 >> 5) - (int)DAT_fff8df90 & 0xffff) >> 8 & 0x80) == 0) {
//   Пример: DAT_fff8df90 = DAT_fff8d800;

// DAT_fff8d800 @ 0xfff8d800 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: Watchdog_Hardware_Poll_Debounce_FUN_00005f6e
//   Пример: DAT_fff8d800 = (short)(DAT_fffff220 >> 5) + 0x2ee;
//   Пример: DAT_fff8df90 = DAT_fff8d800;

// DAT_ffffc861 @ 0xffffc861 | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: Watchdog_Hardware_Poll_Debounce_FUN_00005f6e
//   Пример: if ((DAT_ffffc861 & 0x10) == 0) {

// DAT_fff8d808 @ 0xfff8d808 | 4 байт | uint | [write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d808 = 0;

// DAT_fff8001e @ 0xfff8001e | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: if (((DAT_fff8001e & 0x80) == 0) && ((DAT_fff8001e & 0x40) != 0)) {

// DAT_fff80068 @ 0xfff80068 | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: *(ushort *)((int)&DAT_fff80068 + iVar1) = (ushort)*(byte *)(extraout_r1 + uVar5);

// DAT_fff800a6 @ 0xfff800a6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff800a6 = uVar2 & 0xff;

// DAT_fff8008a @ 0xfff8008a | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: *(ushort *)((int)&DAT_fff8008a + iVar1) = (ushort)*(byte *)(extraout_r1 + 0x11 + uVar5);

// DAT_fff800a8 @ 0xfff800a8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff800a8 = uVar3 & 0xff;

// DAT_fff8009e @ 0xfff8009e | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: *(ushort *)((int)&DAT_fff8009e + iVar1) = (ushort)*(byte *)(extraout_r1 + 0x1b + uVar5);

// DAT_fff800aa @ 0xfff800aa | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff800aa = uVar2 & 0xff;

// DAT_fff8de4c @ 0xfff8de4c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8de4c = DAT_fff8de4c | 0x49;
//   Пример: DAT_fff8de4c = DAT_fff8de4c & 0xffb6;

// DAT_fff8d804 @ 0xfff8d804 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d804 = 1;

// DAT_fff8d924 @ 0xfff8d924 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d924 = &DAT_00030000;

// DAT_00030000 @ 0x00030000 | 4 байт | uint | [read] | flash_rom
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d924 = &DAT_00030000;

// DAT_fff8d938 @ 0xfff8d938 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d938 = 0;

// DAT_fff8d952 @ 0xfff8d952 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: A0_DTC_INIT_FUN_0000151e
//   Пример: DAT_fff8d952 = DAT_fff8d952 & 0xffcf;

// DAT_fff80000 @ 0xfff80000 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: init_mem_clearFUN_00005ee6
//   Пример: for (puVar1 = (undefined4 *)&DAT_fff80000; puVar1 < ARRAY_fff8f008 + 0xff7; puVar1 = puVar1 + 1) {

// DAT_fff800d4 @ 0xfff800d4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: FUN_00000ba0
//   Пример: DAT_fff800d4 = 0;

// DAT_fffc1400 @ 0xfffc1400 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: CAN_init_mailboxFUN_00005efc
//   Пример: DAT_fffc1400 = 1;

// DAT_fffc1408 @ 0xfffc1408 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: CAN_init_mailboxFUN_00005efc
//   Пример: DAT_fffc1408 = 0xf5;

// DAT_ffff0800 @ 0xffff0800 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0800 = 0x96ff;

// DAT_ffff0802 @ 0xffff0802 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0802 = 0x69ff;

// DAT_ffff0804 @ 0xffff0804 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0804 = 0x7600;

// DAT_ffff0806 @ 0xffff0806 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0806 = 0;

// DAT_ffff0810 @ 0xffff0810 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0810 = 0;

// DAT_ffff0812 @ 0xffff0812 | 1 байт | char | [read, write] | peripheral_mirror
//   Используется в: init_hardware_busFUN_00005e36
//   Пример: DAT_ffff0812 = 0x7810;

// DAT_fff8664c @ 0xfff8664c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_clear_3_globals_FUN_000becdc
//   Пример: DAT_fff8664c = 0;

// DAT_fff86650 @ 0xfff86650 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_clear_3_globals_FUN_000becdc
//   Пример: DAT_fff86650 = 0;

// DAT_fff86654 @ 0xfff86654 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: M2_clear_3_globals_FUN_000becdc
//   Пример: DAT_fff86654 = 0;

// DAT_fff852b4 @ 0xfff852b4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N8_bits_check_FUN_00094942
//   Пример: DAT_fff852b4 = DAT_fff852b4 & 0xfffc;

// DAT_fff852ae @ 0xfff852ae | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N19_Subsystem_StateMachine_Handler_FUN_00089da8
//   Пример: if ((((uint)(int)(short)DAT_fff852ae >> 8 & 0x80) == 0) &&
//   Пример: (((uint)(int)(short)DAT_fff852ae >> 8 & 0x40) == 0)) {
//   Пример: if (((uint)(int)(short)DAT_fff852ae >> 8 & 0x40) != 0) {

// DAT_fff852b0 @ 0xfff852b0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N19_Subsystem_StateMachine_Handler_FUN_00089da8
//   Пример: sVar1 = DAT_fff852b0 + 1;
//   Пример: if ((short)(DAT_fff852b0 + 1) == 0) {
//   Пример: sVar1 = DAT_fff852b0;

// DAT_fff8578e @ 0xfff8578e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N22_DECODE_WRITE_DAT_FUN_000be7ce
//   Пример: uVar1 = (*(code *)(in_tbr + 0x40))(DAT_fff8578e,0x8000,0x100);

// DAT_fff852b8 @ 0xfff852b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N11_State_control_FUN_00091dcc
//   Пример: DAT_fff852b8 = DAT_fff84014;

// DAT_fff84014 @ 0xfff84014 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N11_State_control_FUN_00091dcc
//   Пример: DAT_fff852b8 = DAT_fff84014;

// DAT_fff852f4 @ 0xfff852f4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N11_State_control_FUN_00091dcc
//   Пример: DAT_fff852f4 = DAT_fff84016;

// DAT_fff84016 @ 0xfff84016 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N11_State_control_FUN_00091dcc
//   Пример: DAT_fff852f4 = DAT_fff84016;

// DAT_fff803f2 @ 0xfff803f2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f2 = DAT_fff852a0;

// DAT_fff852a0 @ 0xfff852a0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f2 = DAT_fff852a0;

// DAT_fff803f4 @ 0xfff803f4 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f4 = DAT_fff852a2;

// DAT_fff852a2 @ 0xfff852a2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f4 = DAT_fff852a2;

// DAT_fff803f6 @ 0xfff803f6 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f6 = DAT_fff852a4;

// DAT_fff852a4 @ 0xfff852a4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803f6 = DAT_fff852a4;

// DAT_fff803e8 @ 0xfff803e8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803e8 = DAT_fff85298;

// DAT_fff85298 @ 0xfff85298 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: DAT_fff803e8 = DAT_fff85298;

// DAT_fff841a0 @ 0xfff841a0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N1_init_DTC_CALC_STORE_FUN_000949ac
//   Пример: if (DAT_fff841a0 != 0) {

// DAT_fff840a4 @ 0xfff840a4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff840a4 = 10;

// DAT_fff841c6 @ 0xfff841c6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: if ((DAT_fff841c6 == 0) || (DAT_fff8419c == 0)) {

// DAT_fff85526 @ 0xfff85526 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85526 = 0;

// DAT_fff85528 @ 0xfff85528 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85528 = 0;

// DAT_fff85506 @ 0xfff85506 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85506 = 0;

// DAT_fff8561c @ 0xfff8561c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff8561c = 0x78;

// DAT_fff85550 @ 0xfff85550 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85550 = DAT_fff85550 & 0xc3d0;

// DAT_fff85634 @ 0xfff85634 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85634 = DAT_fff85634 & 0xfbfe;

// DAT_fff854ec @ 0xfff854ec | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff854ec = DAT_fff854ec & 0xfff9;

// DAT_fff840e2 @ 0xfff840e2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff840e2 = 0x14;

// DAT_fff843c4 @ 0xfff843c4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff843c4 = 200;

// DAT_fff85544 @ 0xfff85544 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85544 = 0;

// DAT_fff85552 @ 0xfff85552 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: DAT_fff85552 = DAT_fff85552 & 0xd7ff;

// DAT_fff841c8 @ 0xfff841c8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N6_flags_init_FUN_0006000a
//   Пример: if (DAT_fff841c8 == 0) {

// DAT_fff86698 @ 0xfff86698 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N23_DAT_UTIL_FUN_000bef78
//   Пример: if (*DAT_fff86698 == -0x55555556) {
//   Пример: DAT_fff86698 = DAT_fff86698 + 1;
//   Пример: uVar1 = UTIL_Saturating_Sub_FUN_000322be(0xfff90000,DAT_fff86698);

// DAT_fff8669c @ 0xfff8669c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N23_DAT_UTIL_FUN_000bef78
//   Пример: DAT_fff8669c = (*(code *)(in_tbr + 0x60))(uVar1);

// DAT_fff8f000 @ 0xfff8f000 | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: N23_DAT_UTIL_FUN_000bef78
//   Пример: uVar1 = UTIL_Saturating_Sub_FUN_000322be(extraout_r1,&DAT_fff8f000);
//   Пример: DAT_fff86698 = &DAT_fff8f000;

// DAT_fff8669e @ 0xfff8669e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N23_DAT_UTIL_FUN_000bef78
//   Пример: DAT_fff8669e = (*(code *)(in_tbr + 0x60))(uVar1);

// DAT_ffffc830 @ 0xffffc830 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N21_FLAGS_Handler_FUN_000be6a8
//   Пример: DAT_ffffc830 = DAT_ffffc830 & 0xf7;
//   Пример: DAT_ffffc830 = DAT_ffffc830 | 8;

// DAT_ffffc820 @ 0xffffc820 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N21_FLAGS_Handler_FUN_000be6a8
//   Пример: DAT_ffffc820 = DAT_ffffc820 & 0xfd;
//   Пример: DAT_ffffc820 = DAT_ffffc820 | 2;

// DAT_fffec000 @ 0xfffec000 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N3_init_dataRegisters_FUN_000c3230
//   Пример: DAT_fffec000 = 1;

// DAT_fff86c48 @ 0xfff86c48 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N20_DTC_BODY_REMOVE__FUN_000896ca
//   Пример: if ((DAT_fff86c48 & 0x3333) != 0) {

// DAT_fff84fba @ 0xfff84fba | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N20_DTC_BODY_REMOVE__FUN_000896ca
//   Пример: if (((DAT_fff84fba & 0x80) == 0) &&

// DAT_fff847dc @ 0xfff847dc | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff847dc = DAT_fff847dc & 0xf7ff;
//   Пример: DAT_fff847dc = DAT_fff847dc | 0x800;
//   Пример: DAT_fff847dc = DAT_fff847dc | 0x800;

// DAT_fff80300 @ 0xfff80300 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: else if ((DAT_fff80300 == '\0') && (DAT_fff80301 == '\0')) {

// DAT_fff80301 @ 0xfff80301 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: else if ((DAT_fff80300 == '\0') && (DAT_fff80301 == '\0')) {

// DAT_fff86bfc @ 0xfff86bfc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff86bfc = DAT_fff86bfc & 0xffa7 | extraout_r1 & 0x58;

// DAT_fff844d8 @ 0xfff844d8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff844d8 = DAT_fff84b1e;

// DAT_fff84b1e @ 0xfff84b1e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff844d8 = DAT_fff84b1e;

// DAT_fff86b9e @ 0xfff86b9e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff86b9e = extraout_r1_00;

// DAT_fff86b90 @ 0xfff86b90 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff86b90 = DAT_fff86b8e;

// DAT_fff86b8e @ 0xfff86b8e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N15_INIT_routine_FUN_00095788
//   Пример: DAT_fff86b90 = DAT_fff86b8e;

// DAT_fff8662c @ 0xfff8662c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N5_delay_synchro_FUN_000c3388
//   Пример: if (DAT_fff8662c == 0) break;
//   Пример: } while (DAT_fff8662c < 9);
//   Пример: DAT_fff8662c = 8;

// DAT_fff849d6 @ 0xfff849d6 | 4 байт | uint | [write] | peripheral_mirror
//   Используется в: N14_CALL_TBRs_FUN_0008c848
//   Пример: DAT_fff849d6 = N14N3_FUN_0008e134(DAT_fff849ae);

// DAT_fff849ae @ 0xfff849ae | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N14_CALL_TBRs_FUN_0008c848
//   Пример: DAT_fff849d6 = N14N3_FUN_0008e134(DAT_fff849ae);

// DAT_fff85318 @ 0xfff85318 | 4 байт | uint | [write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff85318 = 0x1212;

// DAT_fff8531a @ 0xfff8531a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8531a = 0;

// DAT_fff8531c @ 0xfff8531c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8531c = 0;

// DAT_fff8477e @ 0xfff8477e | 4 байт | uint | [read, unknown, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: (DAT_fff8477e < 0x66)))))) {
//   Пример: else if ((DAT_fff8477e < 0x66) && ((IMMO_ANOTHER_FLAGS_DAT_fff88f60 & 0x21) == 0x21)) {

// DAT_fff86618 @ 0xfff86618 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86618 = (ushort)DAT_fff88e82;

// DAT_fff88e82 @ 0xfff88e82 | 2 байт | ushort | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86618 = (ushort)DAT_fff88e82;

// DAT_fff8661a @ 0xfff8661a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661a = (ushort)DAT_fff88e83;

// DAT_fff88e83 @ 0xfff88e83 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661a = (ushort)DAT_fff88e83;

// DAT_fff8661c @ 0xfff8661c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661c = (ushort)DAT_fff88e84;

// DAT_fff88e84 @ 0xfff88e84 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661c = (ushort)DAT_fff88e84;

// DAT_fff8661e @ 0xfff8661e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661e = (ushort)DAT_fff88e85;

// DAT_fff88e85 @ 0xfff88e85 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8661e = (ushort)DAT_fff88e85;

// DAT_fff86620 @ 0xfff86620 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86620 = (ushort)DAT_fff88e86;

// DAT_fff88e86 @ 0xfff88e86 | 2 байт | ushort | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86620 = (ushort)DAT_fff88e86;

// DAT_fff86622 @ 0xfff86622 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86622 = (ushort)DAT_fff88e87;

// DAT_fff88e87 @ 0xfff88e87 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86622 = (ushort)DAT_fff88e87;

// DAT_fff8660c @ 0xfff8660c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8660c = (ushort)DAT_fff88e8e;

// DAT_fff88e8e @ 0xfff88e8e | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8660c = (ushort)DAT_fff88e8e;

// DAT_fff8660e @ 0xfff8660e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff8660e = (ushort)ERROR_ACK_REG;

// DAT_fff86610 @ 0xfff86610 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86610 = (ushort)DAT_fff88e90;

// DAT_fff88e90 @ 0xfff88e90 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86610 = (ushort)DAT_fff88e90;

// DAT_fff86612 @ 0xfff86612 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86612 = (ushort)DAT_fff88e91;

// DAT_fff88e91 @ 0xfff88e91 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86612 = (ushort)DAT_fff88e91;

// DAT_fff86614 @ 0xfff86614 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86614 = (ushort)DAT_fff88e92;

// DAT_fff88e92 @ 0xfff88e92 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86614 = (ushort)DAT_fff88e92;

// DAT_fff86616 @ 0xfff86616 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86616 = (ushort)DAT_fff88e93;

// DAT_fff88e93 @ 0xfff88e93 | 2 байт | ushort | [read] | peripheral_mirror
//   Используется в: N7_copy_init_config_FUN_000a9270
//   Пример: DAT_fff86616 = (ushort)DAT_fff88e93;

// DAT_fff8588a @ 0xfff8588a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if ((DAT_fff8588a & 0x40) == 0) {
//   Пример: DAT_fff8588a = DAT_fff8588a & 0xffdf;
//   Пример: DAT_fff8588a = DAT_fff8588a | 0x20;

// DAT_fff854e6 @ 0xfff854e6 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if ((DAT_fff854e6 & 8) == 0) {

// DAT_fff85864 @ 0xfff85864 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: uVar1 = DAT_fff85864;
//   Пример: if ((((uint)(int)(short)DAT_fff85864 >> 2 & 0x80) != 0) && (DAT_fff8419c == 0)) {
//   Пример: ((((uint)(int)(short)(DAT_fff85864 & 0xfdff) >> 1 & 0x80) != 0 ||

// DAT_fff85890 @ 0xfff85890 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if (DAT_fff85890 != DAT_fff83284) {
//   Пример: DAT_fff85890 = DAT_fff83284;

// DAT_fff83284 @ 0xfff83284 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if (DAT_fff85890 != DAT_fff83284) {
//   Пример: DAT_fff85890 = DAT_fff83284;

// DAT_fff80d0e @ 0xfff80d0e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: uVar2 = DAT_fff80d0e + 1;
//   Пример: if ((ushort)(DAT_fff80d0e + 1) == 0) {
//   Пример: uVar2 = DAT_fff80d0e;

// DAT_fff82006 @ 0xfff82006 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if ((((DAT_fff82006 & 8) != 0) && ((DAT_fff82006 & 0x10) != 0)) &&

// DAT_fff80d10 @ 0xfff80d10 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: (((DAT_fff80d10 & 1) != 0 || (uVar1 = DAT_fff85864 & 0xfdff, 9 < DAT_fff80d0e)))))) {
//   Пример: DAT_fff80d10 = DAT_fff80d10 & 0xfffe;

// DAT_fff84040 @ 0xfff84040 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if (199 < DAT_fff84040) {
//   Пример: DAT_fff84040 = 0;
//   Пример: DAT_fff8584a = DAT_fff84040;

// DAT_fff82000 @ 0xfff82000 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if ((uVar5 & DAT_fff82000) != 0) {

// DAT_fff843d6 @ 0xfff843d6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: if ((DAT_fff8419c == 0) || (DAT_fff843d6 == 0)) {

// DAT_fff8584a @ 0xfff8584a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_0008638e
//   Пример: DAT_fff8584a = DAT_fff84040;

// DAT_fff84e78 @ 0xfff84e78 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: DAT_fff84e78 = DAT_fff84e78 | 0x4000;
//   Пример: (((uint)(int)(short)DAT_fff84e78 >> 8 & 0x40) != 0)))))) {
//   Пример: DAT_fff84e78 = DAT_fff84e78 & 0xdfff;

// DAT_fff806e4 @ 0xfff806e4 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: if ((((((DAT_fff806e4 & 0xdef8) != 0) || ((DAT_fff806e6 & 0xbd00) != 0)) ||

// DAT_fff806e6 @ 0xfff806e6 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: if ((((((DAT_fff806e4 & 0xdef8) != 0) || ((DAT_fff806e6 & 0xbd00) != 0)) ||

// DAT_fff80714 @ 0xfff80714 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: ((DAT_fff80714 & 0xdef8) != 0)) ||

// DAT_fff80716 @ 0xfff80716 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: (((DAT_fff80716 & 0xbd00) != 0 || ((DAT_fff806fc & 0xdef8) != 0)))) ||

// DAT_fff806fc @ 0xfff806fc | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: (((DAT_fff80716 & 0xbd00) != 0 || ((DAT_fff806fc & 0xdef8) != 0)))) ||

// DAT_fff806fe @ 0xfff806fe | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: ((DAT_fff806fe & 0xbd00) != 0)) {

// DAT_fff8587c @ 0xfff8587c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088c50
//   Пример: DAT_fff8587c = 0xffff;

// DAT_fff8589a @ 0xfff8589a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8589a = 0;

// DAT_fff89054 @ 0xfff89054 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff89054 = 0;

// DAT_fff84ec2 @ 0xfff84ec2 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84ec2 = DAT_fff84ec2 & 0x3fff;
//   Пример: DAT_fff84ec2 = DAT_fff84ec2 & 0xfb9c;

// DAT_fff844b8 @ 0xfff844b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844b8 = 10;
//   Пример: DAT_fff844b8 = 2;

// DAT_fff844b6 @ 0xfff844b6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844b6 = 10;
//   Пример: DAT_fff844b6 = 2;

// DAT_fff844b2 @ 0xfff844b2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844b2 = 10;
//   Пример: DAT_fff844b2 = 2;

// DAT_fff844b0 @ 0xfff844b0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844b0 = 10;
//   Пример: DAT_fff844b0 = 2;

// DAT_fff844ae @ 0xfff844ae | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844ae = 10;
//   Пример: DAT_fff844ae = 2;

// DAT_fff844ac @ 0xfff844ac | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844ac = 10;
//   Пример: DAT_fff844ac = 2;

// DAT_fff844aa @ 0xfff844aa | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844aa = 10;
//   Пример: DAT_fff844aa = 2;

// DAT_fff844a8 @ 0xfff844a8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844a8 = 10;
//   Пример: DAT_fff844a8 = 2;

// DAT_fff844a6 @ 0xfff844a6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844a6 = 10;
//   Пример: DAT_fff844a6 = 2;

// DAT_fff844a4 @ 0xfff844a4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844a4 = 10;
//   Пример: DAT_fff844a4 = 2;

// DAT_fff844a2 @ 0xfff844a2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844a2 = 10;
//   Пример: DAT_fff844a2 = 2;

// DAT_fff844bc @ 0xfff844bc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844bc = 10;
//   Пример: DAT_fff844bc = 2;

// DAT_fff844ba @ 0xfff844ba | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844ba = 10;
//   Пример: DAT_fff844ba = 2;

// DAT_fff8449e @ 0xfff8449e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8449e = 10;
//   Пример: DAT_fff8449e = 2;

// DAT_fff8449c @ 0xfff8449c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8449c = 10;
//   Пример: DAT_fff8449c = 2;

// DAT_fff8449a @ 0xfff8449a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8449a = 10;
//   Пример: DAT_fff8449a = 2;

// DAT_fff84498 @ 0xfff84498 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84498 = 10;
//   Пример: DAT_fff84498 = 2;

// DAT_fff84496 @ 0xfff84496 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84496 = 10;
//   Пример: DAT_fff84496 = 2;

// DAT_fff84494 @ 0xfff84494 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84494 = 10;
//   Пример: DAT_fff84494 = 2;

// DAT_fff84492 @ 0xfff84492 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84492 = 10;
//   Пример: DAT_fff84492 = 2;

// DAT_fff84490 @ 0xfff84490 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84490 = 10;
//   Пример: DAT_fff84490 = 2;

// DAT_fff8448e @ 0xfff8448e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8448e = 10;
//   Пример: DAT_fff8448e = 2;

// DAT_fff8448c @ 0xfff8448c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8448c = 10;
//   Пример: DAT_fff8448c = 2;

// DAT_fff8448a @ 0xfff8448a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8448a = 10;
//   Пример: DAT_fff8448a = 2;

// DAT_fff84488 @ 0xfff84488 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84488 = 10;
//   Пример: DAT_fff84488 = 2;

// DAT_fff84486 @ 0xfff84486 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84486 = 10;
//   Пример: DAT_fff84486 = 2;

// DAT_fff84484 @ 0xfff84484 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84484 = 10;
//   Пример: DAT_fff84484 = 2;

// DAT_fff844b4 @ 0xfff844b4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844b4 = 6;
//   Пример: DAT_fff844b4 = 2;

// DAT_fff85540 @ 0xfff85540 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85540 = 0x1e;

// DAT_fff8552c @ 0xfff8552c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8552c = 0x80;

// DAT_fff8552a @ 0xfff8552a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8552a = 0;

// DAT_fff8561e @ 0xfff8561e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8561e = 0x78;

// DAT_fff85542 @ 0xfff85542 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85542 = 0x1e;

// DAT_fff85504 @ 0xfff85504 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85504 = 0;

// DAT_fff85630 @ 0xfff85630 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85630 = 0x80;

// DAT_fff8562e @ 0xfff8562e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8562e = 0;

// DAT_fff85632 @ 0xfff85632 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85632 = 0x78;

// DAT_fff84190 @ 0xfff84190 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84190 = 0xf;

// DAT_fff84192 @ 0xfff84192 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84192 = 0xaf;

// DAT_fff847b4 @ 0xfff847b4 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff847b4 = DAT_fff847b4 & 0xfcef;
//   Пример: DAT_fff847b4 = DAT_fff847b4 & 0xfff7;
//   Пример: DAT_fff847b4 = DAT_fff847b4 & extraout_r2_01;

// DAT_fff84e4e @ 0xfff84e4e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e4e = 0;

// DAT_fff84e4c @ 0xfff84e4c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e4c = 0;

// DAT_fff84e4a @ 0xfff84e4a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e4a = 0;

// DAT_fff85370 @ 0xfff85370 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85370 = 0xffff;

// DAT_fff858f4 @ 0xfff858f4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858f4 = 1000;

// DAT_fff858f6 @ 0xfff858f6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858f6 = 0;

// DAT_fff858f8 @ 0xfff858f8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858f8 = 0x960;

// DAT_fff858ea @ 0xfff858ea | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858ea = 0x32;

// DAT_fff858e8 @ 0xfff858e8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858e8 = 0x32;

// DAT_fff858f2 @ 0xfff858f2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858f2 = 0x78;

// DAT_fff858f0 @ 0xfff858f0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858f0 = 0x78;

// DAT_fff858ee @ 0xfff858ee | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858ee = 0xff;

// DAT_fff858ec @ 0xfff858ec | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858ec = 0xff;

// DAT_fff84342 @ 0xfff84342 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84342 = 0x78;

// DAT_fff84ea6 @ 0xfff84ea6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84ea6 = 0;

// DAT_fff804ce @ 0xfff804ce | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804ce = 0;

// DAT_fff804d8 @ 0xfff804d8 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804d8 = DAT_fff804d8 & 0xfff8;

// DAT_fff804d0 @ 0xfff804d0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804d0 = DAT_fff804d0 & extraout_r2;

// DAT_fff804e6 @ 0xfff804e6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804e6 = DAT_fff804e6 & 0xfffa;
//   Пример: DAT_fff804e6 = DAT_fff804e6 & extraout_r2_00;

// DAT_fff804e4 @ 0xfff804e4 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804e4 = DAT_fff804e4 & 0xffc0;

// DAT_fff80520 @ 0xfff80520 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: Math_MemSet16_Zero_FUN_000318ac_RAM_INIT(&DAT_fff80520,&DAT_fff806d4);

// DAT_fff806d4 @ 0xfff806d4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: Math_MemSet16_Zero_FUN_000318ac_RAM_INIT(&DAT_fff80520,&DAT_fff806d4);

// DAT_fff80d0a @ 0xfff80d0a | 4 байт | uint | [read] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: Math_MemSet16_Zero_FUN_000318ac_RAM_INIT(&Shadow_DTC_Data_to_0xFFF80B68,&DAT_fff80d0a);

// DAT_fff80aca @ 0xfff80aca | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: Math_MemSet16_Zero_FUN_000318ac_RAM_INIT(&DAT_fff80aca,&Shadow_DTC_Data_to_0xFFF80B68);

// DAT_fff80b38 @ 0xfff80b38 | 1 байт | byte | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80b38 = 0x21;

// DAT_fff84056 @ 0xfff84056 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84056 = 0;

// DAT_fff84436 @ 0xfff84436 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84436 = 200;

// DAT_fff85466 @ 0xfff85466 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85466 = DAT_fff85466 & 0xfebf;

// DAT_fff842c6 @ 0xfff842c6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842c6 = 200;

// DAT_fff84010 @ 0xfff84010 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84010 = 0;

// DAT_fff864b2 @ 0xfff864b2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff864b2 = 0;

// DAT_fff864b6 @ 0xfff864b6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff864b6 = 0;

// DAT_fff8546a @ 0xfff8546a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8546a = DAT_fff8546a & 0xefff;

// DAT_fff84e6c @ 0xfff84e6c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e6c = DAT_fff84e6c & 0xfffd;

// DAT_fff853e0 @ 0xfff853e0 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff853e0 = DAT_fff853e0 & 0xdfff;

// DAT_fff85468 @ 0xfff85468 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85468 = DAT_fff85468 & 0xefff;

// DAT_fff8540a @ 0xfff8540a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8540a = 10;

// DAT_fff84e6e @ 0xfff84e6e | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e6e = DAT_fff84e6e & 0xefff;

// DAT_fff841b4 @ 0xfff841b4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff841b4 = 0x28;

// DAT_fff841b6 @ 0xfff841b6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff841b6 = 0x14;

// DAT_fff8540c @ 0xfff8540c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8540c = 5;

// DAT_fff8540e @ 0xfff8540e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8540e = 3;

// DAT_fff841b8 @ 0xfff841b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff841b8 = 0x78;

// DAT_fff85410 @ 0xfff85410 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85410 = 3;

// DAT_fff85412 @ 0xfff85412 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85412 = 3;

// DAT_fff844be @ 0xfff844be | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844be = 0x3c;

// DAT_fff84e6a @ 0xfff84e6a | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e6a = DAT_fff84e6a & 0x1040;

// DAT_fff84e70 @ 0xfff84e70 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: uVar1 = DAT_fff84e70 & 0x71ff;
//   Пример: uVar1 = DAT_fff84e70 & 0x1ff;
//   Пример: DAT_fff84e70 = uVar1;

// DAT_fff853d4 @ 0xfff853d4 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: if ((DAT_fff853d4 & 0x60) == 0) {

// DAT_fff85414 @ 0xfff85414 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85414 = 3;

// DAT_fff8405c @ 0xfff8405c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8405c = 0;

// DAT_fff842c8 @ 0xfff842c8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842c8 = 400;

// DAT_fff84038 @ 0xfff84038 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84038 = 0;

// DAT_fff84036 @ 0xfff84036 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84036 = 0;

// DAT_fff858a2 @ 0xfff858a2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858a2 = 0;

// DAT_fff858a0 @ 0xfff858a0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858a0 = 0;

// DAT_fff842d4 @ 0xfff842d4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842d4 = 0x28;

// DAT_fff858be @ 0xfff858be | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858be = 0;

// DAT_fff858b6 @ 0xfff858b6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858b6 = 0;

// DAT_fff85432 @ 0xfff85432 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85432 = DAT_fff85420;

// DAT_fff85420 @ 0xfff85420 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85432 = DAT_fff85420;
//   Пример: DAT_fff85430 = DAT_fff85420;

// DAT_fff85430 @ 0xfff85430 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85430 = DAT_fff85420;

// DAT_fff8544c @ 0xfff8544c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8544c = 0;

// DAT_fff85434 @ 0xfff85434 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85434 = 0;

// DAT_fff85436 @ 0xfff85436 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85436 = 0xffff;

// DAT_fff858b8 @ 0xfff858b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858b8 = 0;

// DAT_fff858ba @ 0xfff858ba | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858ba = 0;

// DAT_fff84e72 @ 0xfff84e72 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e72 = 0;

// DAT_fff842e2 @ 0xfff842e2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842e2 = 0x1c20;

// DAT_fff85c12 @ 0xfff85c12 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85c12 = 0xffff;

// DAT_fff85c14 @ 0xfff85c14 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85c14 = 0xffff;

// DAT_fff85c16 @ 0xfff85c16 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85c16 = 0;

// DAT_fff85c18 @ 0xfff85c18 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85c18 = 0;

// DAT_fff858c2 @ 0xfff858c2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858c2 = 0;

// DAT_fff858c0 @ 0xfff858c0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858c0 = 0;

// DAT_fff858c4 @ 0xfff858c4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858c4 = 0;

// DAT_fff84e74 @ 0xfff84e74 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e74 = DAT_fff84e74 & 0x3ff;

// DAT_fff8403c @ 0xfff8403c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8403c = 0;

// DAT_fff8589c @ 0xfff8589c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8589c = 0;

// DAT_fff8431e @ 0xfff8431e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8431e = 0x15c;

// DAT_fff84320 @ 0xfff84320 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84320 = 400;

// DAT_fff84e82 @ 0xfff84e82 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e82 = DAT_fff84e82 & 0xfff7 & extraout_r2_00;
//   Пример: DAT_fff84e82 = DAT_fff84e82 & 0xffef;

// DAT_fff84044 @ 0xfff84044 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84044 = 0;

// DAT_fff85c9e @ 0xfff85c9e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85c9e = DAT_fff85c9e & extraout_r2_00 & extraout_r1_00 & 0xfffd;

// DAT_fff858da @ 0xfff858da | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858da = 0x280;

// DAT_fff858dc @ 0xfff858dc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858dc = DAT_fff858d0;

// DAT_fff858d0 @ 0xfff858d0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858dc = DAT_fff858d0;
//   Пример: DAT_fff858de = DAT_fff858d0;

// DAT_fff858de @ 0xfff858de | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858de = DAT_fff858d0;

// DAT_fff84e80 @ 0xfff84e80 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e80 = DAT_fff84e80 & 0x807f;

// DAT_fff858d8 @ 0xfff858d8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858d8 = 0;

// DAT_fff858e2 @ 0xfff858e2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff858e2 = 0xffff;

// DAT_fff863a6 @ 0xfff863a6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff863a6 = DAT_fff863a6 & 0xffef | 0x60;

// DAT_fff84e50 @ 0xfff84e50 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e50 = 0;

// DAT_fff84326 @ 0xfff84326 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84326 = 0x14;

// DAT_fff84e5e @ 0xfff84e5e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e5e = 0;

// DAT_fff84e5c @ 0xfff84e5c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e5c = 0;

// DAT_fff844ca @ 0xfff844ca | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844ca = 10;

// DAT_fff844c8 @ 0xfff844c8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844c8 = 10;

// DAT_fff847b8 @ 0xfff847b8 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff847b8 = DAT_fff847b8 & extraout_r1_00 & 0xbffd & extraout_r2_00 & 0xe7f7;

// DAT_fff844c0 @ 0xfff844c0 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844c0 = 10;

// DAT_fff841da @ 0xfff841da | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff841da = 200;

// DAT_fff84e66 @ 0xfff84e66 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e66 = DAT_fff84e66 & 0x8df;

// DAT_fff84e68 @ 0xfff84e68 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e68 = DAT_fff84e68 & 0xfe0;

// DAT_fff84e52 @ 0xfff84e52 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e52 = 0;

// DAT_fff847b6 @ 0xfff847b6 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff847b6 = DAT_fff847b6 & 0xfa7f;

// DAT_fff84328 @ 0xfff84328 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84328 = 0x14;

// DAT_fff84eb4 @ 0xfff84eb4 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84eb4 = DAT_fff84eb4 & 0x9fff;

// DAT_fff8429e @ 0xfff8429e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8429e = 0x14;

// DAT_fff84054 @ 0xfff84054 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84054 = 0;

// DAT_fff84050 @ 0xfff84050 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84050 = 0;

// DAT_fff84052 @ 0xfff84052 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84052 = 0;

// DAT_fff84cd0 @ 0xfff84cd0 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84cd0 = DAT_fff84cd0 & 0xe7bf;

// DAT_fff84d82 @ 0xfff84d82 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84d82 = 0;

// DAT_fff84d84 @ 0xfff84d84 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84d84 = 0;

// DAT_fff842b8 @ 0xfff842b8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842b8 = 0x14;

// DAT_fff84378 @ 0xfff84378 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84378 = 0x14;

// DAT_fff84062 @ 0xfff84062 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84062 = 0;

// DAT_fff8405e @ 0xfff8405e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8405e = 0;

// DAT_fff84060 @ 0xfff84060 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84060 = 0;

// DAT_fff84e36 @ 0xfff84e36 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e36 = DAT_fff84e36 & 0xe7bf;

// DAT_fff84e3a @ 0xfff84e3a | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e3a = DAT_fff84e3a & 0xff07;

// DAT_fff803f8 @ 0xfff803f8 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff803f8 = DAT_fff803f8 & 0x3fff;

// DAT_fff84e22 @ 0xfff84e22 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e22 = 0;

// DAT_fff84e24 @ 0xfff84e24 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e24 = 0;

// DAT_fff842ba @ 0xfff842ba | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff842ba = 0x14;

// DAT_fff84462 @ 0xfff84462 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84462 = 10;

// DAT_fff84460 @ 0xfff84460 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84460 = 0x3c;

// DAT_fff8429c @ 0xfff8429c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8429c = 0x18e;

// DAT_fff84464 @ 0xfff84464 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84464 = 10;

// DAT_fff84466 @ 0xfff84466 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84466 = 10;

// DAT_fff84468 @ 0xfff84468 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84468 = 10;

// DAT_fff8446a @ 0xfff8446a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8446a = 10;

// DAT_fff8446c @ 0xfff8446c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8446c = 10;

// DAT_fff84e86 @ 0xfff84e86 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e86 = DAT_fff84e86 & 0xefff;

// DAT_fff85828 @ 0xfff85828 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85828 = DAT_fff85828 & extraout_r1_00 & 0xffbf & extraout_r2_00;

// DAT_fff847ba @ 0xfff847ba | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff847ba = DAT_fff847ba & 0x23ff;

// DAT_fff80d1e @ 0xfff80d1e | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80d1e = 0;

// DAT_fff80030 @ 0xfff80030 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80030 = 0;

// DAT_fff80d20 @ 0xfff80d20 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80d20 = 0;

// DAT_fff84e62 @ 0xfff84e62 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e62 = 0;

// DAT_fff84e60 @ 0xfff84e60 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e60 = 0;

// DAT_fff84e64 @ 0xfff84e64 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e64 = 0;

// DAT_fff85236 @ 0xfff85236 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85236 = DAT_fff85236 & 0x7fff & extraout_r1_00;

// DAT_fff85230 @ 0xfff85230 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85230 = 0;

// DAT_fff84390 @ 0xfff84390 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84390 = 0x50;

// DAT_fff8523a @ 0xfff8523a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8523a = DAT_fff8523a & 0xfffd & extraout_r1_00;

// DAT_fff840e4 @ 0xfff840e4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840e4 = 400;

// DAT_fff840e6 @ 0xfff840e6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840e6 = 400;

// DAT_fff840e8 @ 0xfff840e8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840e8 = 400;

// DAT_fff840ea @ 0xfff840ea | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840ea = 400;

// DAT_fff840ec @ 0xfff840ec | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840ec = 400;

// DAT_fff840ee @ 0xfff840ee | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840ee = 400;

// DAT_fff840f0 @ 0xfff840f0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840f0 = 400;

// DAT_fff840f4 @ 0xfff840f4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840f4 = 400;

// DAT_fff840f6 @ 0xfff840f6 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840f6 = 400;

// DAT_fff840f8 @ 0xfff840f8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840f8 = 400;

// DAT_fff840fa @ 0xfff840fa | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840fa = 400;

// DAT_fff840fc @ 0xfff840fc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840fc = 400;

// DAT_fff840fe @ 0xfff840fe | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff840fe = 400;

// DAT_fff84100 @ 0xfff84100 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84100 = 400;

// DAT_fff84102 @ 0xfff84102 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84102 = 400;

// DAT_fff84104 @ 0xfff84104 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84104 = 400;

// DAT_fff84106 @ 0xfff84106 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84106 = 400;

// DAT_fff84108 @ 0xfff84108 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84108 = 400;

// DAT_fff8410a @ 0xfff8410a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8410a = 400;

// DAT_fff8410c @ 0xfff8410c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8410c = 400;

// DAT_fff8410e @ 0xfff8410e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8410e = 400;

// DAT_fff84110 @ 0xfff84110 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84110 = 400;

// DAT_fff84112 @ 0xfff84112 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84112 = 400;

// DAT_fff84114 @ 0xfff84114 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84114 = 0;

// DAT_fff84116 @ 0xfff84116 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84116 = 0;

// DAT_fff84118 @ 0xfff84118 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84118 = 0;

// DAT_fff8411a @ 0xfff8411a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8411a = 0;

// DAT_fff8411c @ 0xfff8411c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8411c = 0;

// DAT_fff8411e @ 0xfff8411e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8411e = 0;

// DAT_fff84120 @ 0xfff84120 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84120 = 0;

// DAT_fff84124 @ 0xfff84124 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84124 = 0;

// DAT_fff84126 @ 0xfff84126 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84126 = 0;

// DAT_fff84128 @ 0xfff84128 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84128 = 0;

// DAT_fff8412a @ 0xfff8412a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8412a = 0;

// DAT_fff8412c @ 0xfff8412c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8412c = 0;

// DAT_fff8412e @ 0xfff8412e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8412e = 0;

// DAT_fff84130 @ 0xfff84130 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84130 = 0;

// DAT_fff84132 @ 0xfff84132 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84132 = 0;

// DAT_fff84134 @ 0xfff84134 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84134 = 0;

// DAT_fff84136 @ 0xfff84136 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84136 = 0;

// DAT_fff84138 @ 0xfff84138 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84138 = 0;

// DAT_fff8413a @ 0xfff8413a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8413a = 0;

// DAT_fff8413c @ 0xfff8413c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8413c = 0;

// DAT_fff8413e @ 0xfff8413e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8413e = 0;

// DAT_fff84140 @ 0xfff84140 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84140 = 0;

// DAT_fff84142 @ 0xfff84142 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84142 = 0;

// DAT_fff84382 @ 0xfff84382 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84382 = 0x50;

// DAT_fff84384 @ 0xfff84384 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84384 = 0x50;

// DAT_fff8438e @ 0xfff8438e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8438e = 0x50;

// DAT_fff84388 @ 0xfff84388 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84388 = 0x50;

// DAT_fff8438a @ 0xfff8438a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8438a = 0x50;

// DAT_fff8438c @ 0xfff8438c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8438c = 0x50;

// DAT_fff84386 @ 0xfff84386 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84386 = 0x50;

// DAT_fff804ba @ 0xfff804ba | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804ba = DAT_fff804ba & extraout_r1_00;

// DAT_fff804bc @ 0xfff804bc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804bc = 0;

// DAT_fff804be @ 0xfff804be | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff804be = DAT_fff804be & 0x3fff;

// DAT_fff84158 @ 0xfff84158 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84158 = 8;

// DAT_fff8415a @ 0xfff8415a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8415a = 100;

// DAT_fff8415c @ 0xfff8415c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8415c = 0x28;

// DAT_fff8414e @ 0xfff8414e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8414e = 8;

// DAT_fff84150 @ 0xfff84150 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84150 = 0x24;

// DAT_fff84152 @ 0xfff84152 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84152 = 8;

// DAT_fff84154 @ 0xfff84154 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84154 = 100;

// DAT_fff84156 @ 0xfff84156 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84156 = 0x28;

// DAT_fff84144 @ 0xfff84144 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84144 = 10;

// DAT_fff84146 @ 0xfff84146 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84146 = 0x2b;

// DAT_fff84148 @ 0xfff84148 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84148 = 10;

// DAT_fff8414a @ 0xfff8414a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8414a = 100;

// DAT_fff8414c @ 0xfff8414c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8414c = 0x30;

// DAT_fff8416e @ 0xfff8416e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8416e = 10;

// DAT_fff84170 @ 0xfff84170 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84170 = 100;

// DAT_fff84172 @ 0xfff84172 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84172 = 0x30;

// DAT_fff84168 @ 0xfff84168 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84168 = 8;

// DAT_fff8416a @ 0xfff8416a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8416a = 100;

// DAT_fff8416c @ 0xfff8416c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8416c = 0x28;

// DAT_fff8415e @ 0xfff8415e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8415e = 8;

// DAT_fff84160 @ 0xfff84160 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84160 = 0x24;

// DAT_fff84162 @ 0xfff84162 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84162 = 8;

// DAT_fff84164 @ 0xfff84164 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84164 = 100;

// DAT_fff84166 @ 0xfff84166 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84166 = 0x28;

// DAT_fff85238 @ 0xfff85238 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85238 = DAT_fff85238 & 0x107 & extraout_r2_00 & 0xfffd & extraout_r1_00 & 0xfeff;

// DAT_fff8522a @ 0xfff8522a | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8522a = DAT_fff8522a & 0xbfef & extraout_r1_00 & 0xdfbf & extraout_r2_00 & 0x6f55;

// DAT_fff85228 @ 0xfff85228 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85228 = DAT_fff85228 & 0xffef & extraout_r1_00 & 0xffbf & extraout_r2_00 & 0xff55;

// DAT_fff85694 @ 0xfff85694 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85694 = 300;

// DAT_fff8567e @ 0xfff8567e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8567e = 0;

// DAT_fff8567c @ 0xfff8567c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8567c = 0;

// DAT_fff8567a @ 0xfff8567a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8567a = 0;

// DAT_fff85678 @ 0xfff85678 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85678 = 0;

// DAT_fff85696 @ 0xfff85696 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85696 = 0x5dc;

// DAT_fff85686 @ 0xfff85686 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85686 = 0;

// DAT_fff85684 @ 0xfff85684 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85684 = 0;

// DAT_fff85682 @ 0xfff85682 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85682 = 0;

// DAT_fff85680 @ 0xfff85680 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85680 = 0;

// DAT_fff85636 @ 0xfff85636 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85636 = DAT_fff85636 & 0xff;

// DAT_fff8564c @ 0xfff8564c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8564c = 0;

// DAT_fff8564a @ 0xfff8564a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8564a = 0;

// DAT_fff85650 @ 0xfff85650 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85650 = DAT_fff85650 & 0xfcff;

// DAT_fff8564e @ 0xfff8564e | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8564e = DAT_fff8564e & 0xf7f8;

// DAT_fff80786 @ 0xfff80786 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80786 = DAT_fff80786 & extraout_r1_00;

// DAT_fff8571a @ 0xfff8571a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8571a = 0;

// DAT_fff856f8 @ 0xfff856f8 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff856f8 = 0;

// DAT_fff856f0 @ 0xfff856f0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff856f0 = 0;

// DAT_fff85708 @ 0xfff85708 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85708 = 0;

// DAT_fff85700 @ 0xfff85700 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85700 = 0;

// DAT_fff85716 @ 0xfff85716 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85716 = 0;

// DAT_fff85712 @ 0xfff85712 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85712 = 0;

// DAT_fff8570e @ 0xfff8570e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8570e = 0;

// DAT_fff8430a @ 0xfff8430a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8430a = 100;

// DAT_fff8430c @ 0xfff8430c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8430c = 100;

// DAT_fff84318 @ 0xfff84318 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84318 = 0;

// DAT_fff84316 @ 0xfff84316 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84316 = 0x38;

// DAT_fff856c2 @ 0xfff856c2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff856c2 = 0x1c20;

// DAT_fff844d0 @ 0xfff844d0 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844d0 = 0xb4;

// DAT_fff8577c @ 0xfff8577c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff8577c = 0x8000;

// DAT_fff84ea4 @ 0xfff84ea4 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84ea4 = DAT_fff84ea4 & 0xc1ff;

// DAT_fff85772 @ 0xfff85772 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85772 = 0;

// DAT_fff85774 @ 0xfff85774 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85774 = 0;

// DAT_fff85778 @ 0xfff85778 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff85778 = 0;

// DAT_fff80510 @ 0xfff80510 | 4 байт | int | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80510 = DAT_fff80510 & 0x7fff;

// DAT_fff844c4 @ 0xfff844c4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff844c4 = 10;

// DAT_fff80514 @ 0xfff80514 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80514 = DAT_fff80514 & extraout_r1_01;

// DAT_fff80516 @ 0xfff80516 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80516 = 0;

// DAT_fff80518 @ 0xfff80518 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff80518 = 0;

// DAT_fff84e7c @ 0xfff84e7c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: MAIN_Sys_Init_Pending_FUN_00071ac0
//   Пример: DAT_fff84e7c = DAT_fff84e7c | 2;
//   Пример: if (((DAT_fff84e7c & 2) != 0) && ((DAT_fff84e7c & 1) != 0)) {
//   Пример: DAT_fff84e7c = DAT_fff84e7c & 0xfffd & uVar1;

// DAT_fff847bc @ 0xfff847bc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00062c9c
//   Пример: DAT_fff847bc = DAT_fff847bc & 0xfffe;
//   Пример: DAT_fff847bc = DAT_fff847bc | 1;

// DAT_fff844c2 @ 0xfff844c2 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_CAN_IMMO_DAT_FUN_000883a0
//   Пример: DAT_fff844c2 = 0x14;
//   Пример: else if (DAT_fff844c2 == 0) {
//   Пример: DAT_fff844c2 = 0x14;

// DAT_fff84ec4 @ 0xfff84ec4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_CAN_IMMO_DAT_FUN_000883a0
//   Пример: DAT_fff84ec4 = 5;
//   Пример: DAT_fff84ec4 = 4;
//   Пример: DAT_fff84ec4 = 5;

// DAT_fff84562 @ 0xfff84562 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088286
//   Пример: (*(code *)(in_tbr + 0x30))(DAT_fff84562,DAT_fff84568);
//   Пример: if (0x6e < DAT_fff84562) {
//   Пример: else if (DAT_fff84562 < 0x6e) {

// DAT_fff84568 @ 0xfff84568 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088286
//   Пример: (*(code *)(in_tbr + 0x30))(DAT_fff84562,DAT_fff84568);

// DAT_fff80b36 @ 0xfff80b36 | 1 байт | byte | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088286
//   Пример: sVar2 = DAT_fff80b36 + 1;
//   Пример: DAT_fff80b36 = (*(code *)(in_tbr + 0x50))(sVar2);

// DAT_fff806f0 @ 0xfff806f0 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: puVar1 = (ushort *)((int)&DAT_fff806f0 + iVar3);
//   Пример: uVar5 = (uint)((ushort)(&DAT_fff806f0)[uVar7] >> uVar5);
//   Пример: uVar5 = (uint)(ushort)(&DAT_fff806f0)[uVar7] << (uVar6 & 0x1f);

// DAT_fff8476a @ 0xfff8476a | 4 байт | uint | [write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: DAT_fff8476a = 0;
//   Пример: DAT_fff8476a = (&DAT_0004b376)[(uint)uVar7 * 0x10 + (uint)uVar8];

// DAT_fff8476e @ 0xfff8476e | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: DAT_fff8476e = 0;
//   Пример: uVar8 = DAT_fff8476e & 0xf;
//   Пример: uVar7 = DAT_fff8476e >> 4;

// DAT_fff847c2 @ 0xfff847c2 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: else if ((DAT_fff847c2 & 0x10) != 0) {
//   Пример: else if ((DAT_fff847c2 & 0x10) != 0) {

// DAT_0004b376 @ 0x0004b376 | 4 байт | uint | [read] | flash_rom
//   Используется в: B_FUN_00071970
//   Пример: DAT_fff8476a = (&DAT_0004b376)[(uint)uVar7 * 0x10 + (uint)uVar8];
//   Пример: DAT_fff8476c = (&DAT_0004b376)[(uint)uVar7 * 0x10 + (uint)uVar8];

// DAT_fff80708 @ 0xfff80708 | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: puVar1 = (ushort *)((int)&DAT_fff80708 + iVar3);
//   Пример: uVar5 = (uint)((ushort)(&DAT_fff80708)[uVar7] >> uVar5);
//   Пример: uVar5 = (uint)(ushort)(&DAT_fff80708)[uVar7] << (uVar6 & 0x1f);

// DAT_fff8476c @ 0xfff8476c | 4 байт | uint | [write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: DAT_fff8476c = 0;
//   Пример: DAT_fff8476c = (&DAT_0004b376)[(uint)uVar7 * 0x10 + (uint)uVar8];

// DAT_fff84770 @ 0xfff84770 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00071970
//   Пример: DAT_fff84770 = 0;
//   Пример: uVar8 = DAT_fff84770 & 0xf;
//   Пример: uVar7 = DAT_fff84770 >> 4;

// DAT_fff84018 @ 0xfff84018 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: ECU_State_Flags_FUN_00072dae
//   Пример: if (DAT_fff84018 < 0x15) {

// DAT_fff84556 @ 0xfff84556 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: ECU_State_Flags_FUN_00072dae
//   Пример: if (((((ECU_State_Flags & 0x10) == 0) || (4 < DAT_fff84556)) || (4 < DAT_fff8456a)) ||

// DAT_fff8456a @ 0xfff8456a | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: ECU_State_Flags_FUN_00072dae
//   Пример: if (((((ECU_State_Flags & 0x10) == 0) || (4 < DAT_fff84556)) || (4 < DAT_fff8456a)) ||

// DAT_fff8459c @ 0xfff8459c | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: ECU_State_Flags_FUN_00072dae
//   Пример: (uVar1 = DAT_fff84fba, DAT_fff8459c < 0xf6)) {
//   Пример: if (DAT_fff8459c < 0xb1) {

// DAT_fff8522c @ 0xfff8522c | 2 байт | short | [read, write] | peripheral_mirror
//   Используется в: B_FUN_00088254
//   Пример: DAT_fff8522c = DAT_fff8522c & 0xf87f;

// DAT_fff845f4 @ 0xfff845f4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: IMMO_FLAGS_decrypt_FUN_00072d2c
//   Пример: DAT_fff845f4 = UTIL_Decrypt_FUN_00031bf6(DAT_fff845c4);
//   Пример: DAT_fff845f4 = UTIL_Lerp_U32_Q8_FUN_00031e60((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0x12);
//   Пример: uVar1 = UTIL_Q16_16_Round_Sat_FUN_00031c18(DAT_fff845f4);

// DAT_fff845c4 @ 0xfff845c4 | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: IMMO_FLAGS_decrypt_FUN_00072d2c
//   Пример: DAT_fff845f4 = UTIL_Decrypt_FUN_00031bf6(DAT_fff845c4);
//   Пример: uVar3 = UTIL_Decrypt_FUN_00031bf6(DAT_fff845c4);

// DAT_fff842bc @ 0xfff842bc | 4 байт | uint | [read, write] | peripheral_mirror
//   Используется в: IMMO_FLAGS_decrypt_FUN_00072d2c
//   Пример: DAT_fff842bc = 0;
//   Пример: DAT_fff842bc = 0x14;
