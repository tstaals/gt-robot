#include <Arduino.h>
#include <Servo.h>
#include <Sonar.h>

const int pinMLF = 11; // Motor Left Forward
const int pinMLB = 3; // Motor Left Back
const int pinMRF = 5; // Motor Right Forward
const int pinMRB = 6; // Motor Right Back
const int pinGF = 10; // Fire gun
const int pinGT = 2; // Gun fired
const int pinServo = 9; // Servo
const int pinEchoServo = 8; // Receive echo puls on servo
const int pinEchoFront = 7; // Reveibe echo puls front
const int pinTrigServo = 12; // Trigger an puls on servo
const int pinTrigFront = 13; // Trigger an puls in front
const int pinLFLED = A4; // right front led
const int pinRFLED = A5; // left front led

volatile boolean bRightForward = false;
volatile boolean bLeftForward = false;

unsigned long serialSpeed = 115200;

byte incomingCommand = 0;   // for incoming serial data

unsigned long lastTime	= 0;

int pingTimer			= 0;
int pingDelay			= 100;

Servo rServo;

Sonar sonarOne(pinTrigFront, pinEchoFront);
Sonar sonarTwo(pinTrigServo, pinEchoServo);

const byte cMLF = 0x01;
const byte cMLB = 0x02;
const byte cMRF = 0x03;
const byte cMRB = 0x04;
const byte cFire = 0x05;
const byte cServo = 0x06;
const byte cPing = 0x07;
const byte cStop = 0x08;

const byte rSensorOne = 0xA;
const byte rSensorTwo = 0xB;

// handle pin change interrupt for D8 to D13 here
ISR (PCINT0_vect) {
    sonarTwo._echo_isr();
}

// handle pin change interrupt for D0 to D7 here
ISR (PCINT2_vect) {
  if(PCMSK2  & (1 << PD7)) {
    sonarOne._echo_isr();
  }
}

void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit(digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit(digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void stopFiring() {
  digitalWrite(pinGF,LOW);
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
  pinMode(pinLFLED, INPUT);
  pinMode(pinRFLED, INPUT);

  pciSetup(pinEchoFront);
  pciSetup(pinEchoServo);

  attachInterrupt(digitalPinToInterrupt(pinGT), stopFiring, RISING);

  rServo.attach(pinServo);

  delay(500);
  rServo.write(0);

  delay(500);
  rServo.write(180);

  delay(500);
  rServo.write(90);

  digitalWrite(pinLFLED, HIGH);
  digitalWrite(pinRFLED, HIGH);
  delay(500);

  digitalWrite(pinLFLED, LOW);
  digitalWrite(pinRFLED, LOW);
  delay(500);

  digitalWrite(pinLFLED, HIGH);
  digitalWrite(pinRFLED, HIGH);
  delay(500);

  digitalWrite(pinLFLED, LOW);
  digitalWrite(pinRFLED, LOW);
  delay(500);

  Serial.begin(serialSpeed);

  sonarOne.ping();
  sonarTwo.ping();

  lastTime = millis();
}

void shutdown() {
    analogWrite(pinMLF, 0);
    analogWrite(pinMLB, 0);
    analogWrite(pinMRF, 0);
    analogWrite(pinMRB, 0);
}

void setML(bool forward, byte val) {
  if (forward) {
    bLeftForward = true;
    analogWrite(pinMLB, LOW);
    analogWrite(pinMLF, val);
  }
  else {
    bLeftForward = false;
    analogWrite(pinMLB, val);
    analogWrite(pinMLF, LOW);
  }
}

void setMR(bool forward, byte val) {
  if (forward) {
    bRightForward = true;
    analogWrite(pinMRB, LOW);
    analogWrite(pinMRF, val);
  }
  else {
    bRightForward = false;
    analogWrite(pinMRB, val);
    analogWrite(pinMRF, LOW);
  }
}

void fireGun() {
  digitalWrite(pinGF, HIGH);
}

void setServo(byte val) {
  rServo.write(val);
}

void loop()
{
   if (Serial.available()) {
       incomingCommand = Serial.read();
       byte val = 0;

       switch (incomingCommand) {
         case cSTOP:
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
        case cPing:
          sonarTwo.ping();
          break;
        default:
            break;
       }
   }

   if (sonarOne.isFinished()) {
     int range = sonarOne.getRange();

     if (range > 0 && range < 15) {  // Emergency stop when object is detected right in front of robot
       if (bRightForward && bLeftForward) {
         shutdown();
      }
     }

     //Serial.write(rSensorOne);   // 0A
     //Serial.write(range);        //

     pingTimer = 0;
   }

   if (sonarTwo.isFinished()) {
     int range = sonarTwo.getRange();
     Serial.write(rSensorTwo);
     Serial.write(range);
   }

   unsigned long time = millis();
   unsigned long dt   = time - lastTime;
   lastTime = time;

   pingTimer += dt;

   if (pingTimer > pingDelay) {
     pingTimer = 0;
     sonarOne.ping();
   }

}
