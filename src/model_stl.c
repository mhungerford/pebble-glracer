#include <stdlib.h>

#include "model_stl.h"
#include "miniGL/miniGL.h"
#define GL_COLOR_BUFFER_BIT GL_COLOR

#include <stdio.h> //printf for debugging with SDL


//Evil hack for softfloat workarounds
//__extendsfdf2 pulls in all of ieee754-df.S (about 3164 bytes)
//vs. 140 for just extendsfdf2
extern double llvm_extendsfdf2(float a);

static int ship_pos = 3;

void gl_init() {
  glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  //Invert y here, as pebble origin is top-left, opengl is bottom-left
  sll xx = int2sll(FRAMEBUFFER_WIDTH / 2);
  sll yy = int2sll(FRAMEBUFFER_HEIGHT / 2);
  sll near = xx > yy ? xx : yy;
  near = -sllmul2(near);
  
  //for GLRacer, want to do perspective projection
  //glOrtho(-xx, xx, -yy, yy, near, int2sll(30));
  gluPerspective(int2sll(60), int2sll(1), int2sll(10), int2sll(1024));

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslatef(0,0,int2sll(-40));

#if 0
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
  //gluPerspective(40.0, 1.4, -100.0, 0.0);


  GLfloat amb[4] = {dbl2sll(1), dbl2sll(1), dbl2sll(1), int2sll(0)};
  GLfloat dif[4] = {int2sll(1),int2sll(1),int2sll(1),int2sll(0)};
  GLfloat lightpos[] = {int2sll(30), int2sll(64), int2sll(-34), int2sll(1)};
  glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

#else
  //gluPerspective(40, 1.0, 20.0, 100.0);
  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_CULL_FACE);
#endif
}

void gl_drawtunnel(int speed){
  struct point {
    sll x;
    sll y;
  };

  //lets plan for a 16 segment tube for starters
  static struct point tube_center_history[8] = { 
    {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}};
  static const sll tube_width = int2sll(78);

  static int step = 0;
  static int flip = 0;

  for (int deep = 0; deep < 8 - 1; deep++){
    struct point p1 = tube_center_history[deep];
    struct point p2 = tube_center_history[deep+1];

    for (int seg = 0; seg < 16; seg++){
      sll seg_color = dbl2sll(((seg+deep+flip + (seg == 11))%2)?0.25:0.75);
      glColor3f(seg_color, seg_color, seg_color);

      sll rad1 = sllmul(int2sll(seg), slldiv2(CONST_PI_4)); //radians for corner
      struct point corner1 = {
        (slladd(p1.x, sllmul(tube_width, sllcos(rad1)))),
        (slladd(p1.y, sllmul(tube_width, 1.6 * sllsin(rad1))))};

      sll rad2 = sllmul(int2sll(seg + 1), slldiv2(CONST_PI_4)); //radians for corner
      struct point corner2 = {
        (slladd(p1.x, sllmul(tube_width, sllcos(rad2)))),
        (slladd(p1.y, sllmul(tube_width, 1.6 * sllsin(rad2))))};
  

      for (int i=0; i < 2; i++){
        glColor3f(seg_color, seg_color, seg_color);
        if (seg > 8 && seg < 15){ 
          glPolygonMode(GL_FRONT, (i==0) ? GL_FILL : GL_LINE);
          if (i==1) glColor3f(0.0,0.0,0.0);
        } else {
          glPolygonMode(GL_FRONT,GL_LINE);
          i++;
        }
        glBegin(GL_POLYGON);
        if(deep == 0){
          glVertex3f(corner2.x, corner2.y, int2sll(-16 * deep));
          glVertex3f(corner1.x, corner1.y, int2sll(-16 * deep));
        }else{
          glVertex3f(corner2.x, corner2.y, int2sll(-16 * deep + step));
          glVertex3f(corner1.x, corner1.y, int2sll(-16 * deep + step));
        }
        glVertex3f(corner1.x, corner1.y, int2sll(-16 * (deep+1) + step));
        glVertex3f(corner2.x, corner2.y, int2sll(-16 * (deep+1) + step));
        glEnd();
      }
    }
  }

 step = (step + speed) % 16;
 if (step == 0) {
   flip = (flip + 1) % 2;
 }
}

void gl_drawship(int position){
  int color;
  int x = 0;
  int y = 0;
  int ship_width = 20;
  int ship_height = 20;
  int depth = 0;

  for (int i = 0; i < 2; i++) {

    switch (position){
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
        color = 0;
      } else {
        x = -20;
        y = -48;
        color = 1;
      }
      break;
    case 3:
      if (i==0) {
        x = 0;
        y = -50;
        color = 0;
      } else {
        x = 2;
        y = -52;
        color = 1;
      }
      break;
    case 4:
      ship_width = 16;
      ship_height = 16;
      if (i==0) {
        x = 22;
        y = -46;
        color = 0;
      } else {
        x = 24;
        y = -48;
        color = 1;
      }
      break;
    case 5:
      break;
    default:
      break;

    }

    glColor3f(dbl2sll(color), dbl2sll(color), dbl2sll(color));
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex3f(
      int2sll(x + ship_width), 
      int2sll(y - ship_height), 
      int2sll(depth));
    glVertex3f(
      int2sll(x - ship_width), 
      int2sll(y - ship_height), 
      int2sll(depth));
    glVertex3f(
      int2sll(x), 
      int2sll(y), 
      int2sll(depth));
    glEnd();

  }

  /*
     glBegin(GL_POLYGON);
     glVertex3f(  -70,  70, 0);
     glVertex3f(   70,  70, 0);
     glVertex3f(   70, -70, 0);
     glEnd();
     */
}

void move(int position){
  ship_pos = position;
}

void gl_drawframe(uint8_t* model, bool wireframe, uint8_t rotation, bool reset) {
  glPolygonMode(GL_FRONT, (wireframe) ? GL_LINE : GL_FILL);

  sll bg_color = dbl2sll(0); // change this to 0.5 for a gray
  glClearColor(bg_color, bg_color, bg_color,int2sll(0));
  glClear(GL_COLOR_BUFFER_BIT);

  gl_drawtunnel(2);
  gl_drawship(ship_pos);
}
