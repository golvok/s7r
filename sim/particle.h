#include <queue>

struct Point {
	float x;
	float y;
	void set(float x, float y);
	Point(float x, float y);
};

class Particle {
public:
	void setPosition(float x, float y);
private:
	Point position;
	std::queue<Point> destinations; // move to delegate
	Particle(const Particle&);
	Particle& operator=(const Particle&);
};
