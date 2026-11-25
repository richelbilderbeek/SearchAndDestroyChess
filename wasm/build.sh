#!/bin/bash
#
# Build the WebAssembly version of SearchAndDestroyChess
#
# Usage, from the repository root, with Emscripten's em++ in the PATH:
#
#   ./wasm/build.sh [output_folder]
#
# The default output folder is 'build_wasm'.
# Serve the output folder with a web server, for example
# 'python3 -m http.server --directory build_wasm', then open index.html
set -e

out_dir=${1:-build_wasm}
mkdir -p "${out_dir}"

em++ -std=c++17 -O2 \
  -DNDEBUG \
  -I. \
  chess_board.cpp \
  chess_move.cpp \
  chess_piece.cpp \
  coordinat_getter.cpp \
  game.cpp \
  piece_color.cpp \
  piece_type.cpp \
  wasm/main_wasm.cpp \
  -sALLOW_MEMORY_GROWTH=1 \
  --shell-file wasm/shell.html \
  -o "${out_dir}/index.html"

# The sprites are loaded by the browser
mkdir -p "${out_dir}/Sprites"
cp Sprites/*.png "${out_dir}/Sprites/"

echo "WebAssembly version built in '${out_dir}'"
