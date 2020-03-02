#ifndef __STX_PARAMS_H__
#define __STX_PARAMS_H__

#include <glib.h>

G_BEGIN_DECLS

typedef struct _StxParams StxParams;
struct _StxParams {
  guint16 scale_x;
  guint16 scale_y;
  gboolean e6_write;
  guint8 magical_number;
};

StxParams stx_params_default();

G_END_DECLS

#endif
