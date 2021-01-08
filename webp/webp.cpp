#include <emscripten/val.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include "src/webp/encode.h"

#include "dynarr.h"

using namespace emscripten;

extern "C" DynArr encode(std::string img, int width, int height) {
  auto img_in = (uint8_t *)img.c_str();

  uint8_t *buffer;
  auto size = WebPEncodeRGBA(img_in, width, height, width * 4, 1, &buffer);
  if (size <= 0) {
    return DynArr{0, nullptr};
  }
  return DynArr{size, buffer};
}
