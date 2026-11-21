#pragma once
#include <string>
#include "Transaction.h"

class Utils {
    public:
        static std::string formatAmount(long long amount);
        static long long parseAmount(const std::string& amount);
        static std::string toString(TransactionType type);
        static TransactionType parseTransactionType(const std::string& transactionType);
};
