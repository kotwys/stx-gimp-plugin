#include "utils/bytes.h"

uint8_t read_l8(const unsigned char *ptr) {
  auto *uptr = reinterpret_cast<const uint8_t*>(ptr);
  return *uptr;
}

uint16_t read_l16(const unsigned char *ptr) {
  auto *uptr = reinterpret_cast<const uint8_t*>(ptr);
  return *uptr | *(uptr + 1) << 8;
}

void write_l16(unsigned char *buf, const uint16_t val) {
  buf[0] = val & 0xff;
  buf[1] = val >> 8;
}
