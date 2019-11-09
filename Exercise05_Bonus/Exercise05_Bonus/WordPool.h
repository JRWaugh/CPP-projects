#define _CRT_RAND_S
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>

#include <stdlib.h>
class WordPool
{
public:
	void fill(std::string const term, std::ifstream &infile);
	WordPool operator+(WordPool const& wp) const;
	operator std::string() {
		return random_word + ".";
	}
private:
	std::string words, random_word;
	std::string getRandom() const;
};

