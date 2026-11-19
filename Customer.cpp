#include "Customer.h"
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

Customer::Customer() : id(0),name(""), pin("") {}
Customer::Customer(int id, const string& name, const string& pin) {
    setID(id);
    setName(name);
    setPIN(pin);
}
Customer::~Customer() {}
string Customer::getName() const {
    return name;
}
int Customer::getID() const {
    return id;
}
string Customer::getPIN() const {
    return pin;
}
bool Customer::onlyDigits(const std::string& s) {
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}
void Customer::setID(int id) {
    if (id <= 0) {
        throw invalid_argument("ID shouldn't be less than or equal to 0.");
    }
    else {
        this->id = id;
    }
}
void Customer::setPIN(const string& pin) {
    if (pin.length() != 4) {
        throw invalid_argument("Pin must be exactly 4 digits.");
    }
    else if (!onlyDigits(pin)) {
        throw invalid_argument("Pin must contain digits only.");
    }
    else {
        this->pin = pin;
    }
}
void Customer::setName(const string& name) {
    if (name.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw invalid_argument("Name must not be empty.");
    }
    else if (name.length() > 128) {
        throw invalid_argument("Name must not exceed 128 characters.");
    }
    else {
        this->name = name;
    }
}

