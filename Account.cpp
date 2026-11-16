#include "Account.h"
#include "Transaction.h"
#include <vector>
#include <stdexcept>
using namespace std;

Account::Account() : accountID(0),customerID(0),balance(0) {

}
Account::Account(int accountID, int customerID, long long balance,const vector<Transaction>& transactionHistory) {
    setAccountID(accountID);
    setCustomerID(customerID);
    setBalance(balance);
    setTransactionHistory(transactionHistory);
}
Account::~Account() {}
int Account::getAccountID() const {
    return this->accountID;
}
int Account::getCustomerID() const {
    return this->customerID;
}
long long Account::getBalance() const {
    return this->balance;
}
const vector<Transaction>& Account::getTransactionHistory() const {
    return this->transactionHistory;
}
void Account::setAccountID(int accountID) {
    if (accountID <= 0) {
        throw invalid_argument("ID shouldn't be less than or equal to 0.");
    }
    else {
        this->accountID = accountID;
    }
}
void Account::setCustomerID(int customerID) {
    if (customerID <= 0) {
        throw invalid_argument("ID shouldn't be less than or equal to 0.");
    }
    else {
        this->customerID = customerID;
    }
}
void Account::setBalance(long long balance) {
    if (balance < 0) {
        throw invalid_argument("balance cannot be less than 0.");
    }
    else {
        this->balance = balance;
    }
}
void Account::setTransactionHistory(const std::vector<Transaction>& transactionHistory) {
    for (const Transaction& t : transactionHistory) {
        if (t.getType() == TransactionType::NONE) {
            throw invalid_argument("Transaction type shouldn't be NONE.");
        }
        else if (t.getTransactionID() <=0) {
            throw invalid_argument("Transaction IDs should not be less than 0.");
        }
    }
    this->transactionHistory = transactionHistory;
}

