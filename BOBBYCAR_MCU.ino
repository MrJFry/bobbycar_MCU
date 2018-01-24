//------------------------------------ BOBBYCAR_MCU ------------------------//

//LED ANORDNUNG: MCU > (0,1,2,3,4,5,6,7,8,9), (10),   (11,12),       (13,14)
//                      Rücklicht           , Status, Blinker links, Blinker rechts

#include"FastLED.h"                       //Include libs
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

#define NUM_LEDS 10                        //Number of LEDs
#define DATA_PIN 3                        //Data PIN on D3
#define CLOCK_PIN 2                       //Clock PIN D2
#define LED_SPEED 500                     //Cycle speed
uint8_t max_bright=10;                   //define brightness limiter
CRGB leds[NUM_LEDS];                      //LED definition

//INPUTS-------------------------
int sensorPinx = A0;                      //FOR TESTING ONLY!!!!!!!!!!! --> Later for measuring speedvalue
int sensorPiny = A1;                      //FOR TESTING ONLY!!!!!!!!!!!
int BLINKERlinks = 4;                     //Blinker links
int BLINKERrechts = 5;                    //Blinker rechts
int BREAK = 7;                            //BREMSE
int CL15 = 12;                            //Zündungssignal
//int Ubattery = A1;
//int SPEEDsensor =A0;

//OUTPUTS-------------------------
int MOTOR = 10;                           //Motor output pin

//set Sensor variables to 0
int sensorValuex = 0;
int sensorValuey = 0;   
int outputValue = 0;  // value output to the PWM (analog out)

//------------------------------------ setup section ------------------------//

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);                                    //initialize LED Array
  FastLED.setBrightness(max_bright);                                                                    //Limit the brightness
  pinMode(CL15, INPUT);
  LEDstartup();                                                                                         //Perform LEDstartup animation
}

//------------------------------------ loop section------------------------//

void loop() {
  
  while (digitalRead(CL15) != HIGH) {


if(sensorValuey < 200 ) {
  analogWrite(MOTOR, outputValue);
}
else {
  outputValue = 0;
}

outputValue = map(sensorValuey, 550, 1023, 0, 255);
analogWrite(MOTOR, outputValue);


  //--------------------handle light operations--------------------------//
mainlight();

 sensorValuex = analogRead(sensorPinx);
 sensorValuey = analogRead(sensorPiny);

 if(sensorValuex < 200) {
  TURNright();
 }
 else if(sensorValuex > 800) {
  TURNleft();
 }
  else {
  mainlight();
 }

 if(sensorValuey > 800) {
  BREAKlight();
 }
  else {
    mainlight();
  }
//--------------------/handle light operations--------------------------//

  }
 nolight();   //Wenn CL15 aus ist, alle Lichter ausschalten
}





//------------------------------------ functions ------------------------//
void LEDstartup (void) {
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(255,255,255);                    //Give LED RGB value from array
     FastLED.show();                                                                                     //Display LED
     delay(100);
  }
}
void mainlight (void) {
  float redlight[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 0, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 0, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 128, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 0, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 0, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 0, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 0, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 128, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 0, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 0, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(redlight[Ledpos][0],redlight[Ledpos][1],redlight[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
}

void TURNleft (void) {
  float TURNleft1[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 140, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 140, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 128, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 0, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 0, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 0, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 0, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 128, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 0, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 0, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
   float TURNleft0[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 0, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 0, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 128, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 0, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 0, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 0, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 0, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 128, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 0, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 0, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(TURNleft1[Ledpos][0],TURNleft1[Ledpos][1],TURNleft1[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
     delay(250);
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(TURNleft0[Ledpos][0],TURNleft0[Ledpos][1],TURNleft0[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
     delay(250);
}

void TURNright (void) {
  float TURNright1[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 0, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 0, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 128, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 0, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 0, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 0, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 0, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 128, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 140, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 140, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
   float TURNright0[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 0, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 0, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 128, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 0, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 0, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 0, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 0, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 128, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 0, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 0, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(TURNright1[Ledpos][0],TURNright1[Ledpos][1],TURNright1[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
     delay(250);
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(TURNright0[Ledpos][0],TURNright0[Ledpos][1],TURNright0[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
     delay(250);
}
void nolight (void) {
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(0,0,0);                    //Give LED RGB value from array
     FastLED.show();                                                                                     //Display LED
  }
}

void BREAKlight (void) {
  float redlights[NUM_LEDS][3] =                                                                             //Array with R, G, B value of the individual LED
    {
       { 255, 0, 0 }, //0                                                                                        //R, G, B value of LED 0
       { 255, 0, 0 }, //1                                                                                        //R, G, B value of LED 1
       { 255, 0, 0 }, //2                                                                                        //R, G, B value of LED 2
       { 255, 0, 0 }, //3                                                                                        //R, G, B value of LED 3
       { 255, 0, 0 }, //4                                                                                        //R, G, B value of LED 4
       { 255, 0, 0 }, //5                                                                                        //R, G, B value of LED 5
       { 255, 0, 0 }, //6                                                                                        //R, G, B value of LED 6
       { 255, 0, 0 }, //7                                                                                        //R, G, B value of LED 7
       { 255, 0, 0 }, //8                                                                                        //R, G, B value of LED 8
       { 255, 0, 0 }, //9                                                                                        //R, G, B value of LED 9                                                                                             
    };
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(redlights[Ledpos][0],redlights[Ledpos][1],redlights[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
}

  //------------------------------------ END ------------------------//
