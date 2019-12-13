/* Here is the Point class. It is almost similar same that in the program memleakharj.cpp.
	The difference is that it now has a member function distance, that calculates
	distance between two points.
*/

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
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
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

//Class definition for Point
class Point {
	friend ostream& operator<<(ostream& out, const Point& point);
public:
	Point(float x0 = 0.0, float y0 = 0.0);
	~Point();

	bool operator < (const Point& p2) const { return (distance(0.0, 0.0) > p2.distance(0.0, 0.0)); }
	float distance(const Point& p2) const;

	//Had to add this because any temporary Point would cause N to be >= 4
	float distance(float x0, float y0) const { return sqrt((x0-x) * (x0-x) + (y0-y) * (y0-y)); }
	static int count;
private:
	float x;
	float y;
};
int Point::count = 0;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try {
		const Point origin(0.0, 0.0);
		/* In the case that N = 4, the leak will be the first two 
		 * objects dynamically allocated in the vector. The fourth one doesn't complete
		 * construction so it is fine. Origin is not dynamically allocated so it will be
		 * destructed when it goes out of scope.
		 * The leak is smaller if N = 3 because the exception occurs on the second new Point.
		*/

		/******Memory Leak Stuff******/
		/*
		std::vector<Point*> points = { new Point(3.4, 5.6), new Point(2.8, 9.1), new Point(7.1, 0.8) };
		std::sort(points.begin(), points.end(), [&origin](auto const& p1, auto const& p2) { 
			return p1->distance(origin) < p2->distance(origin); });
		for (auto& p : points) {
			delete p;
		}
		points.clear();
		*/


		/******The Fix******/
		//Annoyingly initializer lists always perform copies so I just used emplace_back
		
		std::vector<std::unique_ptr<Point>> points;
		points.emplace_back(std::make_unique<Point>(3.4, 5.6));
		points.emplace_back(std::make_unique<Point>(2.8, 9.1));
		points.emplace_back(std::make_unique<Point>(7.1, 0.8));
		std::sort(points.begin(), points.end(), [&origin](std::unique_ptr<Point>& p1, std::unique_ptr<Point>& p2) {
			return p1->distance(origin) < p2->distance(origin); });		
		for (auto& n : points) {
			std::cout << n->distance(0, 0) << std::endl;
		}
	} 
	catch (Exception e) {
		cout << "Exception explanation is: " << e.what() << endl;
	}

	//with weird < operator defined
	//std::sort(points.begin(), points.end());
	_CrtDumpMemoryLeaks();
	return 0;
	
} 
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
