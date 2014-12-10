#ifndef MOVER_H
#define MOVER_H

#include "particle.h"

#include <vector>

class Sim;

class Mover {
public:
	typedef std::vector<Particle> ParticleRefList;
	Mover(float speed_)
		: targets()
		, speed(speed_)
		, attached_sim(nullptr) {}

	Mover(Mover&& src)
		: targets(std::move(src.targets))
		, speed(src.speed)
		, attached_sim(src.attached_sim) {}

	void update(size_t ticks);
	const ParticleRefList& getTargets() {return targets;}
	void addTarget(Particle&& p);
private:
	friend class Sim;

	ParticleRefList targets;
	// std::queue<Points> destinations;
	float speed;
	Sim* attached_sim;

	void setSim(Sim* s) {attached_sim = s;}

	Mover& operator=(const Mover&);
	Mover(const Mover&);
};

#endif /* MOVER_H */
