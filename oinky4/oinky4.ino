#include <Keyboard.h>
#include <Keyboard_sv_SE.h>
//Adafruit itsybitsy 32u4 16hz 5v
//#define DEBUG_SOLDERING  // Uncomment this line for debugging   
#define NUMPAD
bool delayedStart = true;

int decimal =40;

struct KeyInfo {
  String name;
  char key;
  byte specialKey; // 0 for regular keys, 255 disabled, other values for special keys
  byte lastState; // 0 for released, 1 for pressed, 2 for pressed (debounced)
};

const String NOKEY = "";

const int ROW_NUM = 
#ifdef NUMPAD
5;
#else
12; // Two rows
#endif

const int COLUMN_NUM = 
#ifdef NUMPAD
4; // Two rows
#else
10; // Two rows
#endif

const byte DEBOUNCE_TIME = 4;

const int LEFT_SHIFT_ROW = 9;
const int LEFT_SHIFT_COL = 0;
const int RIGHT_SHIFT_ROW = 8;
const int RIGHT_SHIFT_COL = 7;
const int ALTGR_ROW = 10;
const int ALTGR_COL = 6;
const int PRESSED_STATE_THRESHOLD = DEBOUNCE_TIME;

KeyInfo keyMatrix[ROW_NUM][COLUMN_NUM] = 
#ifdef NUMPAD
{
  {{ "-", KEY_KP_MINUS, 0, 0 },{ "*", KEY_KP_ASTERISK, 0, 0 }, { "div", KEY_KP_SLASH, 0, 0 },{ "pi", 0, 30, 0}},
  {{ "+", KEY_KP_PLUS, 0, 0 },{ "9", '9', 0, 0 }, { "8", '8', 0, 0 },{ "7", '7', 0, 0}},
  {{ "mem", 's', 0, 0 },{ "6", '6', 0, 0 }, { "5", '5', 0, 0 },{ "4", '4', 0, 0}},
  {{ "load", 'o', 0, 0 },{ "3", '3', 0, 0 }, { "2", '2', 0, 0 },{ "1", '1', 0, 0}},
  {{ "ent", KEY_KP_ENTER, 0, 0 },{ ",", ',', 0, 0 }, { ".", '.', 0, 0 },{ "0", '0', 0, 0}}
};
#else
{
  {{ "PS", 0, 5, 0}, { "Esc", KEY_ESC, 0, 0 },{ "F2", KEY_F2, 0, 0 },{ "F4", KEY_F4, 0, 0 },{ "F5", KEY_F5, 0, 0 },{ "F7", KEY_F7, 0, 0 },{ "F9", KEY_F9, 0, 0 },{ "F11", KEY_F11, 0, 0 }, { "Cut", 0, 1, 0}, { "Paste", 0, 3, 0}},
  {{ NOKEY,'_', 255, 0}, {  "F1", KEY_F1, 0, 0 }, { "F3" ,KEY_F3, 0, 0 }, { NOKEY, '_', 255, 0} , { "F6", KEY_F6, 0, 0 }, { "F8", KEY_F8, 0, 0 }, { "F10", KEY_F10, 0, 0 }, { "F12", KEY_F12, 0, 0 }, { "Copy", 0, 2, 0}, { NOKEY, '_', 255, 0}},
  
  {{ "P1", '_', 6, 0},{ "1", '1', 4, 0},{ "2", '2', 4, 0},{ "4", '4', 4, 0},{ "6", '6', 4, 0},{ "8", '8', 4, 0},{ "=", 0, 11, 0},{ ")", 0, 13, 0},{ "Scr", KEY_PRINT_SCREEN, 0, 0} ,{ NOKEY, '_', 255, 0 }},
  {{ "0,'", '0', 4, 0}, { NOKEY, '_', 255, 0},{ "3", '3', 4, 0},{ "5", '5', 4, 0},{ "7", '7', 4, 0},{ "9", '9', 4, 0},{ "(", 0, 12, 0},{ "Back", KEY_BACKSPACE, 0, 0},{ "Pg up", KEY_PAGE_UP, 0, 0},{ NOKEY, '_', 255, 0 }},

  {{ "P2", '_', 7, 0},{ "q", 'q', 0, 0},{ "y", 'y', 0, 0},{ "t", 't', 0, 0},{ "e", 'e', 0, 0},{ "u", 'u', 0, 0},{ "p", 'p', 0, 0},{ "h vinge", 0, 15, 0},{ "Del", KEY_DELETE, 0, 0} ,{ NOKEY, '_', 255, 0 }},
  {{ "Tab", KEY_TAB, 0, 0},{ "z", 'z', 0, 0}, { NOKEY, '_', 255, 0},{ "h", 'h', 0, 0},{ "a", 'a', 0, 0},{ NOKEY, '_', 255, 0 }, { "v vinge", 0, 14, 0},{ "Enter", KEY_RETURN, 0, 0},{ "Home", KEY_HOME, 0, 0},{ NOKEY, '_', 255, 0 }},

  {{ "P3", '_', 8, 0},{ "oe", KEY_O_UMLAUT, 0, 0},{ "d", 'd', 0, 0},{ "f", 'f', 0, 0},{ "o", 'o', 0, 0},{ "j", 'j', 0, 0},{ "k", 'k', 0, 0},{ "¨", ']', 0, 0},{ NOKEY, '_', 255, 0},{ NOKEY, '_', 255, 0 }},
  {{ "Cps", 'KEY_CAPS_LOCK', 0, 0},{ "ae", KEY_A_UMLAUT, 0, 0},{ "w", 'w', 0, 0},{ "s", 's', 0, 0},{ "i", 'i', 0, 0},{ "l", 'l', 0, 0},{ NOKEY, '_', 255, 0}, { "sh ent", 0, 18, 0}/* NON below enter */,{ "pgd", KEY_PAGE_DOWN, 0, 0},{ NOKEY, '_', 255, 0 }},
 
  {{ "P4", '_', 9, 0},{ "aa", KEY_A_RING, 0, 0},{ "g", 'g', 0, 0},{ "v", 'v', 0, 0},{ "n", 'n', 0, 0},{ "comma", ',', 0, 0},{ "#", 0, 16, 0},{ "r shift", KEY_RIGHT_SHIFT, 0, 0},{ "ku", KEY_UP_ARROW, 0, 0},{ NOKEY, '_', 255, 0 }},
  {{ "lshift", KEY_LEFT_SHIFT, 0, 0},{ "b", 'b', 0, 0},{ "c", 'c', 0, 0},{ "r", 'r', 0, 0},{ "m", 'm', 0, 0},{ "dot", '.', 0, 0},{ "UN"/* NON left of shift */, '_', 0, 0},{ NOKEY, '_', 255, 0},{ "end", KEY_END, 0, 0},{ NOKEY, '_', 255, 0 }},

  {{ "P5", '_', 10, 0},{ "win", KEY_LEFT_GUI, 0, 0},{ "UN"/* NON left space */, '_', 0, 0},{ NOKEY, '_', 255, 0},{ NOKEY, '_', 255, 0},{ "UN"/* NON right space */, '_', 0, 0},{ "alt gr", KEY_RIGHT_ALT, 0, 0},{ "r ctrl", KEY_RIGHT_CTRL, 0, 0},{ "kd", KEY_DOWN_ARROW, 0, 0},{ NOKEY, '_', 255, 0} },
  {{ "LCtrl", KEY_LEFT_CTRL, 0, 0},{ "Lalt", KEY_LEFT_ALT, 0, 0},{ "space", ' ', 0, 0},{ "x", 'x', 0, 0},{ NOKEY, '_', 255, 0},{ NOKEY, '_', 255, 0},{ "fn", 'x', 0, 0},{ "kl", KEY_LEFT_ARROW, 0, 0},{ "kr", KEY_RIGHT_ARROW, 0, 0},{ NOKEY, '_', 255, 0 }}
 };
#endif

byte pin_rows[ROW_NUM] = 
#ifdef NUMPAD
{5,3,2,1,0};
#else
{15,23,21, 22, 20, 19,5,3,13,7,1,2}; // Connect to the row pinouts of the KEY_KP
#endif

byte pin_column[COLUMN_NUM] =
#ifdef NUMPAD
{4,6,7,9};
#else
{0,4,6,8,14,16,11,12,9,10};
#endif

bool storedLeftShift =false;
bool storedRightShift =false;
bool storedAltGr =false;

void setup() {
  Serial.begin(9600);
  Serial.println("INIT");
  #ifndef DEBUG_SOLDERING
    if (delayedStart)
    {
    }
    else
    {
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
      Keyboard.begin(KeyboardLayout_sv_SE);
    }
  #endif
  
  for (int i = 0; i < ROW_NUM; i++) {
    pinMode(pin_rows[i], INPUT); 
  }

  for(int i = 0; i < COLUMN_NUM; i++){
    pinMode(pin_column[i],OUTPUT);
    digitalWrite(pin_column[i],LOW); 
  }
}

void loop() {
 
  if (delayedStart)
  {
    if (millis() > 2000)
    {
      Serial.println("keyboard begin");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
      delayedStart = false;
      Keyboard.begin();
    }
  }
  else
  {
    checkKeys();
    delay(1); // Delay for debounce
  }
}

void checkKeys() {
  //Serial.println("checkKeys");
  for (int col = 0; col < COLUMN_NUM; col++) {
    // Activate one column at a time
    digitalWrite(pin_column[col],HIGH); // activate column
    
    for (int row = 0; row < ROW_NUM; row++) 
    {      
      KeyInfo &key  = keyMatrix[row][col];
      
      if (key.lastState < DEBOUNCE_TIME)
      {
        ++key.lastState;
      }
      else
      {
        bool currentState = digitalRead(pin_rows[row]) == HIGH;

        if (currentState)
        { //Key down
          if (key.lastState == DEBOUNCE_TIME)
          {
            ++key.lastState;

            if (key.specialKey) 
            {
              handleSpecialKey(key);
            } 
            else 
            {
              #ifdef DEBUG_SOLDERING
              Serial.print("Pressed: ");
              Serial.print(key.name);
              Serial.print(" (Row: ");
              Serial.print(row);
              Serial.print(", Col: ");
              Serial.print(col);
              Serial.println(")");
              #else

              // if (incomingByte >= 'a' && incomingByte <= 'z') {
              //       // Convert to uppercase
              //       char uppercaseByte = incomingByte - ('a' - 'A');
                    
              //       // Simulate the uppercase keypress
              //       Keyboard.write(uppercaseByte);
              // }
              
              Keyboard.press(key.key);
              #endif
            }
          }
        }
        else
        {//Key up
          
            
            
            if (key.lastState > DEBOUNCE_TIME *2)
            {
            key.lastState = 0;
            if (!key.specialKey) 
            {
              #ifdef DEBUG_SOLDERING
              Serial.print("Released: ");
              Serial.print(key.name);
              Serial.print(" (Row: ");
              Serial.print(row);
              Serial.print(", Col: ");
              Serial.print(col);
              Serial.println(")");
              #else
              Keyboard.release(key.key);
              #endif
            }
            }
            else if (key.lastState > DEBOUNCE_TIME)
            {++key.lastState;}
          }
        
      }

      // // State machine for key states
      // switch (key.lastState) {
      //   case 0:
      //     if (currentState) {
      //       key.lastState = 1;
      //     }
      //     break;
          
      //   case 1:
      //     if (currentState) 
      //     {
      //       key.lastState = 2;
      //       if (key.specialKey) {
      //         handleSpecialKey(key);
      //       } 
      //       else 
      //       {
      //         #ifdef DEBUG_SOLDERING
      //         Serial.print("Pressed: ");
      //         Serial.print(key.name);
      //         Serial.print(" (Row: ");
      //         Serial.print(row);
      //         Serial.print(", Col: ");
      //         Serial.print(col);
      //         Serial.println(")");
      //         #else

      //         // if (incomingByte >= 'a' && incomingByte <= 'z') {
      //         //       // Convert to uppercase
      //         //       char uppercaseByte = incomingByte - ('a' - 'A');
                    
      //         //       // Simulate the uppercase keypress
      //         //       Keyboard.write(uppercaseByte);
      //         // }
              
      //         Keyboard.press(key.key);
      //         #endif
      //       }
      //     } 
      //     else 
      //     {
      //       key.lastState = 0;
      //     }
      //     break;


      //   default:
      //     if (key.lastState < DEBOUNCE_TIME)
      //     {
      //       ++key.lastState;
      //     }
      //     break;
        
      //   case DEBOUNCE_TIME:
      //     if (!currentState) {
      //       key.lastState = 0;
      //       if (!key.specialKey) {
      //         #ifdef DEBUG_SOLDERING
      //         Serial.print("Released: ");
      //         Serial.print(key.name);
      //         Serial.print(" (Row: ");
      //         Serial.print(row);
      //         Serial.print(", Col: ");
      //         Serial.print(col);
      //         Serial.println(")");
      //         #else
      //         Keyboard.release(key.key);
      //         #endif
      //       }
      //     }
      //     break;

      // }
    }
    digitalWrite(pin_column[col],LOW); // Un-activate column
  }
}

void handleSpecialKey(KeyInfo &key) {

#ifdef DEBUG_SOLDERING
      Serial.print("Special: ");
      Serial.println(key.name);
#endif 

   switch (key.specialKey) {
    case 1:  // Cut operation
      #ifdef DEBUG_SOLDERING
      Serial.println("cut");
      #else
      copyPasteInput('x');
      #endif
      break;

    case 2:  // Copy operation
      #ifdef DEBUG_SOLDERING
      Serial.println("copy");
      #else
      copyPasteInput('c');
      #endif
      break;

    case 3:  // Paste operation
      #ifdef DEBUG_SOLDERING
      Serial.println("paste");
      #else
      copyPasteInput('v');
      #endif
      break;

    case 4:  // change numbers shift character
      #ifdef DEBUG_SOLDERING
      Serial.println("numbers");
      #else



      if(hasAltGr())
      {

        switch (key.key) {
          case '5':
            Keyboard.write(45);
            break;
        }
      }
      else if (hasUpperCaseShift())
      {
     
        switch (key.key) {
          case '0':
            storeAwayShift();
            Keyboard.write(92);
            restoreShift();
            // storeAwayShift();
            // Keyboard.press('\\');
            // Keyboard.release('\\');
            // restoreShift();
            
            break;
          case '1':
            storeAwayShift();
            Keyboard.write(64);
            restoreShift();
            //Keyboard.press('2');
            //Keyboard.release('2');
            break;
          case '2':
            storeAwayShift();
            Keyboard.write(45);
            restoreShift();
            // storeAwayShift();
            // Keyboard.press('-');
            // Keyboard.release('-');
            // restoreShift();
            break;
          case '3':
            storeAwayShift();
            Keyboard.write(47);
            restoreShift();
            // storeAwayShift();
            // Keyboard.press('/');
            // Keyboard.release('/');
            // restoreShift();
            break;
          case '4':
            Keyboard.write(124);
            // Keyboard.press('\\');
            // Keyboard.release('\\');
            break;
          case '5':
           
            
              Keyboard.write(38);
            
            // Keyboard.press('7');
            // Keyboard.release('7');
            break;
          case '6':
            Keyboard.write(37);
            // Keyboard.press('5');
            // Keyboard.release('5');
            break;
          case '7':
            Keyboard.write(94);
            // Keyboard.press('6');
            // Keyboard.release('6');

            break;
          case '8':
            storeAwayShift();
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.write(236);
            Keyboard.release(KEY_RIGHT_ALT);
            restoreShift();

            break;
          case '9':
            storeAwayShift();
            Keyboard.write(33);
            restoreShift();
            // Keyboard.press('1');
            // Keyboard.release('1');

            break;
            //code 198 printar tid/datum
        }
      }
      else
      {
 
        Keyboard.press(key.key);
        Keyboard.release(key.key);
      }
      #endif
      break;

    case 5:  // Select program
      #ifdef DEBUG_SOLDERING
      Serial.println("Select program");
      #else
      Keyboard.write(decimal);
      Serial.println(decimal);
      decimal++;
      #endif
      break;
    case 6:  // program 1
      #ifdef DEBUG_SOLDERING
      Serial.println("program 1");
      #else
      Serial.println("temp program 1");
      #endif
      break;

    case 7:  // program 2
      #ifdef DEBUG_SOLDERING
      Serial.println("program 2");
      #else
      Serial.println("temp program 2");
      #endif
      break;

    case 8:  // program 3
      #ifdef DEBUG_SOLDERING
      Serial.println("program 3");
      #else
      Serial.println("temp program 3");
      #endif
      break;

    case 9:  // program 4
      #ifdef DEBUG_SOLDERING
      Serial.println("program 4");
      #else
      Serial.println("temp program 4");
      #endif
      break;

    case 10:  // program 5
      #ifdef DEBUG_SOLDERING
      Serial.println("program 5");
      #else
      Serial.println("temp program 5");
      #endif
      break;

    case 11:  // =?
      #ifdef DEBUG_SOLDERING
      Serial.println("=");
      #else
      if (hasUpperCaseShift())
      {
        Keyboard.write(95);
      }
      else
      {
        Keyboard.write(41);
      }
      #endif
      break;
    case 12:  // (<
      #ifdef DEBUG_SOLDERING
      Serial.println("(");
      #else
      if (hasUpperCaseShift())
      {
        storeAwayShift();
        Keyboard.write(236);
        restoreShift();
      }
      else
      {
        Keyboard.write(42);        
      }
      #endif
      break;
    case 13:  // )>
      #ifdef DEBUG_SOLDERING
      Serial.println(")");
      #else
      if (hasUpperCaseShift())
      {
        Keyboard.write(236);
      }
      else
      {
        Keyboard.write(40);        
      }
      #endif
      break;
    case 14:  // {[
      #ifdef DEBUG_SOLDERING
      Serial.println("[");
      #else
      if (hasUpperCaseShift())
      {
        storeAwayShift();
        Keyboard.press(KEY_RIGHT_ALT);
        Keyboard.write(56);
        Keyboard.release(KEY_RIGHT_ALT);
        restoreShift();
      }
      else
      {
        Keyboard.press(KEY_RIGHT_ALT);
        Keyboard.write(55);//key 0
        Keyboard.release(KEY_RIGHT_ALT);
        
      }
      #endif
      break;
    case 15:  // }]
      #ifdef DEBUG_SOLDERING
      Serial.println("]");
      #else
      if (hasUpperCaseShift())
      {
        storeAwayShift();
        Keyboard.press(KEY_RIGHT_ALT);
        Keyboard.write(57);
        Keyboard.release(KEY_RIGHT_ALT);
        restoreShift();

      }
      else
      {
        Keyboard.press(KEY_RIGHT_ALT);
        Keyboard.write(48);
        Keyboard.release(KEY_RIGHT_ALT);
        
      }
      #endif
      break;
    case 16:  //#_
      #ifdef DEBUG_SOLDERING
      Serial.println("#");
      #else
      if (hasUpperCaseShift())
      {
        Keyboard.write(47);
      }
      else
      {
        Keyboard.write(35);      
      }
      #endif
      break;
    case 18:  //shift enter
      #ifdef DEBUG_SOLDERING
      Serial.println("shift enter");
      #else
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_RETURN);
      Keyboard.release(KEY_RETURN);
      Keyboard.release(KEY_LEFT_SHIFT);
      #endif
      break;
  case 20:  //test
      #ifdef DEBUG_SOLDERING
      Serial.println("non");
      #else
      Serial.println("alt gr");
      #endif
      break;



// Keyboard.press(KEY_RIGHT_ALT);
//      Keyboard.write(236);
//       Keyboard.release(KEY_RIGHT_ALT);
      //Keyboard.write(226);

      // Keyboard.press(KEY_ACUTE_ACC);
      // Keyboard.release(KEY_ACUTE_ACC);

      // Keyboard.press(KEY_UMLAUT);
      // Keyboard.release(KEY_UMLAUT);
      //#endif
      //break;
    // Add more cases for additional special keys
  }
}

void copyPasteInput(char xcv) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(xcv);
  Keyboard.release(xcv);
  Keyboard.release(KEY_LEFT_CTRL);
}

// char upperCaseIfShifted(char letter)
// {
//   if (upperCase())
//   {
//     char uppercaseByte = letter - ('a' - 'A');
//     return uppercaseByte;
//   }
//   else
//   {
//     return letter;
//   }
// }

bool storeAwayAltGr()
{
  storedAltGr = keyMatrix[ALTGR_ROW][ALTGR_COL].lastState > PRESSED_STATE_THRESHOLD;
  if (storedAltGr)
  {
    Keyboard.release(keyMatrix[ALTGR_ROW][ALTGR_COL].key);    
  }
  return storedAltGr;
}
void restoreAltGr()
{
  if (storedAltGr)
  {
    Keyboard.press(keyMatrix[ALTGR_ROW][ALTGR_COL].key);    
  }
}

void storeAwayShift()
{
  storedLeftShift = keyMatrix[LEFT_SHIFT_ROW][LEFT_SHIFT_COL].lastState > PRESSED_STATE_THRESHOLD;
  storedRightShift =  keyMatrix[RIGHT_SHIFT_ROW][RIGHT_SHIFT_COL].lastState > PRESSED_STATE_THRESHOLD;
  
  if (storedLeftShift)
  {
    Keyboard.release(keyMatrix[LEFT_SHIFT_ROW][LEFT_SHIFT_COL].key);    
  }

  if (storedRightShift)
  {
    Keyboard.release(keyMatrix[RIGHT_SHIFT_ROW][RIGHT_SHIFT_COL].key);    
  }
}

void restoreShift()
{
  if (storedLeftShift)
  {
    Keyboard.press(keyMatrix[LEFT_SHIFT_ROW][LEFT_SHIFT_COL].key);    
  }

  if (storedRightShift)
  {
    Keyboard.press(keyMatrix[RIGHT_SHIFT_ROW][RIGHT_SHIFT_COL].key);    
  }
}

bool hasAltGr()
{
  return keyMatrix[ALTGR_ROW][ALTGR_COL].lastState > PRESSED_STATE_THRESHOLD;
}

bool hasUpperCaseShift()
{
  return keyMatrix[LEFT_SHIFT_ROW][LEFT_SHIFT_COL].lastState > PRESSED_STATE_THRESHOLD || 
         keyMatrix[RIGHT_SHIFT_ROW][RIGHT_SHIFT_COL].lastState > PRESSED_STATE_THRESHOLD;
}
