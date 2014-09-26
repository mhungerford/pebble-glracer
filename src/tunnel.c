#include "tunnel.h"

void gl_drawtunnel(int speed){
  struct sll_point {
    sll x;
    sll y;
  };

  //lets plan for a 16 segment tube for starters
  static struct sll_point tube_center_history[8] = { 
    {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}, {0,40}};
  static const sll tube_width = int2sll(78);

  static int step = 0;
  static int flip = 0;

  for (int deep = 0; deep < 8 - 1; deep++){
    struct sll_point p1 = tube_center_history[deep];
    struct sll_point p2 = tube_center_history[deep+1];

    for (int seg = 0; seg < 16; seg++){
      sll seg_color = dbl2sll(((seg+deep+flip + (seg == 11))%2)?0.25:0.75);
      glColor3f(seg_color, seg_color, seg_color);

      sll rad1 = sllmul(int2sll(seg), slldiv2(CONST_PI_4)); //radians for corner
      struct sll_point corner1 = {
        (slladd(p1.x, sllmul(tube_width, sllcos(rad1)))),
        (slladd(p1.y, sllmul(tube_width, 1.6 * sllsin(rad1))))};

      sll rad2 = sllmul(int2sll(seg + 1), slldiv2(CONST_PI_4)); //radians for corner
      struct sll_point corner2 = {
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

