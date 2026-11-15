#pragma once
#include <string>

class Customer {
    private:
        int id;
        std::string name;
        std::string pin;
    public:
        Customer();
        Customer(int id, const std::string& name, const std::string& pin);
        ~Customer();
        std::string getName() const;
        int getID() const;
        std::string getPIN() const;
        void setName(const std::string& name);
        void setPIN(const std::string& pin);
        void setID(int id);
        static bool onlyDigits(const std::string& s);
};

