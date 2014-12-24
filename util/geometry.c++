#include "geometry.h++"
#include <math.h>

/******************************************
 * begin Point function definitions *
 ******************************************/

void Point::set(float _x, float _y) { x = _x; y = _y; }
void Point::set(const Point& src) { x = src.x; y = src.y; }

void Point::offset(float _x, float _y) {
	x += _x;
	y += _y;
}

Point Point::operator- (const Point& rhs) const {
	Point result = *this;
	result -= rhs;
	return result;
}

Point Point::operator+ (const Point& rhs) const {
	Point result = *this;
	result += rhs;
	return result;
}

Point Point::operator* (float rhs) const {
	Point result = *this;
	result *= rhs;
	return result;
}

Point& Point::operator+= (const Point& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Point& Point::operator-= (const Point& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

Point& Point::operator *= (float rhs) {
	this->x *= rhs;
	this->y *= rhs;
	return *this;
}

Point& Point::operator= (const Point& src) {
	this->x = src.x;
	this->y = src.y;
	return *this;
}

Point::Point() : x(0), y(0) { }

Point::Point(const Point& src) :
	x(src.x), y(src.y) {
}

Point::Point(float _x, float _y) : x(_x), y(_y) { }

Point operator*(float lhs, const Point& rhs) {
	return rhs*lhs;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << '{' << p.x << ',' << p.y << '}';
	return os;
}

/******************************************
 * begin BoundBox function definitions *
 ******************************************/

const float& BoundBox::left() const { return bottom_left().x; }
const float& BoundBox::right() const { return top_right().x; }
const float& BoundBox::bottom() const { return bottom_left().y; }
const float& BoundBox::top() const { return top_right().y; }
const Point& BoundBox::bottom_left() const { return bottomleft; }
const Point& BoundBox::top_right() const { return topright; }
float& BoundBox::left() { return bottom_left().x; }
float& BoundBox::right() { return top_right().x; }
float& BoundBox::bottom() { return bottom_left().y; }
float& BoundBox::top() { return top_right().y; }
Point& BoundBox::bottom_left() { return bottomleft; }
Point& BoundBox::top_right() { return topright; }

float BoundBox::get_xcenter() const {
	return (right() + left()) / 2;
}

float BoundBox::get_ycenter() const {
	return (top() + bottom()) / 2;
}

Point BoundBox::get_center() const {
	return Point(get_xcenter(), get_ycenter());
}

float BoundBox::get_width() const {
	return right() - left();
}

float BoundBox::get_height() const {
	return top() - bottom();
}

void BoundBox::offset(const Point& relative_to) {
	this->bottomleft += relative_to;
	this->topright += relative_to;
}

void BoundBox::offset(float by_x, float by_y) {
	this->bottomleft.offset(by_x, by_y);
	this->topright.offset(by_x, by_y);
}

bool BoundBox::intersects(const Point& test_pt) const {
	return intersects(test_pt.x, test_pt.y);
}

bool BoundBox::intersects(float x, float y) const {
	if (x < left() || right() < x || y < bottom() || top() < y) {
		return false;
	} else {
		return true;
	}
}

float BoundBox::area() const {
	return fabs(get_width() * get_height());
}

BoundBox BoundBox::operator+ (const Point& rhs) const {
	BoundBox result = *this;
	result.offset(rhs);
	return result;
}

BoundBox BoundBox::operator- (const Point& rhs) const {
	BoundBox result = *this;
	result.offset(Point(-rhs.x, -rhs.y));
	return result;
}

BoundBox& BoundBox::operator+= (const Point& rhs) {
	this->offset(rhs);
	return *this;
}

BoundBox& BoundBox::operator-= (const Point& rhs) {
	this->offset(Point(-rhs.x, -rhs.y));
	return *this;
}

BoundBox& BoundBox::operator= (const BoundBox& src) {
	this->bottom_left() = src.bottom_left();
	this->top_right() = src.top_right();
	return *this;
}

BoundBox::BoundBox() :
	bottomleft(), topright() {
}

BoundBox::BoundBox(const BoundBox& src) :
	bottomleft(src.bottom_left()), topright(src.top_right()) {
}
BoundBox::BoundBox(float _left, float _bottom, float _right, float _top) :
	bottomleft(_left,_bottom), topright(_right,_top) {
}

BoundBox::BoundBox(const Point& _bottomleft, const Point& _topright) :
	bottomleft(_bottomleft), topright(_topright) {
}

BoundBox::BoundBox(const Point& _bottomleft, float width, float height) :
	bottomleft(_bottomleft) , topright(_bottomleft) {
	topright.offset(width, height);
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
	return sqrt(magnitudeSquared(p));
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

Point Point::getPerpindular(Point p) {
	return Point(p.y,-p.x);
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
