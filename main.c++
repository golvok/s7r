#include "sim/sim.h"
#include "draw/window.h"
#include "util/common.h"

#include <iostream>
#include <memory>
#include <cassert>
#include <cstdlib>

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

class FireworkParticle : public Particle {
private:
	unsigned int lifetime;
	unsigned int age;
public:
	FireworkParticle(Point start, unsigned int initial_lifetime, size_t id)
		: Particle(start, id)
		, lifetime(initial_lifetime)
		, age(0)
		{}

	FireworkParticle(const FireworkParticle& src)
		: Particle(src)
		, lifetime(src.lifetime)
		, age(src.age)
		{}

	void addToAge(unsigned int delta) { age += delta; }
	bool isDead() { return age > lifetime; }
	unsigned int getAge() { return age; }
};

class Firework : public Mover_Impl<FireworkParticle> {
public:
	enum class State {
		DONE,
		FUSE_LIT,
		EXPLODING
	};
private:
	DECL_COPYCON_AND_ASSIGNOP(Firework)
	Point source;
	Point direction;
	const unsigned int num_particles;
	unsigned int ticks_since_lit;
	Firework::State state;
public:
	Firework(Point src, unsigned int num_particles_, Point dir)
		: source(src)
		, direction(dir)
		, num_particles(num_particles_)
		, ticks_since_lit(0)
		, state(State::DONE)
		{
		lightFuse();
	}

	void lightFuse() {
		assert(state == State::DONE && "fuse lit when not done.");
		state = State::FUSE_LIT;
		ticks_since_lit = 0;
		addTarget(FireworkParticle(source, 5, num_particles));
	}

	void update(size_t ticks) override {
		ticks_since_lit += ticks;
		if (state == State::FUSE_LIT) {
			if (ticks_since_lit > 5) {
				for (unsigned int i = 0; i < num_particles; ++i) {
					addTarget(FireworkParticle(source, 10 + (-3 + (rand() % 7)), i));
				}
				state = State::EXPLODING;
			}
		}
		TPtrList dead;
		for (auto& p : targets) {
			p->addToAge(ticks);
			if (p->isDead()) {
				dead.push_back(p);
			} else {
				if (state == State::EXPLODING) {
					p->setPosition(
						p->getPosition()
						+ direction*(ticks)
						+ Point::getPerpindular(direction)*((long int)ticks*(-1 + (rand() % 3)))
					);
				}
			}
		}
		for (auto& p : dead) {
			removeTarget(*p);
			delete p;
		}
		if (state == State::EXPLODING && targets.empty()) {
			state = State::DONE;
			lightFuse();
		}
	}

	State getState() { return state; }
};

struct TestData {
	Sim* sim;
	SimpleMover* m1;
	SimpleMover* m2;
	Firework* f1;
	Firework* f2;
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
		drawFirework(*td.f1);
		drawFirework(*td.f2);
	}

	void drawFirework(Firework& f) {
		switch (f.getState()) {
			case Firework::State::FUSE_LIT:
				cairo_set_source_rgb(getContext(), 1.00, 1.00, 0.00);
			break;
			case Firework::State::EXPLODING:
				cairo_set_source_rgb(getContext(), 1.00, 0.00, 0.00);
			break;
			default:
				cairo_set_source_rgb(getContext(), 0.00, 0.00, 0.00);
			break;
		}
		for (auto& p : f.getTargets()) {
			cairo_arc(getContext(), p->getPosition().x, p->getPosition().y, 2, 0, 2*G_PI);
			cairo_stroke(getContext());
		}
	}
};

int main () {
	srand(time(0));

	TestData td;
	td.sim = new Sim();
	td.m1 = new SimpleMover(1);
	td.m2 = new SimpleMover(2);
	td.f1 = new Firework({100,250},20,{3,0});
	td.f2 = new Firework({100,350},30,{1,-1});

	td.sim->add(*td.m1);
	td.sim->add(*td.m2);
	td.sim->add(*td.f1);
	td.sim->add(*td.f2);

	td.m1->addTarget(Particle({350,300},0));
	td.m2->addTarget(Particle({400,300},1));
	td.m1->addTarget(Particle({450,300},2));

	Window win(800, 600, new S8NDrawer(td));
	win.set_click_function(clicked, &td);
	win.loop();
	return 0;
}
