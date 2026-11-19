#include "BankSystem.h"

#include <set>
#include <vector>
#include "string"
#include "Transaction.h"
#include "Customer.h"
#include "Account.h"
#include <set>
#include <stdexcept>
using namespace std;

BankSystem::BankSystem(const vector<Customer>& customers,const vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID) : customers(0), accounts(0), nextCustomerID(0), nextAccountID(0), nextTransactionID(0) {
    setCustomers(customers);
    setAccounts(accounts);
    setNextCustomerID(nextCustomerID);
    setNextAccountID(nextAccountID);
    setNextTransactionID(nextTransactionID);
}
BankSystem::BankSystem(const vector<Customer>& customers,const vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID, const string& filePath) : customers(0), accounts(0), nextCustomerID(0), nextAccountID(0), nextTransactionID(0), filePath("") {
    setCustomers(customers);
    setAccounts(accounts);
    setNextCustomerID(nextCustomerID);
    setNextAccountID(nextAccountID);
    setNextTransactionID(nextTransactionID);
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
        if (c.getID() <= 0) {
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
    set<int> seen;
    for (const Account& a : accounts) {
        if (a.getAccountID() <= 0) {
            throw invalid_argument("Account ID is invalid.");
        }
        bool inserted = seen.insert(a.getAccountID()).second;
        if (!inserted) {
            throw invalid_argument("Duplicate account ID.");
        }
        if (findCustomer(a.getCustomerID()) == nullptr) {
            throw invalid_argument("Customer ID doesnt exist.");
        }
        long long cmpBalance = 0;
        for (const Transaction& t : a.getTransactionHistory()) {
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
void BankSystem::setNextCustomerID(int customerID) {
    int maxCustomerID = 0;
    for (Customer& c : this->customers) {
        if (c.getID() > maxCustomerID) {
            maxCustomerID = c.getID();
        }
    }
    if (customerID <= maxCustomerID) {
        throw invalid_argument("Customer ID should not be greater than or equal the max ID.");
    }
    this->nextCustomerID = customerID;
}

void BankSystem::setNextAccountID(int accountID) {
    if (accountID <= nextAccountID) {
        throw invalid_argument("Account ID shouldn't be less than or equal to the biggest accountID.");
    }
    this->nextAccountID = accountID;
}
void BankSystem::setNextTransactionID(int transactionID) {
    if (transactionID <= nextTransactionID) {
        throw invalid_argument("Transaction ID shouldn't be less than or equal the biggest transaction ID.");
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
    for (Customer& c : this->customers) {
        if (c.getName() == name) {
            throw runtime_error("Customer name already taken.");
        }
    }
    Customer c = Customer(getNextCustomerID(),name, PIN);
    customers.push_back(c);
}

