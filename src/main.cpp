#include <Arduino.h>
int mode = 0;
int note1[] = {100, 300, 500};
int note2[] = {200, 400, 600};
int idx = 0;

void  setup() {
  pinMode(3, OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
  pinMode(7, INPUT);
}
//pas overflow le nombre de modes + son par changement de mode 
int  tabernak(int mode) {
  mode++;
  if (mode == 3)
      mode = 0;
  tone(8, note1[mode]);
  delay(100);
  noTone(8);
  return (mode);
}

//def des leds pour les modes
void mod1() {
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void mod2() {
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}

void mod3() {
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void  loop() {
  if (digitalRead(2)){
    mode = tabernak(mode);
    delay(300);
  }
  if (digitalRead(7)) 
    note2[mode];
  if (mode == 0){
    mod1();
  }
  else if (mode == 1) {
    mod2();
  }
  else if (mode == 2) {
    mod3();
  }
}