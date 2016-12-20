#include <Arduino.h>
#include <RH_ASK.h>
#include "sodium.h"
//#include "KeyInstruction.h"
//#include "Instructions.h"

#define txpin 5  //RF Transmitter pin = digital pin 4
#define txled 1        //Onboard LED = digital pin 13

RH_ASK driver(2000, 2, txpin, 3, false);

static struct randombytes_implementation rng;

void setup(){

  Serial.begin(9600);
  Serial.print("Setup complete.");
  if (!driver.init()) {
    Serial.println("init failed");
  }
  
  rng.implementation_name = []() -> const char * { return "esp8266"; };
  rng.random = []() -> uint32_t { return 0; };
  rng.buf = [](void * const buf, const size_t size) { };
  rng.stir = NULL;
  rng.close = NULL;
  rng.uniform = NULL;
  int rc = randombytes_set_implementation(&rng);
  Serial.print("RNG setup: ");
  Serial.println(rc);

  uint32_t count;
  uint8_t buf[10];
  //count = k.serialize(buf, 10);
  driver.send(buf, count);
  driver.waitPacketSent();

  unsigned long start;

  x();
  y();
}

void loop(){
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
