#include "src/webp/encode.h"
#include <emscripten/val.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

using namespace emscripten;

thread_local const val Uint8Array = val::global("Uint8Array");

val encode(std::string img, int width, int height) {
  auto img_in = (uint8_t *)img.c_str();
  WebPConfig config;

  // A lot of this is duplicated from Encode in picture_enc.c
  WebPPicture pic;
  WebPMemoryWriter wrt;
  int ok;

  if (!WebPPictureInit(&pic)) {
    // shouldn't happen, except if system installation is broken
    return val::null();
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
  WebPPictureFree(&pic);
  val js_result =
      ok ? Uint8Array.new_(typed_memory_view(wrt.size, wrt.mem)) : val::null();
  WebPMemoryWriterClear(&wrt);
  return js_result;
}
