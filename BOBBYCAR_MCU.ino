//------------------------------------ BOBBYCAR_MCU ------------------------//

//LED ANORDNUNG: MCU > (0,1,2,3,4,5,6,7,8,9), (10),   (11,12),       (13,14)
//                      Rücklicht           , Status, Blinker links, Blinker rechts

#include"FastLED.h"                       //Include libs

#define NUM_LEDS 15                        //Number of LEDs
#define DATA_PIN 3                        //Data PIN on D3
#define CLOCK_PIN 2                       //Clock PIN D2
#define LED_SPEED 500                     //Cycle speed
uint8_t max_bright=255;                   //define brightness limiter
CRGB leds[NUM_LEDS];                      //LED definition

//INPUTS-------------------------
//int sensorPinx = A0;                      //FOR TESTING ONLY!!!!!!!!!!! --> Later for measuring speedvalue
//int sensorPiny = A1;                      //FOR TESTING ONLY!!!!!!!!!!!
int BLINKERlinks = 5;                     //Blinker links
int BLINKERrechts = 4;                    //Blinker rechts
int BREAK = 6;                            //BREMSE
int CL15 = 12;                            //Zündungssignal
bool firststart = true;                   //Erster start?
int Ubattery = A7;                        //Ubattery--------|100K|-----A2-----|200K|-----GND
int SPEEDsensor =A0;                      //

//OUTPUTS-------------------------
int MOTOR = 10;                           //Motor output pin

//set Sensor variables to 0
//int sensorValuex = 0;
//int sensorValuey = 0;   
int speedValue = 0;
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
  
  while (digitalRead(CL15) == LOW) {                                                                 //check if ignition is on

  if(analogRead(Ubattery) < 750) {                                                                      //voll= 13,8V (866 bit) , Leer= 10,5V (716 bit) Faktor: 68,2

    while (digitalRead(CL15) != HIGH) {
      Serial.println("PLEASE CHARGE!");
      Serial.print("Battery: ");
      Serial.println(analogRead(Ubattery));
      pleasecharge();
      nolight();
    }
  }

  if(firststart == true){
    LEDstartupfade();
    firststart = false;
  }
    
 
 //sensorValuex = analogRead(sensorPinx);
 //sensorValuey = analogRead(sensorPiny);
 speedValue = analogRead(SPEEDsensor);
 

if(speedValue > 10 ) {


  Serial.print("Poti Geschwindigkeit---------------------------: ");
  Serial.println(speedValue);
  
  outputValue = map(speedValue, 0, 1024, 0, 255);
  if(digitalRead(BREAK) == LOW) {
    analogWrite(MOTOR, 0); 
  }
  else{
  analogWrite(MOTOR, outputValue);
  Serial.print("Aktuelle Geschwindigkeit: ");
  Serial.println(outputValue);
  }
 
}
else {
  outputValue = 0;
  
  Serial.print("Poti Geschwindigkeit---------------------------: ");
  Serial.println(speedValue);
  
  analogWrite(MOTOR, outputValue);
  Serial.print("Aktuelle Geschwindigkeit: ");
  Serial.println(outputValue);
}



//--------------------handle light operations--------------------------//
  mainlight();



 if(digitalRead(BLINKERrechts) == LOW) {
  TURNright();
 }
 else if(digitalRead(BLINKERlinks) == LOW) {
  TURNleft();
 }
  else {
  mainlight();
 }

 if(digitalRead(BREAK) == LOW) {
  BREAKlight();
  analogWrite(MOTOR, 0);
 }
  else {
    mainlight();
  }


  }
 nolight();   //Wenn CL15 aus ist, alle Lichter ausschalten
 analogWrite(MOTOR, 0);
 
//--------------------/handle light operations---------------------------//
firststart = true;


} //++++++++++++++++++++++END LOOP ++++++++++++++++++++++++++++++++++++++//  



//------------------------------------ functions ------------------------//
void LEDstartup (void) {
  for(int Ledpos = 0; Ledpos < 9; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(255,255,255);                    //Give LED RGB value from array
     FastLED.show();                                                                                     //Display LED
     delay(100);
  }
}
void LEDstartupfade (void) {
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                                
    };

 for(int bright=0; bright < 255; bright = bright+5){
   FastLED.setBrightness(bright);
      for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
        leds[Ledpos] =CRGB(redlight[Ledpos][0],redlight[Ledpos][1],redlight[Ledpos][2]);                    //Give LED RGB value from array
        FastLED.show(); 
        }
     delay(30);
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                                
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 140, 0 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 140, 0 }, //14                                                                                       //R, G, B value of LED 14                                                                                                
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                                 
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 140, 0 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 140, 0 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                                 
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                                 
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
     if(Ledpos == 10){
        if(analogRead(Ubattery) < 750) {
          pleasecharge();
        }
      else{  
      leds[Ledpos] =CRGB(55,0,55);
      FastLED.show(); 
      delay(100);
      leds[Ledpos] =CRGB(0,0,0);
      FastLED.show(); 
      delay(500);
      leds[Ledpos] =CRGB(55,0,55);
      FastLED.show(); 
      }
     }
     else{
     leds[Ledpos] =CRGB(0,0,0);                    //Give LED RGB value from array
     FastLED.show();  
     }
                                                                                    //Display LED
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
       { 0, 255, 10 }, //10                                                                                       //R, G, B value of LED 10
       { 255, 255, 255 }, //11                                                                                       //R, G, B value of LED 11
       { 255, 255, 255 }, //12                                                                                       //R, G, B value of LED 12
       { 255, 255, 255 }, //13                                                                                       //R, G, B value of LED 13
       { 255, 255, 255 }, //14                                                                                       //R, G, B value of LED 14                                                                                         
    };
  for(int Ledpos = 0; Ledpos < NUM_LEDS; Ledpos++) {                                                    //Cycle trough LEDs 
     leds[Ledpos] =CRGB(redlights[Ledpos][0],redlights[Ledpos][1],redlights[Ledpos][2]);                    //Give LED RGB value from array
     FastLED.show(); 
     }
}

void pleasecharge(void) {
  leds[10] =CRGB(255,0,0);
  delay(100);
  FastLED.show(); 
  leds[10] =CRGB(0,0,0);
  delay(100);
  FastLED.show(); 
  
}

void motorfade(void){
  for(int fade=0; fade < 255; fade = fade+5){
    analogWrite(MOTOR, fade);
    Serial.print("Aktuelle Geschwindigkeit: ");
    Serial.println(fade);
    delay(200);
  }
}

  //------------------------------------ END ------------------------//
