#pragma once
enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFERIN,
    TRANSFEROUT
};
class Transaction {
    private:
        int TransactionID;
        TransactionType type;
        long amount;
        int otherTransactionID;
    public:
        Transaction();
        Transaction(int id, TransactionType type, int amount, int otherTransactionID);
        ~Transaction();
        int getTransactionID();
        TransactionType getType();
        long getAmount();
        int getOtherTransactionID();
        void setTransactionID(int id);
        void setTransactionType(TransactionType type);
        void setAmount(long amount);
        void setOtherTransactionID(int otherTransactionID);
};