#include <iostream>
#include <string>
#include "BankSystem.h"
#include "Menu.h"

using namespace std;

int main() {
    BankSystem bank({}, {}, 1, 1, 1, "bank_data.txt"); bank.load();
    Menu menu(bank);
    menu.run();
}
