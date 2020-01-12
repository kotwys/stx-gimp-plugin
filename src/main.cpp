#include <cstring>
#include <libgimp/gimp.h>

#include "stxread.h"

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

  gimp_install_procedure(
    LOAD_PROC, "Load STX texture.",
    "Load images from Winx Club game. Their file suffix is .stx",
    "kotwys", "kotwys", "2020",
    "STX",
    "",
    GIMP_PLUGIN,
    G_N_ELEMENTS(load_args), G_N_ELEMENTS(load_returns),
    load_args, load_returns
  );
  gimp_register_load_handler(LOAD_PROC, "stx", "");

  // TODO: save
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
  } else {
    status = GIMP_PDB_CALLING_ERROR;
  }

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
}
