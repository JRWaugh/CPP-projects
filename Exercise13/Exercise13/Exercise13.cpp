/* Here is the Point class. It is almost similar same that in the program memleakharj.cpp.
	The difference is that it now has a member function distance, that calculates
	distance between two points.
*/

#include <cmath>
#include <iostream>
using namespace std;


//Definition for exception class
class Exception {
public:
	Exception(const char* explanation0) { explanation = explanation0; }
	const char* what() const { return explanation; }
private:
	const char* explanation;
};


#define N 100

//Class definition for Point
class Point {
	friend ostream& operator<<(ostream& out, const Point& point);
public:
	Point(float x0 = 0.0, float y0 = 0.0);
	~Point();
	float distance(const Point& p2) const;
	static int count;
private:
	float x;
	float y;
};
int Point::count = 0;

//-----------------------------------------------------------------
//Implementation of class Point
Point::Point(float x0, float y0) : x(x0), y(y0) {
	count++;
	if (count >= N)
		throw Exception("Does not succeed to create the last apex");
	cout << "Point constructor is done " << (void*)this << endl;
}

Point::~Point() {
	cout << "\nDestructor for Point " << (void*)this << " : ("
		<< x << "," << y << ")" << endl;
}

float Point::distance(const Point& p2) const {
	float deltax = p2.x - x;
	float deltay = p2.y - y;
	return sqrt(deltax * deltax + deltay * deltay);
}


// Friend functions of Point
ostream& operator<<(ostream& out, const Point& point) {
	out << "(" << point.x << "," << point.y << ")";
	return out;
}
