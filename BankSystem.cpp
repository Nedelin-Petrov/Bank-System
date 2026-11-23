#include "BankSystem.h"
#include <set>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <algorithm>
#include "Transaction.h"
#include "Customer.h"
#include "Account.h"
#include "Utils.h"

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
                case TransactionType::NONE:
                    throw invalid_argument("Transaction type cannot be NONE.");
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
Account* BankSystem::findAccount(int accountID) {
    for (Account& a : this->accounts) {
        if (a.getAccountID() == accountID) {
            return &a;
        }
    }
    throw runtime_error("This account doesnt exist!");
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
    save();
    return resultID;
}
int BankSystem::takeTransactionId() {
    int resultID = this->nextTransactionID;
    nextTransactionID++;
    return resultID;
}
void BankSystem::checkOwnership(const Account &account, int customerID) const {
    if (account.getCustomerID() != customerID) {
        throw runtime_error("This account doesnt exist!");
    }
}
int BankSystem::openAccount(int customerID) {
    if (findCustomer(customerID) == nullptr) {
        throw runtime_error("Customer does not exist.");
    }
    Account a = Account(getNextAccountID(), customerID, 0, {});
    accounts.push_back(a);
    int resultID = a.getAccountID();
    nextAccountID++;
    save();
    return resultID;
}
vector<int> BankSystem::getAccountIds(int customerId) const {
    vector<int> resultsVector;
    for (const Account& a : this->accounts) {
        if (a.getCustomerID() == customerId) {
            resultsVector.push_back(a.getAccountID());
        }
    }
    return resultsVector;
}
long long BankSystem::getBalance(int customerID, int accountID) {
    const Account& a = *findAccount(accountID);
    checkOwnership(a, customerID);
    long long result = a.getBalance();
    return result;
}
const vector<Transaction> &BankSystem::getHistory(int customerId, int accountId) {
    const Account& a = *findAccount(accountId);
    checkOwnership(a, customerId);
    const vector<Transaction>& resultHistory = a.getTransactionHistory();
    return resultHistory;
}
void BankSystem::deposit(int customerId, int accountId, long long amount) {
    Account& a = *findAccount(accountId);
    checkOwnership(a, customerId);
    a.deposit(amount,takeTransactionId());
    save();
}
void BankSystem::withdraw(int customerId, int accountId, long long amount) {
    Account& a = *findAccount(accountId);
    checkOwnership(a, customerId);
    a.withdraw(amount,takeTransactionId());
    save();
}
void BankSystem::transfer(int customerId, int fromAccountId, int toAccountId, long long amount) {
    if (fromAccountId != toAccountId) {
        Account& aFrom = *findAccount(fromAccountId);
        checkOwnership(aFrom, customerId);
        Account& aTo = *findAccount(toAccountId);
        Account backupFrom= aFrom;
        Account backupTo = aTo;
        try {
            aFrom.sendTransfer(amount,takeTransactionId(),toAccountId);
            aTo.receiveTransfer(amount,takeTransactionId(),fromAccountId);
        }
        catch (...) {
            aFrom = backupFrom;
            aTo = backupTo;
            throw;
        }
        save();
    }
    else {
        throw runtime_error("Can't transfer to self");
    }
}

void BankSystem::save() const {
    ofstream out(filePath + ".tmp");
    if (!out.is_open()) {
        throw runtime_error("Error opening file for saving.");
    }
    out << "COUNTERS " << nextCustomerID << " " << nextAccountID << " " << nextTransactionID << "\n";
    for (const Customer& c : customers) {
         out << "CUSTOMER " << c.getID() << " " << c.getPIN() << " " << c.getName() << "\n";
    }
    for (const Account& a : accounts) {
        out << "ACCOUNT " << a.getAccountID() << " " << a.getCustomerID() << " " << a.getBalance() << "\n";
    }
    for (const Account& a : accounts) {
        for (const Transaction& t : a.getTransactionHistory()) {
            out << "TRANSACTION " << a.getAccountID() << " " << t.getTransactionID() << " " << Utils::toString(t.getType()) << " " << t.getAmount() << " " << t.getOtherAccountID() << "\n";
        }
    }
    out.close();
    if (out.fail()) {
        filesystem::remove(filePath + ".tmp");
        throw runtime_error("Error writing to file.");
    }
    filesystem::rename(filePath + ".tmp", filePath);
}
void BankSystem::load() {
    if (!filesystem::exists(filePath)) {
        return;
    }
    vector<Customer> loadedCustomers;
    vector<Account> loadedAccounts;
    int nextCustomerID = 0;
    int nextAccountID = 0;
    int nextTransactionID = 0;
    bool counterExists = false;
    string junk;
    vector<int> accountIDs;
    vector<int> customerIDs;
    vector<long long> balances;
    map<int, vector<Transaction>> histories;
    ifstream in(filePath);
    if (!in.is_open()) {
        throw runtime_error("Error opening file for reading.");
    }
    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;
        if (line.empty()) {
            continue;
        }
        istringstream iss(line);
        string type;
        iss >> type;
        if (type == "COUNTERS") {
            iss >> nextCustomerID >> nextAccountID >> nextTransactionID;
            if (iss.fail()) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Error reading from file.");
            }
            if (iss >> junk) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Junk on line.");
            }

            if (counterExists == true) {
                throw runtime_error("LineNo: "+to_string(lineNo)+" Duplicate counters.");
            }
            counterExists = true;
        }
        else if (type == "CUSTOMER") {
            int id;
            string pin, name;
            iss >> id >> pin >> std::ws;
            getline(iss, name);
            if (iss.fail()) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Error reading from file.");
            }
            if (iss >> junk) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Junk on line.");
            }
            try {
            loadedCustomers.push_back(Customer(id,name,pin));
            } catch (const exception& e) {
                throw runtime_error("Line " + to_string(lineNo) + ": " + e.what());
            }
        }
        else if (type == "ACCOUNT") {
            int accountID, customerID;
            long long balance;
            iss >> accountID >> customerID >> balance;
            if (iss.fail()) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Error reading from file.");
            }
            if (iss >> junk) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Junk on line.");
            }
            accountIDs.push_back(accountID);
            customerIDs.push_back(customerID);
            balances.push_back(balance);
        }
        else if (type == "TRANSACTION") {
            int accountID, transactionID, otherAccountID;
            string transcationType;
            long long amount;
            iss >> accountID >> transactionID >> transcationType >> amount >> otherAccountID;
            if (iss.fail()) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Error reading from file.");
            }
            if (iss >> junk) {
                throw runtime_error("LineNo: " + to_string(lineNo) + " Junk on line.");
            }
            try {
            Transaction t = Transaction(transactionID, Utils::parseTransactionType(transcationType),amount, otherAccountID);
            histories[accountID].push_back(move(t));
            }
            catch (const exception& e) {
                throw runtime_error("Line " + to_string(lineNo) + ": " + e.what());
            }
        }
        else {
            throw runtime_error("Invalid fileLine on lineNo: " + to_string(lineNo) + "\nLine message: "+ line);
        }
    }
    if (!counterExists) {
        if (!loadedCustomers.empty() || !accountIDs.empty() || !histories.empty()) {
            throw runtime_error("Corrupted file: missing COUNTERS line.");
        }
        return;
    }
    for (const auto& pair : histories) {
        if (find(accountIDs.begin(), accountIDs.end(), pair.first) == accountIDs.end()) {
            throw runtime_error("Corrupted file: transaction for unknown account " + to_string(pair.first) + ".");
        }
    }
    try {
        for (size_t i = 0; i < accountIDs.size(); i++) {
            loadedAccounts.push_back( Account(accountIDs[i], customerIDs[i], balances[i], histories[accountIDs[i]]) );
        }

        BankSystem tmp(loadedCustomers, loadedAccounts, nextCustomerID, nextAccountID, nextTransactionID, filePath);
        *this = tmp;
    } catch (const exception& e) {
        throw runtime_error(string("Corrupted file: ") + e.what());
    }
}