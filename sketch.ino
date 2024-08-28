#include <MD_MAX72xx.h>
#include "TM1637.h"

TM1637 display;
int stopwatchTime = -1;

MD_MAX72XX led = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, 10, 1);
int hours = 0;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  display.begin(2, 3, 4);

  pinMode(16, INPUT); // Button
  pinMode(15, OUTPUT); // Buzzer
  led.begin();
  led.control(MD_MAX72XX::INTENSITY, 10);
  led.clear();
  led.setPoint(1, 1, true);
  led.update();
}

void count() {
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

int lastState = HIGH;
void loop() {
  int value = digitalRead(16);
  if (lastState != value) {
    lastState = value;
    if (stopwatchTime == -1) {
      stopwatchTime = 3580;
      tone(15, 180, 30);
    } else {
      stopwatchTime = 0;
      updateTime();
      stopwatchTime = -1;
      tone(15, 160, 30);
    }
  }

  delay(1);
  if (stopwatchTime > -1) {
    count();
    delay(999);
  }
}
