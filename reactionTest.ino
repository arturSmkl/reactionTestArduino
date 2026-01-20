#include <edushield.h>

const uint8_t NUMBER_FONT[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001  // F
}; 

const int buttonPin = 2;

const int redPin = 9;
const int greenPin = 5;
const int bluePin = 6;

bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long lastButtonPress = 0;
int buttonHoldDuration = 1000;
bool buttonHoldMode = true;
unsigned long timer = 0;

unsigned int highScore = 9999;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  ledOff();
  displayOff();
  randomSeed(analogRead(A0));
  Serial.begin(9600);
}

void loop() {
  checkButton();
}

void checkButton() {
  bool currentState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentState == LOW) {
    OnButtonPressed();
  }
  if (lastButtonState == LOW && currentState == HIGH) {
    OnButtonReleased();
  } 

  lastButtonState = currentState;
}

void OnButtonPressed () {
  lastButtonPress = millis();
  
  if (!buttonHoldMode) {
    unsigned long reaction = millis() - timer;

    if (reaction < highScore) {
      highScore = reaction;
    }
    
    showResult(reaction);
  }
}

void OnButtonReleased () {
  if (buttonHoldMode) {
    if ((millis() - lastButtonPress) < buttonHoldDuration) {
        Serial.println("single click");

        buttonHoldMode = false;
        ledOff();
        displayOff();
        BeginCountdown();
      }
      else {
        Serial.println("hold");
        showHighScore();
    }
  } else {
    buttonHoldMode = true;
  }
}

void BeginCountdown() {
  delay(1000);
  Display.num(0, 0);
  delay(1000);
  Display.num(0, 1);
  delay(1000);
  Display.num(0, 2);
  delay(1000);
  Display.num(0, 3);
  delay(random(2000, 4001));
  displayOff();
  timer = millis();
}

void showNumber (unsigned int ms) {
  if (ms > 9999) {
    ms = 9999;
  }


  Serial.println(ms);

  uint8_t d1 = ms / 1000;
  uint8_t d2 = (ms / 100) % 10;
  uint8_t d3 = (ms / 10) % 10;
  uint8_t d4 = ms % 10;

  Display.set4(NUMBER_FONT[d1] | 0x80, NUMBER_FONT[d2], NUMBER_FONT[d3], NUMBER_FONT[d4]);  
}

void showResult(unsigned int ms) {
    showNumber(ms);

    if (ms < 200) {
      digitalWrite(greenPin, LOW);
      return;
    }
    if (ms < 240) {
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, LOW);
      return;
    }
    digitalWrite(redPin, LOW);
}

void showHighScore () {
  showNumber(highScore);
  ledOff();
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
}

void displayOff () {
  Display.set4(0x0, 0x0, 0x0, 0x0);
}

void ledOff () {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}
