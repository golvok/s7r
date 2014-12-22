#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdlib.h>
#include <util/geometry.h++>

class Particle {
public:
	Particle(const Point& p, const size_t& id_) : id(id_), position(p) {}
	void setPosition(const Point& p);
	Point getPosition() const;

	typedef size_t idtype;

	idtype getId() const {return id;}
	Particle(const Particle& src)
		: id(src.id)
		, position(src.position)
		{}
private:
	size_t id;
	Point position;
	Particle& operator=(const Particle&);
};

class AgingParticle : public Particle {
private:
	unsigned int lifetime;
	unsigned int age;
public:
	AgingParticle(Point start, unsigned int initial_lifetime, size_t id)
		: Particle(start, id)
		, lifetime(initial_lifetime)
		, age(0)
		{}

	AgingParticle(const AgingParticle& src)
		: Particle(src)
		, lifetime(src.lifetime)
		, age(src.age)
		{}

	void addToAge(unsigned int delta) { age += delta; }
	bool isDead() { return age > lifetime; }
	unsigned int getAge() { return age; }
};

#endif /* PARTICLE_H */
