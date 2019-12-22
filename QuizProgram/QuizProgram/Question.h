#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>

#define UNINIT 0
class Question
{
	
public:
	Question(std::string question = "Empty", std::vector<std::string> answers = {4, "Empty"}, unsigned char correct_answer = UNINIT):
		m_question {question},
		m_answers {answers},
		m_correct_answer {correct_answer}
	{};

	const std::string get_question() const {
		return m_question;
	}
	unsigned const char play_question() {
		// Will return true (1) or false (0) so scores can be tallied easily.
		std::string answer;
		std::shuffle(m_answers.begin(), m_answers.end(), std::mt19937{ std::random_device{}() });
		std::cout << m_question << std::endl; // Print question
		for (int i = 0; i < m_answers.size(); ++i) // Print possible answers
			std::cout << i + 1 << ") " << m_answers[i] << std::endl;

		do {
			std::cout << "Enter your answer: ";
			getline(std::cin, answer);
		} while (answer.find_first_of("1234") && std::cout << "Invalid input." << std::endl);

		if (m_correct_answer == UNINIT)
			m_correct_answer = answer[0];

		return m_correct_answer == answer[0];
	}

	friend std::ostream& operator<<(std::ostream& out, const Question& q) {
		out << q.m_question << " ";
		std::copy(q.m_answers.begin(), q.m_answers.end(), std::ostream_iterator<std::string>(out, " "));
		out << q.m_correct_answer << std::endl;
		return out;
	}

	friend std::istream& operator>>(std::istream& is, Question& q) {
		is >> q.m_question;
		for (auto& a : q.m_answers)
			is >> a;
		is >> q.m_correct_answer;
		return is;
	}
private:
	std::string m_question;
	std::vector<std::string> m_answers;
	unsigned char m_correct_answer;
};

