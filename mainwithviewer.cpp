/**
 * File for setting up the main simulator with viewer. Hit spacebar to advance forward one frame.
 *
 * @author Ante Qu, 
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include "particles.h"
#include "util.h"
#include "viewflip2d/gluvi.h"
#include "shared_main.h"

using namespace std;

char frame_number[100]="frame 0";
Particles* pParticles;
std::string outputpath;
double frametime = .0001;
int stepCount;
bool start;
bool mouse_down = false;
float window_width = 3;
float window_height = 1.5;
float resolution = 600;


void set_view(float &bottom, float &left, float &height)
{
   bottom=0;
   left=0;
   float top=bottom, right=left;
   right = 1;
   top = 1;
   if(right-left > top-bottom)
      height=1.5*(right-left);
   else
      height=1.5*(top-bottom);
   

   left=(left+right)/2-height/2;
   bottom=(top+bottom)/2-height/2;
}

void display(void)
{
   glDisable(GL_LIGHTING);
   glColor3f(1, 1, 1);
   glPointSize(3);
   glBegin(GL_POINTS);
   
   if( pParticles )
      for(unsigned int i=0; i<pParticles->particles.size()-1; ++i) {
         glVertex2fv(pParticles->particles[i]->x.v);
      }
      for (int i = 0; i < 1000; i++) {
         glVertex2fv(Vec2f(3*i/1000.0, 0).v);
         glVertex2fv(Vec2f(3*i/1000.0, 1.5).v);
         glVertex2fv(Vec2f(0, 1.5*i/1000.0).v);
         glVertex2fv(Vec2f(3, 1.5*i/1000.0).v);
      }
   glEnd();
   glPointSize(25);
   glBegin(GL_POINTS);
   if (pParticles) {
      glVertex2fv(pParticles->hapti_particle->x.v);
   }
   glEnd();

}

struct ScreenShotButton : public Gluvi::Button{
   
   ScreenShotButton(const char *label) : Gluvi::Button(label) {}
   void action()
   { ; }
};

void key_handler(unsigned char key, int x, int y)
{
   if(!pParticles)
      return;
   switch(key){
      case ' ':
         start = !start;
         advance_one_frame(*pParticles, frametime);
         ++stepCount;
         printf("===================================================> step %d...\n", stepCount);
         pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), stepCount);
         sprintf(frame_number, "frame %d", stepCount);
         glutPostRedisplay();
         break;

      default:
         ;
   }
}

void display_data() {
   advance_one_frame(*pParticles, frametime);
   ++stepCount;
   printf("===================================================> step %d...\n", stepCount);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), stepCount);
   sprintf(frame_number, "frame %d", stepCount);
   glutPostRedisplay();
}

void timer_func(int value) {
   advance_one_frame(*pParticles, frametime);
   ++stepCount;
   //printf("===================================================> step %d...\n", stepCount);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), stepCount);
   //sprintf(frame_number, "frame %d", stepCount);
   glutPostRedisplay();
   glutTimerFunc(1,timer_func,1);
}

void mouse_check(int button, int state, int x, int y){
   if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
      mouse_down = true;
      pParticles->hapti_particle->x[0] = x/resolution;
      pParticles->hapti_particle->x[1] = window_height-y/resolution;
   }
   if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
      mouse_down = false;
      pParticles->hapti_particle->x[0] = -1;
      pParticles->hapti_particle->x[1] = -1;
   }
}

void motion_check(int x, int y){
   pParticles->hapti_particle->u[0] = (1/frametime)*(x/resolution - pParticles->hapti_particle->x[0]);
   pParticles->hapti_particle->u[1] = (1/frametime)*(1.5-y/resolution - pParticles->hapti_particle->x[1]);
   pParticles->hapti_particle->x[0] = x/resolution;
   pParticles->hapti_particle->x[1] = window_height-y/resolution;
}

int main(int argc, char **argv)
{
   outputpath=".";

   if(argc>1) outputpath=argv[1];
   else printf("using default output path...\n");
   printf("Output sent to %s\n", outputpath.c_str() );

   pParticles = new Particles(window_width, window_height, 1, 0, 2, 1.4);
   Gluvi::init("fluid simulation viewer woohoo", &argc, argv);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), 0);
   stepCount = 0;
   start = false;
   //glutKeyboardFunc(key_handler);
   glutKeyboardFunc(key_handler);
   glutTimerFunc(1,timer_func,1);
   glutMouseFunc(mouse_check);
   glutMotionFunc(motion_check);
   //glutDisplayFunc(display_data);
   float bottom, left, height;
   set_view(bottom, left, height);
   std::cout << left << std::endl;

   Gluvi::PanZoom2D cam(0,0,1.5);
   Gluvi::camera=&cam;
   
   Gluvi::userDisplayFunc=display;

   Gluvi::StaticText frametext(frame_number);
   Gluvi::root.list.push_back(&frametext);
/*
   char ppmfileformat[strlen(file_format)+5];
   sprintf(ppmfileformat, "%s.ppm", file_format);
   ScreenShotButton screenshot("Screenshot", ppmfileformat);
   Gluvi::root.list.push_back(&screenshot);
*/
   Gluvi::run();
   return 0;
}


