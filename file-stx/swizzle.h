/**
 * file-stx - Sparkplug texture plug-in for GIMP
 * Copyright (C) 2020, 2026  kotwys
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

#ifndef __SWIZZLE_H__
#define __SWIZZLE_H__

/**
 * Swizzles or unswizzles pixel data according to PSMT8.
 *
 * @param[in]  input    input pixel data
 * @param[out] output   pixel data after transformation
 * @param      width
 * @param      height
 * @param      swizzle  whether to swizzle (TRUE) or unswizzle (FALSE)
 */
void psmt8_swizzle (const char *input,
                    char       *output,
                    uint32_t    width,
                    uint32_t    height,
                    bool        swizzle);

#endif /* __SWIZZLE_H__ */
