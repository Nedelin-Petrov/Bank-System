#pragma once
#include "BankSystem.h"
#include <string>
class Menu {
    private:
        BankSystem& bank;
        std::string readLine(const std::string& prompt);
        int readInt(const std::string& prompt);
        long long readAmount(const std::string& prompt);
    public:
        Menu(BankSystem& bank);
        void run();
        void choiceLogicRegisterText();
        void registerFlow();
        void loginFlow();
};