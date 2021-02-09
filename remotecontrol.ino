/*
  this sketch uses as well ALA library eliminating Delay()
  https://github.com/bportaluri/ALA/archive/master.zip

   Tutorial page for EZButton: https://arduinogetstarted.com/tutorials/arduino-button-library

   This example:
     + uses debounce for multiple buttons.
     + reads state of multiple buttons
     + detects the pressed and released events of multiple buttons
*/
//define libraries
#include <AlaLedRgb.h>
#include <ezButton.h>
#include <FastLED.h>

//define pins for LED strip and number of LEDS on strip
#define LED_PIN     3
#define NUM_LEDS    30


//create object representing led strip to work with ALA library
AlaLedRgb rgbStrip;

//initial value of animation out of 21
int animation = 0;


//array containing all templates from the ALA library
int animList[21] = {
  ALA_ON,
  ALA_SPARKLE,
  ALA_SPARKLE2,
  ALA_PIXELSHIFTRIGHT,
  ALA_PIXELSMOOTHSHIFTRIGHT,
  ALA_PIXELSMOOTHSHIFTLEFT,
  ALA_MOVINGBARS,
  ALA_COMET,
  ALA_COMETCOL,
  ALA_GLOW,
  ALA_CYCLECOLORS,
  ALA_FADECOLORS,
  ALA_FIRE,
  ALA_BOUNCINGBALLS,
  ALA_BUBBLES,
  ALA_STROBO,
  ALA_GLOW,
  ALA_FADECOLORSLOOP,
  ALA_FADEIN,
  ALA_FADEOUT,
  ALA_BLINKALT
};


//Predefined group of LEDS for MODE1 this represent the lights for the robot Front lights, back lights, left lights, right lights
int allLEDS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
int frontLEDS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
int leftLEDS[] = {9, 10, 11, 12, 13, 14, 15, 16};
int rightLEDS[] = {17, 18, 19, 20, 21, 22, 23, 24};
int backLEDS[] = {25, 26, 27, 28, 29, 30};

//array of lEDS to work with the FASTLed library
CRGB leds[NUM_LEDS];


//buttons on the joystick, button one is the joystick push button
ezButton button1(A5);  // create ezButton object that attach to pin 7;
ezButton button2(A4);  // create ezButton object that attach to pin 8;
ezButton button3(A3);  // create ezButton object that attach to pin 8;

//function to lit all lights
void lightsOnAll() {
  for (int i = 0; i < 30; i++)
  {
    leds[allLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//function to turn all lights off
void lightsOffAll() {
  for (int i = 0; i < 30; i++)
  {
    leds[allLEDS[i] ] = CRGB::Black;
  }
  FastLED.show();
}

//function to lit front lights
void lightsforwardOnly() {
  for (int i = 0; i < 9; i++)
  {
    leds[frontLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//function to lit back lights
void lightsbackOnly() {
  for (int i = 0; i < 6; i++)
  {
    leds[backLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//function to lit right lights
void lightsrightOnly() {
  for (int i = 0; i < 8; i++)
  {
    leds[rightLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//function to lit left lights
void lightsleftOnly() {
  for (int i = 0; i < 8; i++)
  {
    leds[leftLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//fucntion representing mode 2 to start the showcase of lights
void mode2A() {
  rgbStrip.runAnimation();
}


void setup() {
  Serial.begin(9600);

  rgbStrip.initWS2812(30, 3);


  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(3, 500);
  FastLED.setBrightness(255);

  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds
  button3.setDebounceTime(50); // set debounce time to 50 milliseconds


  //clear everything on LED strip and start clean
  FastLED.clear();  // clear all pixel data
  FastLED.show();

}

void loop() {

  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first


  unsigned long button1count = button1.getCount();
  unsigned long button2count = button2.getCount();
  unsigned long button3count = button3.getCount();

  // Serial.println(button1count);
  // Serial.println(button3count);


  //button counters for the 3 buttons
  if (button1count >= 3)
    button1.resetCount();

  if (button2count >= 5)
    button2.resetCount();

  if (button3count >= 21)
    button3.resetCount();

  //if button 1 pressed counter will increment then clear LED strip and start the animation , take note the animation start on second click of the joystick
  if (button1.isPressed()) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    //  Serial.println("The button is pressed");
    updateAnimation();
  }

  //if button 2 pressed counter will increment for either mode1 or mode 2 state then clear LED strip and start the animation .
  if (button2.isPressed()) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    if (animation >= 21) {
      animation = 0;
    }
    animation++;
    updateAnimation();
  }

  //if button 3 pressed counter will decrement for mode 2 only it can cycle 21 led templates.
  if (button3.isPressed()) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    if (animation <= 0) {
      animation = 21;
    }
    animation--;
    updateAnimation();
  }


  //the above statements are to control Mode1 and Mode2, mode 1 is to control white leds front, back, left, right, all. Mode 2 is to enable showcase of different leds templates
  if (button1count == 1 && button2count == 0) {
    lightsforwardOnly();
    Serial.println("Mode1A forward lights On");
  }

  else  if (button1count == 1 && button2count == 1) {
    lightsbackOnly();
    Serial.println("Mode1B back lights On");
  }

  else  if (button1count == 1 && button2count == 2) {
    lightsleftOnly();
    Serial.println("Mode1C left lights On");
  }

  else  if (button1count == 1 && button2count == 3) {
    lightsrightOnly();
    Serial.println("Mode1D right lights On");
  }

  else  if (button1count == 1 && button2count == 4) {
    lightsOnAll();
    Serial.println("Mode1E all lights On");
  }


  if (button1count == 2) {
    // lightsOnAll();
    Serial.println("Mode2A On");
    mode2A();
  }

}
//fucntion to run the 21 animations/templates.
void updateAnimation()
{
  rgbStrip.setAnimation(animList[animation % 21], 3000, alaPalRgb);
}
