/**
 * file-stx - Sparkplug texture plug-in for GIMP
 * Copyright (C) 2020, 2026  kotwys
 *
 * Adapted by kotwys in Sep 2026 from the ReverseBox Python package, which is:
 * Copyright (C) 2024-2025   Bartłomiej Duda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>

#include "swizzle.h"

void
psmt8_swizzle (const char *input,
               char       *output,
               uint32_t    width,
               uint32_t    height,
               bool        swizzle)
{
  uint32_t x, y;

  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          unsigned block_location  = (y & (~0xF)) * width + (x & (~0xF)) * 2;
          unsigned swap_selector   = (((y + 2) >> 2) & 0x1) * 4;
          unsigned pos_y           = (((y & (~3)) >> 1) + (y & 1)) & 0x7;
          unsigned column_location = (pos_y * width * 2)
                                     + ((x + swap_selector) & 0x7) * 4;
          unsigned byte_num        = ((y >> 1) & 1) + ((x >> 2) & 2);
          unsigned swizzle_id      = block_location + column_location
                                     + byte_num;

          if (swizzle)
            output[swizzle_id] = input[y * width + x];
          else
            output[y * width + x] = input[swizzle_id];
        }
    }
}
