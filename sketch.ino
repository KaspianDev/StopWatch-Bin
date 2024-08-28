#include "TM1637.h"

TM1637 display;
int stopwatchTime = -1;

const long DEBOUNCE_DELAY = 150;
const int START_PIN = 27;
const int ALARM_PIN = 28;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  display.begin(2, 3, 4);

  pinMode(START_PIN, INPUT); // Start/Stop Button
  pinMode(ALARM_PIN, INPUT); // Alarm Button
  pinMode(15, OUTPUT); // Buzzer

  incrementAlarm();
}

int alarmMinutes = -1;

void incrementAlarm() {
  alarmMinutes++;
  display.displayTime(alarmMinutes, 00, true);
}

int milis = 0;
void count() {
  if (milis < 999) {
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

int lastOnOffState = 0;
long lastOnOffDebounce = 0;

int lastAlarmState = 0;
long lastAlarmDebounce = 0;
void loop() {
  delay(1);

  int onOffValue = digitalRead(START_PIN);
  if (lastOnOffState != onOffValue) {
    lastOnOffState = onOffValue;
    if ((millis() - lastOnOffDebounce) > DEBOUNCE_DELAY) {
      if (stopwatchTime == -1) {
        stopwatchTime = 0;
        tone(15, 180, 30);
      } else {
        stopwatchTime = 0;
        updateTime();
        stopwatchTime = -1;
        tone(15, 160, 30);
      }
      lastOnOffDebounce = millis();
    }
  }

  int alarmState = digitalRead(ALARM_PIN);
  if (lastAlarmState != alarmState) {
    lastAlarmState = alarmState;
    if ((millis() - lastAlarmDebounce) > DEBOUNCE_DELAY) {
      incrementAlarm();
      tone(15, 170, 10);
      lastAlarmDebounce = millis();
    }
  }

  if (stopwatchTime > -1) {
    count();
  }
}

