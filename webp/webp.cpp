#include <emscripten/val.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include "src/webp/encode.h"

#include "dynarr.h"

using namespace emscripten;

extern "C" DynArr encode(std::string img, int width, int height) {
  auto img_in = (uint8_t *)img.c_str();
  WebPConfig config;

  // A lot of this is duplicated from Encode in picture_enc.c
  WebPPicture pic;
  WebPMemoryWriter wrt;
  int ok;

  if (!WebPPictureInit(&pic)) {
    // shouldn't happen, except if system installation is broken
    return DynArr{0, nullptr};
  }

  // Allow quality to go higher than 0.
  config.qmax = 100;
  config.quality = 75;

  pic.width = width;
  pic.height = height;
  pic.writer = WebPMemoryWrite;
  pic.custom_ptr = &wrt;

  WebPMemoryWriterInit(&wrt);

  ok = WebPPictureImportRGBA(&pic, img_in, width * 4) &&
       WebPEncode(&config, &pic);
  if (!ok) {
    return DynArr{0, nullptr};
  }
  WebPPictureFree(&pic);
  uint8_t *buffer = (uint8_t *)malloc(wrt.size);
  memcpy(buffer, wrt.mem, wrt.size);
  WebPMemoryWriterClear(&wrt);

  return DynArr{wrt.size, buffer};
}
