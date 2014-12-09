#include "sim.h"

Sim::Sim()
	: particles()
	, movers() {

}

bool Sim::update(size_t ticks) {
	for (auto& m : movers) {
		m.second.update(ticks);
	}
	return true;
}
