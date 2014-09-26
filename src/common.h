#pragma once

#include "miniGL/miniGL.h"
#define GL_COLOR_BUFFER_BIT GL_COLOR

enum {
  BLACK = 0,
  GRAY_25 = 63,
  GRAY_50 = 127,
  GRAY_75 = 191,
  WHITE = 255
};

//Racer game is a 2-d game presented in 3d, so characters move on a 2d grid
typedef struct {
  int x;
  int y;
} point;

