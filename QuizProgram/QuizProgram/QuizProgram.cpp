#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include "Question.h"

constexpr auto QUIT = '7';
int main()
{
	std::vector<Question> questions;
	std::string selection;
	std::fstream fs;
	unsigned int score = 0;

	while (selection[0] != QUIT) {
		do {
			std::cout << "=== Quiz Program ===\n1. Initialise\n2. Save to disk\n3. Read from disk" << std::endl <<
				"4. Create new question\n5. Take the quiz\n6. Print report\n7. Exit program" << std::endl <<
				"Input: ";
			getline(std::cin, selection);
		} while (selection.find_first_of("1234567") && std::cout << "Invalid input." << std::endl);

		switch (selection[0]) {
		case '1': // Init
			std::cout << "Init test." << std::endl; // Not sure if init clears memory or clears the file so I didn't write anything.
			break;

		case '2': // Save
			// Read and write are being done in a naive way that is not checking for duplicates.
			fs.open("quiz.txt", std::fstream::in | std::fstream::out | std::fstream::app);
			if (fs.is_open()) {
				for (const auto& q : questions)
					fs << q << std::endl;
				fs.close();
			}
			else
				std::cout << "Could not open file." << std::endl;
			break;

		case '3': // Load
			fs.open("quiz.txt", std::fstream::in | std::fstream::out | std::fstream::app);
			if (fs.is_open()) {
				questions.clear();
				Question q;
				while (fs >> q) {
					questions.push_back(q);
				}
				fs.close();
			}
			std::cout << "Read test." << std::endl;
			break;

		case '4': // Make new question
		{
			std::string question, line;
			std::vector<std::string> answers;
			unsigned char correct_answer;
			
			std::cout << "=== Creating new quiz ===\nEnter question: ";
			getline(std::cin, question);

			std::cout << "Enter 4 alternative answers:" << std::endl;
			while (answers.size() < 4) {
				std::cout << "Answer " << answers.size() + 1 << ": ";
				getline(std::cin, line);
				answers.push_back(line);
			}

			questions.emplace_back(question, answers); //Correct answer is decided and written to question inside play_question()
			questions.back().play_question();
			do {
				std::cout << "Is this question okay (y/n)? ";
				getline(std::cin, line);
				if (line[0] == 'n') {
					questions.pop_back();
					std::cout << "Deleting question." << std::endl;
				}
			} while (line.find_first_of("ny") && std::cout << "Invalid input" << std::endl);

		}
		break;

		case '5': // Take quiz
			score = 0;
			for (auto& q : questions)
				score += q.play_question();
			break;

		case '6': // Print report
			for (const auto& q : questions)
				std::cout << q.get_question() << std::endl;
			break;

		case '7': // Close program
			std::cout << "Closing program." << std::endl;
			break;
		}
	}
}