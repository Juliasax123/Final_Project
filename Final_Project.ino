// set up a switch to control whether the lights are an off white or responding to music and bools to read the state of the switch and bools to read it
int switchPin = ____;
bool switchMode;
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

// set up volume variables to create the boosting/attenuating
int bassVol;
int midVol;
int highVol;

// set up the variables to use in the color mixes for bass mids and highs
float pot1Color;
float pot2Color;
float pot3Color;

// set up variables map the color variables to be within 0 - 255
float mapPot1Color;
float mapPot2Color;
float mapPot3Color;

// set up arrays to have the color mixes for bass, mids, and highs to put into the overall color mix
float bassColor[3] = { 80, 80, 80 };
float midColor[3] = { 160, 160, 160 };
float highColor[3] = { 240, 240, 240 };

// set up a variable for the overall color
float outputColor[3] = {0,0,0};

void setup() {
  AudioMemory(500);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(8);

  // set the switch, button, and pots to inputs
  pinMode(swithcPin, INPUT);

  pinMode(buttonPin, INPUT);

  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(pot3Pin, INPUT);
}

// set up a function to rotate between the button's settings (filter mode, bass color, mid color, and high color)
void changeMenu() {

  // as long as buttonMenu is less than 4 (as the modes are 0-3), add one to continue rotating through
  if (buttonMenu < 4) {
    buttonMenu += 1;
  }

  // if buttonMenu is 4 or above, reset it to 0
  else {
    buttonMenu = 0;
  }
}

// set up a function to check for the moment the button is pressed
void checkButton() {

  // check if the button is pressed currently or not
  buttonState = digitalRead(buttonPin);

  // if the button was previously not pushed and now it is...
  if (lastButtonState != buttonState) {

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
      bassColor = { mapPot1Color, mapPot2Color, mapPot3Color };

      // ADD COMMENT HERE
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
      midColor = { mapPot1Color, mapPot2Color, mapPot3Color };

      // ADD COMMENT HERE
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
      highColor = { mapPot1Color, mapPot2Color, mapPot3Color };

      // ADD COMMENT HERE
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
  for(int i = 0; i < 3; i ++){
    outputColor[i] = (bassColorRatio * bassColor[i]) + (midColorRatio * midColor[i]) + (highColorRatio * highColor[i]);
  }

  // ADD COMMENT HERE
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
  switchState = digitalread(switchMode);

  // if the switch is on the volume and color settings are applied
  if (switchState == HIGH) {
    checkButton();
    checkMode();

    // as long as the color selections are not being edited show the overall color
    if (buttonMenu == 0){
      showColors();
    }
  }

  // if the switch is off display a steady off-white color
  else {
    
  }
}