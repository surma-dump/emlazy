#include <dlfcn.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdio.h>

#include "dynarr.h"

using namespace emscripten;

typedef DynArr (*EncodeFunc)(std::string img, int width, int height);

val Uint8Array = val::global("Uint8Array");
val Blob = val::global("Blob");

val mymain(std::string format, std::string img, int width, int height) {
  double r = val::global("Math").call<double>("random");
  void *handle;
  std::string mimetype;
  if (format == "jpeg") {
    handle = dlopen("/jpeg.wasm", RTLD_LAZY);
  } else if (format == "webp") {
    handle = dlopen("/webp.wasm", RTLD_LAZY);
  } else {
    return val::null();
  }
  EncodeFunc encode = (EncodeFunc)dlsym(handle, "encode");
  if (encode == NULL) {
    printf("Error: %s\n", dlerror());
    return val::null();
  }
  DynArr result = encode(img, width, height);
  dlclose(handle);

  auto js_result = val::null();
  if (result.size != 0) {
    auto buffer =
        Uint8Array.new_(typed_memory_view(result.size, result.buffer));
    free(result.buffer);
    auto segments = val::array();
    segments.call<double>("push", buffer);
    auto options = val::object();
    options.set("type", "image/" + format);
    js_result = Blob.new_(segments, options);
  }

  return js_result;
}

EMSCRIPTEN_BINDINGS(my_module) { function("mymain", &mymain); }
