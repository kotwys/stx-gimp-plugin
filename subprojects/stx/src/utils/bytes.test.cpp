#include <cstdint>
#include <assert.h>
#include <iostream>

#include "utils/bytes.h"

int main() {
  unsigned char test[2] = { 128, 0 };
  uint16_t expected = 128;

  // Take zero byte
  auto actual = read_l16(test);
  assert(expected == actual);

  // Don't take zero byte
  auto actual2 = read_l8(test);
  assert(expected == actual2);
  
  return 0;
}
