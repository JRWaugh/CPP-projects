#include <iostream>
#include <string>

class Athlete {
	friend std::ostream& operator << (std::ostream& os, const Athlete& athlete) {
		os << athlete.getNumber() << " " << athlete.getName() << "\t" << athlete.getTime();
		return os;
	}

public:
	Athlete(std::string name = "", int time = 0, int number = 0) : m_name(name), m_time(time), m_number(number){}
	void setNumber(int const number) { m_number = number; }
	int getNumber() const { return m_number; }
	void setName(std::string const name) { m_name = name; }
	std::string getName() const { return m_name; }
	void setTime(int const time) { m_time = time; }
	int getTime() const { return m_time; }
	bool operator<(const Athlete& b) { return getTime() < b.getTime(); }
	void readTime() {
		std::cout << "Give time for " << m_name << ": ";
		std::cin >> m_time;
	}

private:
	std::string m_name;
	int m_number;
	int m_time;
};