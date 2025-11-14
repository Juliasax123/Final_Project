#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=186,185
AudioFilterStateVariable filter1;        //xy=409,194
AudioMixer4              mixer1;         //xy=572,180
AudioOutputI2S           i2s2;           //xy=755,180
AudioConnection          patchCord1(i2s1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 0, mixer1, 0);
AudioConnection          patchCord3(filter1, 1, mixer1, 1);
AudioConnection          patchCord4(filter1, 2, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s2, 0);
AudioConnection          patchCord6(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=381,98
// GUItool: end automatically generated code

int buttonPin = 24;
bool lastButtonState;
bool buttonState;
bool buttonOn;

int bassPotPin = A11;
int midPotPin = A12;
int highPotPin = A13;

int bassFrq;
int midFrq;
int highFrq;

int bassColor;
int midColor;
int highColor;

void setup() {
  // put your setup code here, to run once:
  AudioMemory(500);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(8);

  pinMode(buttonPin, INPUT);

  pinMode(bassPotPin, INPUT);
  pinMode(midPotPin, INPUT);
  pinMode(highPotPin, INPUT);

  Serial.begin(9600);
}

void checkButton() {
  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (lastButtonState == LOW and buttonState == HIGH) {
    buttonOn = !buttonOn;
    delay(5);
  }

  else (lastButtonState == HIGH and buttonState == LOW); {
    delay(5);
  }
}

void checkMode(){
  if (buttonOn == true){          // if buttonOn is true, enter filter mode
    bassFrq = analogRead(bassPotPin);
    midFrq = analogRead(midPotPin);
    highFrq = analogRead(highPotPin);

    Serial.println ("Frq");
  }

  else{       // else (aka if buttonOn is false), enter color select mode
    bassColor = analogRead(bassPotPin);
    midColor = analogRead(midPotPin);
    highColor = analogRead(highPotPin);

    Serial.println ("Color");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButton();
  checkMode();
}