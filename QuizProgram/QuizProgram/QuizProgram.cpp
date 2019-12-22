#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "Question.h"

enum class Selection { INIT = 1, SAVE, LOAD, NEW, QUIZ, REPORT, QUIT };

int main()
{
	std::vector<Question> questions;
	std::string line;
	std::fstream fs;
	Selection selection = Selection::INIT;

	unsigned int score = 0;

	while (selection != Selection::QUIT) {
		do {
			std::cout << "=== Quiz Program ===\n1. Initialise\n2. Save to disk\n3. Read from disk" << std::endl <<
				"4. Create new question\n5. Take the quiz\n6. Print report\n7. Exit program" << std::endl <<
				"Input: ";
			getline(std::cin, line);
		} while (line.find_first_of("1234567") && std::cout << "Invalid input." << std::endl);

		selection = static_cast<Selection>(line[0] - '0');

		switch (selection) {
		case Selection::INIT:
			std::cout << "Init test." << std::endl; // Not sure if init clears memory or clears the file so I didn't write anything.

			break;

		case Selection::SAVE:
			fs.open("quiz.txt", std::fstream::in | std::fstream::out | std::fstream::app);
			if (fs.is_open()) {
				for (auto& q : questions)
					if (!q.is_written()) {
						q.set_written(true);
						fs << q << std::endl;
					}
				fs.close();
			}
			else
				std::cout << "Could not open file." << std::endl;

			break;

		case Selection::LOAD:
			fs.open("quiz.txt", std::fstream::in | std::fstream::out | std::fstream::app);
			if (fs.is_open()) {
				// Will remove elements from the vector if they're already written because they're about to be read from file again.
				questions.erase(std::remove_if(questions.begin(), questions.end(), [](auto& q) { return q.is_written(); }), questions.end());
				Question q;
				while (fs >> q) {
					q.set_written(true);
					questions.push_back(q);
				}
				fs.close();
			}
			std::cout << "Read test." << std::endl;

			break;

		case Selection::NEW:
		{
			std::string question;
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

		case Selection::QUIZ:
			score = 0;
			if (questions.empty() && std::cout << "There are no questions. Try creating some first or loading some from the file." << std::endl)
				break;

			for (auto& q : questions)
				score += q.play_question();

			break;

		case Selection::REPORT:
			if (questions.empty() && std::cout << "There are no questions. Try creating some first or loading some from the file." << std::endl)
				break;

			for (const auto& q : questions)
				std::cout << q.get_question() << std::endl;

			break;

		case Selection::QUIT:
			std::cout << "Closing program." << std::endl;

			break;
		}
	}
}