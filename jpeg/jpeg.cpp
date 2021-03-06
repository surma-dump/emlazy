#include <cstdlib>
#include <emscripten/val.h>
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "dynarr.h"
#include "jpeglib.h"

extern "C" {
#include "cdjpeg.h"
}

using namespace emscripten;

extern "C" DynArr encode(std::string image_in, int image_width,
                         int image_height) {
  uint8_t *image_buffer = (uint8_t *)image_in.c_str();

  /*******************************************************************
   * This is basically copy-pasted from the MozJPEG sample code
   *
   * See: https://github.com/mozilla/mozjpeg/blob/master/example.txt
   *******************************************************************/
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  uint8_t *output = nullptr;
  unsigned long size = 0;
  jpeg_mem_dest(&cinfo, &output, &size);
  cinfo.image_width = image_width;
  cinfo.image_height = image_height;
  cinfo.input_components = 4;
  cinfo.in_color_space = JCS_EXT_RGBA;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 1, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  int row_stride = image_width * 4;

  while (cinfo.next_scanline < cinfo.image_height) {
    JSAMPROW row_pointer =
        &image_buffer[cinfo.next_scanline *
                      row_stride]; /* pointer to JSAMPLE row[s] */
    (void)jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);

  /************************************************************************
   * End of copy/paste
   ************************************************************************/

  uint8_t *buffer = (uint8_t *)malloc(size);
  memcpy(buffer, output, size);

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
  return DynArr{size, output};
}