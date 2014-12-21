#include "sim.h"
#include <iostream>

Sim::Sim()
	: particles()
	, movers() {

}

void Sim::add(MoverInterface& m) {
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
