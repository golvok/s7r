#ifndef MOVER_H
#define MOVER_H

#include "particle.h"

#include <vector>

class Sim;

class Mover {
public:
	Mover()
		: attached_sim(nullptr) {}

	Mover(Mover&& src)
		: attached_sim(src.attached_sim) {}

	virtual void update(size_t ticks) {(void)ticks;}
	virtual ~Mover() {}
protected:
	friend class Sim;

	Sim* attached_sim;

	void setSim(Sim* s) {attached_sim = s;}

	Mover& operator=(const Mover&);
	Mover(const Mover&);
};

template<class T>
class Mover_Impl : public Mover {
public:
	typedef std::vector<T> TList;
	Mover_Impl()
		: targets() {}

	Mover_Impl(Mover_Impl&& src)
		: targets(std::move(src.targets)) {}

	const TList& getTargets() {return targets;}
	void addTarget(T&& p) {
		targets.push_back(std::forward<T>(p));
	}
protected:
	TList targets;
};

#endif /* MOVER_H */
