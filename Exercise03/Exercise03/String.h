#include <iostream>
#include <cstring>
class String
{
public:
	String(const char* string = "");
	String(const String &s); //copy constructor
	~String();
	void list();
private:
	char *c_string;
	//char c_string[11] Phase 1
};