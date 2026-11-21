#include "Utils.h"
#include "Transaction.h"
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <climits>
using namespace std;

string Utils::formatAmount(long long amount) {
    long long cents = abs(amount) % 100;
    long long euro = abs(amount) / 100;
    string resultCents;
    string resultEuro;
    if (cents < 10) {
        resultCents = "0"+to_string(cents);
    }
    else {
        resultCents = to_string(cents);
    }
    resultEuro = to_string(euro);
    string result = resultEuro + "." + resultCents;
    return result;
}
long long Utils::parseAmount(const std::string& amount) {
    if (amount.empty() == true) {
        throw invalid_argument("Empty amount");
    }

    string euroPart;
    string centPart;
    size_t dotPos = amount.find('.');
    if (dotPos == string::npos) {
        euroPart = amount;
        centPart = "";
    }
    else {
        euroPart = amount.substr(0, dotPos);
        centPart = amount.substr(dotPos + 1);
    }
    if (euroPart.empty() == true) {
        throw std::invalid_argument("Amount is empty");
    }
    for (char c : euroPart) {
        if ( c < '0' || c > '9') {
            throw std::invalid_argument("Amount can only have numbers in it!");
        }
    }
    if (dotPos != std::string::npos) {
        if (centPart.empty() == true) {
            throw std::invalid_argument("Amount is empty");
        }
        if (centPart.size() > 2) {
            throw std::invalid_argument("Invalid amount size");
        }
        for (char c : centPart) {
            if (c < '0' || c > '9') {
                throw std::invalid_argument("Amount can only have numbers in it!");
            }
        }
    }
    long long euros = 0;
    long long cents = 0;
    for (char c : euroPart) {
        int d = c - '0';
        if (euros > (LLONG_MAX - d) / 10) {
            throw overflow_error("Cant go over LLONG_MAX");
        }
        euros = euros * 10 + d;
    }
    for (char c : centPart) {
        int d = c - '0';
        if (cents > (LLONG_MAX - d) / 10) {
            throw overflow_error("Cant go over LLONG_MAX");
        }
        cents = cents * 10 + d;
    }
    if (centPart.size() == 1) {
        cents *= 10;
    }
    if (euros > (LLONG_MAX - cents) / 100) {
        throw overflow_error("Cant go over LLONG_MAX");
    }
    long long result = euros*100 + cents;
    return result;
}
string Utils::toString(TransactionType type) {
    switch (type) {
        case TransactionType::DEPOSIT:
            return "DEPOSIT";
            break;
        case TransactionType::WITHDRAW:
            return "WITHDRAW";
            break;
        case TransactionType::TRANSFER_IN:
            return "TRANSFER_IN";
            break;
        case TransactionType::TRANSFER_OUT:
            return "TRANSFER_OUT";
            break;
        case TransactionType::NONE:
            return "NONE";
            break;
    }
    throw std::invalid_argument("Invalid transaction type");
}
TransactionType Utils::parseTransactionType(const std::string& transactionType) {
   if (transactionType == "DEPOSIT") {
       return TransactionType::DEPOSIT;
   }
    else if (transactionType == "WITHDRAW") {
        return TransactionType::WITHDRAW;
    }
    else if (transactionType == "TRANSFER_IN") {
        return TransactionType::TRANSFER_IN;
    }
    else if (transactionType == "TRANSFER_OUT") {
        return TransactionType::TRANSFER_OUT;
    }
    else {
        throw std::invalid_argument("Invalid transaction type");
    }
}



