#include <cstdint>
#include <assert.h>
#include <iostream>

#include "bytes.h"

int main() {
  char test[2] = { '\x80', '\x00' };
  uint16_t expected = 128;

  // Take zero byte
  auto actual = read_l16(test);
  assert(expected == actual);

  // Don't take zero byte
  auto actual2 = read_l8(test);
  assert(expected == actual2);
  
  return 0;
}
