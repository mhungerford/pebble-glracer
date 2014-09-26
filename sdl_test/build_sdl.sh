DEFINES="-DFRAMEBUFFER_WIDTH=144 -DFRAMEBUFFER_HEIGHT=144"
SRC=(
  main.c 
  ../src/game.c 
  ../src/tunnel.c 
  ../src/racer.c 
  ../src/invader.c 
  ../src/miniGL/draw2d.c 
  ../src/miniGL/miniGL.c 
  )

g++ $DEFINES -g -std=c99 -I. -I../src ${SRC[@]} -o sdl_main.bin -lm -lSDL
