#include <Arduino.h>
#include <RH_ASK.h>
#include "sodium.h"

#define rxled 17
#define rxpin 15
//RH_ASK driver(2000, rxpin, 1, 2);

static struct randombytes_implementation rng;
typedef void (*Block)(void);

void setup() {
  Serial.begin(9600); // Debugging only
  //if (!driver.init())

  rng.implementation_name = []() -> const char * { return "esp8266"; };
  rng.random = []() -> uint32_t { return 0; };
  rng.buf = [](void * const buf, const size_t size) { };
  rng.stir = NULL;
  rng.close = NULL;
  rng.uniform = NULL;
  int rc = randombytes_set_implementation(&rng);

  Serial.println("Setup complete.");
}

void loop() {
  delay(5000);
  Serial.println("Hi.");
  //uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  //uint8_t buflen = sizeof(buf);
  //if (driver.recv(buf, &buflen)) {
    //driver.printBuffer("Got:", buf, buflen);
  //}
  y();
  delay(1000);
}


#define MESSAGE (const unsigned char *) "hello world"
#define MESSAGE_LEN 11
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
void y() {
  unsigned long clock;

  unsigned char nonce[crypto_secretbox_NONCEBYTES];
  unsigned char key[crypto_secretbox_KEYBYTES];
  unsigned char ciphertext[CIPHERTEXT_LEN];

  randombytes_buf(nonce, sizeof nonce);
  randombytes_buf(key, sizeof key);

  clock = micros();
  crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
  clock = micros() - clock;
  Serial.print("crypto_secretbox_easy: "); Serial.println(clock);

  unsigned char decrypted[MESSAGE_LEN];
  clock = micros();
  if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key) != 0) {
    Serial.println("crypto_secretbox_open_easy FORGED OR CORRUPT");
  }
  clock = micros() - clock;
  Serial.print("crypto_secretbox_easy: "); Serial.println(clock);
}
