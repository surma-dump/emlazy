.PHONEY: all clean

all: app.js
app.js: app.cpp

%.js:
	$(CXX) \
		-I $(CODEC_DIR) \
		${CXXFLAGS} \
		${LDFLAGS} \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s ENVIRONMENT=worker \
		-s MAIN_MODULE=1 \
		-s TEXTDECODER=2 \
		-s MODULARIZE=1 \
		-o $@ \
		$+

clean:
	$(RM) app.js app.wasm