#include "invader.h"

#include <stdio.h> //printf for debugging with SDL

#include "miniGL/miniGL.h"

static void invader_draw(struct invader_s *inv);
static void invader_move(struct invader_s *inv, point pt);

void invader_init(struct invader_s *inv) {
  inv->position = (point){0,0};
  inv->draw = &invader_draw;
  inv->move = &invader_move;
}

void invader_move(struct invader_s *inv, point pt) {
  inv->position = pt;
}


void invader_draw(struct invader_s *inv) {
  int color = BLACK;
  int x = 0;
  int y = 0;
  int ship_width = 20;
  int ship_height = 30;
  int depth = inv->position.y * -10;

  for (int i = 0; i < 2; i++) {

    switch (inv->position.x){
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
        color = WHITE;
      } else {
        x = -20;
        y = -48;
        color = BLACK;
      }
      break;
    case 3:
      if (i==0) {
        x = 0;
        y = -50;
        color = WHITE;
      } else {
        x = 2;
        y = -52;
        color = BLACK;
      }
      break;
    case 4:
      ship_width = 16;
      ship_height = 16;
      if (i==0) {
        x = 22;
        y = -46;
        color = WHITE;
      } else {
        x = 24;
        y = -48;
        color = BLACK;
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
      (x - ship_width), 
      (y + ship_height / 2), 
      (depth + 5));
    glVertex3i(
      (x + ship_width), 
      (y + ship_height / 2), 
      (depth + 5));
    glVertex3i(
      (x), 
      (y - ship_height / 2), 
      (depth - 5));
    glEnd();

  }

}

