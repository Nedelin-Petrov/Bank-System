#pragma once
#include <vector>
#include "Transaction.h"

class Account {
    private:
    int accountID;
    int customerID;
    long long balance;
    std::vector<Transaction> transactionHistory;
    public:
    Account();
    Account(int accountID, int customerID, long long balance,const std::vector<Transaction>& transactionHistory);
    ~Account();
    int getAccountID() const;
    int getCustomerID() const;
    long long getBalance() const;
    const std::vector<Transaction>& getTransactionHistory() const;
    void setAccountID(int accountID);
    void setCustomerID(int customerID);
    void setBalance(long long balance);
    void setTransactionHistory(const std::vector<Transaction>& transactionHistory);
};
