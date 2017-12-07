/**
 * Edit this file to set up the initial conditions for the fluid simulation 
 * (what the water looks like, etc). 
 *
 * @author Ante Qu, 2017
 * Based on Bridson's simple_flip2d starter code at http://www.cs.ubc.ca/~rbridson/
 */

#ifndef SHARED_MAIN_H
#define SHARED_MAIN_H


#define SIMULATION_TYPE (APIC) // default simtype: APIC, FLIP, or PIC
#define INIT_DROP_RADIUS (0.05)
#define INIT_FLOOR_SIZE (0.05)
#define USE_SPHERICAL_GRAV (false)
// the following only matter when USE_SPHERICAL_GRAV is true
#define INIT_VEL_MAGNITUDE (0.55)
#define GRAV_CENTER_X (0.5)
#define GRAV_CENTER_Y (0.5)
#define GRAV_FACTOR (0.01)

using namespace std;

void advance_one_step(Particles &particles, double dt)
{
   particles.update_sph(dt);
}

void advance_one_frame(Particles &particles, double frametime)
{
   double t=0;
   double dt;
   bool finished=false;
   while(!finished){
      //dt=2*grid.CFL();
      dt = .0001;
      if(t+dt>=frametime){
         dt=frametime-t;
         finished=true;
      }else if(t+1.5*dt>=frametime)
         dt=0.5*(frametime-t);
      //printf("advancing %g (to %f%% of frame)\n", dt, 100.0*(t+dt)/frametime);
      advance_one_step(particles, dt);
      t+=dt;
   }
}


#endif
