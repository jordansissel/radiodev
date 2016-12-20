#include <Arduino.h>

void setup() {
  Serial.begin(9600); // Debugging only
  //if (!driver.init())
  Serial.println("Setup complete.");
}

void loop() {
  Serial.println("Hi.");
  delay(5000);
}
