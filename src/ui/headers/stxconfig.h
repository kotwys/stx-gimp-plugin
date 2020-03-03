#ifndef __STX_CONFIG_H__
#define __STX_CONFIG_H__

#include <glib.h>

G_BEGIN_DECLS

typedef struct _StxConfig StxConfig;
struct _StxConfig {
  guint16 scale_x;
  guint16 scale_y;
  gboolean e6_write;
  guint16 magical_number;
};

StxConfig stx_config_default();

G_END_DECLS

#endif
