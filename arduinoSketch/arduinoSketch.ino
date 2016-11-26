

#include <Servo.h> 
Servo myservo;
int termo=A0;
int resistencia=4;
int water=10;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false, startW=false;  // whether the string is complete


void setup() {
  Serial.begin(9600);
  pinMode(termo,INPUT);
  pinMode(resistencia,INPUT);
  myservo.attach(9);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  checkMessages();


}

void nextAction(){
  
  String s=inputString.substring(0,2);
  
  if(s=="RT"){ReadTemperature();}
  if(s=="RF"){ReadFlow();}
  if(s=="SL"){SetLed();}
  if(s=="ST"){SetTemp();}
  if(s=="SC"){SetHeater();}
  if(s=="SW"){SetWater();}
  

}

void ReadTemperature(){}
void ReadFlow(){}
void SetLed(){}
void SetTemp(){}
void SetHeater(){}
void SetWater(){
  //0 a 255
  analogWrite(water,(inputString.substring(2)).toInt());
  Serial.println("#WaterDone#");
}


void checkMessages(){
  if (stringComplete) {  
    nextAction();
    // clear the string:
    inputString = "";
    stringComplete = false;
  }}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar=='#'){
      startW=!startW;
      if (!startW){stringComplete = true;}
    }
    else{
      if (startW){inputString += inChar;}
    }  }}

