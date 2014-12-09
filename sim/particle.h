#ifndef PARTICLE_H
#define PARTICLE_H

struct Point {
	static const int POSITIVE_DOT_PRODUCT = 0;
	static const int NEGATIVE_DOT_PRODUCT = 1;

	float x;
	float y;
	void set(float x, float y);
	Point(float x, float y);

	static float deltaX(Point p1, Point p2);
	static float deltaY(Point p1, Point p2);
	static Point delta(Point p1, Point p2);
	static Point multiply(Point p, float constant);
	static Point divide(Point p, float constant);
	static Point add(Point p1, Point p2);
	static float distance(Point p1, Point p2);
	static float magnitude(Point p);
	static float magnitudeSquared(Point p);
	static Point unit(Point p);
	static float dotProduct(Point p1, Point p2);
	static Point project(Point source, Point wall);
	// static Point* farthestPoint(Point p, std::vector<Point> tests);
	// static Point* farthestFromLineWithSides(Point p, Point q, std::vector<Point> tests);
	static float distanceToLine(Point onLine_1, Point onLine_2, Point p);
	static Point perpindictularDeltaVectorToLine(Point onLine_1, Point onLine_2, Point p);
	static Point perpindictularDeltaVectorToLine(Point direction, Point p);

};

class Particle {
public:
	void setPosition(float x, float y);
	Point getPosition();
private:
	Point position;
	Particle(const Particle&);
	Particle& operator=(const Particle&);
};

#endif /* PARTICLE_H */
