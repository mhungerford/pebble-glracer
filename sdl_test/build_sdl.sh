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

gcc $DEFINES -framework Cocoa -g -std=c99 -I. -I../src ${SRC[@]} SDLMain.m -o sdl_main.bin -lm -lSDL
