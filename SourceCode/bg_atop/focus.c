/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2024 Beaver, Background on top

This is the GEGL syntax for "background on top". Paste it in GEGL Graph to use it without installing
id=1 color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]  id=2 color-to-alpha color=white id=3 ] crop
crop
median-blur radius=0 alpha-percentile=50
gaussian-blur std-dev-x=30 std-dev-y=30
id=2 src-atop aux=[ ref=2 layer src=/home/username/Pictures/my_photo_dirname_here.png
gaussian-blur std-dev-x=5 std-dev-y=5 unsharp-mask scale=0 hue-chroma hue=0  ]
opacity value=1

First GEGL plugin of 2024 by the way :D
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*This is GEGL syntax to invert transparency*/
#define syntaxzone \
" color-overlay value=#ffffff dst-over aux=[ color value=#000000  ]  color-to-alpha color=white   crop "\

property_color (value, _("Color"), "#000000")
    description (_("The color to paint over the inverted transparency fill or background image."))
    ui_meta     ("role", "color-primary")

property_double (opacity, _("Global Opacity"), 1.0)
    description (_("Global opacity value of the above color fill or background image. Gimp's build in opacity slider will not work because it reveals the original image."))
    value_range (0.0, 1.5)
    ui_range    (0.0, 1.5)


property_double (blur, _("Gaussian Blur "), 0.0)
   description (_("Gaussian Blur the entire color fill of background image."))
   value_range (0.0, 100.0)
   ui_range    (0.24, 100.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")

property_int (medianradius, _("Median Radius "), 0)
   description (_("Median Blur's radius option that corresponds to a prefixed alpha-percentile=0.0 "))
   value_range (-20, 20)
   ui_range    (-20, 20)
   ui_gamma    (3.0)

property_file_path(image, _("Image file overlay "), "")
    description (_("Upload an image which in default is blurred, files permitted are (png, jpg, raw, svg, bmp, tif, ...) Does not work proper with HSL Color mode. You can remove an image file overlay by going back to the image file select window, removing all text from --location-- then click open. Image file overlay will then go back to its default (None)"))

property_double (blurimage, _("Blur image file"), 15.0)
   description (_("Gaussian Blur for the image upload only.  If there is no image upload this will not work."))
   value_range (0.0, 100.0)
   ui_range    (0.24, 100.0)
   ui_gamma    (3.0)


property_double (sharpen, _("Sharpen Image"), 0.0)
    description(_("Sharpen for the image upload only. If there is no image upload this will not work."))
    value_range (0.0, 30.0)
    ui_range    (0.0, 30.0)
    ui_gamma    (3.0)

property_double (hue, _("Hue Rotation"), 0.0)
    description(_("Hue Rotation for the image upload only.  If there is no image upload this will not work."))
    value_range (-180.0, 180.0)
    ui_range    (-180.0, 180.0)
    ui_gamma    (3.0)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     focus
#define GEGL_OP_C_SOURCE focus.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *it, *gaus, *huelight, *atop, *sharpen, *image, *gaus2, *opacity, *median, *col;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntaxzone,
                                  NULL);

  gaus    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", 
                                  NULL);

  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 0.0, 
                                  NULL);

  col    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  image    = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

  opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  sharpen    = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);

  huelight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  gaus2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", 
                                  NULL);

      gegl_operation_meta_redirect (operation, "value", col, "value");
      gegl_operation_meta_redirect (operation, "blur", gaus, "std-dev-x");
      gegl_operation_meta_redirect (operation, "blur", gaus, "std-dev-y");
      gegl_operation_meta_redirect (operation, "sharpen", sharpen, "scale");
      gegl_operation_meta_redirect (operation, "hue", huelight, "hue");
      gegl_operation_meta_redirect (operation, "medianradius", median, "radius");
      gegl_operation_meta_redirect (operation, "image", image, "src");
      gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
      gegl_operation_meta_redirect (operation, "blurimage", gaus2, "std-dev-x");
      gegl_operation_meta_redirect (operation, "blurimage", gaus2, "std-dev-y");

      gegl_node_link_many (input, it, median, gaus, col, atop, opacity, output, NULL);
      gegl_node_link_many (image, gaus2, sharpen, huelight, NULL);
      gegl_node_connect (atop, "aux", huelight, "output");
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:bg-atop",
    "title",       _("Background on top layer."),
    "reference-hash", "233antifavsproudboys35923409",
    "description", _("Invert transparency of an alpha defined channel and then make a blurred color fill (with possibly an image) that focuses on the subject/object. This puts the background layer on a layer above instead of below like usual."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Light and Shadow/",
    "gimp:menu-label", _("Background on top layer..."),
    NULL);
}

#endif
