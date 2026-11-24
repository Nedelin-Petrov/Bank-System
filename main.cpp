#include <iostream>
#include <string>
#include "BankSystem.h"
#include "Menu.h"

using namespace std;

int main() {
    try {
        BankSystem bank({}, {}, 1, 1, 1, "bank_data.txt"); bank.load();
        Menu menu(bank);
        menu.run();
    }
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
