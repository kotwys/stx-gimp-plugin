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
#include <glib/gi18n.h>
#include <libimagequant.h>
#include <splendente/stx.h>

#include "babl-helper.h"
#include "stx-settings.h"
#include "stx-export.h"
#include "swizzle.h"

/**
 * Quantizes and swizzles the image as required by the PS2 format.
 *
 * @p init must be already populated with width and height.
 *
 * @param[inout] init   the init object for the STX document.
 * @param[in]    buffer the GEGL buffer for the image
 * @param[out]   error
 */
static void
prepare_ps2 (StxDocumentInit  *init,
             GeglBuffer       *buffer,
             GError          **error)
{
  size_t             n_pixels;
  const Babl        *format     = BABL_FORMAT_PALETTE;
  char              *rgba       = NULL;
  liq_attr          *attr       = NULL;
  liq_image         *img        = NULL;
  liq_result        *res        = NULL;
  const liq_palette *palette;
  char              *unswizzled = NULL;
  size_t             i;

  attr = liq_attr_create ();
  if (! attr)
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   _("Could not initialize quantization"));
      return;
    }

  n_pixels = init->width * init->height;
  rgba = g_malloc (n_pixels * 4);
  gegl_buffer_get (buffer, GEGL_RECTANGLE (0, 0, init->width, init->height),
                   1.0, format, rgba,
                   GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);

  img = liq_image_create_rgba (attr, rgba, init->width, init->height, 0);
  if (liq_image_quantize (img, attr, &res) != LIQ_OK)
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   _("Could not generate a palette"));
      goto out;
    }

  unswizzled   = g_malloc (n_pixels);
  init->pixels = g_malloc (n_pixels);
  liq_write_remapped_image (res, img, unswizzled, n_pixels);
  psmt8_swizzle (unswizzled, init->pixels, init->width, init->height, TRUE);

  // liq_get_palette needs to be called after remapping
  palette           = liq_get_palette (res);
  init->color_count = palette->count;
  init->palette     = g_new (uint32_t, init->color_count);
  for (i = 0; i < palette->count; i++)
    {
      char *color = (char *)(init->palette + i);
      color[0]    = palette->entries[i].r;
      color[1]    = palette->entries[i].g;
      color[2]    = palette->entries[i].b;
      color[3]    = palette->entries[i].a / 2;
    }

out:
  if (res)        liq_result_destroy (res);
  if (img)        liq_image_destroy (img);
  if (attr)       liq_attr_destroy (attr);
  if (rgba)       g_free (rgba);
  if (unswizzled) g_free (unswizzled);
}

void
export_image (GFile              *file,
              GimpImage          *image,
              const StxSettings  *settings,
              GError            **error)
{
  GimpDrawable    *drawable   = NULL;
  GeglBuffer      *buffer     = NULL;
  StxDocumentInit  init       = {0};
  StxDocument     *doc        = NULL;
  char            *stx_buffer = NULL;
  size_t           capacity, length;

  init.kind = settings->kind;
  if (settings->has_metadata)
    {
      switch (settings->kind)
      {
      case STX_KIND_PC:
        init.version = 6;
        break;
      case STX_KIND_PS2:
        init.version = 8;
        break;
      }
    }

  drawable = GIMP_DRAWABLE (gimp_image_merge_visible_layers
                            (image, GIMP_CLIP_TO_IMAGE));
  buffer   = gimp_drawable_get_buffer (drawable);

  init.width  = gimp_drawable_get_width (drawable);
  init.height = gimp_drawable_get_height (drawable);

  switch (init.kind)
    {
    case STX_KIND_PC:
      const Babl *format = BABL_FORMAT_PC;
      init.pixels        = g_new (char, init.width * init.height * 4);
      gegl_buffer_get (buffer, GEGL_RECTANGLE (0, 0, init.width, init.height),
                       1.0, format, init.pixels,
                       GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
      break;
    case STX_KIND_PS2:
      prepare_ps2 (&init, buffer, error);
      if (*error) goto out;
      break;
    }

  doc = stx_document_create (&init);
  if (! doc)
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   _("Could not create an STX document"));
      goto out;
    }

  capacity   = stx_document_estimate_size (doc);
  stx_buffer = g_new (char, capacity);
  if (! stx_document_write_buffer (doc, stx_buffer, capacity, &length))
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   _("Could not write the STX document"));
      goto out;
    }

  g_file_replace_contents (file, stx_buffer, length,
                           NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL,
                           error);

out:
  if (buffer)     g_object_unref (buffer);
  if (doc)        stx_document_free (doc);
  if (stx_buffer) g_free (stx_buffer);
}
