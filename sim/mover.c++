#include "mover.h"

void Mover::update(size_t ticks) {
	for(auto& target : targets) {
		Point current_loc = target.getPosition();
		target.setPosition(Point(current_loc.x, current_loc.y + ticks*speed));
	}
}

void Mover::addTarget(Particle&& p) {
	targets.push_back(std::forward<Particle>(p));
}