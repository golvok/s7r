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
}

class Sim {
public:
	Sim();
	bool update(size_t ticks);
	void add(Mover& m);
private:
	Sim& operator=(const Sim&);
	Sim(const Sim&);
	std::vector<Particle*> particles;
	std::vector<Mover*> movers;
};

#endif /* SIM_H */
