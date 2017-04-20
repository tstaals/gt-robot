#include <Arduino.h>
#include <Sonar.h>

Sonar::Sonar(int trigger, int echo) {
  this->_trigger = trigger;
  this->_echo = echo;
}

void Sonar::ping(){
  this->_finished=false;
  digitalWrite(this->_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(this->_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->_trigger, LOW);
}

unsigned int Sonar::getRange() {
  this->_finished=false;
  return (this->_end-this->_start)/58;
}

void Sonar::_echo_isr(){
  switch(digitalRead(this->_echo)){
    case HIGH:
      this->_start=micros();
      break;
    case LOW:
      this->_end=micros();
      this->_finished=true;
      break;
  }
}
