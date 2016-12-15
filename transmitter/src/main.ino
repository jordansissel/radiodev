#include <Arduino.h>
#include <RH_ASK.h>
#include "KeyInstruction.h"
#include "Instructions.h"

#define txpin 5  //RF Transmitter pin = digital pin 4
#define txled 1        //Onboard LED = digital pin 13

RH_ASK driver(2000, 2, txpin, 3, false);

void setup(){
  Serial.begin(9600);
  Serial.print("Setup complete.");
  if (!driver.init()) {
    Serial.println("init failed");
  }

  Serial.println("1");
  KeyInstruction k(0x65);
  Serial.println("2");
  uint32_t count;
  Serial.println("3");
  uint8_t buf[10];
  Serial.println("4");
  count = k.serialize(buf, 10);
  Serial.println("5");
  driver.send(buf, count);
  Serial.println("6");
  driver.waitPacketSent();
  Serial.println("7");
  Serial.println("Sent.");
}

void loop(){
  delay(2000);
}
