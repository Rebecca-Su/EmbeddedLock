#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;
const uint16_t NUMBER_OF_TRIES_MAX = 3;
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

String current = "";
String password = "";
String pin = "";
uint16_t numberOfTries = 1;
bool isSetPassword = true;
bool isSetPIN = true;
bool isEnterPIN = false;

void showEnterPasswordScreen() {
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  String message = "(# Digits)";
  message.replace("#", String(password.length()));
  lcd.print(message);
}

void showEnterPINScreen() {
  lcd.print("Enter PIN");
  lcd.setCursor(0, 1);
  lcd.print("# to return");
}

void showPasswordSetupScreen() {
  lcd.print("Set Password");
  lcd.setCursor(0, 1);
  lcd.print("#=Redo *=Confirm");
}

void showAdminSetupScreen() {
  lcd.print("Set PIN");
  lcd.setCursor(0, 1);
  lcd.print("#=Redo *=Confirm");
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  showAdminSetupScreen();
}

void setPIN(char key) {
  if (key == '#') {
    current = "";
    lcd.clear();
    showAdminSetupScreen();
  } else if (key == '*') {
    isSetPIN = false;
    pin = current;

    current = "";
    lcd.clear();
    showPasswordSetupScreen();
  }
}

void setPassword(char key) {
  if (key == '#') {
    current = "";
    lcd.clear();
    showPasswordSetupScreen();
  } else if (key == '*') {
    isSetPassword = false;
    password = current;

    current = "";
    lcd.clear();
    showEnterPasswordScreen();
  }
}

void enterPassword(char key) {
  if (current.length() >= password.length()) {
    lcd.clear();

    if (current == password) {
      numberOfTries = 1;
      current = "";
      lcd.print("Unlock");
      lcd.setCursor(0, 1);
      lcd.print("Successful");
    } else if (numberOfTries < NUMBER_OF_TRIES_MAX) {
      numberOfTries ++;
      current = "";
      lcd.print("Wrong Password");
    } else {
      numberOfTries = 1;
      current = "";

      lcd.print("Access Denied");
      lcd.setCursor(0, 1);
      String message = "Blocked for # seconds";
      message.replace("#", String(TIMEOUT));
      lcd.print(message);
      delay(1000 * TIMEOUT);

      lcd.clear();
      showEnterPasswordScreen();
    }
  }
}

void enterPIN(char key) {
  if (key == '#') {
    isEnterPIN = false;
    showEnterPasswordScreen();
  }

  if (current.length() >= pin.length()) {
    lcd.clear();

    if (current == pin) {
      current = "";
      isSetPassword = true;
      isEnterPIN = false;
      showPasswordSetupScreen();
    } else {
      current = "";
      isEnterPIN = false;
      lcd.print("Wrong PIN");
      delay(500);
      lcd.clear();
      showEnterPasswordScreen();
    }
  }
}

void processInput(char key) {
  if (current == "") {
    lcd.clear();
  }

  if (key != '*' && key != '#') {
    lcd.print(key);
    current += String(key);
  }

  if (isSetPIN) {
    setPIN(key);
  } else if (isSetPassword) {
    setPassword(key);
  } else if (isEnterPIN) {
    enterPIN(key);
  } else if (key == '*') {
    isEnterPIN = true;
    showEnterPINScreen();
  } else {
    enterPassword(key);
  }
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    processInput(key);
  }
}
