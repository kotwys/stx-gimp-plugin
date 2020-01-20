#include "gimp_interop.h"
#include "structure.h"

StxResult<gint32> to_gimp(const StxImage &data) {
  using Result = StxResult<gint32>;

  gint32 image_id = gimp_image_new(
    data.geometry.width,
    data.geometry.height,
    GIMP_RGB
  );

  gint32 layer_id = gimp_layer_new(
    image_id,
    "Texture",
    data.geometry.width, data.geometry.height,
    GIMP_RGBA_IMAGE,
    100.0,
    GIMP_NORMAL_MODE
  );
  gboolean success = gimp_image_insert_layer(image_id, layer_id, 0, 0);
  
  if (!success) {
    gimp_image_delete(image_id);
    return ERR(StxError::GIMP_ERROR);
  }

  GimpDrawable *drawable = gimp_drawable_get(layer_id);

  GimpPixelRgn rgn_out;
  gimp_pixel_rgn_init(
    &rgn_out, drawable,
    0, 0, data.geometry.width, data.geometry.height,
    TRUE, TRUE
  );

  gimp_pixel_rgn_set_rect(
    &rgn_out, data.image_data,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  delete[] data.image_data;

  gimp_drawable_flush(drawable);
  gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
  gimp_drawable_update(
    drawable->drawable_id,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  gimp_drawable_detach(drawable);

  return OK(image_id);
}

StxResult<StxImage> from_gimp(
  const StxParams &params,
  const gint32 drawable_id
) {
  using Result = StxResult<StxImage>;

  StxImage img;

  guint16 width = gimp_drawable_width(drawable_id);
  guint16 height = gimp_drawable_height(drawable_id);

  
  Geometry geometry = {
    width,
    height,
    params.scale_x,
    params.scale_y
  };
  img.geometry = geometry;

  const bool has_alpha = gimp_drawable_has_alpha(drawable_id);
  const size_t offset = has_alpha ? 4 : 3;

  GimpPixelRgn rgn_in;
  GimpDrawable *drawable = gimp_drawable_get(drawable_id);

  gimp_pixel_rgn_init(&rgn_in, drawable, 0, 0, width, height, FALSE, FALSE);
  size_t pixels = width * height;
  guchar* data = new guchar[pixels * offset];
  gimp_pixel_rgn_get_rect(&rgn_in, data, 0, 0, width, height);
  gimp_drawable_detach(drawable);

  img.image_data = new guchar[pixels * STX_NUM_CHANNELS];

  for (size_t i = 0; i < pixels; i++) {
    size_t src_pos = i * offset;
    size_t dst_pos = i * STX_NUM_CHANNELS;
    // RGBA -> BGRA
    img.image_data[dst_pos] = data[src_pos + 2];
    img.image_data[dst_pos + 1] = data[src_pos + 1];
    img.image_data[dst_pos + 2] = data[src_pos];
    img.image_data[dst_pos + 3] = has_alpha ? data[src_pos + 3] : 0xff;
  }

  delete[] data;

  return OK(img);
}
