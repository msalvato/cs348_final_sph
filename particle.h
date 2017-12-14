/**
 * Declaration for the Particles struct, which holds the particles.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "vec2.h"

struct Particle{

   //Grid &grid;
   Vec2f x, u; // positions and velocities
   float density;
   Vec2f force;
   std::vector<Particle*> neighbors;
   bool hapti_particle;
   Particle(Vec2f x, Vec2f u, bool hapti_particle) {
      this->x = x;
      this->u = u;
      this->density = 0;
      this->hapti_particle = hapti_particle;
   };
};

#endif
