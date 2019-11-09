#define _CRT_RAND_S
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "WordPool.h"
#ifdef _DEBUG // Active only in Debug Configuration
#define _CRTDBG_MAP_ALLOC // Visual Studio Mem Leak detector activated
#include <crtdbg.h>
#endif

using namespace std;
string giveStatement(string filename) {
	//I am not particularly happy with how I did any of this.
	WordPool g, a, s, v;
	ifstream inputFile(filename);
	g.fill("G", inputFile); a.fill("A", inputFile); s.fill("S", inputFile); v.fill("V", inputFile);
	return g + a + s + v + g + a + s;
}

int main(void) {
	cout << giveStatement("oracle.dat") << endl << endl;
#ifdef _DEBUG
	if (_CrtDumpMemoryLeaks())
		cout << "WARNING: There was a memory leak in your program" << endl;
	else
		cout << "Your program has no memory leaks, congratulations!" << endl;
#endif
	system("PAUSE");
}