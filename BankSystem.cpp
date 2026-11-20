#include "BankSystem.h"

#include <set>
#include <vector>
#include <string>
#include "Transaction.h"
#include "Customer.h"
#include "Account.h"
#include <set>
#include <stdexcept>
using namespace std;

BankSystem::BankSystem(const vector<Customer>& customers,const vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID) : nextCustomerID(0), nextAccountID(0), nextTransactionID(0) {
    setNextCustomerID(nextCustomerID);
    setNextAccountID(nextAccountID);
    setNextTransactionID(nextTransactionID);
    setCustomers(customers);
    setAccounts(accounts);
}
BankSystem::BankSystem(const vector<Customer>& customers,const vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID, const string& filePath) : nextCustomerID(0), nextAccountID(0), nextTransactionID(0), filePath("bank_data.txt") {
    setNextCustomerID(nextCustomerID);
    setNextAccountID(nextAccountID);
    setNextTransactionID(nextTransactionID);
    setCustomers(customers);
    setAccounts(accounts);
    setFilePath(filePath);
}
BankSystem::~BankSystem() {}
const vector<Account> &BankSystem::getAccounts() const {
    return this->accounts;
}
const vector<Customer> &BankSystem::getCustomers() const {
    return this->customers;
}
int BankSystem::getNextCustomerID() const {
    return this->nextCustomerID;
}
int BankSystem::getNextAccountID() const {
    return this->nextAccountID;
}
int BankSystem::getNextTransactionID() const {
    return this->nextTransactionID;
}
const string& BankSystem::getFilePath() const {
    return this->filePath;
}
void BankSystem::setCustomers(const vector<Customer>& customers) {
    set<int> seen;
    for (const Customer& c : customers) {
        if (c.getID() <= 0 || c.getID() >= nextCustomerID) {
            throw invalid_argument("Customer ID is invalid.");
        }
        bool inserted = seen.insert(c.getID()).second;
        if (!inserted) {
            throw invalid_argument("Duplicate customer ID.");
        }
    }
    this->customers = customers;
}
void BankSystem::setAccounts(const vector<Account>& accounts) {
    set<int> seenAcc;
    set<int> seenTra;
    for (const Account& a : accounts) {
        if (a.getAccountID() <= 0 || a.getAccountID() >= nextAccountID) {
            throw invalid_argument("Account ID is invalid.");
        }
        bool insertedAcc = seenAcc.insert(a.getAccountID()).second;
        if (!insertedAcc) {
            throw invalid_argument("Duplicate account ID.");

        }
        if (findCustomer(a.getCustomerID()) == nullptr) {
            throw invalid_argument("Customer ID doesnt exist.");
        }
        long long cmpBalance = 0;
        for (const Transaction& t : a.getTransactionHistory()) {
            if (t.getTransactionID() <= 0 || t.getTransactionID() >= nextTransactionID) {
                throw invalid_argument("Transaction ID is invalid.");
            }
            bool insertedTra = seenTra.insert(t.getTransactionID()).second;
            if (!insertedTra) {
                throw invalid_argument("Duplicate Transaction ID.");
            }
            switch (t.getType()) {
                case TransactionType::WITHDRAW:
                case TransactionType::TRANSFER_OUT:
                    cmpBalance -= t.getAmount();
                    break;
                case TransactionType::DEPOSIT:
                case TransactionType::TRANSFER_IN:
                    cmpBalance += t.getAmount();
                    break;
            }
        }
        if (cmpBalance != a.getBalance()) {
            throw invalid_argument("Balance isn't the same as the account balance.");
        }
    }
    this->accounts = accounts;
}

int BankSystem::maxCustomerID() const {
    int maxCustomerID = 0;
    for (const Customer& c : this->customers) {
        if (c.getID() > maxCustomerID) {
            maxCustomerID = c.getID();
        }
    }
    return maxCustomerID;
}
int BankSystem::maxAccountID() const {
    int maxAccountID = 0;
    for (const Account& a : this->accounts) {
        if (a.getAccountID() > maxAccountID) {
            maxAccountID = a.getAccountID();
        }
    }
    return maxAccountID;
}
int BankSystem::maxTransactionID() const {
    int maxTransactionID = 0;
    for (const Account& a : accounts) {
        for (const Transaction& t : a.getTransactionHistory()) {
            if (t.getTransactionID() > maxTransactionID) {
                maxTransactionID = t.getTransactionID();
            }
        }
    }
    return maxTransactionID;
}
void BankSystem::setNextCustomerID(int customerID) {
    int maxID = maxCustomerID();
    if (customerID <= maxID) {
        throw invalid_argument("Customer ID should be greater than the max ID.");
    }
    this->nextCustomerID = customerID;
}

void BankSystem::setNextAccountID(int accountID) {
    int maxID = maxAccountID();
    if (accountID <= maxID) {
        throw invalid_argument("Account ID should be greater than the max ID");
    }
    this->nextAccountID = accountID;
}
void BankSystem::setNextTransactionID(int transactionID) {
    int maxID = maxTransactionID();
    if (transactionID <= maxID) {
        throw invalid_argument("Transaction ID should be greater than the max ID.");
    }
    this->nextTransactionID = transactionID;
}
void BankSystem::setFilePath(const string& filePath) {
    if (filePath.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw invalid_argument("FilePath must not be empty.");
    }
    this->filePath = filePath;
}
Customer* BankSystem::findCustomer(int customerID) {
    for (Customer& c : this->customers) {
        if (c.getID() == customerID) {
            return &c;
        }
    }
    return nullptr;
}
int BankSystem::login(int customerID, const std::string& PIN) {
    for (const Customer& c : this->customers) {
        if ((c.getID() == customerID) && (c.getPIN() == PIN)) {
            return c.getID();
        }
    }
    throw runtime_error("Wrong credentials.");
}
int BankSystem::registerCustomer(const std::string &name, const std::string &PIN) {
    Customer c = Customer(getNextCustomerID(),name, PIN);
    customers.push_back(c);
    int resultID = c.getID();
    nextCustomerID++;
    return resultID;
}

