#include "WordPool.h"
#include <algorithm>


void WordPool::fill(std::string const term, std::ifstream &infile) {
	std::string line("");
	while (std::getline(infile, line) && line != ("[" + term + "]"));
	while (std::getline(infile, line) && line.length() > 0)
		words += line + "_";
	random_word = getRandom();
	random_word[0] = toupper(random_word[0]);
}

WordPool WordPool::operator+(WordPool const& wp) const {
	WordPool temp;
	temp.random_word = random_word + " " + wp.getRandom();
	return temp;
}

std::string WordPool::getRandom() const {
	unsigned int random;
	errno_t x = rand_s(&random);
	unsigned int count = (unsigned int)((double)random / ((double)UINT_MAX + 1) * std::count(words.begin(), words.end(), '_')) + 1;
	unsigned int pos = 0;
	for (unsigned int i = 0; i < count - i; ++i) {
		pos = words.find('_', pos);
		++pos;
	}
	return words.substr(pos, words.find('_', pos) - pos);
}
