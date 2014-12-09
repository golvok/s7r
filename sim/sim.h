#ifndef SIM_H
#define SIM_H

#include "particle.h"
#include "mover.h"

#include <stdlib.h>
#include <unordered_map>

namespace std {
	template <> struct hash<Particle> {
		size_t operator()(const Particle& p) const {
			return std::hash<size_t>()(p.getId());
		}
	};
	template <> struct hash<Mover> {
		size_t operator()(const Mover& m) const {
			return std::hash<Particle>()(m.getTarget());
		}
	};
}

class Sim {
public:
	Sim();
	bool update(size_t ticks);
	void addParticle(Point p);
private:
	Sim& operator=(const Sim&);
	Sim(const Sim&);
	std::unordered_map<Particle::idtype,Particle> particles;
	std::unordered_map<Particle::idtype,Mover> movers;
};

#endif /* SIM_H */
