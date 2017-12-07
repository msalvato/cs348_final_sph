#include <math>

double poly6Kernel(Particle p1, Particle p2, h) {
	double r_squared = distanceSquared(p1, p2);
	if r_squared < h {
	}

}

double distanceSquared(Particle p1, Particle p2) {
	return pow((p1.x.x - p2.x.x),2) + pow((p1.x.y - p2.x.y), 2);
}