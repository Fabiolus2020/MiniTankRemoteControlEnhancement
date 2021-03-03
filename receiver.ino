//Fabiolus 2021/02/23
//the_fabiolous@otmail.com
#include <FastLED.h>
#include <AlaLedRgb.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 myRadio(8, 9); // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

struct package
{
  int joyposX;
  int joyposY;
  int potValue1;
  int potValue2;
  int modeCount;
  int buttonJoystickCount;
  int buttonForwardCount;
  int animation1Count;
  int animation2Count;
};

typedef struct package Package;
Package data;

//define pins for LED strip and number of LEDS on strip
#define LED_PIN     5
#define NUM_LEDS    27

//create object representing led strip to work with ALA library
AlaLedRgb rgbStrip;


//I Predefined groups of LEDS for MODE1 this represent the lights for the robot Front lights, back lights, left lights, right lights
int allLEDS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
int frontLEDS[] = {4, 5, 6, 7, 8};
int leftLEDS[] = {23, 24, 25, 26, 0, 1, 2, 3};
int rightLEDS[] = {9, 10, 11, 12, 13, 14, 15, 16, 17};
int backLEDS[] = {18, 19, 20, 21, 22};


//array containing all templates from the ALA library for Mode 2
int animList[] = {
  ALA_SPARKLE,
  ALA_SPARKLE2,
  ALA_PIXELSMOOTHSHIFTRIGHT,
  ALA_PIXELSMOOTHSHIFTLEFT,
  ALA_COMET,
  ALA_BUBBLES,
  ALA_BLINKALT
};

//array of lEDS to work with the FASTLed library
CRGB leds[NUM_LEDS];

//#define FASTLED_ALLOW_INTERRUPTS 0

#define enA 10   // Note: Pin 9 in previous video ( pin 10 is used for the SPI communication of the NRF24L01)
#define in1 4
#define in2 A5
#define enB 3   // Note:  Pin 10 in previous video
#define in3 6
#define in4 7

// Set initial motor direction as forward and speed at 0
int motorSpeedA = 0;
int motorSpeedB = 0;
int motordirection = 0;
int motorspeed1 = 0;
int motorspeed2 = 0;

int lastanimation1CountButtonState = 0;
int animation1CountButtonState = 0;

//I created 5 functions that represent the 5 differents template in Mode1, lights for the front, lights for the back, left,right and all light bright white.
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

  for (int i = 0; i < 5; i++)
  {
    leds[frontLEDS[i] ] = CRGB::White;
  }

  FastLED.show();
}

//function to lit back lights
void lightsbackOnly() {

  for (int i = 0; i < 5; i++)
  {
    leds[backLEDS[i] ] = CRGB::White;
  }
  FastLED.show();
}

//function to lit right lights
void lightsrightOnly() {

  for (int i = 0; i < 9; i++)
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


void modeOne()
{
  if (data.joyposX < 470)
  {
    // This is forward
    motordirection = 1;
    //Determine Motor Speeds

    motorspeed1 = map(data.joyposX, 470, 0, 0, 255);
    motorspeed2 = map(data.joyposX, 470, 0, 0, 255);

  }
  else  if (data.joyposX > 600)
  {
    // This is backward
    motordirection = 2;

    //Determine Motor Speeds
    motorspeed1 = map(data.joyposX, 600, 1023, 0, 255);
    motorspeed2 = map(data.joyposX, 600, 1023, 0, 255);
  }
  else if (data.joyposY < 470) {
    // This is left
    motordirection = 3;

    //Determine Motor Speeds
    motorspeed1 = map(data.joyposY, 470, 0, 0, 255);
    motorspeed2 = map(data.joyposY, 470, 0, 0, 255);
  }
  else if (data.joyposY > 600) {
    // This is right
    motordirection = 4;

    motorspeed1 = map(data.joyposY, 600, 1023, 0, 255);
    motorspeed2 = map(data.joyposY, 600, 1023, 0, 255);
  }
  else
  {
    // This is Stopped
    motordirection = 0;
    motorspeed1 = 0;
    motorspeed2 = 0;
  }

  if (motordirection == 1)
  {
    // Motors are forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else  if (motordirection == 2)
  {
    // Motors are bacward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  else  if (motordirection == 3)
  {
    // Motors are bacward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else  if (motordirection == 4)
  {
    // Motors are bacward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  // Drive Motors
  analogWrite(enA, motorspeed1);
  analogWrite(enB, motorspeed2);
  // delay(30);
}


void modeTwo()
{
  // X-axis used for forward and backward control
  if (data.joyposX < 470) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.joyposX, 470, 0, 0, 255);
    motorSpeedB = map(data.joyposX, 470, 0, 0, 255);
  }
  else if (data.joyposX > 550) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.joyposX, 550, 1023, 0, 255);
    motorSpeedB = map(data.joyposX, 550, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // Y-axis used for left and right control
  //this is left
  if (data.joyposY < 470) {

    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.joyposY, 470, 0, 0, 255);
    motorSpeedB = map(data.joyposY, 470, 0, 0, 255);
  }

  if (data.joyposY > 550) {

    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.joyposY, 550, 1023, 0, 255);
    motorSpeedB = map(data.joyposY, 550, 1023, 0, 255);
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
}

void modeThree()
{
  // X-axis used for forward and backward control
  if (data.joyposX > 600) {
    // Set Motor A backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue1, 0, 1023, 0, 255);
  }
  else if (data.joyposX < 400) {
    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue1, 0, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // Y-axis used for left and right control
  //this is left
  if (data.joyposY < 400) {

    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue1, 0, 1023, 0, 255);
  }

  if (data.joyposY > 600) {

    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue1, 0, 1023, 0, 255);
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B

}

void modeFour()
{
  // X-axis used for forward and backward control
  if (data.joyposX > 600) {
    // Set Motor A backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue2, 0, 1023, 0, 255);
  }
  else if (data.joyposX < 400) {
    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue2, 0, 1023, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // Y-axis used for left and right control
  //this is left
  if (data.joyposY < 400) {

    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue2, 0, 1023, 0, 255);
  }

  if (data.joyposY > 600) {

    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(data.potValue1, 0, 1023, 0, 255);
    motorSpeedB = map(data.potValue2, 0, 1023, 0, 255);
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B

}



//function representing mode 2 to start the showcase of lights
void mode2A() {
  rgbStrip.runAnimation();
}

void setup() {

  while (!Serial);

  Serial.begin(9600);
  myRadio.begin();
  //set the address
  myRadio.openReadingPipe(0, address);
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  //Set module as receiver
  myRadio.startListening();
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // Set Motor to low initially
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);


  //initialize LED strip
  rgbStrip.initWS2812(30, 5);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 1200);
  FastLED.setBrightness(10);

  //clear everything on LED strip and start clean
  FastLED.clear();  // clear all pixel data
  FastLED.show();

}

void loop() {

  if ( myRadio.available())
  {


    myRadio.read( &data, sizeof(data) );


    animation1CountButtonState = data.animation1Count;

    if (animation1CountButtonState != lastanimation1CountButtonState) {

      FastLED.clear();  // clear all pixel data
      FastLED.show();

      lastanimation1CountButtonState = animation1CountButtonState;

    }




    if (data.modeCount == 1)
    {

      modeOne();
    }
    if (data.modeCount == 2)
    {
      modeTwo();
    }
    if (data.modeCount == 3)
    {
      modeThree();
    }
    if (data.modeCount == 4)
    {
      modeFour();
    }


    //the above statements are to control Mode1 and Mode2, mode 1 is to control white leds front, back, left, right, all. Mode 2 is to enable showcase of different leds templates


    if (data.buttonJoystickCount == 3) {
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }

    if (data.animation1Count == 1 && data.buttonJoystickCount == 1) {
      lightsforwardOnly();
    }
    else if
    (data.animation1Count == 2 && data.buttonJoystickCount == 1) {
      lightsbackOnly();
    }
    else if
    (data.animation1Count == 3 && data.buttonJoystickCount == 1) {
      lightsleftOnly();
    }
    else if
    (data.animation1Count == 4 && data.buttonJoystickCount == 1) {
      lightsrightOnly();
    }
    else if
    (data.animation1Count == 5 && data.buttonJoystickCount == 1) {
      lightsOnAll();
    }



    if (data.buttonJoystickCount == 2) {
      updateAnimation2();
      mode2A();
    }


  }


}
void updateAnimation2()
{
  rgbStrip.setAnimation(animList[data.animation2Count % 7], 3000, alaPalRgb);
}
