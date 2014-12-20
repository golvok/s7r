#ifndef MOVER_H
#define MOVER_H

#include "particle.h"

#include <vector>
#include <algorithm>

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
	typedef std::vector<T*> TPtrList;
	Mover_Impl()
		: targets() {}

	Mover_Impl(Mover_Impl&& src)
		: targets(std::move(src.targets)) {}

	const TPtrList& getTargets() {return targets;}
	void addTarget(T&& p) {
		targets.push_back(new T(std::forward<T>(p)));
	}
	void removeTarget(T& p_to_remove) {
		targets.erase(
			std::find(targets.begin(), targets.end(), &p_to_remove)
		);
	}
protected:
	TPtrList targets;
};

#endif /* MOVER_H */
