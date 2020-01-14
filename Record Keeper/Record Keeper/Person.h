#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <iostream>

using namespace std;

class Person
{
    friend std::ostream& operator<<(std::ostream& out, const Person& person) {
        return out << person.name << " " << person.email << " " << person.number << " " << person.city << " " << person.relative;
    }
    friend std::istream& operator>>(std::istream& is, Person& person) {
        return is >> person.name >> person.email >> person.number >> person.city >> person.relative;     
    }

private:
    std::string name, email, number, city;
    unsigned int relative; // Treating relative as a group ID.
    bool written;

public:
    Person(const std::string name_ = "", const std::string email_ = "", const std::string number_ = "", const std::string city_ = "") :
        name{ name_ }, email{ email_ }, number{ number_ }, city{ city_ }
    {
        written = false;
        relative = 0;
    };

    const std::string& get_name() const {
        return name;
    }

    const std::string& get_city() const {
        return city;
    }

    void set_relative(const unsigned int relative_) {
        relative = relative_;
    }

    unsigned int get_relative() {
        return relative;
    }

    void set_written(const bool written_) {
        written = written_;
    }

    const bool is_written() const {
        return written;
    }

    static std::optional<Person> new_record() {
        Person person;
        std::cout << "=== Creating new record ===\nEnter quit at any time to quit." << std::endl;
        std::cout << "Enter person's name: "; getline(std::cin, person.name); if (person.name == "quit") return nullopt;
        std::cout << "Enter person's email: "; getline(std::cin, person.email); if (person.email == "quit") return nullopt;
        std::cout << "Enter person's phone number: "; getline(std::cin, person.number); if (person.number == "quit") return nullopt;
        std::cout << "Enter person's city: "; getline(std::cin, person.city); if (person.city == "quit") return nullopt;
        return person;
    }
};

