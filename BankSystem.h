#pragma once
#include <vector>
#include "string"
#include "Transaction.h"
#include "Customer.h"
#include "Account.h"

class BankSystem {
    private:
    std::vector<Customer> customers;
    std::vector<Account> accounts;
    int nextCustomerID;
    int nextAccountID;
    int nextTransactionID;
    std::string filePath;
    Customer* findCustomer(int id);
    Account* findAccount(int id);
    int takeTransactionId();
    void checkOwnership(const Account& account, int customerID) const;
    int maxCustomerID() const;
    int maxAccountID() const;
    int maxTransactionID() const;
    public:
    BankSystem(const std::vector<Customer>& customers,const std::vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID);
    BankSystem(const std::vector<Customer>& customers,const std::vector<Account>& accounts, int nextCustomerID, int nextAccountID, int nextTransactionID,const std::string& filePath);
    ~BankSystem();
    const std::vector<Customer>& getCustomers() const;
    const std::vector<Account>& getAccounts() const;
    int getNextCustomerID() const;
    int getNextAccountID() const;
    int getNextTransactionID() const;
    const std::string& getFilePath() const;
    void setNextCustomerID(int customerID);
    void setNextAccountID(int accountID);
    void setNextTransactionID(int transactionID);
    void setCustomers(const std::vector<Customer>& customers);
    void setAccounts(const std::vector<Account>& accounts);
    void setFilePath(const std::string& filePath);
    int registerCustomer(const std::string& name, const std::string& pin);
    int login(int customerId, const std::string& pin);
    int openAccount(int customerId);
    std::vector<int> getAccountIds(int customerId) const;
    // long long getBalance(int customerId, int accountId);
    // void deposit(int customerId, int accountId, long long amount);
    // void withdraw(int customerId, int accountId, long long amount);
    // void transfer(int customerId, int fromAccountId, int toAccountId, long long amount);
    // const std::vector<Transaction>& getHistory(int customerId, int accountId);
    // void save() const;
    // void load();

    //note трябва да направиш и меню и tostring но май да не е в banksystem
    //Регистрация: менюто чете име и PIN → registerCustomer взема ID от nextCustomerId, създава Customer (конструкторът валидира и може да хвърли), добавя във вектора, увеличава брояча. Ред: първо създаваш обекта, чак после увеличаваш брояча, иначе при хвърляне губиш номер.
    // Вход: login(id, pin) намира клиента и сравнява PIN. При провал хвърля. Съобщението за "няма такъв клиент" и "грешен PIN" най-добре да е едно и също, за да не подсказваш кои ID съществуват.
    // Сметка: openAccount(customerId) проверява, че клиентът съществува, създава Account с баланс 0 и празна история.
    // Депозит/теглене: намери сметката → checkOwnership → takeTransactionId() → извикай метода на Account. Account проверява останалото сам.

    //Превод: най-важният поток.
    // намери и двете сметки (хвърля, ако липсва някоя),
    // провери собствеността на изпращащата,
    // провери, че не са една и съща,
    // вземи две ID-та за транзакции,
    // направи копия на двете сметки,
    // try { from.sendTransfer(...); to.receiveTransfer(...); }
    // catch (...) { върни копията; throw; }
    // при успех: save().
    // Старт: load() → зарежда клиенти, сметки, истории и възстановява броячите (най-голямото съществуващо ID + 1). Изход/след всяка промяна: save().
};

