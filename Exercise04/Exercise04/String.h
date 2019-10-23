#include <iostream>
#include <cstring>
class String
{
	friend std::ostream& operator << (std::ostream& os, const String &string);

public:
	String(const char* string = "");
	String(const String& s);
	~String();
	void list();
	String operator + (const String &string) const;
	const String operator = (const String &string);
private:
	char* c_string;
};