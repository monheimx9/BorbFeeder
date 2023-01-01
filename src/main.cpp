#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(9,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(9,HIGH);
  delay(170);
  digitalWrite(9,LOW);
  delay(70);
}