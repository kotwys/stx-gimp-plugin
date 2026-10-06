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

#include <stdbool.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <glib/gi18n.h>
#include <splendente/stx.h>

#include "stx-load.h"
#include "stx-settings.h"
#include "stx-export.h"
#include "stx-export-dialog.h"

#define LOAD_PROC   "file-stx-load"
#define EXPORT_PROC "file-stx-export"

#define PLUG_IN_BINARY "file-stx"

#define STX_KINDS \
  X(STX_KIND_PC,  "pc",  _("PC (BGRA)")) \
  X(STX_KIND_PS2, "ps2", _("PlayStation® 2 (indexed)"))

struct _Stx
{
  GimpPlugIn parent_instance;
};

#define STX_TYPE (stx_get_type())
G_DECLARE_FINAL_TYPE (Stx, stx, STX,, GimpPlugIn)
G_DEFINE_TYPE (Stx, stx, GIMP_TYPE_PLUG_IN)

static GList          * stx_query_procedures (GimpPlugIn  *plug_in);
static GimpProcedure  * stx_create_procedure (GimpPlugIn  *plug_in,
                                              const gchar *name);

static GimpValueArray * stx_load             (GimpProcedure         *procedure,
                                              GimpRunMode            run_mode,
                                              GFile                 *file,
                                              GimpMetadata          *metadata,
                                              GimpMetadataLoadFlags *flags,
                                              GimpProcedureConfig   *config,
                                              gpointer               run_data);
static GimpValueArray * stx_export           (GimpProcedure         *procedure,
                                              GimpRunMode            run_mode,
                                              GimpImage             *image,
                                              GFile                 *file,
                                              GimpExportOptions     *options,
                                              GimpMetadata          *metadata,
                                              GimpProcedureConfig   *config,
                                              gpointer               run_data);

static const char *
stx_get_kind_string (StxKind kind)
{
  switch (kind)
    {
    #define X(id, str, desc) case id: return str;
    STX_KINDS
    #undef X
    default: return NULL; // suppress compiler warning
    }
}

static void
stx_class_init (StxClass *klass)
{
  GimpPlugInClass *plug_in_class = GIMP_PLUG_IN_CLASS (klass);

  plug_in_class->query_procedures = stx_query_procedures;
  plug_in_class->create_procedure = stx_create_procedure;
}

static void
stx_init (Stx *stx)
{
}

static GList *
stx_query_procedures (GimpPlugIn *plug_in)
{
  GList *list = NULL;

  list = g_list_append (list, g_strdup (LOAD_PROC));
  list = g_list_append (list, g_strdup (EXPORT_PROC));

  return list;
}

static GimpProcedure *
stx_create_procedure (GimpPlugIn  *plug_in,
                      const gchar *name)
{
  GimpProcedure *procedure = NULL;

  if (g_strcmp0 (name, LOAD_PROC) == 0)
    {
      procedure = gimp_load_procedure_new (plug_in, name,
                                           GIMP_PDB_PROC_TYPE_PLUGIN,
                                           stx_load, NULL, NULL);

      gimp_procedure_set_documentation (procedure,
                                        _("Loads Sparkplug textures"),
                                        _("Loads files in the Sparkplug "
                                          "texture format. For PS2, the "
                                          "unswizzling code from ReverseBox "
                                          "is used."),
                                        NULL);
    }
  else if (g_strcmp0 (name, EXPORT_PROC) == 0)
    {
      procedure = gimp_export_procedure_new (plug_in, name,
                                             GIMP_PDB_PROC_TYPE_PLUGIN,
                                             FALSE, stx_export, NULL, NULL);

      gimp_procedure_set_image_types (procedure, "RGB*");

      gimp_procedure_set_documentation (procedure,
                                        _("Exports Sparkplug textures"),
                                        _("Exports images as standalone "
                                          "Sparkplug texture files (STX). "
                                          "For PS2, uses libimagequant for "
                                          "color quantization and the swizzling "
                                          "code from ReverseBox."),
                                        NULL);

      gimp_file_procedure_set_format_name (GIMP_FILE_PROCEDURE (procedure),
                                           _("STX"));

      gimp_export_procedure_set_capabilities (GIMP_EXPORT_PROCEDURE (procedure),
                                              GIMP_EXPORT_CAN_HANDLE_RGB |
                                              GIMP_EXPORT_CAN_HANDLE_ALPHA,
                                              NULL, NULL, NULL);

      #define X(id, str, desc) str, id, desc, NULL,
      gimp_procedure_add_choice_argument
        (procedure, "kind", _("Texture _kind"), NULL,
         gimp_choice_new_with_values (STX_KINDS/*, */ NULL),
         "pc", G_PARAM_READWRITE);
      #undef X

      gimp_procedure_add_boolean_argument
        (procedure, "write-metadata", _("Write optional _metadata"), NULL,
         TRUE, G_PARAM_READWRITE);
    }

  gimp_procedure_set_menu_label (procedure, _("Sparkplug texture"));
  gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure), "stx");
  gimp_file_procedure_set_magics (GIMP_FILE_PROCEDURE (procedure),
                                  "0,byte,0x22&1,byte,0&2,byte,0");

  gimp_procedure_set_attribution (procedure,
                                  "kotwys",
                                  "kotwys",
                                  "2020, 2026");

  return procedure;
}

static GimpValueArray *
stx_load (GimpProcedure         *procedure,
          GimpRunMode            run_mode,
          GFile                 *file,
          GimpMetadata          *metadata,
          GimpMetadataLoadFlags *flags,
          GimpProcedureConfig   *config,
          gpointer               run_data)
{
  GimpValueArray *return_vals;
  GimpImage      *image;
  GError         *error = NULL;

  image = load_image (file, &error);
  if (! image)
    return gimp_procedure_new_return_values (procedure,
                                             GIMP_PDB_EXECUTION_ERROR,
                                             error);

  return_vals = gimp_procedure_new_return_values (procedure,
                                                  GIMP_PDB_SUCCESS,
                                                  NULL);
  GIMP_VALUES_SET_IMAGE (return_vals, 1, image);

  return return_vals;
}

static GimpValueArray *
stx_export (GimpProcedure       *procedure,
            GimpRunMode          run_mode,
            GimpImage           *image,
            GFile               *file,
            GimpExportOptions   *options,
            GimpMetadata        *metadata,
            GimpProcedureConfig *config,
            gpointer             run_data)
{
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;
  GimpImage         *orig_image;
  GimpExportReturn   export = GIMP_EXPORT_IGNORE;
  GError            *error  = NULL;
  StxSettings        settings = {0};

  orig_image = image;

  switch (run_mode)
  {
    case GIMP_RUN_NONINTERACTIVE:
      break;
    case GIMP_RUN_INTERACTIVE:
    case GIMP_RUN_WITH_LAST_VALS:
      // Override the config with the actual STX settings if present. The
      // parasite would be then updated to match the user settings
      if (stx_restore_settings (orig_image, &settings))
        {
          g_object_set (config,
                        "kind",           stx_get_kind_string (settings.kind),
                        "write-metadata", settings.has_metadata,
                        NULL);
        }
      break;
  }

  export = gimp_export_options_get_image (options, &image);

  if (run_mode == GIMP_RUN_INTERACTIVE)
    {
      gimp_ui_init (PLUG_IN_BINARY);
      if (! export_dialog (procedure, config, image))
        {
          status = GIMP_PDB_CANCEL;
        }
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      g_object_get (config,
                    "write-metadata", &settings.has_metadata,
                    NULL);
      settings.kind = gimp_procedure_config_get_choice_id (config, "kind");

      export_image (file, image, &settings, &error);
      if (error)
        status = GIMP_PDB_EXECUTION_ERROR;
      else
        stx_save_settings (&settings, orig_image);
    }

  if (export == GIMP_EXPORT_EXPORT)
    gimp_image_delete (image);

  return gimp_procedure_new_return_values (procedure, status, error);
}

GIMP_MAIN (STX_TYPE)
