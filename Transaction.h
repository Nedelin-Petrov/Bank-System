#pragma once
#include <ostream>
enum class TransactionType {
    DEPOSIT,
    WITHDRAW,
    TRANSFER_IN,
    TRANSFER_OUT,
    NONE
};
class Transaction {
    private:
        int transactionID;
        TransactionType type;
        long long amount;
        int otherAccountID;
        void validate() const;
    public:
        Transaction();
        Transaction(int id, TransactionType type, long long amount, int otherAccountID);
        ~Transaction();
        int getTransactionID() const;
        TransactionType getType() const;
        long long getAmount() const;
        int getOtherAccountID() const;
        void setTransactionID(int id);
        void setTransactionType(TransactionType type);
        void setAmount(long long amount);
        void setOtherAccountID(int otherAccountID);
};
std::ostream& operator<<(std::ostream& os, const Transaction& t);