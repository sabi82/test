
#include <stdlib.h>
#include <stdio.h>
#include "readwritepng.h"

extern 
void read_png_file(const char *filename, ImagePNG *Image) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) exit(EXIT_FAILURE);

  png_infop info = png_create_info_struct(png);
  if(!info) exit(EXIT_FAILURE);

  if(setjmp(png_jmpbuf(png))) exit(EXIT_FAILURE);

  png_init_io(png, fp);

  png_read_info(png, info);

  Image->width      = png_get_image_width(png, info);
  Image->height     = png_get_image_height(png, info);
  Image->color_type = png_get_color_type(png, info);
  Image->bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(Image->bit_depth == 16)
    png_set_strip_16(png);

  if(Image->color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(Image->color_type == PNG_COLOR_TYPE_GRAY && Image->bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(Image->color_type == PNG_COLOR_TYPE_RGB ||
     Image->color_type == PNG_COLOR_TYPE_GRAY ||
     Image->color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(Image->color_type == PNG_COLOR_TYPE_GRAY ||
     Image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  Image->row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * Image->height);
  for(int y = 0; y < Image->height; y++) {
    Image->row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, Image->row_pointers);

  fclose(fp);
}

extern void 
write_png_file(const char *filename, ImagePNG *Image) {
  FILE *fp = fopen(filename, "wb");
  if(!fp) exit(EXIT_FAILURE);

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) exit(EXIT_FAILURE);

  png_infop info = png_create_info_struct(png);
  if (!info) exit(EXIT_FAILURE);

  if (setjmp(png_jmpbuf(png))) exit(EXIT_FAILURE);

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
	       png,
	       info,
	       Image->width, Image->height,
	       8,
	       PNG_COLOR_TYPE_RGBA,
	       PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
	       );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  png_write_image(png, Image->row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < Image->height; y++) {
    free(Image->row_pointers[y]);
  }
  free(Image->row_pointers);
  fclose(fp);
}
