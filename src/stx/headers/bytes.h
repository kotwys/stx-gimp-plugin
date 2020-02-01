#pragma once
#include <cstdint>

uint8_t read_l8(const unsigned char *ptr);
uint16_t read_l16(const unsigned char *ptr);

void write_l16(unsigned char *buf, const uint16_t val);
