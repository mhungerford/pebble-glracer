#include <stdint.h>
#include <stdbool.h>

#include "common.h"

#include "racer.h"
#include "invader.h"
#include "tunnel.h"

#include "miniGL/miniGL.h"
#define GL_COLOR_BUFFER_BIT GL_COLOR

#define MAX_LANE 6
#define MAX_INVADERS 10

struct invader_s invaders[MAX_INVADERS];

void move_left(void) {
  if (racer.position.x > 0) {
    racer.position.x--;
  }
}

void move_right(void) {
  if (racer.position.x < MAX_LANE) {
    racer.position.x++;
  }
}

void gl_init() {
  glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  //for GLRacer, want to do perspective projection
  gluPerspective(int2sll(60), int2sll(1), int2sll(10), int2sll(1024));

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslatef(0,0,int2sll(-40));

  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_CULL_FACE);

  for (int i = 0; i < MAX_INVADERS; i++) {
    invader_init(&invaders[i]);
  }
}

void gl_drawframe(void) {
  sll bg_color = dbl2sll(0); // change this to 0.5 for a gray
  glClearColor(bg_color, bg_color, bg_color,int2sll(0));
  glClear(GL_COLOR_BUFFER_BIT);


  gl_drawtunnel(4);
  for (int i = 0; i < MAX_INVADERS; i++) {
    struct invader_s *inv = &invaders[i];
    //inv->move(inv, (point){i+3,i+1});
    //inv->draw(inv);
  }
  racer.draw(&racer);
}
