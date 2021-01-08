// Workaround for
// https://github.com/emscripten-core/emscripten/issues/12568
var setImmediate;
importScripts("./app.js");
Module({
  onRuntimeInitialized() {
    this.FS.createLazyFile(
      "/",
      "jpeg.wasm",
      new URL("/jpeg/jpeg.wasm", location).toString(),
      true /* can read */,
      false /* can write */
    );
    this.FS.createLazyFile(
      "/",
      "webp.wasm",
      new URL("/webp/webp.wasm", location).toString(),
      true /* can read */,
      false /* can write */
    );
    addEventListener("message", ({ data }) => {
      const { format, image } = data;
      const result = this.mymain(format, image.data, image.width, image.height);
      postMessage(result);
    });
  }
});
