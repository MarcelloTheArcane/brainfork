//BF typing system

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define RED 0x1
#define ORANGE 0x3
#define BLUE 0x4
#define WHITE 0x7

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

char code[] = {
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int codePointer = 0;

char characters[] = { ' ', '+', '-', '>', '<', '[', ']', '.', ','};
byte character = 0;

boolean dirtyArray[sizeof(code)] = {};

String printBuffer = "";
boolean refreshLetter = false;
int brackets = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  delay(750);
  
  Serial.println("Started");
  lcd.setBacklight(WHITE);
  lcd.print("  BRAINFORK v1  ");
  delay(500);
  
  int button = lcd.readButtons();
  while(!(button & BUTTON_SELECT)){
    button = lcd.readButtons();
    delay(50);
    }
  lcd.clear();
}

void loop() {
  int button = lcd.readButtons();
  refreshLetter = false;
  
  if(button){    
    if(button & BUTTON_DOWN){
      character++;
      if(character > (sizeof(characters) - 1)){  //-1 to avoid null
        character = 0;
      }
      dirtyArray[codePointer] = true;
    }

    if(button & BUTTON_UP){
      character--;
      if(character < 0 || character > sizeof(characters)){
        character = (sizeof(characters) - 1);
      }
      dirtyArray[codePointer] = true;
    }

    if(button & BUTTON_LEFT){
      codePointer--;
      character = findNumber(codePointer);
      if(codePointer < 0 || codePointer > sizeof(code)){
        codePointer = 0;
      }
    }

    if(button & BUTTON_RIGHT){
      codePointer++;
      character = findNumber(codePointer);
      if(codePointer > (sizeof(code) - 1)){
        codePointer = sizeof(code) - 1;
      }

      if((findNumber(codePointer - 1) == 0) && ((codePointer > 1) && codePointer < sizeof(code))){
        codePointer--;
      }
    }

    if(button & BUTTON_SELECT){
      String printBuffer = "";
      for(int i = 0; i < sizeof(code); i++){
        printBuffer.concat(code[i]);
      }
      Serial.println(printBuffer);
    }
  }
  
  delay(100);
  removeSpaces();
  refresh();
  //checkBrackets();
}

void refresh(){
  lcd.setCursor(codePointer, 0);
  if(dirtyArray[codePointer]){
   lcd.print(characters[character]);
  }
  code[codePointer] = characters[character];
}

int findNumber(int pointer){
  int letter = 0;
  while(characters[letter] != code[pointer]){
    letter++;
  }
  Serial.print("found ");
  Serial.print(characters[letter]);
  Serial.print(" at ");
  Serial.println(pointer);
  return letter;
}

void removeSpaces(){
  if(findNumber(codePointer + 1) != 0 && findNumber(codePointer = 0)){
    for(int i = codePointer; i < sizeof(code) - codePointer; i++){
      code[i] = code[i + 1];
    }
  }
}

/* Check Brackets function
void checkBrackets(){
  if(dirtyArray[codePointer] && code[codePointer] == '['){
    brackets++;
  }else if(dirtyArray[codePointer] && code[codePointer] == ']'){
    brackets--;
  }
  
  Serial.print("Brackets: ");
  Serial.println(brackets);
  if(brackets != 0){
    lcd.setBacklight(ORANGE);
  }else{
    lcd.setBacklight(WHITE);
  }
  dirtyArray[codePointer] = false;
}
*/
