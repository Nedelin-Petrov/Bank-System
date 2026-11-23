#include "Menu.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <stdexcept>

Menu::Menu(BankSystem& bank) : bank(bank) {}
void Menu::run() {
    std::cout << "----------------------\n";
    std::cout << "Welcome to the Bank!\n";
    std::cout << "----------------------\n";
    while (true) {
        choiceLogicRegisterText();
        char choice;
        std::cin >> choice;
        switch (choice) {
            case '1': {

            }
            break;
            case '2': {

            }
            break;
            case '0': {

            }
            break;
            default:
                throw std::invalid_argument("Непозната опция");
        }
    }

}
std::string Menu::readLine(const std::string &prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("Input closed");
    }
    return line;
}
int Menu::readInt(const std::string &prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            size_t pos;
            int value = std::stoi(line,&pos);
            if (pos != line.size()) {
                throw std::invalid_argument("Extra characters");
            }
            return value;
        }
        catch (const std::logic_error &) {
            std::cout << "Please enter a whole number.\n";
        }
    }
}
long long Menu::readAmount(const std::string &prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            return Utils::parseAmount(line);
        }
        catch (const std::exception& e) {
            std::cout << "Invalid amount: " << e.what() << "\n";
        }
    }
}
void Menu::choiceLogicRegisterText() {
    std::cout<<"1. Register\n";
    std::cout<<"2. Login\n";
    std::cout<<"0. Quit\n";
    std::cout<<"Choose action: ";
}

