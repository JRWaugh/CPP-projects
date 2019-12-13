#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <ctime>

using namespace std;
using namespace std::chrono;
std::mutex m;

class Dealer;
class Car {
	friend std::ostream& operator<<(std::ostream& out, const Car& car);
public:
	Car(std::shared_ptr<Dealer> owner = nullptr, const char* m = "Ford", const char* l = nullptr, int ml = 0):
		model(m), 
		license(l ? l : (rand() % 2 ? "FOO-" : "BAR-") + std::to_string(rand() % 999 + 1)), 
		mileage(ml ? ml : rand() % 10000), 
		mDealer(owner)
	{}
	~Car() { std::cout << model << " " << license << " deleted" << std::endl; };
	void Read();
	std::string GetLicense() { return license; }
	void setDealer(std::weak_ptr<Dealer> dealer) { mDealer = dealer; }
private:
	std::weak_ptr<Dealer> mDealer;
	std::string model;
	std::string license;
	int mileage;
};

void Car::Read()
{
	//std::cout << "Enter car information" << std::endl;
	// replace the following with your own code
	model = "Seat";
	license = (rand() % 2 ? "ZAP-" : "ZIP-") + std::to_string(rand() % 999 + 1);
	mileage = 10000 + rand() % 10000;
}

/* ---------------------- */

class Website {
public:
	Website(const char* n = nullptr) : name(n ? n : "www.cars" + std::to_string(rand() % 99 + 1) + ".com") {}
	~Website() { std::cout << name << " deleted" << std::endl; };
	void advertise(std::weak_ptr<Car> car) { listing.push_back(car); }
	void print(std::ostream& out = std::cout) {
		lock_guard<mutex> lg(m);
		out << name << std::endl; 
		listing.erase(std::remove_if(listing.begin(), listing.end(), [&out](auto car) {
			auto carContent = car.lock();
			if (carContent)
				out << *carContent;
			return !carContent;
			}
		), listing.end());
		out << name << " end of list" << std::endl;
	}
	void run() {
		while (!listing.empty()) {
			time_point<high_resolution_clock> start = high_resolution_clock::now();
			std::this_thread::sleep_for(seconds(10));
			print();
		}
	}
private:
	std::vector<std::weak_ptr<Car>> listing;
	std::string name;
};

class Dealer: public std::enable_shared_from_this<Dealer> {
	friend std::ostream& operator<<(std::ostream& out, const Dealer& dealer);
public:
	Dealer(const char* name_ = "John Doe") : name(name_) {};
	~Dealer() { std::cout << name << " deleted" << std::endl; };
	void buy();
	void sell();
	void add(std::shared_ptr<Car> car) { 
		car->setDealer(this->shared_from_this());
		cars.push_back(car); 
		sites.erase(std::remove_if(sites.begin(), sites.end(), [&car](auto site) {
			auto siteContent = site.lock();
			if (siteContent)
				siteContent->advertise(car);
			return !siteContent;
			}), sites.end());
	}
	void add_site(std::weak_ptr<Website> w) { sites.push_back(w); }
	std::string getName() { return name; }
private:
	std::string name;
	std::vector<std::shared_ptr<Car>> cars;
	std::vector<std::weak_ptr<Website>> sites;
};

void Dealer::buy()
{
	std::shared_ptr<Car> car(std::make_shared<Car>());
	car->Read();
	add(car);
}

void Dealer::sell()
{
	lock_guard<mutex> lg(m);
	std::cout << *this; // print my list of cars
	std::cout << "Enter license of car you want to buy" << std::endl;
	std::string license;
	std::cin >> license;
	auto ci = std::find_if(cars.begin(), cars.end(), [&license](auto c) {return license == c->GetLicense(); });
	if (ci != cars.end()) {
		cars.erase(ci);
	}
}


std::ostream& operator<<(std::ostream& out, const Dealer& dealer)
{
	std::cout << dealer.name << "'s cars for sale" << std::endl;
	for (auto car : dealer.cars) std::cout << *car;
	std::cout << "End of " << dealer.name << "'s cars listing" << std::endl;

	return out;
}

std::ostream& operator<<(std::ostream& out, const Car& car)
{
	if (auto dealer = car.mDealer.lock())
		out << "Dealer: " << dealer->getName() << std::endl;
	out << "Model: " << car.model << std::endl << "License: " << car.license << std::endl << "Mileage: " << car.mileage << std::endl;
	return out;
}

/* ---------------------- */



void car_sales()
{
	std::cout << "Car sales started" << std::endl;
	std::shared_ptr<Car> ca(new Car);
	std::shared_ptr<Car> cb(new Car);
	std::shared_ptr<Website> wa(new Website("www.autos.com"));
	std::shared_ptr<Website> wb(new Website("www.bilars.com"));
	std::shared_ptr<Website> wc(new Website("www.cars.com"));
	std::shared_ptr<Dealer> a(std::make_shared<Dealer>("Alan Aldis"));
	std::shared_ptr<Dealer> b(std::make_shared<Dealer>("Bill Munny"));
	{
		std::shared_ptr<Dealer> c(std::make_shared<Dealer>("Casey Ball"));

		a->add_site(wa);
		a->add_site(wb);
		b->add_site(wb);
		b->add_site(wc);
		c->add_site(wa);
		c->add_site(wb);
		c->add_site(wc);

		a->buy();
		a->buy();
		a->buy();
		a->buy();

		b->buy();
		b->buy();
		b->buy();

		c->buy();
		c->buy();
		c->add(std::move(ca));
		c->add(std::move(cb));

		thread wa_t(&Website::run, wa);
		thread wb_t(&Website::run, wb);
		thread wc_t(&Website::run, wc);
		char selection;
		do {
			std::cout << "Choose who to buy from (q to quit):\n" <<
				"1. " << a->getName() << endl <<
				"2. " << b->getName() << endl <<
				"3. " << c->getName() << endl <<
				"Input: ";

			cin >> selection;
			switch (selection) {
			case '1': a->sell();
				break;
			case '2': b->sell();
				break;
			case '3': c->sell();
				break;
			case 'q': 
				cout << "Closing program." << endl;
				break;
			default:
				cout << "Invalid input." << endl;
				break;
			}				
		} while (selection != 'q');
		wa_t.join();
		wb_t.join();
		wc_t.join();
	}
	std::cout << "Car sales ended" << std::endl;

}

int main(int argc, char** argv) {
	_CrtMemState s1;
	_CrtMemCheckpoint(&s1);

	srand(time(NULL));

	car_sales();

	_CrtMemState s2, s3;
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2)) {
		_CrtDumpMemoryLeaks();
		_CrtMemDumpStatistics(&s3);
	}

	return 0;
}