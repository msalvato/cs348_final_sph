/**
 * Implementation of the Particles functions. Most of your edits should be in here.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "particles.h"
#include "particle.h"
#include "util.h"
#include "kernels.cpp"

using namespace std;

Particles::Particles(float width, float height, int resolution, int x_particles, int y_particles) {
   float start_x = 1.14;
   float start_y = .1;
   float space_x = .0204;
   float space_y = .016;

   for (int i = 0; i < y_particles; i ++ ){
      for (int j = 0; j < x_particles; j++){
         add_particle(Vec2f(start_x + space_x*j, start_y + space_y*i), Vec2f(0,0));
      }
   }
}

void Particles::
add_particle(const Vec2f &px, const Vec2f &pu)
{
   particles.push_back(new Particle(px,pu,0));
   np++;
}


void Particles::
update_sph(float dt) {
   for (Particle* p : particles){
      findNeighbors(p, particles);
      particleDensity(p);
   }
   for (Particle* p : particles){
      p->force = particlePressureForce(p);
      p->force += particleViscosityForce(p);
      p->force += particleGravityForce(p);
      // mass
   }
   for (Particle *p : particles) {
      p->u += p->force*dt/p->density;
      p->x += p->u*dt;
      if (p->x[0] >= 3) {
            p->u[0] = -.4*p->u[0];
            p->x[0] = 3;
      }
      if (p->x[1] >= 1.5) {
            p->u[1] = -.4*p->u[1];
            p->x[1] = 1.5;
      }
      if (p->x[0] <= 0) {
            p->u[0] = -.4*p->u[0];
            p->x[0] = 0;
      }
      if (p->x[1] <= 0) {
            p->u[1] = -.4*p->u[1];
            p->x[1] = 0;
      }
   }
}

void Particles::
write_to_file(const char *filename_format, ...)
{
   va_list ap;
   va_start(ap, filename_format);
   char *filename;
   vasprintf(&filename, filename_format, ap);
   FILE *fp=fopen(filename, "wt");
   free(filename);
   va_end(ap);

   fprintf(fp, "%d\n", np);
   for(int p=0; p<np; ++p)
      fprintf(fp, "%.5g %.5g\n", particles[p]->x[0], particles[p]->x[1]);
   fclose(fp);
}

