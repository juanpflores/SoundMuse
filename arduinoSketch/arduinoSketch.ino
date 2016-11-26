

#include <Servo.h> 

#define flowPin 2


Servo myservo;
int termo=A0;
int resistencia=4;
int water=10;
int r=3,g=5,b=6;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false, startW=false;  // whether the string is complete



// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(flowPin);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}




void setup() {
  Serial.begin(9600);
  pinMode(termo,INPUT);
  pinMode(resistencia,INPUT);
  myservo.attach(9);
  pinMode(flowPin, INPUT);
  digitalWrite(flowPin, HIGH);
  lastflowpinstate = digitalRead(flowPin);
  useInterrupt(true);
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
void ReadFlow(){
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;  
  
  Serial.println(liters);
}
void ReadTemperature(){
  
  Serial.println(analogRead(termo));
  
}
void SetLed(){
  int red=(inputString.substring(2,5)).toInt(),green=(inputString.substring(5,8)).toInt(),blue=(inputString.substring(8,11)).toInt();
  analogWrite(r,red);
  analogWrite(g,green);
  analogWrite(b,blue);
  Serial.println("Done");
}
void SetTemp(){
  int pos=(inputString.substring(2)).toInt();
  myservo.write(pos);
  Serial.println("Done");
}
void SetHeater(){//test with led 
  if((inputString.substring(2)).toInt()==0){
    digitalWrite(resistencia,LOW);
  }
  else{ digitalWrite(resistencia,HIGH);}
  Serial.println("Done");
}
void SetWater(){//Check it with the water pump and tip
  //0 a 255
  analogWrite(water,(inputString.substring(2)).toInt());
  Serial.println("Done");
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


void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

