#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;
const uint16_t NUMBER_OF_TRIES_MAX = 3;
const uint16_t NUMBER_OF_DIGITS = 4;
const uint16_t TIMEOUT = 30;

/* Display */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Keypad setup */
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'#', '0', '*'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

/* Enter password */
void showDefaultScreen() {
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  String message = "(# Digits)";
  message.replace("#", String(NUMBER_OF_DIGITS));
  lcd.print(message);
}

void showSetupScreen() {
  lcd.print("Setup (Max 16)");
  lcd.setCursor(0, 1);
  lcd.print("#=Redo *=Confirm");
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  showSetupScreen();
}

String current = "";
String PASSWORD = "";
uint16_t NUMBER_OF_TRIES = 1;
bool isFirstTime = true;

void processInput(char key) {
  if (current == "") {
    lcd.clear();
  }

  if (key != '*' && key != '#') {
    lcd.print(key);
    current += String(key);
  }

  if (isFirstTime) {
    if (key == '#') {
      current = "";
      lcd.clear();
      showSetupScreen();
    } else if (key == '*') {
      isFirstTime = false;
      PASSWORD = current;
      current = "";
      lcd.clear();
      showDefaultScreen();
    }
  } else {
    if (current.length() >= PASSWORD.length()) {
      lcd.clear();
      if (current == PASSWORD) {
        NUMBER_OF_TRIES = 1;
        current = "";
        /* This will be replaced by the actual unlock of the lock*/ 
        lcd.print("Unlock");
        lcd.setCursor(0, 1);
        lcd.print("Successful");
      } else if (NUMBER_OF_TRIES < NUMBER_OF_TRIES_MAX) {
        NUMBER_OF_TRIES ++;
        current = "";
        lcd.print("Wrong Password");
      } else {
        NUMBER_OF_TRIES = 1;
        current = "";

        lcd.print("Access Denied");
        lcd.setCursor(0, 1);
        String message = "Blocked for # seconds";
        message.replace("#", String(TIMEOUT));
        lcd.print(message);
        delay(1000 * TIMEOUT);

        lcd.clear();
        showDefaultScreen();
      }
    }
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    processInput(key);
  }
}
