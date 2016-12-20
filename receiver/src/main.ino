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
  y();
  delay(2000);
}

#define MESSAGE (const unsigned char *) "hello world. everything is on fire."
#define MESSAGE_LEN 35
#define CIPHERTEXT_LEN (crypto_box_MACBYTES + MESSAGE_LEN)
void x() {
  unsigned long start;
  unsigned char alice_publickey[crypto_box_PUBLICKEYBYTES];
  unsigned char alice_secretkey[crypto_box_SECRETKEYBYTES];
  crypto_box_keypair(alice_publickey, alice_secretkey);

  unsigned char bob_publickey[crypto_box_PUBLICKEYBYTES];
  unsigned char bob_secretkey[crypto_box_SECRETKEYBYTES];
  crypto_box_keypair(bob_publickey, bob_secretkey);

  unsigned char nonce[crypto_box_NONCEBYTES];
  unsigned char ciphertext[CIPHERTEXT_LEN];
  randombytes_buf(nonce, sizeof nonce);
  Serial.print("Plain text: "); Serial.write((const uint8_t*) MESSAGE, MESSAGE_LEN); Serial.println();

  start = micros();
  if (crypto_box_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce,
                      bob_publickey, alice_secretkey) != 0) {
    Serial.println("crypto_box_easy: ERROR!?");
  }
  Serial.print("crypto_box_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");

  Serial.print("Ciphertext : "); Serial.write((const uint8_t*) ciphertext, CIPHERTEXT_LEN); Serial.println();

  unsigned char decrypted[MESSAGE_LEN];
  start = micros();
  if (crypto_box_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce,
                           alice_publickey, bob_secretkey) != 0) {
    Serial.println("crypto_box_open_easy FORGERY OR CORRUPTION");
  }
  Serial.print("crypto_box_open_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");
  Serial.print("Decrypted text: "); Serial.write((const uint8_t*) decrypted, MESSAGE_LEN); Serial.println();
}

void y() {
  unsigned int ciphertext_len = (crypto_secretbox_MACBYTES + MESSAGE_LEN);
  unsigned long start;

  unsigned char nonce[crypto_secretbox_NONCEBYTES];
  unsigned char key[crypto_secretbox_KEYBYTES];
  unsigned char ciphertext[ciphertext_len];

  randombytes_buf(nonce, sizeof nonce);
  randombytes_buf(key, sizeof key);

  start = micros();
  crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
  Serial.print("crypto_secretbox_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");

  unsigned char decrypted[MESSAGE_LEN];
  start = micros();
  if (crypto_secretbox_open_easy(decrypted, ciphertext, ciphertext_len, nonce, key) != 0) {
    Serial.println("crypto_secretbox_open_easy FORGED OR CORRUPT");
  }
  Serial.print("crypto_secretbox_open_easy Duration: ");
  Serial.print(micros() - start);
  Serial.println("us");
}
