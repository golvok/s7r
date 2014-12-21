#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdlib.h>
#include <util/geometry.h>

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

#endif /* PARTICLE_H */
