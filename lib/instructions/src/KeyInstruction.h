
#include <stdint.h>

class KeyInstruction {
public:
  // The keycode to send
  uint8_t keycode;

  KeyInstruction(uint8_t keycode);
  uint32_t serialize(uint8_t *buf, uint32_t bufsize);
  uint32_t deserialize(uint8_t *buf, uint32_t bufsize);
};
