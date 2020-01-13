#include <cstring>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "dialog.h"
#include "stxread.h"
#include "stxwrite.h"
#include "value.h"

#define LOAD_PROC "file_stx_load"
#define SAVE_PROC "file_stx_save"

static void query();
static void run(
  const gchar *name,
  const gint nparams,
  const GimpParam *param,
  gint *nreturn_vals,
  GimpParam **return_vals
);

GimpPlugInInfo PLUG_IN_INFO = {
  nullptr,
  nullptr,
  query,
  run
};

MAIN()

static void query() {
  static const GimpParamDef load_args[] = {
    { GIMP_PDB_INT32, "run-mode", "Run mode" },
    { GIMP_PDB_STRING, "filename", "File name" },
    { GIMP_PDB_STRING, "raw-filename", "File name entered" },
  };

  static const GimpParamDef load_returns[] = {
    { GIMP_PDB_IMAGE, "image", "Output image" }
  };

  static const GimpParamDef save_args[] = {
    { GIMP_PDB_INT32, "run-mode", "Run mode" },
    { GIMP_PDB_IMAGE, "image", "Input image" },
    { GIMP_PDB_DRAWABLE, "drawable", "Drawable to export" },
    { GIMP_PDB_STRING, "filename", "The name of the file to export the image in" },
    { GIMP_PDB_STRING, "raw-filename", "The name of the file to export the image in" },
    { GIMP_PDB_INT32, "e6-write", "Write E6 section?" },
    { GIMP_PDB_INT8, "magical-number", "Some magical number" },
    { GIMP_PDB_INT32, "scale-x", "Scale X" },
    { GIMP_PDB_INT32, "scale-y", "Scale Y" }
  };

  gimp_install_procedure(
    LOAD_PROC, "Load STX texture.",
    "Load textures in STX format.",
    "kotwys", "kotwys", "2020",
    "STX",
    "",
    GIMP_PLUGIN,
    G_N_ELEMENTS(load_args), G_N_ELEMENTS(load_returns),
    load_args, load_returns
  );
  gimp_register_load_handler(LOAD_PROC, "stx", "");

  gimp_install_procedure(
    SAVE_PROC, "Save STX texture.",
    "Export images in STX texture format.",
    "kotwys", "kotwys", "2020",
    "STX",
    "RGB*",
    GIMP_PLUGIN,
    G_N_ELEMENTS(save_args), 0,
    save_args, NULL
  );
  gimp_register_save_handler(SAVE_PROC, "stx", "");
}

GimpPDBStatusType load_stx(const char *filename, gint32 &image_id) {
  stxread reader(filename);
  if (!reader.good()) {
    g_message("Initializing error.\n");
    return GIMP_PDB_EXECUTION_ERROR;
  }

  StxError err = reader.process();
  if (err != StxError::SUCCESS) {
    g_message("Processing error.\n");
    return GIMP_PDB_EXECUTION_ERROR;
  }

  err = reader.to_image(image_id);
  if (err != StxError::SUCCESS) {
    g_message("Drawing error.\n");
    return GIMP_PDB_EXECUTION_ERROR;
  }

  return GIMP_PDB_SUCCESS;
}

GimpPDBStatusType save_stx(
  const char* filename,
  gint32 drawable_id,
  const StxParams &params
) {
  stxwrite writer(filename, params);
  if (!writer.good())
    return GIMP_PDB_EXECUTION_ERROR;

  StxError err = writer.write(drawable_id);
  if (err != StxError::SUCCESS)
    return GIMP_PDB_EXECUTION_ERROR;

  return GIMP_PDB_SUCCESS;
}

static void run(
  const gchar *name,
  const gint nparams,
  const GimpParam *param,
  gint *nreturn_vals,
  GimpParam **return_vals
) {
  static GimpParam values[2];
  *return_vals = values;
  *nreturn_vals = 1;
  values[0].type = GIMP_PDB_STATUS;

  GimpPDBStatusType status = GIMP_PDB_SUCCESS;

  if (strcmp(name, LOAD_PROC) == 0) {
    if (nparams != 3) {
      status = GIMP_PDB_CALLING_ERROR;
    } else {
      const char *filename = param[1].data.d_string;
      gint32 image_id;
      status = load_stx(filename, image_id);
      
      if (status == GIMP_PDB_SUCCESS) {
        *nreturn_vals = 2;
        values[1].type = GIMP_PDB_IMAGE;
        values[1].data.d_image = image_id;
      }
    }
  } else if (strcmp(name, SAVE_PROC) == 0) {
    auto run_mode = static_cast<GimpRunMode>(param[0].data.d_int32);
    gint32 image_id = param[1].data.d_int32;
    gint32 drawable_id = param[2].data.d_int32;
    
    StxParams params = default_params();
    
    if (run_mode != GIMP_RUN_NONINTERACTIVE) {
      gimp_ui_init(DIALOG_ID, FALSE);

      static auto capabilities
        = GIMP_EXPORT_CAN_HANDLE_RGB
        | GIMP_EXPORT_CAN_HANDLE_ALPHA;

      GimpExportReturn export_ = gimp_export_image(
        &image_id, &drawable_id, "STX",
        static_cast<GimpExportCapabilities>(capabilities)
      );

      if (export_ == GIMP_EXPORT_CANCEL) {
        values[0].data.d_status = GIMP_PDB_CANCEL;
        return;
      }

      gimp_get_data(SAVE_PROC, &params);
      if (run_mode == GIMP_RUN_INTERACTIVE) {
        if (!save_dialog(params))
          status = GIMP_PDB_CANCEL;
      }
    } else {
      if (nparams != 9) {
        status = GIMP_PDB_CALLING_ERROR;
      } else {
        params.e6_write = param[5].data.d_int32;
        params.magical_number = param[6].data.d_int8;
        params.scale_x = param[7].data.d_int32;
        params.scale_y = param[8].data.d_int32;
      }
    }

    if (status == GIMP_PDB_SUCCESS) {
      status = save_stx(
        param[3].data.d_string,
        drawable_id, params
      );
      if (status == GIMP_PDB_SUCCESS) {
        gimp_set_data(SAVE_PROC, &params, sizeof(params));
      }
    }
  } else {
    status = GIMP_PDB_CALLING_ERROR;
  }

  values[0].data.d_status = status;
}
