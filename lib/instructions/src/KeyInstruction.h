
#include <stdint.h>
#include "Keyboard.h" // Needed here to satisfy the PlatformIO dependency detector

class KeyInstruction {
public:
  // The keycode to send
  uint8_t keycode;

  KeyInstruction(uint8_t keycode);
  uint32_t serialize(uint8_t *buf, uint32_t bufsize);
  uint32_t deserialize(uint8_t *buf, uint32_t bufsize);

  void execute();
};
