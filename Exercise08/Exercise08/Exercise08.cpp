#include <iostream>
#include "LimitedCounter.h"
 
using namespace std;
int main() {
	LimitedCounter lc(3, 5); //initial value 3, upper limit 5
	cout << lc++ << endl; //output should be 3
	cout << ++lc << endl; //output should be 5
	lc.reset();
	for (int i = 0; i < 9; i++) {
		lc++;
		cout << lc << endl; //output is 1 2 3 4 5 5 5 5 5
	}
	cout << lc.getCount() << endl; //output is 5
	cout << (lc < 7);//Testing the comparison operator //output is 1
	cout << (lc < 1);//Testing the comparison operator //output is 0
	return 0;
}
