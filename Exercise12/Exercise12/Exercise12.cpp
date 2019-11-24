/* ex_memleak.cpp -- lab12 exercise program
*
* This programs has classes Point and Triangle. Triangle consists
* three points. which are reserved separately from dynamic memory.
* Therefore Triangle has three pointers to Points. Memory for the
* Triangle itself has also been reserved from the dynamic memory.
*
* Program can generate an exception at the Nth Point constructor call.
* N is now 4 which means that all those three Points (in Triangle) can
* be created.
*/

#include <iostream>
using namespace std;

// N spesifies which constructor of the Point fails (throws an exception)
#define N 4
#ifdef _DEBUG // Active only in Debug
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

// Class definition for Point
class Point {
	friend ostream& operator<<(ostream& out, const Point& point);
public:
	Point(float x0 = 0.0, float y0 = 0.0);
	Point(const Point& p0);
	~Point();
	static int count;
private:
	float x;
	float y;
};
int Point::count = 0;
class Triangle {
	friend ostream& operator<<(ostream& out, const Triangle& triangle);
public:
	Triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	~Triangle();
private:
	//Using unique_ptr because they're newer and better apparently
	unique_ptr<Point> apex1;
	unique_ptr<Point> apex2;
	unique_ptr<Point> apex3;
	/*Point* apex1;
	Point* apex2;
	Point* apex3;*/
};

// Definition for exception class
class Exception {
public:
	Exception(const char* explanation0) { explanation = explanation0; }
	const char* what() const { return explanation; }
private:
	const char* explanation;
};

// Main function
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	try {
		//switching to auto_ptr solves problem because its destructor is called when it goes out of scope of the try block
		unique_ptr<Triangle> p(new Triangle(1.0, 1.0, 2.0, 2.0, 3.0, 3.0));
		cout << *p;
		throw Exception ("Exception after Triangle has been created");		
	} 
	catch (Exception e) {
		cout << "Exception explanation is: " << e.what() << endl;
		cout << "Exception is managed now" << endl;
	} 

	cout << "Constructor for point is executed " << Point::count << " times" << endl;
	system("PAUSE");
	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------
// Implementation of class Point
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
// Friend functions of Point
ostream& operator<<(ostream& out, const Point& point) {
	out << "(" << point.x << "," << point.y << ")";
	return out;
}

//-----------------------------------------------------------------
// Implementation of class Triangle
Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3) : 
	apex1(new Point(x1, y1)), apex2(new Point(x2, y2)), apex3(new Point(x3, y3)) {

	/*apex1 = new Point(x1, y1);
	apex2 = new Point(x2, y2);
	apex3 = new Point(x3, y3);*/
	cout << "Triangle constructor is done " << (void*)this << endl;
}
Triangle::~Triangle() {
	cout << endl << "Destructor for Triangle " << endl;

	/*No longer need the destructor because the auto pointers now own the dynamic objects and will deallocate the memory when a Triangle
	object goes out of scope*/
	/*delete apex1;
	delete apex2;
	delete apex3;*/
}

// Friend functions of Triangle
ostream& operator<<(ostream& out, const Triangle& triangle) {
	out << "Triangle is : ";
	out << *triangle.apex1 << " " << *triangle.apex2 << " " << *triangle.apex3 << endl;
	return out;
}

