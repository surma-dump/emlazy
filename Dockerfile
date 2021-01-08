FROM emscripten/emsdk:2.0.11
RUN apt-get update && apt-get install -qqy autoconf libtool pkg-config
ENV CFLAGS "-O3 -flto"
ENV CXXFLAGS "${CFLAGS} -std=c++17"
ENV LDFLAGS "${CFLAGS} -s PTHREAD_POOL_SIZE=navigator.hardwareConcurrency --bind"
# Build and cache standard libraries with these flags
RUN emcc ${CXXFLAGS} ${LDFLAGS} -s MAIN_MODULE=1 -xc++ /dev/null -o /dev/null
WORKDIR /src
CMD ["sh", "-c", "emmake make -j`nproc`"]
