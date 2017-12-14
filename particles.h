/**
 * Declaration for the Particles struct, which holds the particles.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include "vec2.h"
#include "particle.h"

struct Particles{

   std::vector<Particle*> particles;
   Particle* hapti_particle;
   int np;

   Particles(float width, float height, float start_x, float start_y, float end_x, float end_y);

   void add_particle(const Vec2f &px, const Vec2f &pu, const bool hapti_particle);
   void write_to_file(const char *filename_format, ...);
   void update_sph(float dt);

   private:
   template<class T> void accumulate(T &accum, float q, int i, int j, float fx, float fy);
   template<class T> void affineFix(T &accum, Vec2f c, int i, int j, float fx, float fy);
};

#endif
