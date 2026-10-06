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

#include <stdio.h>
#include <stdbool.h>

#include <libgimp/gimp.h>
#include <glib/gi18n.h>
#include <splendente/stx.h>

#include "babl-helper.h"
#include "stx-load.h"
#include "stx-settings.h"
#include "swizzle.h"

static void
draw_texture_ps2 (GeglBuffer *buffer,
                  const char *pixel_data,
                  uint32_t   *colors,
                  size_t      width,
                  size_t      height)
{
  const Babl  *format = BABL_FORMAT_PALETTE;
  size_t       x, y;
  union {
    uint32_t whole;
    char     rgba[4];
  } color;

  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          size_t  offset   = y * width + x;
          uint8_t color_id = pixel_data[offset];
          color.whole      = colors[color_id];
          color.rgba[3]    = color.rgba[3] * 2;
          gegl_buffer_set (buffer, GEGL_RECTANGLE (x, y, 1, 1), 0, format,
                           &color, 4);
        }
    }
}

GimpImage *
load_image (GFile   *file,
            GError **error)
{
  GimpImage    *image = NULL;
  GimpLayer    *layer;
  char         *file_contents = NULL;
  size_t        file_length;
  StxReadState *state     = NULL;
  uint16_t      stx_error = 0;
  GeglBuffer   *buffer    = NULL;
  StxDocument  *doc       = NULL;
  StxTexture   *tx        = NULL;
  StxSettings   settings  = {0};
  size_t        width, height;
  char         *pixel_data;

  g_file_load_contents (file, NULL, &file_contents, &file_length, NULL, error);
  if (*error)
    return NULL;

  state = stx_read_state_init ();
  doc = stx_document_read (file_contents, file_length, state, &stx_error);
  if (stx_error)
    {
      const char *error_msg = stx_error_string (stx_error);
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   _("Error loading STX file: %s"),
                   error_msg);
      goto out;
    }
  stx_extract_settings (doc, &settings);

  tx         = stx_document_get_texture (doc);
  pixel_data = stx_texture_get_data (tx, &width, &height);

  // We always create an RGBA image since GIMP handles indexed colours
  // separately from the alpha channel in contrary to STX.
  image = gimp_image_new (width, height, GIMP_RGB);
  stx_save_settings (&settings, image);
  layer = gimp_layer_new (image, NULL,
                          width, height,
                          GIMP_RGBA_IMAGE, 100,
                          gimp_image_get_default_new_layer_mode (image));
  gimp_image_insert_layer (image, layer, NULL, 0);
  buffer = gimp_drawable_get_buffer (GIMP_DRAWABLE (layer));

  switch (stx_texture_kind (tx))
    {
    case STX_KIND_PS2:
      uint32_t *colors     = stx_texture_get_palette (tx, NULL);
      char     *unswizzled = g_new (char, width * height);
      psmt8_swizzle (pixel_data, unswizzled, width, height, FALSE);
      draw_texture_ps2 (buffer, unswizzled, colors, width, height);
      g_free (unswizzled);
      break;
    case STX_KIND_PC:
      const Babl *format = BABL_FORMAT_PC;
      gegl_buffer_set (buffer, GEGL_RECTANGLE (0, 0, width, height),
                       0, format, pixel_data, GEGL_AUTO_ROWSTRIDE);
      break;
    }

out:
  if (file_contents) g_free (file_contents);
  if (state)         stx_read_state_free (state);
  if (doc)           stx_document_free (doc);
  if (buffer)        g_object_unref (buffer);

  return image;
}
