#include "sim/sim.h"
#include "draw/window.h"
#include "util/common.h"

#include <iostream>
#include <memory>

class SimpleMover : public Mover_Impl<Particle> {
	DECL_COPYCON_AND_ASSIGNOP(SimpleMover)
public:
	SimpleMover(float speed_)
		: Mover_Impl()
		, speed(speed_)
		, counter(0)
		{}

	void update(size_t ticks) override {
		counter += ticks;
		if (counter < reset_at_count) {
			for(auto& target : targets) {
				Point current_loc = target->getPosition();
				target->setPosition(Point(current_loc.x, current_loc.y + ticks*speed));
			}
		} else {
			counter = 0;
			for(auto& target : targets) {
				Point current_loc = target->getPosition();
				target->setPosition({current_loc.x, 300});
			}
		}
	}
private:
	float speed;
	unsigned int counter;
	const unsigned int reset_at_count = 40;
};

struct TestData {
	Sim* sim;
	SimpleMover* m1;
	SimpleMover* m2;
};

void* clicked(void* data) {
	(void)data;
	// puts("clicked!");
	static_cast<TestData*>(data)->sim->update(1);
	return 0;
}

class S8NDrawer : public DrawerObject {
	DECL_COPYCON_AND_ASSIGNOP(S8NDrawer)
private:
	TestData& td;
public:
	S8NDrawer(TestData& td_) : td(td_) {}

	void draw() override {
		cairo_set_source_rgb(getContext(), 0.00, 1.00, 0.00);
		cairo_set_line_width(getContext(),1);
		for (auto& p : td.m1->getTargets()) {
			cairo_arc(getContext(), p->getPosition().x, p->getPosition().y, 2, 0, 2*G_PI);
			cairo_stroke(getContext());
		}
		for (auto& p : td.m2->getTargets()) {
			cairo_arc(getContext(), p->getPosition().x, p->getPosition().y, 2, 0, 2*G_PI);
			cairo_stroke(getContext());
		}
	}
};

int main () {
	TestData td;
	td.sim = new Sim();
	td.m1 = new SimpleMover(1);
	td.m2 = new SimpleMover(2);

	td.sim->add(*td.m1);
	td.sim->add(*td.m2);

	td.m1->addTarget(Particle({350,300},0));
	td.m2->addTarget(Particle({400,300},1));
	td.m1->addTarget(Particle({450,300},2));

	Window win(800, 600, new S8NDrawer(td));
	win.set_click_function(clicked, &td);
	win.loop();
	return 0;
}
