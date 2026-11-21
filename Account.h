#pragma once
#include <vector>
#include "Transaction.h"

class Account {
    private:
    int accountID;
    int customerID;
    long long balance;
    std::vector<Transaction> transactionHistory;
    void applyTransaction(int transactionID, TransactionType type, long long amount, int otherAccountID);
    public:
    Account();
    Account(int accountID, int customerID, long long balance,const std::vector<Transaction>& transactionHistory);
    ~Account();
    void deposit(long long amount, int transactionID);
    void withdraw(long long amount, int transactionID);
    void sendTransfer(long long amount, int transactionID, int toAccountID);
    void receiveTransfer(long long amount, int transactionID, int fromAccountID);
    int getAccountID() const;
    int getCustomerID() const;
    long long getBalance() const;
    const std::vector<Transaction>& getTransactionHistory() const;
    void setAccountID(int accountID);
    void setCustomerID(int customerID);
    void setBalance(long long balance);
    void setTransactionHistory(const std::vector<Transaction>& transactionHistory);
};
std::ostream& operator<<(std::ostream& os, const Account& a);
