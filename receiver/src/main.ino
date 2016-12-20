#include <Arduino.h>
#include <RH_ASK.h>
#include "sodium.h"

#define rxled 17
#define rxpin 15
//RH_ASK driver(2000, rxpin, 1, 2);

void setup() {
  Serial.begin(9600); // Debugging only
  //if (!driver.init())
  Serial.println("Setup complete.");
}

void loop() {
  Serial.println("Hi.");
  //uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  //uint8_t buflen = sizeof(buf);
  //if (driver.recv(buf, &buflen)) {
    //driver.printBuffer("Got:", buf, buflen);
  //}
  delay(5000);
  y();
  delay(5000);
}

#define MESSAGE (const unsigned char *) "hello world. everything is on fire."
#define MESSAGE_LEN 35
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
void y() {
  unsigned long start;

  unsigned char nonce[crypto_secretbox_NONCEBYTES];
  unsigned char key[crypto_secretbox_KEYBYTES];
  unsigned char ciphertext[CIPHERTEXT_LEN];

  randombytes_buf(nonce, sizeof nonce);
  randombytes_buf(key, sizeof key);

  start = micros();
  crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
  Serial.print("crypto_secretbox_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");

  unsigned char decrypted[MESSAGE_LEN];
  start = micros();
  if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key) != 0) {
    Serial.println("crypto_secretbox_open_easy FORGED OR CORRUPT");
  }
  Serial.print("crypto_secretbox_open_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");
}
