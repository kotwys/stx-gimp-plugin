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
#include <splendente/stx.h>

#include "stx-settings.h"

#define PARASITE_NAME "stx-settings"

void
stx_extract_settings (StxDocument *doc,
                      StxSettings *settings)
{
  StxTexture *tx;
  tx = stx_document_get_texture (doc);
  settings->kind         = stx_texture_kind (tx);
  settings->has_metadata = stx_document_get_version (doc) != 0;
}

void
stx_save_settings (const StxSettings *settings,
                   GimpImage         *image)
{
  GimpParasite  *parasite;

  // Parasite is attached only for the current session.
  parasite = gimp_parasite_new (PARASITE_NAME, 0,
                                sizeof(StxSettings),
                                settings);
  gimp_image_attach_parasite (image, parasite);
  gimp_parasite_free (parasite);
}

bool
stx_restore_settings (GimpImage   *image,
                      StxSettings *settings)
{
  GimpParasite      *parasite;
  const StxSettings *retrieved;
  uint32_t           num_bytes;

  g_return_val_if_fail (settings != NULL, FALSE);

  parasite = gimp_image_get_parasite (image, PARASITE_NAME);
  if (! parasite)
    return FALSE;

  retrieved = gimp_parasite_get_data (parasite, &num_bytes);
  if (num_bytes != sizeof(StxSettings))
    {
      // Something fishy happened with the parasite, ignore.
      gimp_parasite_free (parasite);
      return FALSE;
    }

  *settings = *retrieved;
  gimp_parasite_free (parasite);
  return TRUE;
}

