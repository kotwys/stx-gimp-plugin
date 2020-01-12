#include <cstdint>

uint8_t read_l8(const char *ptr) {
  return *ptr;
}

uint16_t read_l16(const char *ptr) {
  return *ptr | *(ptr + 1) << 8;
}
