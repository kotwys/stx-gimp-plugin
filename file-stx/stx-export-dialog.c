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

#include "stx-export-dialog.h"

static void kind_changed (GimpProcedureConfig *config,
                          const GParamSpec    *spec,
                          GtkLabel            *widget);

bool
export_dialog (GimpProcedure       *procedure,
               GimpProcedureConfig *config,
               GimpImage           *image)
{
  GtkWidget *dialog;
  GtkWidget *kind_hint;
  bool       run;

  dialog = gimp_export_procedure_dialog_new (GIMP_EXPORT_PROCEDURE (procedure),
                                             config,
                                             image);
  gimp_window_set_transient (GTK_WINDOW (dialog));

  // Force a combo box view
  gimp_procedure_dialog_get_widget (GIMP_PROCEDURE_DIALOG (dialog),
                                    "kind", GTK_TYPE_COMBO_BOX);

  kind_hint = gimp_procedure_dialog_get_label (GIMP_PROCEDURE_DIALOG (dialog),
                                               "kind-hint", NULL,
                                               FALSE, FALSE);
  gtk_label_set_xalign          (GTK_LABEL (kind_hint), 0.0);
  gtk_label_set_line_wrap       (GTK_LABEL (kind_hint), TRUE);
  gtk_label_set_max_width_chars (GTK_LABEL (kind_hint), 50);
  gimp_label_set_attributes     (GTK_LABEL (kind_hint),
                                 PANGO_ATTR_STYLE, PANGO_STYLE_ITALIC,
                                 -1);
  g_signal_connect (config, "notify::kind",
                    G_CALLBACK (kind_changed), kind_hint);
  kind_changed (config, NULL, GTK_LABEL (kind_hint));

  gimp_procedure_dialog_fill (GIMP_PROCEDURE_DIALOG (dialog),
                              "write-metadata",
                              "kind", "kind-hint",
                              NULL);

  run = gimp_procedure_dialog_run (GIMP_PROCEDURE_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  return run;
}

static void
kind_changed (GimpProcedureConfig *config,
              const GParamSpec    *spec,
              GtkLabel            *widget)
{
  StxKind new_kind;
  new_kind = gimp_procedure_config_get_choice_id (config, "kind");
  if (new_kind == STX_KIND_PS2)
    gtk_label_set_text (widget,
                        _("WARNING: A palette of at most 256 colors "
                          "will be created for the texture which may result "
                          "in some loss of quality."));
  else
    gtk_label_set_text (widget, "");
}
