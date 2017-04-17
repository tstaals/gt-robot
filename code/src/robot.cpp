#include <Arduino.h>
#include <Servo.h>

const int pinMLF = 3; // Motor Left Forward
const int pinMLB = 9; // Motor Left Back
const int pinMRF = 5; // Motor Right Forward
const int pinMRB = 6; // Motor Right Back
const int pinGF = 10; // Fire gun
const int pinGT = 3; // Gun fired
const int pinServo = 11; // Servo
const int pinEchoServo = 7; // Receive echo puls on servo
const int pinEchoFront = 8; // Reveibe echo puls front
const int pinTrigServo = 12; // Trigger an puls on servo
const int pinTrigFront = 13; // Trigger an puls in front
int pos = 0; // variable to store the servo position
unsigned long serialSpeed = 115200;

byte incomingCommand = 0;   // for incoming serial data

Servo rServo;

const byte cNull = 0x00;
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

  attachInterrupt(digitalPinToInterrupt(pinEchoFront), temp, RISING);
  attachInterrupt(digitalPinToInterrupt(pinEchoServo), temp, RISING);
  attachInterrupt(digitalPinToInterrupt(pinGT), temp, RISING);

  rServo.attach(pinServo);
  rServo.write(0);

  Serial.begin(serialSpeed);
  Serial.print("Ready\r\n");
}

void shutdown() {
    analogWrite(pinMLF, LOW);
    analogWrite(pinMLB, LOW);
    analogWrite(pinMRF, LOW);
    analogWrite(pinMRB, LOW);
}

void setML(bool forward, byte val) {
  if (forward) {
    Serial.print("forward with val");

    analogWrite(pinMLB, LOW);
    analogWrite(pinMLF, val);
  }
  else {
    Serial.print("backward with val");

    analogWrite(pinMLB, val);
    analogWrite(pinMLF, LOW);
  }
  Serial.println(val);
  //Serial.println(val*val, DEC);
}

void setMR(bool forward, byte val) {
  if (forward) {
    Serial.print("forward with val");

    analogWrite(pinMRB, LOW);
    analogWrite(pinMRF, val);
  }
  else {
    Serial.print("backward with val");

    analogWrite(pinMRB, val);
    analogWrite(pinMRF, LOW);
  }
  Serial.println(val);
}

void fireGun() {
  Serial.println("firing gun");
  digitalWrite(pinGF, HIGH);
}

void setServo(byte val) {
  Serial.println("set servo");
  rServo.write(val);
}

void temp() {
  digitalWrite(pinGF, LOW);
}

void loop()
{
   if (Serial.available()) {
       incomingCommand = Serial.read();
       byte val = 0;

       Serial.print("I received: ");
       Serial.println(incomingCommand);
       switch (incomingCommand) {
         case cNull:
          shutdown();
          break;
         case cMLF:
            val = Serial.read();
            setML(true,val);
            break;
         case cMLB:
            val = Serial.read();
            setML(false,val);
            break;
        case cMRF:
          val = Serial.read();
          setMR(true, val);
          break;
        case cMRB:
          val = Serial.read();
          setMR(false, val);
          break;
        case cFire:
          fireGun();
          break;
        case cServo:
          val = Serial.read();
          setServo(val);
          break;
        default:
            break;
       }
   }
}
