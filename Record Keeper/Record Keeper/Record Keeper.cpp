// Record Keeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm>

#include "Person.h"

using namespace std;

enum class Selection { INIT = 1, SAVE, LOAD, NEW, DELETE, SEARCH, REPORT, QUIT };

int main()
{
    std::vector<vector<Person>> people; //Vector of vector of persons, where each vector of Person is a group of relatives. So overall it is a vector of relatives.
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

        selection = static_cast<Selection>(input[0] - '0');
        switch (selection) {
        case Selection::INIT:
            // Not sure if init clears memory or clears the file but I just made it do both.
            do {
                std::cout << "This will clear all records in memory and in file.\nAre you sure you wish to continue (y/n)? ";
                getline(std::cin, input);
            } while (input.find_first_of("yn") && std::cout << "Invalid input." << std::endl);

            switch (input[0]) {
            case 'y':
                std::cout << "Clearing all records..." << std::endl;
                people.clear();
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
            fs.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::app);
            if (fs.is_open()) {
                for (auto& relatives : people) {
                    for (auto& person : relatives)
                        if (!person.is_written()) {
                            person.set_written(true);
                            fs << person << std::endl;
                        }
                }
                fs.close();
            }
            else
                std::cout << "Could not open file." << std::endl;
            break;
        case Selection::LOAD:
            fs.open("record.txt", std::fstream::in);
            if (fs.is_open()) {
                // Get rid of all the people who have already been written to file. Too lazy to do it in a smarter way.
                people.erase(std::remove_if(people.begin(), people.end(), [](auto& relatives) {
                    relatives.erase(std::remove_if(relatives.begin(), relatives.end(), [](auto& person) {
                        return person.is_written();
                        }), relatives.end());
                    return relatives.empty();
                    }), people.end());

                Person person;
                while (fs >> person) {
                    person.set_written(true); // Must be written because it has come from the file.

                    // It should be required that the file is read from if it's not empty.
                    // If it's not empty, the below if statement will find the highest relative ID so far in the file.
                    if (person.get_relative() >= relative_id)
                        relative_id = person.get_relative() + 1;

                    auto iter = find_if(people.begin(), people.end(), [&person](auto& relatives) {
                        return relatives[0].get_relative() == person.get_relative();
                        });

                    if (iter != people.end())
                        iter->push_back(person);
                    else {
                        people.push_back(vector<Person>());
                        people.back().push_back(person);
                    }
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
                bool found_relative = false;
                std::cout << "If this person has a relative already recorded in the phone book, enter their name now." << std::endl <<
                    "If not, just type quit.\nInput: ";
                getline(std::cin, relative_name);
                if (relative_name != "quit") {
                    auto relatives = people.begin();
                    for (relatives; relatives != people.end() && input[0] != 'y'; ++relatives) {
                        for (auto& relative : *relatives) {
                            if (relative.get_name() == relative_name) {
                                do {
                                    std::cout << relative << std::endl << "Is this the person you meant (y/n)? ";
                                    getline(std::cin, input);
                                } while (input.find_first_of("yn") && std::cout << "Invalid input." << std::endl);
                            }
                        }
                    }

                    if (input[0] != 'y') {
                        std::cout << "No relatives were found." << std::endl;
                        //This pushes back a new vector of relatives and then pushes back the newly created person into this vector.
                        people.push_back(vector<Person>());
                        person.value().set_relative(relative_id++);
                        people.back().push_back(person.value());
                    }
                    else {
                        --relatives; //Needs to be decremented because of how the for loop works.
                        person.value().set_relative((*relatives)[0].get_relative());
                        relatives->push_back(person.value());
                    }
                }
                else {
                    people.push_back(vector<Person>());
                    person.value().set_relative(relative_id++);
                    people.back().push_back(person.value());
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
            people.erase(std::remove_if(people.begin(), people.end(), [&input](auto& relatives) {
                relatives.erase(std::remove_if(relatives.begin(), relatives.end(), [&input](auto& person) {
                    // Short circuit evaluation prevents the print to std::cout from happening if the first condition is false.
                    return (person.get_name() == input) && std::cout << input << " removed." << std::endl;
                    }), relatives.end());
                return relatives.empty();
                }), people.end());

            break;
        case Selection::SEARCH:
            std::cout << "Enter city name to search for: ";
            getline(std::cin, input);
            for (auto& relatives : people) {
                for (auto& person : relatives) {
                    if (person.get_city() == input)
                        std::cout << person << std::endl;
                }
            }
            break;
        case Selection::REPORT:
            std::cout << "Printing all records in phonebook. Relatives are separated by newline." << std::endl;
            for (auto& relatives : people) {
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
