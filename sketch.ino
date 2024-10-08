#include "TM1637.h"

TM1637 display;
int stopwatchTime = -1;

const long DEBOUNCE_DELAY = 150;
const int BUZZER_PIN = 5;
const int START_PIN = 27;
const int ALARM_PIN = 28;

void setup() {
  Serial1.begin(115200);
  display.begin(2, 3, 4);

  pinMode(START_PIN, INPUT); // Start/Stop Button
  pinMode(ALARM_PIN, INPUT); // Alarm Button
  pinMode(BUZZER_PIN, OUTPUT); // Buzzer

  incrementAlarm();
}

int alarmMinutes = -1;
boolean shouldBlink = false;

void incrementAlarm() {
  if (alarmMinutes == -2) alarmMinutes += 2;
  alarmMinutes++;
  if (alarmMinutes > 59) {
    alarmMinutes = 0;
  }

  display.displayTime(alarmMinutes, 00, true);
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

int minutes = 0;

void refreshTime() {
  minutes = stopwatchTime / 60;
  if (minutes > 59) {
    stopwatchTime = 1;
  }

  int seconds = stopwatchTime % 60;
  display.displayTime(minutes, seconds, true);
}

void updateTime() {
  refreshTime();

  if (alarmMinutes > -1 && minutes > 0 && minutes == alarmMinutes) {
    stopwatchTime = -1;
    shouldBlink = true;
  }
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
        shouldBlink = false;
        tone(BUZZER_PIN, 180, 30);
      } else {
        stopwatchTime = 0;
        updateTime();
        alarmMinutes = -2;
        stopwatchTime = -1;
        tone(BUZZER_PIN, 160, 30);
      }
      lastOnOffDebounce = millis();
    }
  }

  int alarmState = digitalRead(ALARM_PIN);
  if (lastAlarmState != alarmState) {
    lastAlarmState = alarmState;
    if (stopwatchTime == -1) {
      if ((millis() - lastAlarmDebounce) > DEBOUNCE_DELAY) {
        incrementAlarm();
        tone(BUZZER_PIN, 170, 20);
        lastAlarmDebounce = millis();
      }
    } else {
      tone(BUZZER_PIN, 220, 20);
    }
  }

  if (shouldBlink) {
    shouldBlink = false;
    for (int i = 0; i < 3; i++ ) {
      display.displayClear();
      delay(330);
      display.displayTime(alarmMinutes, 00, true);
      tone(BUZZER_PIN, 260, 30);
      delay(300);
    }
    display.displayTime(00, 00, true);
    alarmMinutes = -2;
  }

  if (stopwatchTime > -1) {
    count();
  }
}
