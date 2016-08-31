#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte mode = 0;
byte modePointer = 1;
String modes[] = {"Load", "Save"};

char code[1024] = {'-', '-', '<', '-', '<', '<', '+', '[', '+', '[', '<', '+', '>', '-', '-', '-', '>',
                   '-', '>', '-', '>', '-', '<', '<', '<', ']', '>', ']', '<', '<', '-', '-', '.', '<', '+', '+', '+',
                   '+', '+', '+', '.', '<', '<', '-', '.', '.', '<', '<', '.', '<', '+', '.', '>', '>', '.', '>', '>',
                   '.', '<', '<', '<', '.', '+', '+', '+', '.', '>', '>', '.', '>', '>', '-', '.', '<', '<', '<', '+',
                   '.'
                  };

void setup() {
  lcd.begin(16, 2);
  delay(500);
}

void loop() {
  int button = lcd.readButtons();

  if (mode == 0) {
    if (button & BUTTON_UP) {
      modePointer--;
      if (modePointer > sizeof(modes) - 1) {
        modePointer = 1;
      }
    }

    if (button & BUTTON_DOWN) {
      modePointer++;
      if (modePointer > sizeof(modes) - 1) {
        modePointer = sizeof(modes) - 1;
      }
    }

    if (button & BUTTON_SELECT) {
      mode = modePointer;
    }

    if (modePointer == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> " + modes[0]);
      lcd.setCursor(0, 1);
      lcd.print(modes[1]);
    }

    if (modePointer == 2) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("> " + modes[1]);
      lcd.setCursor(0, 0);
      lcd.print(modes[0]);
    }
  }

  if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Loading...");
    for (int i = 0; i < min(EEPROM.length(), sizeof(code)); i++) {
      code[i] = EEPROM.read(i);
    }
    lcd.clear();
    lcd.print("Loaded");
    delay(750);
    mode = 0;
  }

  if (mode == 2) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Saving...");
    for (int i = 0; i < min(EEPROM.length(), sizeof(code)); i++) {
      EEPROM.write(i, code[i]);
    }
    lcd.clear();
    lcd.print("Saved");
    delay(750);
    mode = 0;
  }
  delay(150);
}
