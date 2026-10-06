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

#ifndef __STX_SETTINGS_H__
#define __STX_SETTINGS_H__

typedef struct
{
  StxKind kind;
  bool    has_metadata;
} StxSettings;

/**
 * Extracts settings from @p doc.
 *
 * @param[in]  doc      STX document to extract settings from
 * @param[out] settings
 */
void stx_extract_settings (StxDocument *doc,
                           StxSettings *settings);

/**
 * Saves the settings as a parasite of @p image.
 *
 * @param setings STX settings
 * @param image   GIMP image to attach the parasite to
 */
void stx_save_settings    (const StxSettings *settings,
                           GimpImage         *image);

/**
 * Retrieves STX settings previously attached to @p image through a parasite.
 *
 * @param[in]  image    GIMP image
 * @param[out] settings STX settings
 *
 * @return TRUE if a parasite was attached to the image
 */
bool stx_restore_settings (GimpImage   *image,
                           StxSettings *settings);

#endif /* __STX_SETTINGS_H__ */
