#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int mode = 0;
int note1[] = {100, 300, 500};
int note2[] = {200, 400, 600};
int idx = 0;
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
int tabernak(int mode)
{
  mode++;
  if (mode == 3)
    mode = 0;
  tone(8, note1[mode]);
  delay(100);
  noTone(8);
  return (mode);
}

void launchSeeds()
{
  int ib = 0;
  analogWrite(motorPin, 255);
  delay(20);
  analogWrite(motorPin, 140);
  delay(1000);
  while (ib < 140)
  {
    analogWrite(motorPin, (140 - ib));
    delay(10);
    ib++;
  }
}

bool canFeed(bool bypass = false)
{
  tmElements_t tm;
  if (!bypass)
  {
    if (tm.Hour > 7 && tm.Hour < 20)
    {
      return true;
    }
    else {
      delay(30000);
      return false;
    }
  }
  else
  {
    return true;
  }
}

// def des leds pour les modes
void mod1()
{
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
}

void mod2()
{
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, LOW);
}

void mod3()
{
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, HIGH);
}

void loop(){
  while (canFeed(false))
  {
    if (digitalRead(btn1))
    {
      mode = tabernak(mode);
      delay(300);
    }
    if (digitalRead(btn2))
    note2[mode];
    if (mode == 0)
    {
      mod1();
    }
    else if (mode == 1)
    {
      mod2();
    }
    else if (mode == 2)
    {
      mod3();
    }
  }
}
