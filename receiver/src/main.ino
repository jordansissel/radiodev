#include <Arduino.h>
#include <RH_ASK.h>
#include "KeyInstruction.h"
#include "Instructions.h"

#define rxled 17
#define rxpin 15
RH_ASK driver(2000, rxpin, 1, 2);

void setup() {
  Serial.begin(9600); // Debugging only
  if (!driver.init())
  Serial.println("init failed");
  Serial.print("Setup complete.");
}
void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {
    driver.printBuffer("Got:", buf, buflen);

    switch (static_cast<Instructions>(buf[0])) {
    case Instructions::KeyInstruction:
      Serial.println("Got a key instruction");
      break;
    default:
      Serial.println("Unknown instruction.");
      break;
    }
  }
}
