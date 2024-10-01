#!/bin/sh
# Support for explicitly targeting the legacy EdgeHTML browser
# (edge version prior to the chromium-based edge) via -sMIN_EDGE_VERSION
# was removed. Using -sLEGACY_VM_SUPPORT should still work if anyone
# still wanted to target this or any other legacy browser.
# See https://github.com/emscripten-core/emscripten/blob/main/ChangeLog.md#3151---121323
make -f Makefile.emscripten clean; make -f Makefile.emscripten && cd bin/src && $HOME/emsdk/upstream/emscripten/emcc -flto -O2 -fno-rtti -fno-exceptions */*.o */*/*.o main.o -o index.html -sASSERTIONS=0 -sUSE_SDL=2 -sUSE_SDL_MIXER=2 -sSDL2_MIXER_FORMATS='["mp3"]' -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sLEGACY_VM_SUPPORT=1 -sWASM=0 -sPOLYFILL_OLD_MATH_FUNCTIONS=1 -sUSE_SDL_TTF=2 -sASYNCIFY -sASYNCIFY_IGNORE_INDIRECT -sASYNCIFY_ONLY=@../../src/plat/web/funcs.txt -sENVIRONMENT=web --preload-file ../../data/@data/ --preload-file ../../fonts/@fonts/ --preload-file ../../gfx/@gfx/ --preload-file ../../music/@music/ --preload-file ../../sound/@sound/ -Wl,-u,fileno && cp ../../src/plat/web/index.html index.html && $HOME/emsdk/upstream/emscripten/emrun index.html