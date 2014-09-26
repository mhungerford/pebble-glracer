#include "racer.h"

#include <stdio.h> //printf for debugging with SDL

#include "miniGL/miniGL.h"

static void racer_draw(struct racer_s *rs);

struct racer_s racer = {
  .position = {0,10},
  .draw = &racer_draw,
};

// Draw the racer as 2 offset shapes, to have a nice clear shadow effect
// makes seeing the main character easier too
void racer_draw(struct racer_s *rs) {
  int color = BLACK;
  int x = 0;
  int y = 0;
  int ship_width = 20;
  int ship_height = 20;
  int depth = 0;

  for (int i = 0; i < 2; i++) {

    switch (rs->position.x){
    case 0:
      break;
    case 1:
      break;
    case 2:
      ship_width = 16;
      ship_height = 16;
      if (i==0) {
        x = -22;
        y = -46;
        color = BLACK;
      } else {
        x = -20;
        y = -48;
        color = WHITE;
      }
      break;
    case 3:
      if (i==0) {
        x = 0;
        y = -50;
        color = BLACK;
      } else {
        x = 2;
        y = -52;
        color = WHITE;
      }
      break;
    case 4:
      ship_width = 16;
      ship_height = 16;
      if (i==0) {
        x = 22;
        y = -46;
        color = BLACK;
      } else {
        x = 24;
        y = -48;
        color = WHITE;
      }
      break;
    case 5:
      break;
    default:
      break;

    }

    glColor3i(color, color, color);
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex3i(
      (x + ship_width), 
      (y - ship_height), 
      (depth));
    glVertex3i(
      (x - ship_width), 
      (y - ship_height), 
      (depth));
    glVertex3i(
      (x), 
      (y), 
      (depth));
    glEnd();

  }
}


