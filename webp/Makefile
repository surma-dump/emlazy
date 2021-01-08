CODEC_URL = https://github.com/webmproject/libwebp/archive/d2e245ea9e959a5a79e1db0ed2085206947e98f2.tar.gz
CODEC_DIR = node_modules/libwebp
CODEC_BUILD := $(CODEC_DIR)/build
ENVIRONMENT = worker

OUT := webp.wasm

.PHONY: all clean

all: $(OUT)

$(OUT): webp.cpp

%.wasm: $(CODEC_BUILD)/libwebp.a
	$(CXX) \
		-I $(CODEC_DIR) \
		${CXXFLAGS} \
		${LDFLAGS} \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s MODULARIZE=1 \
		-s ENVIRONMENT=$(ENVIRONMENT) \
		-s SIDE_MODULE=1 \
		-o $@ \
		$+

%/libwebp.a: %/Makefile
	$(MAKE) -C $(@D)

%/Makefile: $(CODEC_DIR)/CMakeLists.txt
	emcmake cmake \
		$(CMAKE_FLAGS) \
		-DCMAKE_DISABLE_FIND_PACKAGE_Threads=1 \
		-DWEBP_BUILD_ANIM_UTILS=0 \
		-DWEBP_BUILD_CWEBP=0 \
		-DWEBP_BUILD_DWEBP=0 \
		-DWEBP_BUILD_GIF2WEBP=0 \
		-DWEBP_BUILD_IMG2WEBP=0 \
		-DWEBP_BUILD_VWEBP=0 \
		-DWEBP_BUILD_WEBPINFO=0 \
		-DWEBP_BUILD_WEBPMUX=0 \
		-DWEBP_BUILD_EXTRAS=0 \
		-B $(@D) \
		$(<D)

$(CODEC_DIR)/CMakeLists.txt:
	mkdir -p $(CODEC_DIR)
	curl -sL $(CODEC_URL) | tar xz --strip 1 -C $(CODEC_DIR)

clean:
	$(RM) $(OUT_JS) $(OUT_WASM)
	$(MAKE) -C $(CODEC_BASELINE_BUILD_DIR) clean
	$(MAKE) -C $(CODEC_SIMD_BUILD_DIR) clean
