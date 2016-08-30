//BF typing system

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define RED 0x1  //For errors, eg when committing and unmatched loops present
#define ORANGE 0x3  //For warnings, such as unmatching loops
#define BLUE 0x4  //Blue's a cool colour, right?
#define WHITE 0x7  //Strictly closer to Pantone 643 C, but its the best I can do

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

char code[] = {  //16 characters while in pre-alpha.  Larger stack to follow...
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int codePointer = 0;

char characters[] = { ' ', '+', '-', '>', '<', '[', ']', '.', ','};  //Space to delete..
byte character = 0;

String printBuffer = "";  //Debug
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
      int tempChar = character;
      character++;
      if(character > (sizeof(characters) - 1)){  //-1 to avoid null
        character = 0;
      }
      
      if(tempChar != character){
        refreshLetter = true;
      }
    }

    if(button & BUTTON_UP){
      int tempChar = character;
      character--;
      if(character < 0 || character > sizeof(characters)){
        character = (sizeof(characters) - 1);
      }
      
      if(tempChar != character){
        refreshLetter = true;
      }
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

      if((findNumber(codePointer - 1) == 0) && ((codePointer > 1) && codePointer < sizeof(code))){  //Stops you continuing if you are leaving a space
        codePointer--;
      }
      
      if(codePointer = 1 && findNumber(0) == 0){  //Removed bug regarding starting space being allowed
        codePointer--;
      }
    }

    if(button & BUTTON_SELECT){  //Purely to print current code.  Test and exit functionality to follow
      String printBuffer = "";
      for(int i = 0; i < sizeof(code); i++){
        printBuffer.concat(code[i]);
      }
      Serial.println(printBuffer);
    }
  }
  
  delay(100);  //Breathing space - may need to be longer?
  removeSpaces();
  refresh();
  //checkBrackets();
}

void refresh(){
  lcd.setCursor(codePointer, 0);
  if(refreshLetter){  //Only updates character if it needs to - prevents overwriting with previous character value
   lcd.print(characters[character]);
  }
  code[codePointer] = characters[character];
}

int findNumber(int pointer){
  int letter = 0;
  while(characters[letter] != code[pointer]){
    letter++;
  }
  Serial.print("found ");  //General verbosity for debugging
  Serial.print(characters[letter]);
  Serial.print(" at ");
  Serial.println(pointer);
  return letter;
}

void removeSpaces(){  //Needs work - not successful at the moment (hence the work needed)
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
