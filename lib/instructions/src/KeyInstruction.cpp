#include "KeyInstruction.h"
#include "Instructions.h"
#include <stdint.h>

KeyInstruction::KeyInstruction(uint8_t keycode) {
  this->keycode = keycode;
}

uint32_t KeyInstruction::serialize(uint8_t *buf, uint32_t bufsize) {
  if (bufsize < 2) {
    return -1; // Not enough bytes.
  }

  buf[0] = static_cast<uint8_t>(Instructions::KeyInstruction);
  buf[1] = this->keycode;
  return 2;
}

uint32_t KeyInstruction::deserialize(uint8_t *buf, uint32_t bufsize) {
  if (bufsize < 2) {
    return -1; // Not enough bytes.
  }

  this->keycode = buf[1];
  return 2;
}

void KeyInstruction::execute() {
  Keyboard.press(this->keycode);
  Keyboard.releaseAll();
}
