#pragma once
#include <cstdint>

uint8_t read_l8(const char *ptr);
uint16_t read_l16(const char *ptr);

void write_l16(char *buf, const uint16_t val);
