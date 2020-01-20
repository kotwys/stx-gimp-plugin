#include "bytes.h"

uint8_t read_l8(const char *ptr) {
  auto *uptr = reinterpret_cast<const uint8_t*>(ptr);
  return *ptr;
}

uint16_t read_l16(const char *ptr) {
  auto *uptr = reinterpret_cast<const uint8_t*>(ptr);
  return *uptr | *(uptr + 1) << 8;
}

void write_l16(char *buf, const uint16_t val) {
  buf[0] = val & 0xff;
  buf[1] = val >> 8;
}
