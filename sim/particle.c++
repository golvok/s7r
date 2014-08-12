#include "particle.h"

Point::Point(float x_, float y_) :
	x(x_), y(y_) { }

void Point::set(float x, float y) {
	this->x = x;
	this->y = y;
}

void Particle::setPosition(float x, float y) {
	this->position.set(x,y);
}
