#include "sim/sim.h"
#include "draw/window.h"

#include <iostream>
#include <memory>

struct TestData {
	Sim* sim;
	Mover* m1;
	Mover* m2;
};

void* clicked(void* data) {
	(void)data;
	// puts("clicked!");
	static_cast<TestData*>(data)->sim->update(10);
	puts("updated");
	return 0;
}

void* draw_stuff(cairo_t* cr, void* data) {
	TestData* td = static_cast<TestData*>(data);
	cairo_set_source_rgb(cr, 0.00, 1.00, 0.00);
	cairo_set_line_width(cr,1);
	for (auto& p : td->m1->getTargets()) {
		cairo_arc(cr, p.getPosition().x, p.getPosition().y, 2, 0, 2*G_PI);
		cairo_stroke(cr);
	}
	for (auto& p : td->m2->getTargets()) {
		cairo_arc(cr, p.getPosition().x, p.getPosition().y, 2, 0, 2*G_PI);
		cairo_stroke(cr);
	}
	puts("drew");
	return 0;
}

int main () {
	TestData td;
	td.sim = new Sim();
	td.m1 = new Mover(5);
	td.m2 = new Mover(10);

	td.sim->add(*td.m1);
	td.sim->add(*td.m2);

	td.m1->addTarget(Particle({350,300},0));
	td.m2->addTarget(Particle({400,300},1));
	td.m1->addTarget(Particle({450,300},2));

	Window win(800, 600, draw_stuff, &td);
	win.set_click_function(clicked, &td);
	win.loop();
	return 0;
}
