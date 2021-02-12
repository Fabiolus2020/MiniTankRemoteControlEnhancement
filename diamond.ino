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
int animation1 = 1;
int animation2 = 0;

//array containing all templates from the ALA library for Mode 2
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


//I Predefined groups of LEDS for MODE1 this represent the lights for the robot Front lights, back lights, left lights, right lights
int allLEDS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
int frontLEDS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
int leftLEDS[] = {9, 10, 11, 12, 13, 14, 15, 16};
int rightLEDS[] = {17, 18, 19, 20, 21, 22, 23, 24};
int backLEDS[] = {25, 26, 27, 28, 29, 30};


//array of lEDS to work with the FASTLed library
CRGB leds[NUM_LEDS];


//button one is the joystick push button and is used to switch from OFF/Mode1/Mode2 button 2 and 3 are push buttons they are used to move between templates in either Mode 1 or Mode 2 (button2 is forward and button3 is backward)
ezButton button1(A5);  // create ezButton object that attach to pin 7;
ezButton button2(A4);  // create ezButton object that attach to pin 8;
ezButton button3(A3);  // create ezButton object that attach to pin 8;



//I created functions that represent the 5 differents template in Mode1, lights for the front, lights for the back, left,right and all light bright white.
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

//function representing mode 2 to start the showcase of lights
void mode2A() {
  rgbStrip.runAnimation();
}


void setup() {
  Serial.begin(9600);

  //initialize LED strip
  rgbStrip.initWS2812(30, 3);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(3, 500);
  FastLED.setBrightness(255);

  //The below I use for debounce issue using the EZbutton library
  button1.setDebounceTime(70); // set debounce time to 50 milliseconds
  button2.setDebounceTime(70); // set debounce time to 50 milliseconds
  button3.setDebounceTime(70); // set debounce time to 50 milliseconds


  //clear everything on LED strip and start clean
  FastLED.clear();  // clear all pixel data
  FastLED.show();

}

void loop() {

  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first

  //from EZbutton library to keep count of button1 and simply reset once it reaches 3
  unsigned long button1count = button1.getCount();


  //if button 1 is pressed counter1 will increment then look to see if over 3 and reset if needed then clear LED strip.
  if (button1.isPressed()) {
    animation1 = 1;
    animation2 = 0;
    if (button1count >= 3)
      button1.resetCount();
    FastLED.clear();  // clear all pixel data
    FastLED.show();

  }

  //if button 2 is pressed and button1counter will increment for either mode1 or mode 2 state then clear LED strip and start the animation .
  if (button2.isPressed() && button1count == 1) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    if (animation1 >= 5) {
      animation1 = 1;
    }
    animation1++;
  }

  if (button2.isPressed() && button1count == 2) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    if (animation2 >= 21) {
      animation2 = 0;
    }
    animation2++;
    updateAnimation2();

  }




  //if button 3 pressed counter will decrement for mode 2 only it can cycle 21 led templates.
  if (button3.isPressed() && button1count == 1) {
    FastLED.clear();  // clear all pixel data
    FastLED.show();
    if (animation1 <= 1) {
      animation1 = 5;
    }
    animation1--;
  }

  if (button3.isPressed() && button1count == 2) {
    if (animation2 <= 0) {
      animation2 = 21;
    }
    animation2--;
    updateAnimation2();

  }


  //the above statements are to control Mode1 and Mode2, mode 1 is to control white leds front, back, left, right, all. Mode 2 is to enable showcase of different leds templates
  if (button1count == 1)  {

    if (animation1 == 1) {
      lightsforwardOnly();
    }
    else if
    (animation1 == 2) {
      lightsbackOnly();
    }
    else if
    (animation1 == 3) {
      lightsleftOnly();
    }
    else if
    (animation1 == 4) {
      lightsrightOnly();
    }
    else if
    (animation1 == 5) {
      lightsOnAll();
    }
  }

  if (button1count == 2) {

    if (animation2 >= 21) {
      animation2 = 0;
    }

    updateAnimation2();
    mode2A();

  }


  Serial.println(animation1);
  Serial.println(animation2);

}
//fucntion to run the 21 animations/templates.
void updateAnimation2()
{
  rgbStrip.setAnimation(animList[animation2 % 21], 3000, alaPalRgb);
}
