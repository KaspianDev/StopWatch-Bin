#include "TM1637.h"

TM1637 display;
int stopwatchTime = 0;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  display.begin(2, 3, 4);
  display.displayTime(00, 00, true);
  
  pinMode(16, INPUT); // Button
  pinMode(15, OUTPUT); // Buzzer
}

void loop() {
  int newValue = digitalRead(16);

Serial1.println(newValue);
  if (newValue == HIGH) {
    if (stopwatchTime == 0) {
      stopwatchTime = 1;
      tone(15, 150, 50);
    } else {
      stopwatchTime = 0;
    }
  }

  delay(100);
}
