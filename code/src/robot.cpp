#include <Arduino.h>
#include <Servo.h>
#include <Sonar.h>

const int pinMLF = 3; // Motor Left Forward
const int pinMLB = 9; // Motor Left Back
const int pinMRF = 5; // Motor Right Forward
const int pinMRB = 6; // Motor Right Back
const int pinGF = 10; // Fire gun
const int pinGT = 3; // Gun fired
const int pinServo = 11; // Servo
const int pinEchoServo = 8; // Receive echo puls on servo
const int pinEchoFront = 7; // Reveibe echo puls front
const int pinTrigServo = 12; // Trigger an puls on servo
const int pinTrigFront = 13; // Trigger an puls in front

unsigned long serialSpeed = 115200;

byte incomingCommand = 0;   // for incoming serial data

unsigned long lastTime	= 0;

int pingTimer			= 0;
int pingDelay			= 60;

Servo rServo;

Sonar sonarOne(pinTrigFront, pinEchoFront);
Sonar sonarTwo(pinTrigServo, pinEchoServo);

const byte cNull = 0x00;
const byte cMLF = 0x01;
const byte cMLB = 0x02;
const byte cMRF = 0x03;
const byte cMRB = 0x04;
const byte cFire = 0x05;
const byte cServo = 0x06;

const byte rSensorOne = 0x10;
const byte rSensorTwo = 0x11;

void temp() {

}

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{
    sonarTwo._echo_isr();
}

ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
{
  if(PCMSK2  & (1 << PD7)) {
    sonarOne._echo_isr();
  }
}

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void setup()
{
  pinMode(pinMLF, OUTPUT);
  pinMode(pinMLB, OUTPUT);
  pinMode(pinMRF, OUTPUT);
  pinMode(pinMRB, OUTPUT);
  pinMode(pinGF, OUTPUT);
  pinMode(pinGT, INPUT);
  pinMode(pinTrigServo, OUTPUT);
  pinMode(pinEchoFront, INPUT);
  pinMode(pinTrigFront, OUTPUT);
  pinMode(pinEchoServo, INPUT);

  pciSetup(pinEchoFront);
  pciSetup(pinEchoServo);

  attachInterrupt(digitalPinToInterrupt(pinGT), temp, RISING);

  rServo.attach(pinServo);
  delay(50);

  rServo.write(0);

  delay(50);
  rServo.write(180);

  delay(50);
  rServo.write(90);

  Serial.begin(serialSpeed);
  Serial.print("Ready\r\n");

  sonarOne.ping();
  sonarTwo.ping();
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

// void temp() {
//   digitalWrite(pinGF, LOW);
// }

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
   if (sonarOne.isFinished()) {
     int range = sonarOne.getRange();
     if (range > 250) {
       Serial.write(0);
     }
     else if (range < 15) {
       Serial.println("auto stop");
     }
     else {
       Serial.write(range);
     }

   }
   if (sonarTwo.isFinished()) {
     //Serial.println(sonarTwo.getRange());
   }

   unsigned long time = millis();
   unsigned long dt   = time - lastTime;
   lastTime 	     = time;

   pingTimer += dt;
   if(pingTimer > pingDelay){

      pingTimer = 0;
   	   sonarOne.ping();
     }

}
