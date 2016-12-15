#include <Arduino.h>
#include <RH_ASK.h>
#include "sodium.h"

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
  }

  x();
  delay(1000);
}

#define MESSAGE (const unsigned char *) "test"
#define MESSAGE_LEN 4
#define CIPHERTEXT_LEN (crypto_box_MACBYTES + MESSAGE_LEN)
void x() {
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

  if (crypto_box_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce,
                      bob_publickey, alice_secretkey) != 0) {
    /* error */
  }

  Serial.print("Ciphertext : "); Serial.write((const uint8_t*) ciphertext, CIPHERTEXT_LEN); Serial.println();

  unsigned char decrypted[MESSAGE_LEN];
  if (crypto_box_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce,
                           alice_publickey, bob_secretkey) != 0) {
    /* message for Bob pretending to be from Alice has been forged! */
  }
  Serial.print("Decrypted text: "); Serial.write((const uint8_t*) decrypted, MESSAGE_LEN); Serial.println();
}
