#include "mover.h"

void Mover::update(size_t ticks) {
	Point current_loc = target.getPosition();
	target.setPosition(Point(current_loc.x, current_loc.y + ticks*speed));
}
