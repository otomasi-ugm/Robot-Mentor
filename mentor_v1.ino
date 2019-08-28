#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "MServo.h"
#include "MButton.h"
#include "menu.h"
#include "variable.h"
#include "serial.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
MServo* axis = new MServo[numAxis];
MButton btn;

void setup() {
  lcd.begin(16, 2);
  btn.btnNum = 4;
  btn.attach(btnPin, INPUT_PULLUP);
  btn.setState(btnState);
  btn.setActive(LOW);
  loadEEPROM();
  for (int n = 0; n < numAxis; n++) {
    axis[n].attach(potPin[n], dirPin[n], pwmPin[n]);
    axis[n].setAngleConstrain(minAngle[n], maxAngle[n]);
    axis[n].setPotConstrain(minPot[n], maxPot[n]);
  }
  /* Reverse the Elbow direction */
  axis[ELBOW].reverse = true;
  axis[BASE].write(100);
  Serial.begin(115200);
}

void loop() {
  mainMenu();
}

void lcd_print(uint8_t x, uint8_t y, const char* fmtstr, ...)
{
  char lcd_string[21];
  va_list ap;

  va_start(ap, fmtstr);
  vsprintf(lcd_string, fmtstr, ap);
  va_end(ap);

  lcd.setCursor(x, y);
  lcd.print(lcd_string);
}

void mainMenu() {
  String menu[] = {"Run System", "Calibrate Sensor", "About", "               "};
  lcd.clear();
  int index = 0;
  btn.setIndex(index);
  while (1) {
    index = btn.getIndex(3);
    showMenu(lcd, menu, index);
    if (btn.getBtn() == OK) {
      switch (index) {
        case 0:
          runSystem();
          break;
        case 1:
          calibrateMenu();
          break;
      }
      btn.setIndex(index);
    }
  }
}

void runSystem() {
  lcd.clear();
  lcd.print("Running");
  for (int i = 0; i < numAxis; i++) {
    data[i] = axis[i].getAngle();
  }
  while (1) {
    parsing(data, numAxis);
    lcd_print(0, 1, "%3d %3d %3d %3d", data[0], data[1], data[2], data[3]);
    for (int i = 0; i < numAxis; i++) {
      axis[i].write(data[i]);
    }
    axis[BASE].update();
    axis[SHOULDER].update();
    axis[ELBOW].update();
    if (btn.getBtn() == CANCEL) {
      while (btn.getBtn() == CANCEL) {}
      break;
    }
  }
}

void calibrateMenu() {
  lcd.clear();
  int index = 0;
  btn.setIndex(index);
  while (1) {
    index = btn.getIndex(5);
    showMenu(lcd, nameAxis, index);
    if (btn.getBtn() == OK) {
      while (btn.getBtn() == OK) {}
      calibrateAxis(index);
    }
    else if (btn.getBtn() == CANCEL) {
      while (btn.getBtn() == CANCEL) {}
      break;
    }
  }
}

void calibrateAxis(int num) {
  lcd.clear();
  int set = 0;
  while (1) {
    lcd.setCursor(0, 0);
    lcd.print("Calibrate ");
    lcd.print(nameAxis[num]);
    if (btn.getBtn() == CANCEL) {
      while (btn.getBtn() == CANCEL) {}
      break;
    }
    else if (btn.getBtn() == OK) {
      while (btn.getBtn() == OK) {}
      calibrateAxisConstrain(num, set);
    }
    lcd.setCursor(0, 1);
    if (set == 0) {
      lcd.print("~");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Min");

    lcd.setCursor(8, 1);
    if (set == 1) {
      lcd.print("~");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Max");

    set = btn.getIndex(2);
  }
}

void calibrateAxisConstrain(int num, int mode) { // Mode => 0 = Min ; 1 = Max
  lcd.clear();
  while (1) {
    /* Display Section */
    lcd.setCursor(0, 0);
    lcd.print(nameAxis[num]);
    if (mode == 0) {
      lcd.print(" MIN");
    }
    else if (mode == 1) {
      lcd.print(" MAX");
    }
    lcd_print(0, 1, "<    %4d      >", axis[num].movingAverage());

    /* Button Section */
    if (btn.getBtn() == UP) {
      axis[num].move(200);
    }
    else if (btn.getBtn() == DOWN) {
      axis[num].move(-200);
    }
    else if (btn.getBtn() == CANCEL) {
      while (btn.getBtn() == CANCEL) {}
      lcd.clear();
      break;
    }
    else if (btn.getBtn() == OK) {
      lcd.clear();
      if (mode == 0) {
        axisData[num].minPot = axis[num].movingAverage();
      }
      else if (mode == 1) {
        axisData[num].maxPot = axis[num].movingAverage();
      }
      lcd.print("Saving Data");
      EEPROM.put(axisDataAddr, axisData);
      for (int i = 1; i <= 5; i++) {
        lcd.print(".");
        delay(200);
      }
      lcd.clear();
      break;
    }

    if (!btn.isPressed()) {
      axis[num].move(0);
    }
  }
}

void loadEEPROM() {
  EEPROM.get(axisDataAddr, axisData);
  for (int n = 0; n < numAxis; n++) {
    if ((axisData[n].minPot != 0) && (axisData[n].maxPot != 0)) {
      minPot[n] = axisData[n].minPot;
      maxPot[n] = axisData[n].maxPot;
    }
  }
}

