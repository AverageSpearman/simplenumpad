#include <set.h>

#include <Keyboard.h>
int row1 = 9;
int row2 = 8;
int row3 = 7;
int row4 = 6;
int row5 = 5;

int col1 = 15;
int col2 = 14;
int col3 = 16;
int col4 = 10;

int keypadPins[9] = {row1,row2,row3,row4,row5,col1,col2,col3,col4};
int keypadStatus[20];
int lastKeypadStatus[20];
Set keyPresses;
Set lastKeyPresses;

int waitRepeatCount = 10;
int curWaitCount = 0;
//char keyMatrix[20] = {'0', ',', '.', '\n',
//                      '1', '2', '3', ',',
//                      '4', '5', '6', ',',
//                      '7', '8', '9', '+',
//                      'L', '/', '*', '-'};

int keyMatrix[20] = {234, ',', 235, 224,
                      225, 226, 227, ',',
                      228, 229, 230, ',',
                      231, 232, 233, 223,
                      219, 220, 221, 222};
//Matrix
//[LCK  /   *   - ]
//[ 7   8   9   + ]
//[ 3   5   6   N ]
//[ 1   2   3   N ]
//[ 0   N   .  ENT]

//Val
//[LCK  /   *   - ]
//[ 7   8   9   + ]
//[ 3   5   6   N ]
//[ 1   2   3   N ]
//[ 0   N   .  ENT]

//220 '\334' Keypad / 
//221 '\335' Keypad * 
//222 '\336' Keypad - 
//223 '\337' Keypad + 
//224 '\340' Keypad ENTER 
//225 '\341' Keypad 1 and End 
//226 '\342' Keypad 2 and Down Arrow 
//227 '\343' Keypad 3 and PageDn 
//228 '\344' Keypad 4 and Left Arrow 
//229 '\345' Keypad 5 
//230 '\346' Keypad 6 and Right Arrow 
//231 '\347' Keypad 7 and Home 
//232 '\350' Keypad 8 and Up Arrow 
//233 '\351' Keypad 9 and PageUp 
//234 '\352' Keypad 0 and Insert /235 '\353' Keypad . and Delete
//219 Keypad Numlock
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i=0; i<9; i++)
  {
    pinMode(keypadPins[i], INPUT);
    digitalWrite(keypadPins[i], LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  getKeypadStatus();
  
  if(!array_cmp(keypadStatus, lastKeypadStatus, 20, 20)){
    curWaitCount = 0;
    //Keyboard.releaseAll();
    Serial.print("Keys changed:");
    keyPresses.clear();
    for(int i = 0; i < 20; i++){
      if(keypadStatus[i] < 1) break;
      keyPresses.add(keypadStatus[i]-1);
      Serial.print(keyMatrix[keypadStatus[i]-1]);
    }
    Set released = (lastKeyPresses - keyPresses);
    Set pressed = (keyPresses - lastKeyPresses );
    if(!pressed.isEmpty()) Keyboard.press(keyMatrix[pressed.first()]);
    if(!released.isEmpty()) Keyboard.release(keyMatrix[released.first()]);
    Serial.print(" ");
    Serial.print(released.count());
    Serial.print(" Released ");
    Serial.print(pressed.count());
    Serial.print(" Pressed ");
    
    Serial.print("\n");
    
  }
  
  
  array_copy(keypadStatus, lastKeypadStatus, 20, 20);  
  lastKeyPresses.clear();
  for(int i = 0; i < 20; i++){
    if(lastKeypadStatus[i] < 1) break;
    lastKeyPresses.add(lastKeypadStatus[i]-1);
  }
}

void getKeypadStatus()
{
  int rowPins[5] = {row1, row2, row3, row4, row5};  // row pins are 2, 7, 6, and 1 of the keypad
  int columnPins[4] = {col1, col2, col3, col4};  // column pins are pins 2, 4, and 5 of the keypad
  memset(keypadStatus, 0, sizeof(keypadStatus));
  int keypadIndex = 0;
  /* initialize all pins, inputs w/ pull-ups */
  for (int i=0; i<9; i++)
  {
    pinMode(keypadPins[i], INPUT);
    digitalWrite(keypadPins[i], HIGH);
  }
  
  for (int col=0; col<4; col++)
  {  // initial for loop to check all 4 cols
    pinMode(columnPins[col], OUTPUT);  // set the row pin as an output
    digitalWrite(columnPins[col], LOW);  // pull the row pins low
    for (int row=0; row<5; row++)
    {  // embedded for loop to check all 5 rows of each column
      if (!digitalRead(rowPins[row]))
      {
        int bitPos = ((row+1)*4 + (col+1) - 5)+1;
        //keypadStatus |= 1 << ((row+1)*4 + (col+1) - 5);  // set the status bit of the keypad return value
        keypadStatus[keypadIndex] = bitPos;  // set the status bit of the keypad return value
        keypadIndex++;
        if(keypadIndex == 20) keypadIndex = 0;
      }
    }
    pinMode(columnPins[col], INPUT);  // reset the row pin as an input
    digitalWrite(columnPins[col], HIGH);  // pull the row pin high
  }
}
void array_copy(int* a, int* b, int len_a, int len_b){
  int n;
  for(n = 0; n < len_a; n++) b[n] = a[n];
}
boolean array_cmp(int *a, int *b, int len_a, int len_b){
      int n;

      // if their lengths are different, return false
      if (len_a != len_b) return false;

      // test each element to be the same. if not, return false
      for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;

      //ok, if we have not returned yet, they are equal :)
      return true;
}
