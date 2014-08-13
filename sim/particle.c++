#include "particle.h"
#include "math.h"

Point::Point(float x_, float y_) :
	x(x_), y(y_) { }

void Point::set(float x, float y) {
	this->x = x;
	this->y = y;
}

float Point::deltaX(Point p1, Point p2) {
	return p2.x - p1.x;
}

float Point::deltaY(Point p1, Point p2) {
	return p2.y - p1.y;
}

Point Point::delta(Point p1, Point p2) {
	return Point(deltaX(p1, p2), deltaY(p1, p2));
}

Point Point::multiply(Point p, float constant) {
	return Point(p.x * constant, p.y * constant);
}

Point Point::divide(Point p, float constant) {
	return Point(p.x / constant, p.y / constant);
}

Point Point::add(Point p1, Point p2) {
	return Point(p1.x + p2.x, p1.y + p2.y);
}

float Point::distance(Point p1, Point p2) {
	return sqrt(pow(deltaX(p1, p2), 2) + pow(deltaY(p1, p2), 2));
}

float Point::magnitude(Point p) {
	return magnitude(p);
}

float Point::magnitudeSquared(Point p) {
	return pow(p.x, 2) + pow(p.y, 2);
}

Point Point::unit(Point p) {
	return divide(p, (float) magnitude(p));
}

float Point::dotProduct(Point p1, Point p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

Point Point::project(Point source, Point wall) {
	return multiply(wall, dotProduct(wall, source) / magnitudeSquared(wall));
}

// Point* Point::farthestPoint(Point p, std::vector<Point> tests) {
// 	int farthestIndex = 0;
// 	float farthestDistance = distancef(p, tests[0]);
// 	for (int i = 1; i < tests.size(); ++i) {
// 		float distance = distancef(p, tests[i]);
// 		if (farthestDistance < distance) {
// 			farthestIndex = i;
// 			farthestDistance = distance;
// 		}
// 	}
// 	return tests[farthestIndex];
// }

// Point* farthestFromLineWithSides(Point p, Point q, std::vector<Point> tests) {
// 	Point line = delta(p, q);
// 	Point farthests[2];
// 	float farthestDistance[2];
// 	for (int i = 0; i < tests.size(); ++i) {
// 		Point relativeToP = delta(p, tests[i]);
// 		float distance = magnitude(perpindictularDeltaVectorToLine(line, relativeToP));
// 		int dotProductSign = dotProduct(line, relativeToP) < 0 ? NEGATIVE_DOT_PRODUCT : POSITIVE_DOT_PRODUCT;
// 		if (farthestDistance[dotProductSign] < distance) {
// 			farthests[dotProductSign] = tests[i];
// 			farthestDistance[dotProductSign] = distance;
// 		}
// 	}
// 	return farthests;
// }

float Point::distanceToLine(Point onLine_1, Point onLine_2, Point p) {
	return magnitude(perpindictularDeltaVectorToLine(onLine_1, onLine_2, p));
}

Point Point::perpindictularDeltaVectorToLine(Point onLine_1, Point onLine_2, Point p) {
	return perpindictularDeltaVectorToLine(delta(onLine_1, onLine_2), delta(p, onLine_1));
}

Point Point::perpindictularDeltaVectorToLine(Point direction, Point p) {
	return delta(p, project(p, direction));
}

// public std:string toString() {
// 	return "(" + x + ',' + y + ')';
// }

void Particle::setPosition(float x, float y) {
	this->position.set(x,y);
}

Point Particle::getPosition() {
	return position;
}
