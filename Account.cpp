#include "Account.h"
#include "Transaction.h"
#include <vector>
#include <stdexcept>
#include <climits>
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

void Account::applyTransaction(int transactionID, TransactionType type, long long amount, int otherAccountID) {
    if (amount <= 0) {
        throw invalid_argument("Amount cannot be less than or equal to 0.");
    }
    else if (type == TransactionType::NONE) {
        throw invalid_argument("Transaction type cannot be NONE.");
    }
    else if (type == TransactionType::WITHDRAW or type == TransactionType::TRANSFER_OUT) {
        if (amount > balance) {
            throw runtime_error("Amount cannot be greater than the balance.");
        }
    }
    else if (type == TransactionType::DEPOSIT or type == TransactionType::TRANSFER_IN) {
        if (balance > LLONG_MAX - amount ) {
            throw overflow_error("The amount will be greater than the balance limit.");
        }
    }
    else if ((type == TransactionType::TRANSFER_IN || type == TransactionType::TRANSFER_OUT) and (this->accountID == otherAccountID)) {
        throw invalid_argument("Cannot do a transfer to yourself.");
    }
    Transaction t = Transaction(transactionID,type, amount, otherAccountID);
    long long newBalance = this->balance;
    switch (type) {
        case TransactionType::WITHDRAW:
        case TransactionType::TRANSFER_OUT:
            newBalance = this->balance - amount;
            break;
        case TransactionType::DEPOSIT:
        case TransactionType::TRANSFER_IN:
            newBalance = this->balance + amount;
            break;
        default:
            throw invalid_argument("Invalid transaction type.");
            break;
    }
    transactionHistory.push_back(t);
    balance = newBalance;
}
void Account::deposit(long long amount, int transactionID) {
    applyTransaction(transactionID, TransactionType::DEPOSIT, amount, 0);
}
void Account::withdraw(long long amount, int transactionID) {
    applyTransaction(transactionID, TransactionType::WITHDRAW, amount, 0);
}

void Account::sendTransfer(long long amount, int transactionID, int toAccountID) {
    applyTransaction(transactionID, TransactionType::TRANSFER_OUT, amount, toAccountID);
}
void Account::receiveTransfer(long long amount, int transactionID, int fromAccountID) {
    applyTransaction(transactionID, TransactionType::TRANSFER_IN, amount, fromAccountID);
}
