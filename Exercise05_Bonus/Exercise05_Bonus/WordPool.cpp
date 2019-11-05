#include "WordPool.h"

void WordPool::fill(std::string const term, std::ifstream infile) {
	std::string line;
	while (std::getline(infile, line)) {
		if ("[" + line + "]" == term) {
			while (std::getline(infile, line) && line.length() > 0) {
				words += line + " ";
			}
		}
	}
}