#include "Transaction.h"
#include "Utils.h"
#include <stdexcept>
#include <string>
#include <ostream>
using namespace std;

Transaction::Transaction() : transactionID(0), type(TransactionType::NONE) ,amount(0), otherAccountID(0) {}
Transaction::Transaction(int id, TransactionType type, long long amount, int otherAccountID) {
    setTransactionID(id);
    setTransactionType(type);
    setAmount(amount);
    setOtherAccountID(otherAccountID);
    validate();
}
Transaction::~Transaction() {}

int Transaction::getTransactionID() const {
    return transactionID;
}
TransactionType Transaction::getType() const {
    return type;
}
long long Transaction::getAmount() const {
    return amount;
}
int Transaction::getOtherAccountID() const {
    return otherAccountID;
}
void Transaction::setTransactionID(int id) {
    if (id <= 0) {
        throw invalid_argument("ID shouldn't be less than or equal to 0.");
    }
    else {
        this->transactionID = id;
    }
}
void Transaction::setTransactionType(TransactionType type) {
    if (type == TransactionType::NONE) {
        throw invalid_argument("Transaction type cannot be NONE.");
    }
    else {
        this->type = type;
    }
}
void Transaction::setAmount(long long amount) {
    if (amount <= 0) {
        throw invalid_argument("Amount cannot be less than or equal to 0.");
    }
    else {
        this->amount = amount;
    }
}
void Transaction::setOtherAccountID(int otherAccountID) {
    if (otherAccountID < 0) {
        throw invalid_argument("OtherAccountID cannot be negative.");
    }
    else {
        this->otherAccountID = otherAccountID;
    }
}
void Transaction::validate() const {
    switch (type) {
        case TransactionType::DEPOSIT:
        case TransactionType::WITHDRAW:
            if (otherAccountID != 0) {
                throw invalid_argument("When performing deposit or withdraw transaction, otherAccountID has to be 0.");
            }
            break;
        case TransactionType::TRANSFER_IN:
        case TransactionType::TRANSFER_OUT:
            if (otherAccountID <= 0) {
                throw invalid_argument("When performing transfer_in or transfer_out, otherAccountID has to be a positive number from a real otherAccountID.");
            }
            break;
        case TransactionType::NONE:
            throw invalid_argument("Transaction type cannot be NONE.");
    }
}
std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    os <<  "#" << t.getTransactionID() <<"  " << Utils::toString(t.getType()) << "  "<< Utils::formatAmount(t.getAmount());
    if (t.getOtherAccountID() != 0) {
        if (t.getType() == TransactionType::TRANSFER_IN) {
            os << "  <- account " <<t.getOtherAccountID();
        }
        else {
           os << "  -> account " <<t.getOtherAccountID();
       }
    }
    return os;
}