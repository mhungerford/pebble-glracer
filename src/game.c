#include <stdint.h>
#include <stdbool.h>

#include "common.h"

#include "game.h"

#include "miniGL/miniGL.h"
#define GL_COLOR_BUFFER_BIT GL_COLOR

#define MAX_LANE 6
#define MAX_INVADERS 10

#define RADIUS       60
#define STEP_LONGITUDE   15         /* 22.5 makes 8 bands like original Boing */
#define STEP_LATITUDE  15

#define GRID_SIZE   RADIUS * 5

#define DIST_BALL   RADIUS * 2
#define VIEW_SCENE_DIST DIST_BALL * 3 + 200

typedef struct {
  sll x;
  sll y;
  sll z;
} vertex_t;

//struct invader_s invaders[MAX_INVADERS];
static int acceleration;
static int velocity;
static int position;

void move_left(void) {

}

void move_right(void) {

}

sll sin_deg(sll in) {
  return sllsin(sllmul(in, dbl2sll(PI/180)));
}

sll cos_deg(sll in) {
  return sllcos(sllmul(in, dbl2sll(PI/180)));
}

void gl_init() {
  glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  //for GLRacer, want to do perspective projection
  gluPerspective(int2sll(60), dbl2sll(0.95), int2sll(10), int2sll(1024));

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslatef(int2sll(0),int2sll(-45),int2sll(-100));
  //glRotatef(20, 1.0, 0.0, 0.0);

  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_CULL_FACE);

  acceleration = -1;
  velocity = 0;
  position = 78;
}

void
DrawGrid( void )
{
  int       row, col;
  const int   rowTotal  = 12;         /* must be divisible by 2 */
  const int   colTotal  = rowTotal;       /* must be same as rowTotal */
  //const sll widthLine = int2sll(0);          /* should be divisible by 2 */
  const sll sizeCell  = dbl2sll(GRID_SIZE / rowTotal);
  const sll z_offset  = int2sll(-10);
  sll     xl, xr;
  sll     yt, yb;

  glColor3f(int2sll(255), int2sll(255), int2sll(255));

  //glPushMatrix();
  //glDisable( GL_CULL_FACE );

  // Another relative Z translation to separate objects.
  //glTranslatef( 0.0, 0.0, DIST_BALL );
  glPolygonMode(GL_FRONT, GL_LINE);
  //glTranslatef(0, -velocity, 0);
  // Draw vertical lines (as skinny 3D rectangles).
  for ( col = 0; col <= colTotal; col++ )
  {
    // Compute co-ords of line.
    xl = slladd(dbl2sll(-GRID_SIZE / 2), sllmul(int2sll(col), sizeCell));
    xr = xl;

    yt =  dbl2sll(GRID_SIZE / 2);
    yb = dbl2sll(-GRID_SIZE / 2);

    glBegin( GL_POLYGON );
      // purple
    //if(velocity > 0) {
    glVertex3f( xr, yt, z_offset );   // NE
    glVertex3f( xl, yt, z_offset );   // NW
    glVertex3f( xl, yb, z_offset );   // SW
    glVertex3f( xr, yb, z_offset );   // SE
  //}

    glEnd();
  }

  // Draw horizontal lines (as skinny 3D rectangles).
  for ( row = 0; row <= rowTotal; row++ )
  {
    // Compute co-ords of line.
    yt = sllsub(dbl2sll(GRID_SIZE / 2), sllmul(int2sll(row), sizeCell));
    yb = yt;

    xl = dbl2sll(-GRID_SIZE / 2);
    xr = dbl2sll(GRID_SIZE / 2);
//glTranslatef(0.0, -velocity, 0.0);
    glBegin( GL_POLYGON );

    //glColor3f( RGB_PURPLE );      // purple
    //if(velocity > 0) {
    glVertex3f( xr, yt, z_offset );   // NE
    glVertex3f( xl, yt, z_offset );   // NW
    glVertex3f( xl, yb, z_offset );   // SW
    glVertex3f( xr, yb, z_offset );   // SE
    //printf("(%d, %d)\n", yb, yt);
  //} else {
    //glVertex3f( xr, yt - position , z_offset );   // NE
    //glVertex3f( xl, yt - position, z_offset );   // NW
    //glVertex3f( xl, yb - position, z_offset );   // SW
    //glVertex3f( xr, yb - position, z_offset );   // SE
    //printf("(%d, %d)\n", yb, yt);
  //}
    glEnd();
    //glTranslatef(0.0, velocity, 0.0);
  }
  //printf("velocity: %d position: %d\n", velocity, position);
  }

void
DrawBoingBallBand( sll long_lo,
                   sll long_hi )
{
  vertex_t    vert_ne;    // "ne" means south-east and so on
  vertex_t    vert_nw;
  vertex_t    vert_sw;
  vertex_t    vert_se;
  //vertexNormal  vert_norm;
  int     lat_deg;
  static int    colorToggle = 0;

  // Iterate thru the points of a latitude circle.
  // A latitude circle is a 2D set of X,Z points.
  for ( lat_deg = 0;
          lat_deg <= (180 - STEP_LATITUDE);
          lat_deg += STEP_LATITUDE )
  {
    // Color this polygon with red or white.
    if ( colorToggle )
      glColor3f( int2sll(GRAY_25), int2sll(GRAY_25), int2sll(GRAY_25) );
    else
      glColor3f( int2sll(GRAY_75), int2sll(GRAY_75), int2sll(GRAY_75) );

    colorToggle = ! colorToggle;

    // Change color if drawing shadow.
    /*if ( drawBallHow == DRAW_BALL_SHADOW )
      glColor3f( GRAY_75, GRAY_75, GRAY_75 );*/

    // Assign each Y.
    vert_ne.y = slladd(sllmul(cos_deg(long_hi), int2sll(RADIUS)), int2sll(position));
    vert_nw.y = slladd(sllmul(cos_deg(long_hi), int2sll(RADIUS)), int2sll(position));
    vert_sw.y = slladd(sllmul(cos_deg(long_lo), int2sll(RADIUS)), int2sll(position));
    vert_se.y = slladd(sllmul(cos_deg(long_lo), int2sll(RADIUS)), int2sll(position));

    // Assign each X,Z with sin,cos values scaled by latitude radius indexed by longitude.
    // Eg, long=0 and long=180 are at the poles, so zero scale is sin(longitude),
    // while long=90 (sin(90)=1) is at equator.
    vert_ne.x = sllmul(cos_deg(int2sll(lat_deg)), sllmul(int2sll(RADIUS), sin_deg( slladd(long_lo, int2sll(STEP_LONGITUDE)))));
    vert_se.x = sllmul(cos_deg( int2sll(lat_deg) ), sllmul(int2sll(RADIUS), sin_deg( long_lo )));
    vert_nw.x = sllmul(cos_deg( slladd(int2sll(lat_deg), int2sll(STEP_LATITUDE)) ), sllmul(int2sll(RADIUS), sin_deg( slladd(long_lo, int2sll(STEP_LONGITUDE)) )));
    vert_sw.x = sllmul(cos_deg( slladd(int2sll(lat_deg), int2sll(STEP_LATITUDE)) ), sllmul(int2sll(RADIUS), sin_deg( long_lo )));

    vert_ne.z = sllmul(sin_deg( int2sll(lat_deg) ), sllmul(int2sll(RADIUS), sin_deg( slladd(long_lo, int2sll(STEP_LONGITUDE)) )));
    vert_se.z = sllmul(sin_deg( int2sll(lat_deg) ), sllmul(int2sll(RADIUS), sin_deg( long_lo )));
    vert_nw.z = sllmul(sin_deg( slladd(int2sll(lat_deg), int2sll(STEP_LATITUDE) )), sllmul(int2sll(RADIUS), sin_deg( slladd(long_lo, int2sll(STEP_LONGITUDE) ))));
    vert_sw.z = sllmul(sin_deg( slladd(int2sll(lat_deg), int2sll(STEP_LATITUDE))), sllmul(int2sll(RADIUS), sin_deg( long_lo )));

    // Draw the facet.
    glBegin( GL_POLYGON );

    //CrossProduct( &vert_norm, &vert_ne, &vert_nw, &vert_sw );
    //glNormal3f( vert_norm.x, vert_norm.y, vert_norm.z );

    glVertex3f( vert_ne.x, vert_ne.y, vert_ne.z );
    glVertex3f( vert_nw.x, vert_nw.y, vert_nw.z );
    glVertex3f( vert_sw.x, vert_sw.y, vert_sw.z );
    glVertex3f( vert_se.x, vert_se.y, vert_se.z );

    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawing at things!");

    //printf("(%f, %f)\n", vert_ne.x, vert_ne.y);

    glEnd();
   }

  // Toggle color so that next band will opposite red/white colors than this one.
   colorToggle = ! colorToggle;

  // This circular band is done.
   //return;
}



void gl_drawframe(void) {
  if(position <= 0 && velocity <= 0) {
    velocity = -velocity;
  } else {
    position += velocity;
    velocity += acceleration;
  }
  if(velocity < 0) {
    position--;
  }

  sll bg_color = dbl2sll(0); // change this to 0.5 for a gray
glClearColor(bg_color, bg_color, bg_color, dbl2sll(0));
glClear(GL_COLOR_BUFFER_BIT);
//static int rotation = -20;
//glPushMatrix();

/*if (velocity < 0) {
  glTranslatef( 0.0, velocity, 0.0 );
  DrawGrid();
  glTranslatef( 0.0, -velocity, 0.0 );
} else {
  DrawGrid();
}*/

//glPolygonMode(GL_FRONT, GL_FILL);
//if(velocity > 0)
//glTranslatef( 0.0, velocity, 0.0 );
//glRotatef(-10.0, 1.0, 0.0, 0.0);
//glRotatef( -10.0, 0.0, 0.0, 1.0 );
//rotation -= 20;
DrawGrid();
glPolygonMode(GL_FRONT, GL_FILL);
  int lon_deg;

  for ( lon_deg = 0;
          lon_deg < 180;
          lon_deg += STEP_LONGITUDE )
  {
    // Draw a latitude circle at this longitude.
    DrawBoingBallBand( int2sll(lon_deg), int2sll(lon_deg + STEP_LONGITUDE) );
  }
  //glTranslatef( 0.0, -velocity, 0.0 );
  //DrawGrid();
  //glPopMatrix();
  
}
