// CacheSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <Vector>
#include <string>
#include "Cache.h"
#include "MainMemory.h"

int main()
{
    int choice = 0;
	std::vector<std::shared_ptr<Cache>> caches;
	std::shared_ptr<MainMemory> mainMemory(new MainMemory());
	while (choice != 4) {
		std::cout << "Cache Simulator\n1. Cache/memory initialisation\n2. Read trace file from disk\n3. Print report\n4. Close program" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 1: 
		{
			caches.clear();
			unsigned int cacheCount, blockSize, setSize, totalSize, hitTime, memoryHitTime;
			std::cout << "Enter memory access time: ";
			std::cin >> memoryHitTime;
			std::cout << "Enter number of cache levels: ";
			std::cin >> cacheCount;
			while (cacheCount--) {
				std::cout << "Enter block size as a power of 2: 2^";
				std::cin >> blockSize;
				std::cout << "Enter set size: ";
				std::cin >> setSize;
				std::cout << "Enter total cache size as a power of 2: 2^";
				std::cin >> totalSize;
				std::cout << "Enter hit time: ";
				std::cin >> hitTime;
				caches.emplace_back(new Cache(blockSize, setSize, totalSize, hitTime));
			}

			mainMemory->setAccessTime(memoryHitTime);

			for (unsigned int i = 0; i < caches.size(); ++i) {
				if (i == caches.size() - 1)
					caches[i]->setLowerMem(mainMemory);
				else
					caches[i]->setLowerMem(caches[i + 1]);
			}
		}
			break;
		case 2: {
			std::string filename, line;
			std::cout << "Enter filename of trace file: ";
			std::cin >> filename;
			std::ifstream myfile(filename);
			if (myfile.is_open()) {
				char instruction;
				unsigned int address, cycles, totalCycles = 0;
				while (myfile >> instruction >> std::hex >> address >> cycles) {
					totalCycles += cycles;
					switch (instruction) {
					case 'l':
						totalCycles += caches[0]->readAddress(address);
						break;
					case 's':
						totalCycles += caches[0]->writeAddress(address);
						break;
					default:
						std::cout << "Error reading trace file line I guess!" << std::endl;
					}
				}
				unsigned int hitCount = 0, missCount = 0;
				for (auto n : caches) {
					hitCount += n->getHitCount();
					missCount += n->getMissCount();
				}
				std::cout << "Hit count: " << hitCount << "Miss count: " << missCount << std::endl;
				std::cout << "Total cycles: " << totalCycles << std::endl;
				for (auto n : caches) {
					n->resetCache();
				}
			}
		}
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}
}