#include <Arduino.h>
#include <RH_ASK.h>
#include "sodium.h"
//#include "KeyInstruction.h"
//#include "Instructions.h"

#define txpin 5  //RF Transmitter pin = digital pin 4
#define txled 1        //Onboard LED = digital pin 13

RH_ASK driver(2000, 2, txpin, 3, false);

void setup(){

  Serial.begin(9600);
  Serial.print("Setup complete.");
  if (!driver.init()) {
    Serial.println("init failed");
  }
  
  int rc = randombytes_set_implementation(&rng);
  Serial.println("RNG setup", rc);

  Serial.println("1");
  //KeyInstruction k(0x65);
  Serial.println("2");
  uint32_t count;
  Serial.println("3");
  uint8_t buf[10];
  Serial.println("4");
  //count = k.serialize(buf, 10);
  Serial.println("5");
  driver.send(buf, count);
  Serial.println("6");
  driver.waitPacketSent();
  Serial.println("7");
  Serial.println("Sent.");
  x();
}

void loop(){
  delay(2000);
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

struct randombytes_implementation rng = {
  .implementation_name = () const char * -> { return "esp8266" },
  .random = () uint32_t -> { return 0; },
  .buf = (void * const buf, const size_t size) void -> { },
  .stir = NULL,
  .close = NULL,
  .uniform = NULL,
};

