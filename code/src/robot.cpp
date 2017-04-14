#include <Arduino.h>
#include <Servo.h>

int pinMLF = 3; // Motor Left Forward
int pinMLB = 9; // Motor Left Back
int pinMRF = 5; // Motor Right Forward
int pinMRB = 6; // Motor Right Back
int pinGF = 10; // Fire gun
int pinGT = 3; // Gun fired
int pinServo = 11; // Servo
int pinEchoServo = 7; // Receive echo puls on servo
int pinEchoFront = 8; // Reveibe echo puls front
int pinTrigServo = 12; // Trigger an puls on servo
int pinTrigFront = 13; // Trigger an puls in front

unsigned long serialSpeed = 115200;

int incomingCommand = 0;   // for incoming serial data

const byte cMLF = 0x01;
const byte cMLB = 0x02;
const byte cMRF = 0x03;
const byte cMRB = 0x04;
const byte cFire = 0x05;
const byte cServo = 0x06;

void setup()
{
  pinMode(pinMLF, OUTPUT);
  pinMode(pinMLB, OUTPUT);
  pinMode(pinMRF, OUTPUT);
  pinMode(pinMRB, OUTPUT);
  pinMode(pinGF, OUTPUT);
  pinMode(pinGT, INPUT);
  pinMode(pinEchoFront, INPUT);
  pinMode(pinEchoServo, INPUT);
  pinMode(pinTrigServo, OUTPUT);
  pinMode(pinTrigFront, OUTPUT);

  Serial.begin(serialSpeed);
  Serial.print("Ready\r\n");
}

void setMLF(unsigned int val) {
  analogWrite(pinMLF, val);
  Serial.println(val*val, DEC);
}

void setMLB(unsigned int val) {
  analogWrite(pinMLB,val);
}

void fireGun() {
  digitalWrite(pinGF, HIGH);
}

void loop()
{
   if (Serial.available()) {
       incomingCommand = Serial.read();
       int val = 0;

       Serial.print("I received: ");
       Serial.println(incomingCommand, DEC);
       switch (incomingCommand) {
         case cMLF:
            val = Serial.read();
            setMLF(val);
            break;
         case cMLB:
            val = Serial.read();
            setMLB(val);
            break;
        case cMRF:
          break;
        case cMRB:
          break;
        case cFire:
          break;
        case cServo:
          break;
        default:
            break;
       }
   }
}
