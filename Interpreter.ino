/*
 *  + tape++, codePointer++
 *  - tape--, codePointer++
 *  > tapePointer++, codePointer++
 *  < tapePointer--, codePointer++
 *  [ if tape[tapePointer] = 0 { while code[codePointer] != ']' { codePointer++ }} else { codePointer++ }
 *  ] if tape[tapePointer] = 0 { codePointer++ } else { j = -1;
 *  . Serial.print(String(tape[tapePointer])), codePointer++
 *  , Serial.flush(); while(!Serial.available()); tape[tapePointer] = Serial.read();, codePointer++
 */

char code[] = {'+', '>', '+', '<', '+', '+', '.', '>', '>', ',', '.'};

byte tape[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 
unsigned int codePointer = 0;
unsigned int tapePointer = 0;
int i, j = 0;
boolean complete = false;
char readBuffer[3];

void setup() {
  Serial.begin(9600);
  delay(750);
  Serial.println("Started!");
  Serial.print("code length: ");
  Serial.print('\t');
  Serial.println(sizeof(code));

  delay(100);
}

void loop() {
  while(!complete){
    
    Serial.print("found ");
    Serial.println(String(code[codePointer]));
    
    switch(code[codePointer]){
      case '+':
      tape[tapePointer]++;
      codePointer++;
      break;

      case '-':
      tape[tapePointer]--;
      codePointer++;
      break;

      case '>':
      tapePointer++;
      codePointer++;
      break;

      case '<':
      tapePointer--;
      codePointer++;
      break;

      case '.':
      Serial.print(String(tape[tapePointer]));
      codePointer++;
      break;

      case ',':
      Serial.flush();
      while(!Serial.available());
      tape[tapePointer] = char(Serial.parseInt());
      codePointer++;
      break;
      
      default:
      codePointer++;
      break;
    }
    delay(100);
    Serial.print("tapePointer, tapeValue: ");
    Serial.print("     ");
    Serial.print(tapePointer);
    Serial.print("     ");
    Serial.println(tape[tapePointer]);

    if(codePointer == sizeof(code)){
      complete = true;
      Serial.println("Complete!");
    }
  }
}
