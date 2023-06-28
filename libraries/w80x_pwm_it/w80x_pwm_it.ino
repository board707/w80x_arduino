void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(PB12, PWM_OUT);
pinMode(LED_BUILTIN_1, OUTPUT);
digitalWrite(LED_BUILTIN_1, LOW);
PWM_HandleTypeDef *hpwm = getPWMHandle(PB12);
uint32_t freq = setPWMFreq(hpwm, 1000);
if (freq > 0) {
  Serial.printf("Set PWM0 freq = %d\n", freq);
  analogWrite(PB12, 64);
  setPWM_Inverse(hpwm, false, false);
  delay(3000);
  digitalWrite(LED_BUILTIN_1, HIGH);
  delay(1000);
  setPWM_OneShotMode(hpwm, false, 8, true);
  delay(500);
  setPWM_Inverse(hpwm, false, true);
  
}
else { Serial.println("Fail to change frequency of PWM"); }

}

void loop() {
  // put your main code here, to run repeatedly:

}
