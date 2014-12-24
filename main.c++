#include <sim/sim.h++>
#include <draw/window.h++>
#include <util/common.h++>

#include <iostream>
#include <memory>
#include <cassert>
#include <cstdlib>

class SimpleMover : public Mover<Particle> {
	DECL_COPYCON_AND_ASSIGNOP(SimpleMover)
public:
	SimpleMover(float speed_)
		: Mover()
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

enum class FireworkState {
	DONE,
	FUSE_LIT,
	EXPLODING
};

class Firework : public StatefulMover<AgingParticle,FireworkState> {
private:
	DECL_COPYCON_AND_ASSIGNOP(Firework)
	Point source;
	Point direction;
	const unsigned int num_particles;
	unsigned int ticks_since_lit;
public:
	Firework(Point src, unsigned int num_particles_, Point dir)
		: StatefulMover<ParticleType,MoverState>(MoverState::DONE)
		, source(src)
		, direction(dir)
		, num_particles(num_particles_)
		, ticks_since_lit(0)
		{
		lightFuse();
	}

	void lightFuse() {
		assert(isInState(MoverState::DONE) && "fuse lit when not done.");
		setState(MoverState::FUSE_LIT);
		ticks_since_lit = 0;
		addTarget(AgingParticle(source, 5, num_particles));
	}

	void update(size_t ticks) override {
		ticks_since_lit += ticks;
		if (isInState(MoverState::FUSE_LIT)) {
			if (ticks_since_lit > 5) {
				for (unsigned int i = 0; i < num_particles; ++i) {
					addTarget(AgingParticle(source, 10 + (-3 + (rand() % 7)), i));
				}
				setState(MoverState::EXPLODING);
			}
		}
		ParticlePtrList dead;
		for (auto& p : targets) {
			p->addToAge(ticks);
			if (p->isDead()) {
				dead.push_back(p);
			} else {
				if (isInState(MoverState::EXPLODING)) {
					p->setPosition(
						p->getPosition()
						+ direction*(ticks*(0.6 + rand()*(0.6)/RAND_MAX))
						+ Point::getPerpindular(direction)*(ticks*(-1 + (rand()*(2.0)/RAND_MAX)))
					);
				}
			}
		}
		for (auto& p : dead) {
			removeTarget(*p);
		}
		if (isInState(MoverState::EXPLODING) && targets.empty()) {
			setState(MoverState::DONE);
			lightFuse();
		}
	}
};

class Train : public Particle {
private:
	DECL_COPYCON_AND_ASSIGNOP(Train)
public:
	Train(std::vector<Point> path_, size_t id)
		: Particle(path_.at(0), id)
		, path(path_)
		, nextPointIter(path.begin() + 1)
		, lastPoint(path.at(0))
		, nextPoint(path.at(1))
		{}
	Train(Train&& src)
		: Particle(src)
		, path(std::move(src.path))
		, nextPointIter(src.nextPointIter)
		, lastPoint(src.lastPoint)
		, nextPoint(src.nextPoint)
		{}

	const std::vector<Point>& getPath() { return path; }
private:
	std::vector<Point> path;
	std::vector<Point>::iterator nextPointIter;
	Point lastPoint;
	Point nextPoint;

	void advancePosition() {
		lastPoint = nextPoint;
		++nextPointIter;
		if (nextPointIter == path.end()) {
			nextPointIter = path.begin();
		}
		nextPoint = *nextPointIter;
	}

	friend class TrainMaster;
};

class TrainMaster : public Mover<Train> {
private:
	DECL_COPYCON_AND_ASSIGNOP(TrainMaster)
public:
	TrainMaster(std::vector<std::vector<Point>>&& path_list) {
		size_t i = 0;
		for (auto& path : path_list) {
			addTarget(Train(path, i));
			++i;
		}
	}

	void update(size_t ticks) override {
		const float speed = 10;
		for (Train* p : targets) {
			float distanceFromLast = Point::distance(p->lastPoint,p->getPosition());
			float distanceBetweenCurrentPair = Point::distance(p->lastPoint,p->nextPoint);
			if (distanceBetweenCurrentPair < (distanceFromLast + ticks*speed)) {
				p->advancePosition();
				if (p->nextPointIter == p->path.begin()) {
					p->advancePosition();
				}
				Point deltaFromNewLast =
					Point::unit(Point::delta(p->lastPoint,p->nextPoint)) * (ticks*speed - (distanceBetweenCurrentPair - distanceFromLast));
				p->setPosition(p->lastPoint + deltaFromNewLast);
			} else {
				Point movementVector =
					Point::unit(Point::delta(p->lastPoint,p->nextPoint)) * (ticks*speed);
				p->setPosition(p->getPosition() + movementVector);
			}
		}
	}
private:
};

struct TestData {
	Sim* sim;
	SimpleMover* m1;
	SimpleMover* m2;
	Firework* f1;
	Firework* f2;
	TrainMaster* tm1;
};

void* clicked(void* data) {
	(void)data;
	// puts("clicked!");
	static_cast<TestData*>(data)->sim->update(1);
	return 0;
}

class S7RDrawer : public DrawerObject {
	DECL_COPYCON_AND_ASSIGNOP(S7RDrawer)
private:
	TestData& td;
public:
	S7RDrawer(TestData& td_) : td(td_) {}

	void draw() override {
		setDrawColour(0.00, 1.00, 0.00);
		setDrawLineWidth(1);
		for (auto& p : td.m1->getTargets()) {
			drawCircle(p->getPosition(), 2);
		}
		for (auto& p : td.m2->getTargets()) {
			drawCircle(p->getPosition(), 2);
		}
		drawFirework(*td.f1);
		drawFirework(*td.f2);
		drawTrains(*td.tm1);
	}

	void drawFirework(Firework& f) {
		setDrawLineWidth(1);
		switch (f.getState()) {
			case Firework::MoverState::FUSE_LIT:
				setDrawColour(1.00, 0.70, 0.00);
			break;
			case Firework::MoverState::EXPLODING:
				setDrawColour(1.00, 0.00, 0.00);
			break;
			default:
				setDrawColour(0.00, 0.00, 0.00);
			break;
		}
		for (auto& p : f.getTargets()) {
			drawCircle(p->getPosition(), 2);
		}
	}

	void drawTrains(TrainMaster& tm) {
		setDrawLineWidth(1);
		for (auto& p : tm.getTargets()) {
			setDrawColour(0.00, 0.00, 1.00);
			for(auto pathPoint = p->getPath().begin(); pathPoint != p->getPath().end(); ++pathPoint) {
				if (pathPoint == p->getPath().begin()) {
					// std::cout << *pathPoint << std::endl;
					cairo_move_to(getContext(), (*pathPoint).x, (*pathPoint).y);
				} else {
					// std::cout << "to " << *pathPoint << std::endl;
					cairo_line_to(getContext(), (*pathPoint).x, (*pathPoint).y);
				}
				// std::cout << "end\n\n";
			}
			stroke();
			setDrawColour(0.00, 0.50, 1.00);
			drawCircle(p->getPosition(), 4);
		}
	}
};

int main () {
	srand(time(0));

	TestData td;
	td.sim = new Sim();
	td.m1 = new SimpleMover(1);
	td.m2 = new SimpleMover(2);
	td.f1 = new Firework({100,250},20,{7,0});
	td.f2 = new Firework({100,350},30,{1,-1});
	td.tm1 = new TrainMaster({
		{ {10, 10} , {100,100}, {100,150}, {300,150} },
		{ {10, 110}, {100,200}, {100,250}, {300,250} },
		{
			{400,400}, {470.71,429.29}, {500,500}, {470.71,570.71},
			{400,600}, {329.29,570.71}, {300,500}, {329.29,429.29},
			{400,400}
		},
	});

	td.sim->add(*td.m1);
	td.sim->add(*td.m2);
	td.sim->add(*td.f1);
	td.sim->add(*td.f2);
	td.sim->add(*td.tm1);

	td.m1->addTarget(Particle({350,300},0));
	td.m2->addTarget(Particle({400,300},1));
	td.m1->addTarget(Particle({450,300},2));

	Window win(800, 800, std::unique_ptr<S7RDrawer>(new S7RDrawer(td)));
	win.set_click_function(clicked, &td);
	win.loop();

	delete td.sim;
	delete td.m1;
	delete td.m2;
	delete td.f1;
	delete td.f2;
	delete td.tm1;

	return 0;
}
