#include "sim.h"

Sim::Sim()
	: particles()
	, movers() {

}

void Sim::add(Mover& m) {
	m.setSim(this);
	movers.push_back(&m);
}

bool Sim::update(size_t ticks) {
	std::cout << "sim::update(" << ticks << ")\n";
	for (auto& m : movers) {
		m->update(ticks);
	}
	return true;
}
