#ifdef UNIT_TEST

#include "KeyInstruction.h"
#include "Instructions.h"

void test_serialization(void) {
  KeyInstruction original(100);
  KeyInstruction result;
  uint8_t buf[5];

  original.serialize(buf, 5);
  result.deserialize(buf, 5);

  TEST_ASSERT_EQUAL(original.keycode, result.keycode);
}

#endif
