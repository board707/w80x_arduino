#include "Arduino.h"
// Tested on Air103 with SG90 servo
// Uses advanced PWM signal control
// see PWM.md in DOC folder

// Pin definition
#define servo_pin PB0

//Function prototype 
void setAngle(uint8_t, uint8_t);

//Variables
PWM_HandleTypeDef *hpwm;

void setup() {

  pinMode(servo_pin,PWM_OUT);
  hpwm = getPWMHandle(servo_pin);
  // SG90 can operate at 350 hertz
  setPWM_Freq(hpwm, 350);

}

void loop() {
  for (uint8_t angle = 0; angle<=180; angle++) setAngle(servo_pin,angle);
  delay(200);
}

void setAngle(uint8_t pin,uint8_t angle){
  // duty from 40 to 230 corresponds to changing 
  // the angle from 0 to 180 degrees in steps of about 0.9
  uint8_t duty = map(angle,0,180,40,230);
  analogWrite(pin, duty);
  }