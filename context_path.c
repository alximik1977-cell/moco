// ═══ Путь вызовов от entry point до NORM_START2 ═══
// Всего функций: 8
// Путь: 0000_0_TBR_1_FUN_00005da0 → M_0_FUN_00010a00 → M_1_FUN_00010c2c → M_2_MAIN_APP_CYCLE2_FUN_00011852 → A2_FUN_000120c4 → FUN_000122bc → FUN_00012720 → FUEL_Injectors_FUN_000183dc

// ═══ [1/8] 0000_0_TBR_1_FUN_00005da0 @ 0x00005da0 ═══
// Сигнатура: undefined 0000_0_TBR_1_FUN_00005da0()
// Размер: 72 байт
// Вызывает: FUN_0000112e, O_I_Polling_Timer_Setup_FUN_00005f40, M_0_FUN_00010a00, Watchdog_init_FUN_00000b00, init_hardwareFUN_00005de8, Watchdog_Hardware_Poll_Debounce_FUN_00005f6e, A0_DTC_INIT_FUN_0000151e, init_mem_clearFUN_00005ee6, FUN_00000ba0, M0_Start_FUN_00030a00, UTIL_checksums8_FUN_0000117a, CAN_init_mailboxFUN_00005efc, init_hardware_busFUN_00005e36
// Вызывается из: 0000_0_TBR_0_START_FUN_0000101c
// ⚠️ Тело функции не найдено в C-файле
void 0000_0_TBR_1_FUN_00005da0(void) { /* тело не найдено */ }

// ═══ [2/8] M_0_FUN_00010a00 @ 0x00010a00 ═══
// Сигнатура: undefined M_0_FUN_00010a00()
// Размер: 8 байт
// Вызывает: M_1_FUN_00010c2c
// Вызывается из: 0000_0_TBR_1_FUN_00005da0

void M_0_FUN_00010a00(void)

{
  M_1_FUN_00010c2c();
  return;
}

// ═══ [3/8] M_1_FUN_00010c2c @ 0x00010c2c ═══
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

// ═══ [4/8] M_2_MAIN_APP_CYCLE2_FUN_00011852 @ 0x00011852 ═══
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

// ═══ [5/8] A2_FUN_000120c4 @ 0x000120c4 ═══
// Сигнатура: undefined A2_FUN_000120c4()
// Размер: 428 байт
// Вызывает: J_Err_FUN_0000080c, FUN_00012ee6, FUN_00016dfc, FUN_000122bc, FUN_0001229c, FUN_00012306, FUN_00012a4c, FUN_00012320
// Вызывается из: M_2_MAIN_APP_CYCLE2_FUN_00011852

void A2_FUN_000120c4(void)

{
  ushort uVar1;
  short sVar2;
  ushort uVar3;
  
  sVar2 = FUN_00012320();
  if (sVar2 != 0) {
    _DAT_fff88200 = _DAT_fff88200 | 8;
  }
  if ((_DAT_fff88200 & 8) != 0) {
    FUN_00012306();
    return;
  }
  if (DAT_fff88309 != '\x10') {
    if (DAT_fff88309 == ' ') {
      FUN_0001229c();
      return;
    }
    if (DAT_fff88309 == '0') {
      FUN_000122bc();
      return;
    }
    if (DAT_fff88309 == '@') {
      J_Err_FUN_0000080c();
      return;
    }
    if (DAT_fff88309 == 'p') {
      FUN_00012a4c();
    }
    return;
  }
  uVar3 = DAT_fff88338 | 0xa000;
  if (DAT_fff88346 != 0) {
    DAT_fff88346 = DAT_fff88346 + -1;
  }
  if (DAT_fff88348 != 0) {
    DAT_fff88348 = DAT_fff88348 + -1;
  }
  if (DAT_fff88346 != 0) goto LAB_000121e8;
  uVar1 = DAT_fff88338 & 0x1c;
  if (uVar1 == 0) {
    DAT_fff88266 = DAT_fff88252;
    uVar3 = DAT_fff88338 & 0xffe1 | 0xa008;
  }
  else {
    if (uVar1 == 8) {
      DAT_fff88268 = DAT_fff88252;
      uVar3 = DAT_fff88338 & 0xffe1 | 0xa00c;
    }
    else {
      if ((uVar1 != 0xc) || (DAT_fff88348 != 0)) goto LAB_000121e2;
      if ((DAT_fff88338 & 2) == 0) {
        DAT_fff8826a = DAT_fff88252;
        uVar3 = DAT_fff88338 & 0xffe3 | 0xa00e;
      }
      else {
        DAT_fff8826c = DAT_fff88252;
        uVar3 = DAT_fff88338 & 0xffe1 | 0xa000;
      }
    }
    DAT_fff88348 = 0x28;
  }
LAB_000121e2:
  DAT_fff88346 = 10;
LAB_000121e8:
  if (DAT_fff8830a == '\x10') {
    DAT_fff85376 = uVar3 & 0xff9f | 0x4020;
  }
  else if (DAT_fff8830a == ' ') {
    DAT_fff85376 = uVar3 & 0xff9f | 0x4040;
  }
  else if (DAT_fff8830a == 'p') {
    DAT_fff85376 = uVar3 & 0xff9f | 0x4820;
  }
  else if (DAT_fff8830a == -0x80) {
    DAT_fff85376 = uVar3 & 0xb79f | 0x20;
  }
  else {
    DAT_fff85376 = uVar3 & 0xff9f | 0x4020;
  }
  DAT_fff8828e = DAT_fff8537c._2_2_;
  DAT_fff88338 = DAT_fff85376;
  FUN_00012ee6();
  FUN_00016dfc();
  return;
}

// ═══ [6/8] FUN_000122bc @ 0x000122bc ═══
// Сигнатура: undefined FUN_000122bc()
// Размер: 74 байт
// Вызывает: FUN_000125d2, FUN_0001253c, FUN_00012720, FUN_0001298a, FUN_000129e4
// Вызывается из: A2_FUN_000120c4

void FUN_000122bc(void)

{
  if (DAT_fff8830a == '\x10') {
    FUN_0001253c();
    return;
  }
  if (DAT_fff8830a == ' ') {
    FUN_000125d2();
    return;
  }
  if (DAT_fff8830a == '0') {
    FUN_00012720();
    return;
  }
  if (DAT_fff8830a == '@') {
    FUN_0001298a();
    return;
  }
  if (DAT_fff8830a == 'P') {
    FUN_000129e4();
  }
  return;
}

// ═══ [7/8] FUN_00012720 @ 0x00012720 ═══
// Сигнатура: undefined FUN_00012720()
// Размер: 542 байт
// Вызывает: FUN_00018fcc, FUEL_Injectors_FUN_000183dc, FUN_00018720
// Вызывается из: FUN_000122bc

undefined8 FUN_00012720(void)

{
  int iVar1;
  undefined4 in_r1;
  short extraout_r1;
  short extraout_r1_00;
  short extraout_r1_01;
  uint extraout_r2;
  uint extraout_r2_00;
  uint extraout_r2_01;
  
  if (DAT_fff88322 != 0) {
    DAT_fff88322 = DAT_fff88322 + -1;
  }
  iVar1 = (int)DAT_fff88322;
  if (iVar1 != 0) goto LAB_0001297e;
  if (DAT_fff8830b == '\x10') {
    if (DAT_fff8831a < 0xc) {
      FUN_00018fcc(0x9c4,1);
      DAT_fff84aaa = 2;
      DAT_fff84a44 = 1;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 1;
      DAT_fff84a40 = extraout_r1 + -0x8000;
      DAT_fff84aa8 = DAT_fff84aa8 & 0xfffe;
      DAT_fff84a3e = extraout_r1;
      FUEL_Injectors_FUN_000183dc(1,extraout_r2 & 0xffff);
      DAT_fff8831a = DAT_fff8831a + 1;
      iVar1 = (int)(short)DAT_fff8831a;
      if (iVar1 == 0) {
        iVar1 = (short)DAT_fff8831a + -1;
        DAT_fff8831a = (ushort)iVar1;
      }
    }
    else {
      DAT_fff84a44 = 0;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 0;
      DAT_fff84aaa = 0;
      iVar1 = FUN_00018720(1);
LAB_0001296e:
      DAT_fff8831c = 0;
LAB_00012972:
      DAT_fff8831e = 0;
    }
  }
  else if (DAT_fff8830b == ' ') {
    if (0xb < DAT_fff8831c) {
      DAT_fff84a44 = 0;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 0;
      DAT_fff84aaa = 0;
      iVar1 = FUN_00018720(2);
      DAT_fff8831a = 0;
      goto LAB_00012972;
    }
    FUN_00018fcc(0x9c4,2);
    DAT_fff84aaa = 2;
    DAT_fff84a44 = 2;
    DAT_fff84a46 = 0;
    DAT_fff8553e = 2;
    DAT_fff84a40 = extraout_r1_00 + -0x8000;
    DAT_fff84aa8 = DAT_fff84aa8 & 0xfffe;
    DAT_fff84a3e = extraout_r1_00;
    FUEL_Injectors_FUN_000183dc(2,extraout_r2_00 & 0xffff);
    DAT_fff8831c = DAT_fff8831c + 1;
    iVar1 = (int)(short)DAT_fff8831c;
    if (iVar1 == 0) {
      iVar1 = (short)DAT_fff8831c + -1;
      DAT_fff8831c = (ushort)iVar1;
    }
  }
  else {
    iVar1 = (int)DAT_fff8830b;
    if (iVar1 != 0x30) {
      DAT_fff84a44 = 0;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 0;
      DAT_fff84aaa = 0;
      DAT_fff8831a = 0;
      goto LAB_0001296e;
    }
    if (DAT_fff8831e < 0xc) {
      FUN_00018fcc(0x9c4,4);
      DAT_fff84aaa = 2;
      DAT_fff84a44 = 4;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 4;
      DAT_fff84a40 = extraout_r1_01 + -0x8000;
      DAT_fff84aa8 = DAT_fff84aa8 & 0xfffe;
      DAT_fff84a3e = extraout_r1_01;
      FUEL_Injectors_FUN_000183dc(4,extraout_r2_01 & 0xffff);
      DAT_fff8831e = DAT_fff8831e + 1;
      iVar1 = (int)(short)DAT_fff8831e;
      if (iVar1 == 0) {
        iVar1 = (short)DAT_fff8831e + -1;
        DAT_fff8831e = (ushort)iVar1;
      }
    }
    else {
      DAT_fff84a44 = 0;
      DAT_fff84a46 = 0;
      DAT_fff8553e = 0;
      DAT_fff84aaa = 0;
      iVar1 = FUN_00018720(4);
      DAT_fff8831a = 0;
      DAT_fff8831c = 0;
    }
  }
  DAT_fff88322 = 4;
LAB_0001297e:
  return CONCAT44(in_r1,iVar1);
}

// ═══ [8/8] FUEL_Injectors_FUN_000183dc @ 0x000183dc ═══
// Сигнатура: undefined FUEL_Injectors_FUN_000183dc()
// Размер: 218 байт
// Вызывается из: FUN_00013076, FUN_00012720

void FUEL_Injectors_FUN_000183dc(void)

{
  uint extraout_r1;
  uint extraout_r2;
  uint uVar1;
  int iVar2;
  code *in_tbr;
  
  (*in_tbr)();
  uVar1 = DAT_fffff220 >> 5 & 0xffff;
  if (((extraout_r2 & 0xffff) - (uVar1 + 2) >> 8 & 0x80) == 0) {
    iVar2 = (DAT_fffff540 >> 8) + (extraout_r2 - uVar1 & 0xffff);
  }
  else {
    iVar2 = (DAT_fffff540 >> 8) + 2;
  }
  if ((extraout_r1 & 1) != 0) {
    DAT_fffff610 = iVar2 << 8;
    DAT_fffff600 = DAT_fffff600 & 0xfffc | 2;
    DAT_fffff60c._1_1_ = (byte)DAT_fffff60c & 0xef;
    DAT_fffff60e._1_1_ = (byte)DAT_fffff60e | 0x10;
  }
  if ((extraout_r1 & 2) != 0) {
    DAT_fffff614 = iVar2 << 8;
    DAT_fffff600 = DAT_fffff600 & 0xfff3 | 8;
    DAT_fffff60c._1_1_ = (byte)DAT_fffff60c & 0xdf;
    DAT_fffff60e._1_1_ = (byte)DAT_fffff60e | 0x20;
  }
  if ((extraout_r1 & 4) != 0) {
    DAT_fffff618 = iVar2 << 8;
    DAT_fffff600 = DAT_fffff600 & 0xffcf | 0x20;
    DAT_fffff60c._1_1_ = (byte)DAT_fffff60c & 0xbf;
    DAT_fffff60e._1_1_ = (byte)DAT_fffff60e | 0x40;
  }
  (*(in_tbr + 0x10))();
  return;
}
