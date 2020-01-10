#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>

#define UNINIT "0"
class Question
{
	
public:
	Question(std::string question = R"(Empty)"	, std::vector<std::string> answers = {4, "Empty"}, std::string correct_answer = UNINIT, bool written = false):
		m_question {question},
		m_answers {answers},
		m_correct_answer {correct_answer},
		m_written {written}
	{};

	const std::string get_question() const {
		return m_question;
	}
	unsigned const char play_question() {
		// Will return true (1) or false (0) so scores can be tallied easily.
		std::string answer;
		std::shuffle(m_answers.begin(), m_answers.end(), std::mt19937{ std::random_device{}() });
		std::cout << m_question << std::endl; // Print question
		for (unsigned int i = 0; i < m_answers.size(); ++i) // Print possible answers
			std::cout << i + 1 << ") " << m_answers[i] << std::endl;

		do {
			std::cout << "Enter your answer: ";
			getline(std::cin, answer);
		} while (answer.find_first_of("1234") && std::cout << "Invalid input." << std::endl);

		if (m_correct_answer == UNINIT)
			m_correct_answer = m_answers[answer[0] - '0' - 1];

		// Having fun making some unreadable code using ternary operator and short circuit evaluation.
		return m_correct_answer == m_answers[answer[0] - '0' - 1] ? (true && std::cout << "Correct!" << std::endl) :  (std::cout << "Incorrect!" << std::endl && false);
	}

	const bool is_written() const {
		return m_written;
	}

	void set_written(const bool written) {
		m_written = written;
	}

	friend std::ostream& operator<<(std::ostream& out, const Question& q) {
		out << q.m_question << std::endl;
		std::copy(q.m_answers.begin(), q.m_answers.end(), std::ostream_iterator<std::string>(out, "\n"));
		out << q.m_correct_answer << std::endl;
		return out;
	}

	friend std::istream& operator>>(std::istream& is, Question& q) {
		getline(is, q.m_question);
		for (auto& a : q.m_answers)
			getline(is, a);
		getline(is, q.m_correct_answer);
		return is;
	}
private:
	std::string m_question;
	std::vector<std::string> m_answers;
	std::string m_correct_answer;
	bool m_written;
};

