#ifndef SIM_H
#define SIM_H

#include <sim/particle.h++>
#include <sim/mover.h++>

#include <unordered_map>

namespace std {
	template <> struct hash<Particle> {
		size_t operator()(const Particle& p) const {
			return std::hash<size_t>()(p.getId());
		}
	};
}

class Sim {
	typedef std::vector<Particle*> ParticleList;
public:
	Sim();
	bool update(size_t ticks);
	void add(MoverInterface& m);
private:
	Sim& operator=(const Sim&);
	Sim(const Sim&);
	ParticleList particles;
	std::vector<MoverInterface*> movers;
};

#endif /* SIM_H */
