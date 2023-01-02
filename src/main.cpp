#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int melody1[10][2] = {{1174,500},{987,250},{789,250},{659,250},{1174,250},{987,250},{783,250},{987,500},{783,500},{987,500}};
int difficulty = 0;
int note1[] = {100, 300, 500};
int note2[] = {200, 400, 600};
int idx = 0;
int jdx = 0;
int buzzerPin = 8;
int motorPin = 9;
int ledPin1 = 3;
int ledPin2 = 4;
int ledPin3 = 5;
int btn1 = 2;
int btn2 = 7;

void setup()
{
  // Arduino 9600 Beauds -- ESP32 115200 Beauds
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
}

// pas overflow le nombre de modes + son par changement de mode
int definemode(int mode)
{
  mode++;
  if (mode == 3)
    mode = 0;
  tone(buzzerPin, note1[mode]);
  delay(100);
  noTone(buzzerPin);
  return (mode);
}

void launchSeeds()
{
  int ib = 0;
  analogWrite(motorPin, 255);
  delay(20);
  analogWrite(motorPin, 140);
  delay(1000);
  while (ib <= 140)
  {
    analogWrite(motorPin, (140 - ib));
    delay(10);
    ib++; //ib += 10;
  }
}

void melody(int melodyNum = 1, int difficulty = 0) 
{
  noTone(buzzerPin);
  idx = 0;
  while (idx < 10)
  {
    tone(buzzerPin, melody1[idx][0]);
    delay(melody1[idx][1]);
    idx++;
  }
  noTone(buzzerPin);
}

bool canFeed(bool bypass = false)
{
  tmElements_t tm;
  if (!bypass)
  {
    if (tm.Hour > 7 && tm.Hour < 20)
      return true;
    else {
      delay(30000);
      return false;
    }
  }
  return true;
}


void  difficultyLED(int difficulty){
  switch (difficulty){
    case 0 :
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      break;
    case 1 :
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, LOW);
      break;
    case 2 :
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
      break;
  }
}

void  borbinput(int difficulty){
  idx = 0;
  jdx = 0;
  switch (difficulty){
    case 0 :
      while (idx < 30){
        if(digitalRead(btn2)){
          canFeed(true);
          idx = 31;
        }
        delay(1000);
        idx++;
      }
    case 1 :
      while (idx < 30){
        if(digitalRead(btn2) && jdx == 2){
          jdx++;
          if (jdx == 2){
            canFeed(true);
            idx = 31;
          }
        }
        delay(1000);
        idx++;
      }
    case 2 :
      while (idx < 30){
        if(digitalRead(btn2) && jdx == 3){
          jdx++;
          if (jdx == 2){
            canFeed(true);
            idx = 31;
        }
        delay(1000);
        idx++;
      }
    }
  }
}

void loop(){
  if (canFeed(true)){
    if (digitalRead(btn1)){
      difficulty = definemode(difficulty);
      difficultyLED(difficulty);
      delay(300);
    }
    if (digitalRead(btn2)){
      melody(1,10);
      delay(1000);
      borbinput(difficulty);
    }
  }
}