//INPUTS-------------------------
int BLINKERlinks = 5;                     //Blinker links
int BLINKERrechts = 4;                    //Blinker rechts
int BREAK = 6;                            //BREMSE
int CL15 = 12;                            //Zündungssignal
bool firststart = true;                   //Erster start?
int Ubattery = A7;                        //Ubattery--------|100K|-----A2-----|200K|-----GND
int SPEEDsensor =A0; //


//OUTPUTS-------------------------
int MOTOR = 10; //Motor output pin


void setup() {
  pinMode(CL15, INPUT);
}

void loop() {

  if(digitalRead(BREAK) == LOW) {
    motorfade();
    analogWrite(MOTOR, 255); 
  }
  else{
  analogWrite(MOTOR, 0);
  }
 
}




void motorfade(void){
  for(int fade=0; fade < 255; fade = fade+5){
    analogWrite(MOTOR, fade);
    delay(200);
  }
}
