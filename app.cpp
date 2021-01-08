// #include <cstdlib>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

int main() {
  double r = val::global("Math").call<double>("random");
  if (r > 0.5) {
    printf("JPEG!\n");
  } else {
    printf("WEBP!\n");
  }
  return 0;
}