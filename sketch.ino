#include "TM1637.h"

TM1637 display;
int stopwatchTime = -1;

int hours = 0;

const int START_PIN = 27;
const int ALARM_PIN = 28;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  display.begin(2, 3, 4);

  pinMode(START_PIN, INPUT); // Button
  pinMode(15, OUTPUT); // Buzzer
}

int milis = 0;
void count() {
  if (milis < 1000) {
    milis++;
    return;
  }
  milis = 0;
  stopwatchTime++;

  updateTime();
}

void updateTime() {
  int minutes = stopwatchTime / 60;
  if (minutes > 59) {
    stopwatchTime = 1;
  }

  int seconds = stopwatchTime % 60;
  display.displayTime(minutes, seconds, true);
}

int lastState = 0;
void loop() {
  delay(1);

  int value = digitalRead(START_PIN);
  if (lastState != value) {
    lastState = value;
    if (stopwatchTime == -1) {
      stopwatchTime = 0;
      tone(15, 180, 30);
    } else {
      stopwatchTime = 0;
      updateTime();
      stopwatchTime = -1;
      tone(15, 160, 30);
    }
  }

  if (stopwatchTime > -1) {
    count();
  }
}

