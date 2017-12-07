/**
 * Declaration for the Particles struct, which holds the particles.
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include "grid.h"
#include "vec2.h"
#include "particle.h"

struct Particles{

   //Grid &grid;
   std::vector<Particle*> particles;
   int np;

   //Particles(Grid &grid_, SimulationType simType_)
   //   :grid(grid_), np(0),
   //    sum(grid_.pressure.nx+1, grid_.pressure.ny+1), simType( simType_ )
   //{}

   Particles(float width, float height, int resolution, int x_particles, int y_particles);

   void add_particle(const Vec2f &px, const Vec2f &pu);
   void write_to_file(const char *filename_format, ...);
   void update_sph(float dt);

   private:
   template<class T> void accumulate(T &accum, float q, int i, int j, float fx, float fy);
   template<class T> void affineFix(T &accum, Vec2f c, int i, int j, float fx, float fy);
   Vec2f computeC(Array2f &ufield, int i, int j, float fx, float fy);
};

#endif
