// below is the code from the online system design tool
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s1;                //xy=176,165
AudioFilterStateVariable filter3;  //xy=293,371
AudioFilterStateVariable filter2;  //xy=313,275
AudioFilterStateVariable filter1;  //xy=319,149
AudioAnalyzePeak peak3;            //xy=431,452
AudioAnalyzePeak peak2;            //xy=460,268
AudioAnalyzePeak peak1;            //xy=503,68
AudioMixer4 mixer1;                //xy=640,206
AudioOutputI2S i2s2;               //xy=812,215
AudioConnection patchCord1(i2s1, 0, filter1, 0);
AudioConnection patchCord2(i2s1, 0, filter2, 0);
AudioConnection patchCord3(i2s1, 0, filter3, 0);
AudioConnection patchCord4(filter3, 2, peak3, 0);
AudioConnection patchCord5(filter3, 2, mixer1, 2);
AudioConnection patchCord6(filter2, 1, peak2, 0);
AudioConnection patchCord7(filter2, 1, mixer1, 1);
AudioConnection patchCord8(filter1, 0, peak1, 0);
AudioConnection patchCord9(filter1, 0, mixer1, 0);
AudioConnection patchCord10(mixer1, 0, i2s2, 0);
AudioConnection patchCord11(mixer1, 0, i2s2, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=362,51
// GUItool: end automatically generated code

// include the neopixel library
#include <Adafruit_NeoPixel.h>

// set up a switch to control whether the lights are an off white or responding to music and bools to read the state of the switch and bools to read it
int switchPin = 28;
bool switchState;

// set up a button to switch between modes and bools to read what mode you are in
int buttonPin = 24;
bool lastButtonState;
bool buttonState;

// set up a menu to rotate between filter mode, bass color select, mid color select, and high color select
int buttonMenu = 0;

// set up the three pots
int pot1Pin = A11;
int pot2Pin = A12;
int pot3Pin = A13;

// set up a variable to set the number of neopixels
int numPixels = 50;
int pixelPin = 29;

// set up the neopixels
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(numPixels, pixelPin, NEO_GRB);

// set up volume variables to create the boosting/attenuating
float bassVol;
float midVol;
float highVol;

// set up the variables to use in the color mixes for bass, mids, and highs
int pot1Color;
int pot2Color;
int pot3Color;

// set up variables map the color variables to be within 0 - 255
int mapPot1Color;
int mapPot2Color;
int mapPot3Color;

// set up arrays to have the color mixes for bass, mids, and highs to put into the overall color mix
int bassColor[3];
int midColor[3];
int highColor[3];

// set up a variable for the overall color
int outputColor[3];

void setup() {

  // set up the audio board
  AudioMemory(500);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(8);

  // set the switch, button, and pots to inputs
  pinMode(switchPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(pot3Pin, INPUT);

  // set the neopixels to output
  pinMode(pixelPin, OUTPUT);

  // initialize / reset the neopixels
  neopixel.begin();
  neopixel.clear();
  neopixel.show();

  // set up serial to be able to see the things being printed
  Serial.begin(9600);

  // set the frequencies for the lowpass, bandpass, and highpass filters
  filter1.frequency(299);
  filter2.frequency(1999);
  filter3.frequency(2000);
}

// set up a function to rotate between the button's settings (filter mode, bass color, mid color, and high color)
void changeMenu() {

  // as long as buttonMenu is less than 3 (as there are 4 modes 0-3, but after 3 it needs to reset to 0), add one to continue rotating through
  if (buttonMenu < 3) {
    buttonMenu += 1;
  }

  // if buttonMenu is 3 or above, reset it to 0
  else {
    buttonMenu = 0;
  }
}

// set up a function to check for the moment the button is pressed
void checkButton() {

  // check if the button is pressed currently or not
  buttonState = digitalRead(buttonPin);

  // if the button was previously not pushed and now it is...
  if (lastButtonState == LOW and buttonState == HIGH) {

    // change the mode
    changeMenu();
    delay(5);
  }

  // set lastButtonState to what buttonState jsut was to get an accurate reading for the next loop
  lastButtonState = buttonState;
}

// set up a function to check what mode you are in and respond accordingly
void checkMode() {

  // if buttonMenu is 0 enter filter mode
  if (buttonMenu == 0) {

    // read in the values for the volume and set the volume variables to those values
    bassVol = analogRead(pot1Pin);
    midVol = analogRead(pot2Pin);
    highVol = analogRead(pot3Pin);

    // sets the level of the bass, mids, and highs
    mixer1.gain(0, bassVol);
    mixer1.gain(1, midVol);
    mixer1.gain(2, highVol);

    // print out "Filter mode" to check if it is working
    Serial.println("Filter mode");
  }

  else {

    // read in the values for the colors and set the pot color variables to those values
    pot1Color = analogRead(pot1Pin);
    pot2Color = analogRead(pot2Pin);
    pot3Color = analogRead(pot3Pin);

    // map the color variables to be within 0 - 255 for colors
    mapPot1Color = map(pot1Color, 0, 1023, 0, 255);
    mapPot2Color = map(pot2Color, 0, 1023, 0, 255);
    mapPot3Color = map(pot3Color, 0, 1023, 0, 255);

    // if buttonMenu is 1 enter bass color selection mode
    if (buttonMenu == 1) {

      // set the mapped pot color values to the overall bass color value
      bassColor[0] = mapPot1Color;
      bassColor[1] = mapPot2Color;
      bassColor[2] = mapPot3Color;

      // print out "Bass color mode" to check if it is working
      Serial.println("Bass color mode");

      // reads the amplitude of the line in
      if (peak1.available() == true) {
        int peak = peak1.read() * 50.0;

        // dispaly the bass color on the pixels
        for (int i = 0; i < peak; i++) {
          neopixel.setPixelColor(i, mapPot1Color, mapPot2Color, mapPot3Color);
          neopixel.show();
        }
      }
    }

    // if buttonMenu is 2 enter mid color selection mode
    else if (buttonMenu == 2) {

      // set the mapped pot color values to the overall mid color value
      midColor[0] = mapPot1Color;
      midColor[1] = mapPot2Color;
      midColor[2] = mapPot3Color;

      // print out "Mid color mode" to check if it is working
      Serial.println("Mid color mode");

      // reads the amplitude of the line in
      if (peak1.available() == true) {
        int peak = peak1.read() * 50.0;

        // dispaly the mid color on the pixels
        for (int i = 0; i < peak; i++) {
          neopixel.setPixelColor(i, mapPot1Color, mapPot2Color, mapPot3Color);
          neopixel.show();
        }
      }
    }

    // if buttonMenu is 3 enter high color selection mode
    else if (buttonMenu == 3) {
      highColor[0] = mapPot1Color;
      highColor[1] = mapPot2Color;
      highColor[2] = mapPot3Color;

      // print out "High color mode" to check if it is working
      Serial.println("High color mode");

      // reads the amplitude of the line in
      if (peak1.available() == true) {
        int peak = peak1.read() * 50.0;

        // dispaly the high color on the pixels
        for (int i = 0; i < peak; i++) {
          neopixel.setPixelColor(i, mapPot1Color, mapPot2Color, mapPot3Color);
          neopixel.show();
        }
      }
    }
  }
}

// set up a function to display the overall color based on the colors selected for each frequncy range and their volume settings
void showColors() {

  // mix the colors together taking their volumes into account
  for (int i = 0; i < 3; i++) {
    outputColor[i] = (bassColor[i]) + (midColor[i]) + (highColor[i]);
  }

  // reads the amplitude of the line in
  if (peak1.available() == true) {
    int peak = peak1.read() * 50.0;

    // dispaly the overall color on the pixels
    for (int i = 0; i < peak; i++) {
      neopixel.setPixelColor(i, outputColor[0], outputColor[1], outputColor[2]);
      neopixel.show();
    }
  }
}

void loop() {

  // read the siwtch's state
  switchState = digitalRead(switchPin);

  // if the switch is on...
  if (switchState == HIGH) {

    // reset the button menu to 0 so everytime the switch is turned on it enters filter mode
    buttonMenu = 0;

    // apply the filter and color settings
    checkButton();
    checkMode();

    // as long as the color selections are not being edited show the overall color
    if (buttonMenu == 0) {
      showColors();
    }
  }

  // if the switch is off display a steady color set by the pots
  else {

    // read in the values for the colors and set the pot color variables to those values
    pot1Color = analogRead(pot1Pin);
    pot2Color = analogRead(pot2Pin);
    pot3Color = analogRead(pot3Pin);

    // map the color variables to be within 0 - 255 for colors
    mapPot1Color = map(pot1Color, 0, 1023, 0, 255);
    mapPot2Color = map(pot2Color, 0, 1023, 0, 255);
    mapPot3Color = map(pot3Color, 0, 1023, 0, 255);

    // reads the amplitude of the line in
    if (peak1.available() == true) {
      int peak = peak1.read() * 50.0;

      // dispaly the color on the pixels
      for (int i = 0; i < peak; i++) {
        neopixel.setPixelColor(i, mapPot1Color, mapPot2Color, mapPot3Color);
        neopixel.show();
      }
    }

    // print out "Switch off" to check if it is working
    Serial.println("Switch off");
  }
}