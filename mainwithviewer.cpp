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
Grid* pGrid;
Particles* pParticles;
std::string outputpath;
double frametime = .0001;
int stepCount;
bool start;


void set_view(float &bottom, float &left, float &height)
{
   bottom=0;
   left=0;
   float top=bottom, right=left;
   right = 1;
   top = 1.5;
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
      for(unsigned int i=0; i<pParticles->particles.size(); ++i) {
         glVertex2fv(pParticles->particles[i]->x.v);
      }
      for (int i = 0; i < 1000; i++) {
         glVertex2fv(Vec2f(3*i/1000.0, 0).v);
         glVertex2fv(Vec2f(3*i/1000.0, 1.5).v);
         glVertex2fv(Vec2f(0, 1.5*i/1000.0).v);
         glVertex2fv(Vec2f(3, 1.5*i/1000.0).v);
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
   if( !pGrid || !pParticles)
      return;
   switch(key){
      case ' ':
         start = !start;
         advance_one_frame(*pParticles, .0001);
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
   advance_one_frame(*pParticles, .0001);
   ++stepCount;
   printf("===================================================> step %d...\n", stepCount);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), stepCount);
   sprintf(frame_number, "frame %d", stepCount);
   glutPostRedisplay();
}

void timer_func(int value) {
   advance_one_frame(*pParticles, .0001);
   ++stepCount;
   printf("===================================================> step %d...\n", stepCount);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), stepCount);
   sprintf(frame_number, "frame %d", stepCount);
   glutPostRedisplay();
   glutTimerFunc(1,timer_func,1);
}

int main(int argc, char **argv)
{
   float gravity = 9.8;
   pGrid = new Grid(gravity, 10, 10, 1);
   
   outputpath=".";

   if(argc>1) outputpath=argv[1];
   else printf("using default output path...\n");
   printf("Output sent to %s\n", outputpath.c_str() );

   pParticles = new Particles(3, 1.5, 400, 35, 70);
   Gluvi::init("fluid simulation viewer woohoo", &argc, argv);
   pParticles->write_to_file("%s/frameparticles%04d", outputpath.c_str(), 0);
   stepCount = 0;
   start = false;
   //glutKeyboardFunc(key_handler);
   glutKeyboardFunc(key_handler);
   glutTimerFunc(1,timer_func,1);
   //glutDisplayFunc(display_data);
   float bottom, left, height;
   set_view(bottom, left, height);
   
   Gluvi::PanZoom2D cam(bottom, left, height);
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


