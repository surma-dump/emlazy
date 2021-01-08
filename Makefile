.PHONEY: all clean

all: app.js
app.js: app.cpp

%.js:
	$(CXX) \
		-I $(CODEC_DIR) \
		${CXXFLAGS} \
		${LDFLAGS} \
		--bind \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s ENVIRONMENT=worker \
		-s MAIN_MODULE=1 \
		-s TEXTDECODER=2 \
		-o $@ \
		$+

clean:
	$(RM) app.js app.wasm