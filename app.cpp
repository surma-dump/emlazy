#include <dlfcn.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdio.h>

#include "dynarr.h"

using namespace emscripten;

typedef DynArr (*EncodeFunc)(std::string img, int width, int height);

thread_local const val Uint8Array = val::global("Uint8Array");

std::string data =
    std::string("\xff\x00\x00\xff") + std::string("\x00\xff\x00\xff") +
    std::string("\x00\x00\xff\xff") + std::string("\xff\xff\xff\xff");

val mymain() {
  double r = val::global("Math").call<double>("random");
  void *handle;
  if (r > 0.5) {
    handle = dlopen("/jpeg.wasm", RTLD_LAZY);
  } else {
    handle = dlopen("/webp.wasm", RTLD_LAZY);
  }
  EncodeFunc encode = (EncodeFunc)dlsym(handle, "encode");
  if (encode == NULL) {
    printf("Error: %s\n", dlerror());
    return val::null();
  }
  DynArr result = encode(data, 2, 2);
  dlclose(handle);

  auto js_result = val::null();
  if (result.size != 0) {
    js_result = Uint8Array.new_(typed_memory_view(result.size, result.buffer));
  }
  free(result.buffer);

  return js_result;
}

EMSCRIPTEN_BINDINGS(my_module) { function("mymain", &mymain); }
