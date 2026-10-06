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

#include <libgimp/gimp.h>

#include "babl-helper.h"

const Babl *
babl_format_bgra8 ()
{
  return babl_format_new (babl_model ("R~G~B~A"),
                          babl_type ("u8"),
                          babl_component ("B~"),
                          babl_component ("G~"),
                          babl_component ("R~"),
                          babl_component ("A"),
                          NULL);
}
