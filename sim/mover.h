#ifndef MOVER_H
#define MOVER_H

#include "particle.h"

#include <vector>
#include <unordered_set>
#include <algorithm>

class Sim;

class MoverInterface {
public:
	MoverInterface()
		: attached_sim(nullptr) {}

	MoverInterface(MoverInterface&& src)
		: attached_sim(src.attached_sim) {}

	virtual void update(size_t ticks) {(void)ticks;}
	virtual ~MoverInterface() {}
protected:
	friend class Sim;

	Sim* attached_sim;

	void setSim(Sim* s) {attached_sim = s;}

	MoverInterface& operator=(const MoverInterface&);
	MoverInterface(const MoverInterface&);
};

template<class ParticleType>
class Mover : public MoverInterface {
public:
	typedef std::vector<ParticleType*> ParticlePtrList;
	typedef std::unordered_set<ParticleType*> ParticlePtrSet;
	Mover()
		: targets() {}

	Mover(Mover&& src)
		: targets(std::move(src.targets)) {}

	~Mover() {
		while (targets.empty() == false) {
			removeTarget(*targets.front());
		}
	}

	const ParticlePtrList& getTargets() {return targets;}
	void addTarget(ParticleType&& p) {
		targets.push_back(new ParticleType(std::forward<ParticleType>(p)));
	}
	void removeTarget(ParticleType& p_to_remove) {
		targets.erase(
			std::find(targets.begin(), targets.end(), &p_to_remove)
		);
		delete &p_to_remove;
	}
protected:
	ParticlePtrList targets;
};



#endif /* MOVER_H */
