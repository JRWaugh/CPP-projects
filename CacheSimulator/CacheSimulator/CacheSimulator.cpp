#include <iostream>
#include <fstream>
#include <sstream>
#include <Vector>
#include <string>
#include <cmath>
#include "Cache.h"
#include "MainMemory.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	/*	ASSUMPTIONS	
	 *	1. The bus between the cache and main memory is sufficiently wide to fill any block in one access. 
	 *	2. One block reading from a larger block will manage to correctly access the data requested.
	 */
	unsigned int choice = 0;
	std::string input;
	std::vector<std::shared_ptr<Cache>> caches;
	std::shared_ptr<MainMemory> mainMemory(new MainMemory());

	while (choice != 3) {
		do {
			std::cout << "=== Cache Simulator ===\n1. Cache/memory initialisation\n2. Read trace file from disk\n3. Exit program" << std::endl;
			getline(std::cin, input);
			std::istringstream(input) >> choice;
		} while (input.find_first_of("123"));
		switch (choice) {
		case 1:
		{
			caches.clear();
			unsigned int memoryHitTime, cacheCount, totalSize, blockSize, setSize, policy, hitTime;

			do {
				std::cout << "Enter memory access time: ";
				getline(std::cin, input);
				std::istringstream(input) >> memoryHitTime;
			} while (input.find_first_of("0123456789"));
			mainMemory->setAccessTime(memoryHitTime);

			do {
				std::cout << "Enter number of cache levels: ";
				getline(std::cin, input);
				std::istringstream(input) >> cacheCount;
			} while (input.find_first_of("123456789"));
			
			for (unsigned int i = 0; i < cacheCount; i++) {
				std::cout << "Enter information for cache " << i + 1 << std::endl;
				
				do {
					std::cout << "Enter block size as a power of 2: 2^";
					getline(std::cin, input);
					std::istringstream(input) >> blockSize;
				} while (input.find_first_of("0123456789"));
				

				do {
					std::cout << "Enter set size: ";
					getline(std::cin, input);
					std::istringstream(input) >> setSize;
				} while (input.find_first_of("123456789"));

				do {
					std::cout << "Enter total cache size as a power of 2 (must be at least 2^" << log2((1U << blockSize) * setSize) << "): 2^";
					getline(std::cin, input);
					std::istringstream(input) >> totalSize;
				} while (input.find_first_of("0123456789") || totalSize < log2((1U << blockSize) * setSize));
				
				do {
					std::cout << "Select replacement policy (1. FIFO, 2. LRU, 3. Random): ";
					getline(std::cin, input);
					std::istringstream(input) >> policy;
				} while (input.find_first_of("123"));

				do {
					std::cout << "Enter hit time: ";
					getline(std::cin, input);
					std::istringstream(input) >> hitTime;
				} while (input.find_first_of("0123456789"));
				
				caches.emplace_back(new Cache(blockSize, setSize, totalSize, (Policy)policy, hitTime));
			}

			for (unsigned int i = 0; i < caches.size(); ++i) {
				if (i == caches.size() - 1)
					caches[i]->setLowerMem(mainMemory);
				else
					caches[i]->setLowerMem(caches[i + 1]);
			}
		}
		break;

		case 2: {
			std::string filename;
			std::ifstream myfile;
			if (caches.empty()) {
				std::cout << "Cannot read trace file until the cache has been initialised." << std::endl;
				break;
			}
			do {
				std::cout << "Enter filename of trace file: ";
				getline(std::cin, filename);
				myfile.open(filename);
				if (myfile.is_open()) {
					unsigned char instruction;
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
							std::cout << "Error reading trace file line." << std::endl;
							break;
						}
					}
					
					std::cout << "Report for each memory level:" << std::endl;
					for (unsigned int i = 0; i < caches.size(); ++i) {
						std::cout << "Cache level " << i + 1 << ":\n" << *caches[i] << std::endl;
					}
					std::cout << "Total cycles: " << totalCycles << std::endl;
					
					for (auto& n : caches) {
						n->resetCache();
					}
				}
				else {
					std::cout << "File could not be opened." << std::endl;
				}
			} while (!myfile.is_open());
		}
			break;
		case 3:
			std::cout << "Closing program." << std::endl;
			break;
		default:
			std::cout << "Invalid input." << std::endl;
			break;
		}
	}
	return 0;
	_CrtDumpMemoryLeaks();
}