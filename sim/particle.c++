#include "particle.h++"

void Particle::setPosition(const Point& p) {
	this->position = p;
}

Point Particle::getPosition() const {
	return position;
}
