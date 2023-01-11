#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int melody1[10][2] = {{1174, 500}, {987, 250}, {789, 250}, {659, 250}, {1174, 250}, {987, 250}, {783, 250}, {987, 500}, {783, 500}, {987, 500}};
int difficulty = 0;
int note1[] = {261, 293, 329, 349, 392, 440, 493, 523};
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
  while (!Serial)
    ;                       // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get); // the function to get the time from the RTC
  if (timeStatus() != timeSet)
  {
    Serial.println("Unable to sync with the RTC");
    while (timeStatus() != timeSet)
    {
      Serial.println("Unable to sync with the RTC");
    }
  }
  else
    Serial.println("RTC has set the system time");
}

// pas overflow le nombre de modes + son par changement de mode
int definemode(int mode)
{
  mode++;
  if (mode == 8)
    mode = 0;
  tone(buzzerPin, note1[mode]);
  Serial.print(note1[mode]);
  delay(130);
  noTone(buzzerPin);
  return (mode);
}

void launchSeeds()
{
  int ib = 0;
  analogWrite(motorPin, 140);
  delay(210);
  while (ib <= 140)
  {
    analogWrite(motorPin, (140 - ib));
    delay(8);
    ib++; // ib += 10;
  }
}

void melody(int melodyNum = 1, int difficulty = 0)
{
  noTone(buzzerPin);
  int ledPins[3] = {ledPin1, ledPin2, ledPin3};
  int ledInt = 1;
  int currentNote;
  idx = 0;
  while (idx < difficulty)
  {
    if (currentNote < melody1[idx][0])
      ledInt++;
    else
      ledInt--;
    if (ledInt > 2)
      ledInt = 0;
    else if (ledInt < 0)
      ledInt = 2;
    currentNote = melody1[idx][0];
    digitalWrite(ledPins[ledInt], HIGH);
    tone(buzzerPin, currentNote);
    delay(melody1[idx][1]);
    idx++;
    digitalWrite(ledPins[ledInt], LOW);
  }
  noTone(buzzerPin);
}

bool canFeed(bool bypass = false)
{
  if (!bypass)
  {
    if (hour() >= 6 && hour() < 20)
      return true;
    else
    {
      delay(30000);
      return false;
    }
  }
  return true;
}

void difficultyLED(int difficulty)
{
  switch (difficulty)
  {
  case 0:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    break;
  case 1:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    break;
  case 2:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
    break;
  case 3:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    break;
  case 4:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    break;
  case 5:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    break;
  case 6:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
    break;
  case 7:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    break;
  }
}

void borbinput(int difficulty)
{
  idx = 0;
  jdx = 0;

  while (idx < difficulty && jdx < 30000)
  {
    if (digitalRead(btn2))
    {
      delay(150);
      tone(buzzerPin, melody1[idx][0]);
      while (digitalRead(btn2))
        delay(100);
      delay(50);
      noTone(buzzerPin);
      Serial.print("Button pressed ");
      Serial.print(idx + 1);
      Serial.print(" times");
      Serial.println();
      idx++;
      if (idx >= difficulty)
      {
        launchSeeds();
        break;
      }
    }
    jdx++;
    delay(1);
  }
}
void loop()
{
  if (canFeed(false))
  {
    if (digitalRead(btn1))
    {
      difficulty = definemode(difficulty);
      difficultyLED(difficulty);
    }
    if (digitalRead(btn2))
    {
      if (difficulty != 0)
      {
        melody(1, difficulty);
        difficultyLED(difficulty);
        borbinput(difficulty);
      }
      else
      {
        melody(1, difficulty + 1);
        difficultyLED(difficulty);
        delay(10);
        launchSeeds();
      }
    }
  }
}
/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣬⡄⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⠏⠉⢹⡞⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠁⠀⠀⠘⣧⢿⡇⠀⠀⠀⠀⣤⡶⠾⠶⣶⣄⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠘⠻⣧⠀⠀⠀⣼⡟⠀⠀⢠⢏⣿⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣇⠀⠀⠀⠀⠀⢻⡆⠀⣸⡟⠀⠀⠀⣿⣸⠃⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀⠸⣿⠀⣿⠁⠀⠀⠀⠛⡟⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣄⣀⠀⠀⠀⠀⢻⣾⣿⠀⠀⠀⠀⢰⡇⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡄⠀⠀⠀⠀⠀⠈⢿⣅⡀⠀⠀⠀⣼⡇⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡇⠀⠀⠀⠀⠀⠀⠘⣯⠁⠀⠀⠈⣿⡀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⠀⠀⠀⠀⠀⠀⠀⢹⣇⠀⠀⠀⢻⣷⠾⠛⢷⣦
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡄⠀⠀⠀⠀⠀⠀⠀⣿⡆⠀⠀⢸⡇⠀⠀⣸⣽
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣇⠀⠀⠀⠀⠀⠀⠰⠛⢷⠀⠀⠈⡇⠀⠀⢷⠇
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⣿⣄⡀⠀⠀⠀⠀⠀⢀⣾⡇⠀⠀⣇⠀⠀⣾⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣶⣶⣶⣶⣶⣶⣦⣤⣤⣤⣤⣤⣀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠈⠘⣇⠀⠚⣿⠀⢸⡏⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣴⡾⠟⠋⠁⠁⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠛⢿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⡿⠀⡾⠁⠀
⠀⠀⠀⢀⣤⣶⡶⣾⣟⣋⡥⠞⠁⠀⠀⠀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠞⠉⢿⣆⠀⠀⠀⠀⠀⠀⠀⢿⡇⣴⠇⢰⡇⠀⠀
⠀⠀⢀⡿⠉⠘⡇⠈⠻⣧⠀⠀⠀⠀⠀⠀⢳⡄⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠹⣷⡄⠀⠀⠀⠀⠀⢸⣇⣿⠀⣾⠁⠀⠀
⠀⠀⣸⡇⢀⣠⡟⠀⠀⠙⣷⣤⣀⣤⣤⣴⡾⢿⣶⣤⣄⣀⡀⢀⣠⡴⠋⠀⠀⠀⠀⠀⠀⠀⠙⣿⣄⠀⠀⠀⠀⠘⣿⡇⣠⡟⠀⠀⠀
⠀⠀⣿⠾⠋⠁⠀⠀⠀⠀⢸⣯⠉⠉⠁⠀⠀⠀⠈⠉⠉⠙⠛⠛⢻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣧⡀⠀⠀⠀⢸⣆⣾⠁⠀⠀⠀
⠀⣸⡟⣀⣀⠄⠀⠀⠀⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⣄⠀⠀⠈⣿⡏⠀⠀⠀⠀
⣰⡿⠋⠁⠀⠀⠀⠀⠀⠤⣄⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣧⡀⠀⠘⣿⠀⠀⠀⠀
⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⡷⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀⠙⢿⣆⠀⢹⣷⠀⠀⠀
⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⢿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠹⣧⡀⢿⡆⠀⠀
⢹⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⠏⠀⠙⠳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢷⠘⣷⠀⠀
⠀⢻⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣄⢀⣀⣠⣤⣤⣤⡴⠶⠾⢿⣤⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⣿⠆⠀
⠀⠀⢻⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⣭⣭⣄⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⢀⣿⠀⠀
⠀⠀⠀⢻⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠙⠓⠒⠒⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠋⣾⡇⠀⠀
⠀⠀⠀⠀⢿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡿⠀⠀⠀
⠀⠀⠀⠀⠈⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡿⠁⠀⠀⠀
⠀⠀⠀⠀⠀⢹⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠃⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢻⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠃⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠸⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⠇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣿⡌⠻⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡏⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢹⣧⠀⠀⠙⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡿⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠿⠀⠀⠀
*/
