// irirB (irirB.ino) v1.0
// 
// CopyRight (c) 2021 Yuto Yazawa
// 
// Released under MIT license.
// see LICENSE
// 

#include <LiquidCrystal_I2C.h>
#include "Studuino.h"

//Studuinoボードのクラス(?)
Studuino board;
//0x27 16*2 LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

int data[5] = {32767, 32767, 32767, 32767, 32767};

void sort() {
  int ichi = 0;
  for (int h = 0; h < 19; h++) {
    for (int i = 0; i < 4; i++) {
      if (data[i] > data[i + 1]) {
        ichi = data[i];
        data[i] = data[i + 1];
        data[i + 1] = ichi;
      }
    }
  }
}

void irmain() {
  byte btn, irb, hit, okk;
  int stime = 0;
  int temmie = 0;
  //hOI!!!!!! i'm tEMMIE!!
  hit = 0;
  okk = 0;
  lcd.setCursor(0, 0);
  lcd.print("RU READY?");
  board.Timer(0.5);
  while (1) {
    btn = board.GetPushSwitchValue(PORT_A0);
    btn = 1 - btn;
    if (btn) {
      break;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WAIT FOR TIMER");
  board.Timer(0.5);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("start!");
  stime = millis();
  while (1) {
    btn = board.GetPushSwitchValue(PORT_A0);
    irb = board.GetPushSwitchValue(PORT_A3);
    btn = 1 - btn;
    irb = 1 - irb;
    if (btn) {
      break;
    }
    if (irb) {
      hit = 1;
      break;
    }
  }
  temmie = millis();
  if (stime % 1000) {
    stime = (stime - stime % 1000) / 1000;
  }
  else {
    stime = stime / 1000;
  }
  if (temmie % 1000) {
    temmie = (temmie - temmie % 1000) / 1000;
  }
  else {
    temmie = temmie / 1000;
  }
  temmie = temmie - stime;
  if (hit) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("oops! UR OUT!");
    board.Timer(0.75);
    lcd.clear();
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Congrats!");
  if (temmie < data[4]) {
    lcd.clear();
    data[4] = temmie;
    sort();
  }
  board.Timer(0.75);
  lcd.clear();
  return;
}

void rank() {
  int curs = 0;
  byte btn, irb;
  for (;;) {
    btn = board.GetPushSwitchValue(PORT_A0);
    irb = board.GetPushSwitchValue(PORT_A3);
    btn = 1 - btn;
    irb = 1 - irb;
    lcd.setCursor(0, 0);
    lcd.print(curs + 1, 10);
    lcd.setCursor(2, 0);
    lcd.print(data[curs], 10);
    lcd.setCursor(0, 1);
    lcd.print(curs + 2, 10);
    lcd.setCursor(2, 1);
    lcd.print(data[curs + 1], 10);
    if (btn) {
      lcd.clear();
      curs++;
    }
    if (irb) {
      lcd.clear();
      return;
    }
    if (curs == 4) {
      curs = 0;
    }
    board.Timer(0.2);
  }
}

void setup() {
  //init LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  //init irirB
  board.InitSensorPort(PORT_A0, PIDPUSHSWITCH);
  board.InitSensorPort(PORT_A3, PIDPUSHSWITCH);
}

void loop() {
  //メイン処理の初期化(紛らわしい
  byte irb;
  byte irb2;
  byte curs = 0;
  lcd.setCursor(0, 0);
  lcd.print("* IRIRB");
  lcd.setCursor(0, 1);
  lcd.print("  RANKING");
  //こっからメイン処理
  for (;;) {
    //どのボタンが押されているか取得
    irb = board.GetPushSwitchValue(PORT_A3);
    irb2 = board.GetPushSwitchValue(PORT_A0);
    //直感的にするために値を反転
    irb = 1 - irb;
    irb2 = 1 - irb2;
    //
    if (irb && !curs) {
      lcd.setCursor(0, 0);
      lcd.print("* IRIRB");
      lcd.setCursor(0, 1);
      lcd.print("  RANKING");
      curs = 1 - curs;
    }
    else {
      if (irb && curs) {
        lcd.setCursor(0, 0);
        lcd.print("  IRIRB");
        lcd.setCursor(0, 1);
        lcd.print("* RANKING");
        curs = 1 - curs;
      }
    }
    if (irb2 && !curs) {
      rank();
    }
    if (irb2 && curs) {
      lcd.clear();
      irmain();
    }
    board.Timer(0.1);
  }
}
