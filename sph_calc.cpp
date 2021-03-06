#include <cmath>
#include <vector>
#include "vec2.h"
#include "particle.h"

#define MATH_PI 3.1415926f
#define POLY6_CONSTANT 315.0f/(64.0f*MATH_PI)
#define VISCOSITY_CONSTANT 45.0f/(MATH_PI)
#define SPIKY_CONSTANT 45.0f/(MATH_PI)

static float radius = .03; //
static float rho = 1000; //
static float viscosity = 12000; //
static float gas_constant = 10000;  /// 
static float mass = .2; //
static float sigma = 80000;

float calcPressure(float density) {
	return std::max(gas_constant*(density-rho), 0.0f);
}

float poly6Kernel(Vec2f r1, Vec2f r2, float h) {
	float r_squared = mag2(r1-r2);
	if (r_squared > h*h) {
		return 0;
	}
	return (POLY6_CONSTANT/pow(h, 9))*pow((pow(h,2) - r_squared),3);
}

Vec2f poly6KernelGradient(Vec2f r1, Vec2f r2, float h) {
	if (r1[0] == r2[0] && r1[1] == r2[1]) {
		return Vec2f(0,0);
	}
	float r_squared = mag2(r1-r2);
	float r = mag(r1-r2);
	Vec2f r_normalized = (r1-r2)/r;
	if (r_squared > h*h) {
		return Vec2f(0,0);
	}
	return (-6*POLY6_CONSTANT/pow(h, 9))*pow((pow(h,2) - r_squared),2)*r_normalized;
}

float poly6KernelLaplacian(Vec2f r1, Vec2f r2, float h) {
	float r_squared = mag2(r1-r2);
	float r = mag(r1-r2);
	if (r_squared > h*h) {
		return 0;
	}
	return (-6*POLY6_CONSTANT/pow(h, 9))*(pow(h,4) - 6*pow(h,2)*r_squared + 5*r_squared*r_squared);
}


float viscosityKernelLaplacian(Vec2f r1, Vec2f r2, float h) {
	if (r1 == r2) {
		return 0;
	}
	float r = mag(r1-r2);
	if (r > h) {
		return 0;
	}
	return (VISCOSITY_CONSTANT/pow(h,6))*(h - r);
}

Vec2f spikyKernelGradient (Vec2f r1, Vec2f r2, float h){
	// direction needs ot be applied
	if (r1[0] == r2[0] && r1[1] == r2[1]) {
		return 0;
	}
	float r = mag(r1-r2);
	Vec2f r_normalized = (r1-r2)/r;
	if (r > h) {
		return Vec2f(0,0);
	}
	return -(SPIKY_CONSTANT/pow(h,6))*pow(h - r, 2)*r_normalized;
}

void particleDensity(Particle* p) {
	float density = 0;
	for (Particle* neighbor : p->neighbors) {
		//if (neighbor->hapti_particle) {
		//	density += 10*mass*poly6Kernel(p->x, neighbor->x, radius);
		//}
		//else{
			density += mass*poly6Kernel(p->x, neighbor->x, radius);
		//}
	}
	p->density = density;
}

Vec2f particlePressureForce(Particle* p) {//Vec2f r1, float rho_1, std::vector<Vec2f> positions, std::vector<float> densities) {
	// all neighbors are unit mass kthx
	Vec2f pressure_force = Vec2f(0,0);
	float pressure = calcPressure(p->density);
	for (Particle* neighbor : p->neighbors) {
		float pressure2 = calcPressure(neighbor->density);
		if (neighbor->hapti_particle) {
			pressure_force += 10*mass*((pressure + pressure2)/(2.0f*neighbor->density))*spikyKernelGradient(p->x, neighbor->x, radius);
		}
		else{
			pressure_force += mass*((pressure + pressure2)/(2.0f*neighbor->density))*spikyKernelGradient(p->x, neighbor->x, radius);
		}
	}
	//pressure_force *= -1.0/(p_1);
	pressure_force *= -1.0f;

	return pressure_force;
}

Vec2f particleViscosityForce(Particle* p) {//Vec2f r1, float rho_1, Vec2f v_1,  std::vector<Vec2f> positions, std::vector<float> densities, std::vector<Vec2f> velocities) {
	// all particles are unit mass kthx
	Vec2f viscosity_force = Vec2f(0,0);
	for (Particle* neighbor : p->neighbors) {
		if (neighbor->hapti_particle) {
			viscosity_force += 10*mass*((neighbor->u - p->u)/(neighbor->density))*viscosityKernelLaplacian(p->x, neighbor->x, radius);
		}
		else {
			viscosity_force += mass*((neighbor->u - p->u)/(neighbor->density))*viscosityKernelLaplacian(p->x, neighbor->x, radius);
		}	

	}
	viscosity_force *= viscosity;
	
	return viscosity_force;
}

Vec2f particleSurfaceTension(Particle* p) {//Vec2f r1, float rho_1, Vec2f v_1,  std::vector<Vec2f> positions, std::vector<float> densities, std::vector<Vec2f> velocities) {
	// all particles are unit mass kthx
	Vec2f tension_force = Vec2f(0,0);
	Vec2f n = Vec2f(0,0);
	float mag_n = 0;
	for (Particle* neighbor : p->neighbors) {
		Vec2f n_temp = (mass/(neighbor->density))*poly6KernelGradient(p->x, neighbor->x, radius);
		n += n_temp;
		mag_n += mag(n_temp);
	}
	float kappa = 0;
	if (mag_n > 100) {
		for (Particle* neighbor : p->neighbors) {
			kappa += (mass/(neighbor->density))*poly6KernelLaplacian(p->x, neighbor->x, radius);
		}
		kappa /= mag_n;
		kappa = -kappa;
	}
	else{
		return Vec2f(0,0);
	}
	tension_force = sigma*kappa*n;
	return tension_force;
}

Vec2f particleGravityForce(Particle* p) {
	return Vec2f(0, -12000.f*p->density);
}

void findNeighbors(Particle* p, std::vector<Particle*> particles){
	std::vector<Particle*> neighbors = *(new std::vector<Particle*>);
	for(Particle* p2 : particles) {
		if (mag2(p->x - p2->x) < radius*radius) {
			neighbors.push_back(p2);
		}
	}
	p->neighbors = neighbors;
}


