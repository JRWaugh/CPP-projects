#include <iostream>
#include <fstream>
#include <sstream>
#include <Vector>
#include <string>
#include "Cache.h"
#include "MainMemory.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

enum class Choice { INIT = 1, READ, REPORT, EXIT };

int main()
{
	//	This program simulates a write-back/write-allocate cache.
	//	This sim assumes the bus between the cache and main memory is sufficiently wide to fill any block in one access.
	Choice choice = Choice::INIT;
	unsigned int totalCycles = 0;
	std::string input, last_file;
	std::ifstream myfile;
	std::vector<std::shared_ptr<Cache>> caches;

	while (choice != Choice::EXIT) {
		while(std::cout << 
			"=== Cache Simulator ===\n1. Cache/memory initialisation\n2. Read trace file from disk\n3. Print report\n4. Exit program\nInput selection: " && 
			getline(std::cin, input) &&
			input.find_first_of("1234") )
			std::cout << "Invalid input." << std::endl;
		choice = (Choice)(input[0] - '0');
		std::cout << std::endl;

		switch (choice) {
		case Choice::INIT:
		{
			unsigned int memoryHitTime, cacheCount, blockSize, setSize, totalSize, policy, hitTime;
			caches.clear();
			
			while(std::cout << "Enter memory access time: " && getline(std::cin, input) && input.find_first_of("0123456789"))
				std::cout << "Invalid input." << std::endl;
			std::istringstream(input) >> memoryHitTime;

			while (std::cout << "Enter number of cache levels: " && getline(std::cin, input) && input.find_first_of("123456789"))
				std::cout << "Invalid input." << std::endl;
			std::istringstream(input) >> cacheCount;

			for (unsigned int i = 0; i < cacheCount; i++) {
				std::cout << "\nL" << i + 1 << " Cache Initialisation:" << std::endl;

				while(std::cout << "Enter block size as a power of 2: 2^" && getline(std::cin, input) && input.find_first_of("0123456789"))
					std::cout << "Invalid input." << std::endl;
				std::istringstream(input) >> blockSize;

				while(std::cout << "Enter set size as a power of 2: 2^" && getline(std::cin, input) && input.find_first_of("0123456789"))
					std::cout << "Invalid input." << std::endl;
				std::istringstream(input) >> setSize;

				while(std::cout << "Enter total cache size as a power of 2 (must be at least 2^" << blockSize + setSize << "): 2^" && 
					getline(std::cin, input) && 
					std::istringstream(input) >> totalSize && 
					input.find_first_of("0123456789") || 
					totalSize < blockSize + setSize )
					std::cout << "Invalid input." << std::endl;
				

				while(std::cout << "Select replacement policy (1. FIFO, 2. LRU, 3. Random): " && getline(std::cin, input) && input.find_first_of("123"))
					std::cout << "Invalid input." << std::endl;
				std::istringstream(input) >> policy;

				while(std::cout << "Enter hit time: " && getline(std::cin, input) && input.find_first_of("0123456789"))
					std::cout << "Invalid input." << std::endl;
				std::istringstream(input) >> hitTime;

				caches.emplace_back(new Cache(blockSize, setSize, totalSize, (Policy)policy, hitTime, memoryHitTime));
			}

			for (unsigned int i = 0; i < caches.size() - 1; ++i)
				caches[i]->setLowerMem(caches[i + 1]);

			std::cout << std::endl;
		}
		break;

		case Choice::READ:
			if (caches.empty())
				std::cout << "Cannot read trace file until the cache has been initialised." << std::endl << std::endl;
			else {				
				do {
					std::cout << "Enter filename of trace file: ";
					getline(std::cin, input);
					myfile.open(input);

					if (myfile.is_open()) {
						unsigned char instruction;
						unsigned int address, cycles;

						totalCycles = 0;
						for (auto& n : caches) {
							if (input != last_file) // New file requires invalidating all blocks
								n->invalidateCache();
							n->resetCacheStats();
						}
								
						while (myfile >> instruction >> std::hex >> address >> cycles) {
							switch (instruction) {
							case 'l':
								totalCycles += caches[0]->readAddress(address) + cycles;
								break;
							case 's':
								totalCycles += caches[0]->writeAddress(address) + cycles;
								break;
							default:
								std::cout << "Error reading trace file line." << std::endl;
								break;
							}
						}
						std::cout << "Simulation complete." << std::endl << std::endl;
					}
					else
						std::cout << "File could not be opened." << std::endl << std::endl;
				} while (!myfile.is_open());

				myfile.close();
				last_file = input;
			}
			break;

		case Choice::REPORT:
			if (totalCycles == 0)
				std::cout << "Cannot print a report until a simulation has been run." << std::endl << std::endl;
			else {
				std::cout << "Report for each memory level:" << std::endl;
				for (unsigned int i = 0; i < caches.size(); ++i)
					std::cout << "Cache level " << i + 1 << ":\n" << *caches[i] << std::endl;
				std::cout << left << setw(20) << "Total cycles: " << right << setw(10) << totalCycles << std::endl << std::endl;
			}
			break;

		case Choice::EXIT:
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