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

#ifndef __BABL_HELPER_H__
#define __BABL_HELPER_H__

#define BABL_FORMAT_PC      (babl_format_bgra8 ())
#define BABL_FORMAT_PALETTE (babl_format ("R~G~B~A u8"))

/**
 * Creates a Babl format for the STX PC format.
 */
const Babl * babl_format_bgra8 ();

#endif /* __BABL_H__ */
