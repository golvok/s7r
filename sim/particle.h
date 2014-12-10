#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdlib.h>

/**
 * A point datatype.
 */
struct Point {
	float x;
	float y;

	void set(float x, float y);
	void set(const Point& src);

	/**
	 * Behaves like a 2 argument plusequals.
	 */
	void offset(float x, float y);

	/**
	 * These add the given point to this point in a
	 * componentwise fashion, ie x = x + rhs.x
	 *
	 * Naturally, {+,-} don't modify and {+,-}= do.
	 */
	Point operator+ (const Point& rhs) const;
	Point operator- (const Point& rhs) const;
	Point operator* (float rhs) const;
	Point& operator+= (const Point& rhs);
	Point& operator-= (const Point& rhs);
	Point& operator*= (float rhs);

	/**
	 * Assign that point to this one - copy the components
	 */
	Point& operator= (const Point& src);

	Point();
	Point(const Point& src);
	Point(float x, float y);

	static const int POSITIVE_DOT_PRODUCT = 0;
	static const int NEGATIVE_DOT_PRODUCT = 1;

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

Point operator*(float lhs, const Point& rhs);

/**
 * Represents a rectangle, used as a bounding box.
 */
class BoundBox {
public:
	/**
	 * These return their respective edge/point's location
	 */
	const float& left() const;
	const float& right() const;
	const float& bottom() const;
	const float& top() const;
	float& left();
	float& right();
	float& bottom();
	float& top();  

	const Point& bottom_left() const;
	const Point& top_right() const;
	Point& bottom_left();
	Point& top_right();

	/**
	 * Calculate and return the center
	 */
	float get_xcenter() const;
	float get_ycenter() const;
	Point get_center() const;

	/**
	 * Calculate and return the width/height
	 * ie. right/top - left/bottom respectively.
	 */
	float get_width() const;
	float get_height() const;

	/**
	 * These behave like the plusequal operator
	 * They add their x and y values to all corners
	 */
	void offset(const Point& make_relative_to);
	void offset(float by_x, float by_y);

	/**
	 * Does the given point coinside with this bbox?
	 * Points on the edges or corners are included.
	 */
	bool intersects(const Point& test_pt) const;
	bool intersects(float x, float y) const;

	/**
	 * Calculate and return the area of this rectangle.
	 */
	float area() const;

	/**
	 * These add the given point to this bbox - they
	 * offset each corner by this point. Usful for calculating
	 * the location of a box in a higher scope, or for moving
	 * it around as part of a calculation
	 *
	 * Naturally, the {+,-} don't modify and the {+,-}= do.
	 */
	BoundBox operator+ (const Point& rhs) const;
	BoundBox operator- (const Point& rhs) const;
	BoundBox& operator+= (const Point& rhs);
	BoundBox& operator-= (const Point& rhs);

	/**
	 * Assign that box to this one - copy it's left, right, bottom, and top.
	 */
	BoundBox& operator= (const BoundBox& src);

	BoundBox();
	BoundBox(const BoundBox& src);
	BoundBox(float left, float bottom, float right, float top);
	BoundBox(const Point& bottomleft, const Point& topright);
	BoundBox(const Point& bottomleft, float width, float height);
private:
	Point bottomleft;
	Point topright;
};


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
