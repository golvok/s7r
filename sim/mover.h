#ifndef MOVER_H
#define MOVER_H

#include "particle.h"

class Mover {
public:
	Mover(Particle& target_, float speed_)
		: target(target_)
		, speed(speed_) {}

	void update(size_t ticks);
	Particle& getTarget() {return target;}
	const Particle& getTarget() const {return target;}
private:
	Particle& target;
	// std::queue<Points> destinations;
	float speed;

	Mover& operator=(const Mover&);
	Mover(const Mover&);
};

#endif /* MOVER_H */
