#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <numeric>
#include <iterator>
#include <functional>
#include "Athlete.h"

using namespace std;

struct field_reader : ctype<char> { // specify '\n' as the only separator
	field_reader() : ctype<char>(get_table()) {}
	static ctype_base::mask const* get_table() {
		static vector<ctype_base::mask>
			rc(table_size, ctype_base::mask());
		rc['\n'] = ctype_base::space;
		return &rc[0];
	}
};


int main()
{
	
	std::vector<Athlete> athletes;
	std::string line;
	std::ifstream fp("athletes.txt");
	fp.imbue(locale(locale(), new field_reader()));// use our own separator list
	
	copy(istream_iterator<string>{fp}, istream_iterator<string>{}, back_inserter(athletes));
	shuffle(athletes.begin(), athletes.end(), std::mt19937{ std::random_device{}() });
	for_each(athletes.begin(), athletes.end(), [i = 1](Athlete &a) mutable {a.setNumber(i++);});
	for_each(athletes.begin(), athletes.end(), mem_fn(&Athlete::readTime));
	sort(athletes.begin(), athletes.end());
	copy(athletes.begin(), athletes.end(), std::ostream_iterator<Athlete>(std::cout, "\n"));
}