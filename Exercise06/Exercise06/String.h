#include <iostream>
#include <cstring>
constexpr unsigned int max_string_length = 10;
class String
{
	friend std::ostream& operator << (std::ostream& os, const String& string);

public:
	String(const char* string = "");
	String(const String& s);
	~String();
	void list();
	String operator + (const String& string) const;
	String& operator++();
	String operator++(int);
	const String& operator = (const String& string);
	char& operator[](int i);
	operator const char* ();
	static int count;
private:
	char* c_string;
};