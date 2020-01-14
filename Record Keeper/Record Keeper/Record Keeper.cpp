#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Person.h"

using namespace std;
using Relatives = vector<Person>;
enum class Selection { INIT = 1, SAVE, LOAD, NEW, DELETE, SEARCH, REPORT, QUIT };

int main()
{
    std::vector<Relatives> phonebook; //Vector of vector of persons, where each vector of Person is a group of relatives. So overall it is a vector of relatives.
    std::string input;
    std::fstream fs;
    unsigned int relative_id = 0;
    Selection selection = Selection::INIT;
    bool read_from_file = false;

    fs.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (fs.is_open()) {
        if (fs.peek() == std::ifstream::traits_type::eof())
            read_from_file = true; //Because the file is empty.
        fs.close();
    }

    while (selection != Selection::QUIT) {
        do {
            std::cout << "=== Record Keeper ===\n1. Initialise\n2. Save to disk\n3. Read from disk" << std::endl <<
                "4. Add new person\n5. Remove a person\n6. Find people in a city\n7. Print all contract info\n8. Exit program" << std::endl <<
                "Input: ";
            getline(std::cin, input);
        } while (input.find_first_of("12345678") && std::cout << "Invalid input." << std::endl);

        switch (selection = static_cast<Selection>(input[0] - '0')) {
        case Selection::INIT:
            do {
                std::cout << "This will clear all records in memory and in file.\nAre you sure you wish to continue (y/n)? ";
                getline(std::cin, input);
            } while (input.find_first_of("yn") && std::cout << "Invalid input." << std::endl);

            switch (input[0]) {
            case 'y':
                std::cout << "Clearing all records..." << std::endl;
                phonebook.clear();
                fs.open("record.txt", std::ofstream::out | std::ofstream::trunc);
                fs.close();
                break;

            case 'n':
                std::cout << "Returning to main menu." << std::endl;
                break;

            default:
                std::cout << "Invalid input." << std::endl;
                break;
            }
            break;

        case Selection::SAVE:
            // Saves everybody to file who hasn't already been saved.
            fs.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::app);
            if (fs.is_open()) {
                for (auto& relatives : phonebook)
                    for (auto& person : relatives)
                        if (!person.is_written()) {
                            person.set_written(true);
                            fs << person << std::endl;
                        }
                fs.close();
            }
            else
                std::cout << "Could not open file." << std::endl;
            break;

        case Selection::LOAD:
            fs.open("record.txt", std::fstream::in);
            if (fs.is_open()) {
                // Gets rid of all the people who have already been written to file. Too lazy to do it in a smarter way.
                phonebook.erase(std::remove_if(phonebook.begin(), phonebook.end(), [](auto& relatives) {
                    relatives.erase(std::remove_if(relatives.begin(), relatives.end(), [](auto& person) {
                        return person.is_written();
                        }), relatives.end());
                    return relatives.empty();
                    }), phonebook.end());

                Person person;
                while (fs >> person) {
                    person.set_written(true);

                    if (person.get_relative() >= relative_id)
                        relative_id = person.get_relative() + 1;

                    auto relatives_iter = find_if(phonebook.begin(), phonebook.end(), [&person](auto& relatives) {
                        return relatives.front().get_relative() == person.get_relative();
                        });

                    if (relatives_iter != phonebook.end())
                        relatives_iter->push_back(person);
                    else
                        phonebook.emplace_back(Relatives{ person });
                }
                fs.close();
                read_from_file = true;
            }
            break;

        case Selection::NEW:
            if (!read_from_file && std::cout << "Please load phonebook from file before adding new persons." << std::endl)
                break;

            if (auto person = Person::new_record()) {
                std::string relative_name;
                std::cout << "If this person has a relative already recorded in the phone book, enter their name now.\n"
                    "If not, just type quit.\nInput: ";
                getline(std::cin, relative_name);

                if (relative_name == "quit") {
                    person.value().set_relative(relative_id++);
                    phonebook.emplace_back(Relatives{ person.value() });
                }
                else {
                    auto relatives_iter = find_if(phonebook.begin(), phonebook.end(), [&relative_name, &input](auto& relatives) {
                        return relatives.end() != find_if(relatives.begin(), relatives.end(), [&relative_name, &input](auto& relative) {
                            if (relative.get_name() == relative_name) {
                                do {
                                    std::cout << relative << std::endl << "Is this the person you meant (y/n)? ";
                                    getline(std::cin, input);
                                } while (input.find_first_of("yn") && std::cout << "Invalid input." << std::endl);

                                return input[0] == 'y';
                            }
                            });
                        });

                    if (relatives_iter != phonebook.end()) {       
                        person.value().set_relative(relatives_iter->front().get_relative());
                        relatives_iter->push_back(person.value());
                    }
                    else {
                        std::cout << "No relatives were found." << std::endl;
                        person.value().set_relative(relative_id++);
                        phonebook.emplace_back(Relatives{ person.value() });
                    }
                }
            }
            break;

        case Selection::DELETE:
            std::cout << "Enter name of person to remove from record: ";
            getline(std::cin, input);

            /* Below are two sets Erase + Remove_if algorithms. Remove_if moves all elements where the (lambda) function
             * returns true and returns a pointer to the first removed item in the vector. Erase takes this returned pointer and erases
             * all elements between it and the vector .end() pointer by unallocating memory. The inner Erase + Remove_if
             * will delete the person searched for. The outer Erase + Remove_if will delete a vector if it is empty
             * after a person has been removed. */
            phonebook.erase(std::remove_if(phonebook.begin(), phonebook.end(), [&input](auto& relatives) {
                relatives.erase(std::remove_if(relatives.begin(), relatives.end(), [&input](auto& person) {
                    // Short circuit evaluation prevents the print to std::cout from happening if the first condition is false.
                    return (person.get_name() == input) && std::cout << input << " removed." << std::endl;
                    }), relatives.end());
                return relatives.empty();
                }), phonebook.end());
            break;

        case Selection::SEARCH:
            std::cout << "Enter city name to search for: ";
            getline(std::cin, input);
            for (auto& relatives : phonebook)
                for (auto& person : relatives)
                    if (person.get_city() == input)
                        std::cout << person << std::endl;
            break;

        case Selection::REPORT:
            std::cout << "Printing all records in phonebook. Relatives are separated by newline." << std::endl;
            for (auto& relatives : phonebook) {
                for (auto& person : relatives)
                    std::cout << person << std::endl;
                std::cout << std::endl;
            }
            break;

        case Selection::QUIT:
            std::cout << "Exiting program." << std::endl;
            break;

        default:
            std::cout << "Invalid input." << std::endl;
            break;
        }
    }
}
