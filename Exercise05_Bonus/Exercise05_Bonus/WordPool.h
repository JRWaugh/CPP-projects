#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
class WordPool
{
public:
	void fill(std::string const term, std::ifstream infile);
private:
	std::string words;
};

