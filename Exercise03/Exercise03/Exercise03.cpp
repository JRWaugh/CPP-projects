#include <iostream>
#include "String.h"

//void f(String &s); Phase 3. Destructor is not called when using a reference parameter because the object is not going out of scope
void f(String s);

int main() {
	String s("abcdefg");
	//String s2 = s; Tested if this works. It does.
	s.list();
	f(s);
	s.list(); //Phase 2: memory error without copy constructor here because the destructor was called when String s went out of f()'s scope

	return 0;
}


void f(String s) {
	s.list();
}