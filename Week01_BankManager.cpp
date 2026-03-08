/*
* Week 8 Assignment - Recursive Functions
* By: Eldon Salman
* Date: March 8th 2026
*/

// All of the needed libraries
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#if __has_include(<crtdbg.h>)
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

using namespace std;

// Custom exception class for bank-related errors, such as invalid account removal/index
class BankException : public exception {
private:
    string message;

public:
    explicit BankException(const string& msg) : message(msg) {
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#if defined(_DEBUG)
struct CrtLeakCheckSetup {
    CrtLeakCheckSetup() {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }
    ~CrtLeakCheckSetup() {
        _CrtDumpMemoryLeaks();
    }
};

static CrtLeakCheckSetup g_crtLeakCheckSetup;
#endif



// Helper Function to Round to the Nearest Cent, first template function
template <typename T>
T roundCurrency(T value) {
    return static_cast<T>(round(value * static_cast<T>(100)) / static_cast<T>(100));
}

// My Composition class, just keeps track of transactions but more may be added soon
class TransactionHistory {
private:
    int totalTransactions;
    double totalDeposited;
    double totalWithdrawn;

public:
    // Constructors
    TransactionHistory() {
        totalTransactions = 0;
        totalDeposited = 0.0;
        totalWithdrawn = 0.0;
    }
    TransactionHistory(int trans, double deposited, double withdrawn) {
        totalTransactions = trans;
        totalDeposited = deposited;
        totalWithdrawn = withdrawn;
    }

    // Getters
    int getTotalTransactions() const {
        return totalTransactions;
    }
    double getTotalDeposited() const {
        return totalDeposited;
    }
    double getTotalWithdrawn() const {
        return totalWithdrawn;
    }

    // Helper methods, record deposit and withdrawal, this will be used in the actual transaction functions to keep track of the history
    void recordDeposit(double amount) {
        totalDeposited += amount;
        totalTransactions++;
    }
    void recordWithdrawal(double amount) {
        totalWithdrawn += amount;
        totalTransactions++;
    }
};



// Base Account class, parent of Savings + Checking
class Account {
protected:
    int accountNumber;
    string memberName;
    double balance;

public:
    // Enum for account type
    enum AccountType { Savings = 1, Checking = 2 };

    // Constructors
    Account() {
        accountNumber = 0;
        memberName = "Unnamed";
        balance = 0.0;
    }
    Account(int accNum, const string& name, double bal) {
        accountNumber = accNum;
        memberName = name;
        balance = bal;
    }

    // Getters
    int getAccountNumber() const {
        return accountNumber;
    }
    string getMemberName() const {
        return memberName;
    }
    double getBalance() const {
        return balance;
    }

    // Setters
    void setAccountNumber(int accNum) {
        accountNumber = accNum;
    }
    // No use, utilize if account changing in future
    void setMemberName(const string& name) {
        memberName = name;
    }
    void setBalance(double bal) {
        balance = bal;
    }

    // Virtual print function to be called by derived classes, this will print the common account info and then the derived classes will add their own info
    virtual void print() const {
        cout << left << setw(20) << accountNumber
            << setw(20) << memberName
            << "$" << fixed << setprecision(2) << setw(20) << balance
            << endl;
    }

    // Virtual toStream function for file output, this will be overridden by derived classes to add their own info as well
    virtual void toStream(ostream& os) const {
        os << "Account #" << accountNumber
            << " | " << memberName
            << " | Balance: $" << fixed << setprecision(2) << balance;
    }

    // Virtual method to record transactions
    virtual void recordTransaction(bool isDeposit, double amount) = 0;

    // Overload form of output, directly outputs the account info to the stream, this will call the toStream function which will be overridden by derived classes to add their own info as well
    friend ostream& operator<<(ostream& os, const Account& account) {
        account.toStream(os);
        return os;
    }

    virtual ~Account() = default;
};

// Template class for a dynamic array, this will be used to store the accounts in the AccountManager
template <typename T>
class DynamicArray {
private:
    T* accountsList;
    int count; // ACTUAL number of accounts in the array
    int capacity; // Coding number of accounts the array can hold before resizing

    void resize() {
        int newCapacity = capacity + 1;
        T* newList = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newList[i] = accountsList[i];
        }
        for (int i = count; i < newCapacity; i++) {
            newList[i] = T();
        }
        delete[] accountsList;
        accountsList = newList;
        capacity = newCapacity;
    }

public:
    DynamicArray() {
        count = 0; // Start with 0 accounts
        capacity = 1; // `capacity` starts at 1 to allow for the first account to be added without resizing
        accountsList = new T[capacity];
        for (int i = 0; i < capacity; i++) {
            accountsList[i] = T();
        }
    }

    // Adds an account to the manager, checks for null and resizes if needed, returns true if successful
    bool add(const T& value) {
        if (count >= capacity) {
            resize();
        }
        accountsList[count++] = value;
        return true;
    }

    bool removeAt(int index) {
        if (index < 0 || index >= count) {
            throw out_of_range("DynamicArray: removeAt invalid index");
        }
        for (int i = index; i < count - 1; i++) {
            accountsList[i] = accountsList[i + 1];
        }
        accountsList[count - 1] = T();
        count--;
        return true;
    }

    // Getter for accounts at a specific index, returns nullptr if index is out of bounds
    T getAt(int index) const {
        if (index < 0 || index >= count) {
            throw out_of_range("DynamicArray: getAt invalid index");
        }
        return accountsList[index];
    }

    T operator[](int index) const {
        return getAt(index);
    }

    // Getter for account count
    int size() const {
        return count;
    }

    // Recursive print function
    void printAllRecursive(int startIndex = 0) const {
        if (startIndex >= count) {
            return;
        }

        if (accountsList[startIndex] != nullptr) {
            accountsList[startIndex]->print();
            cout << endl;
        }

        printAllRecursive(startIndex + 1);
    }

    // Destructor to clean up memory, deletes all accounts and then the array itself
    ~DynamicArray() {
        delete[] accountsList;
    }
};


// Made to manage the accounts
class AccountManager {
private:
    DynamicArray<Account*> accounts;

public:
    // Adds an account to the manager, this calls upon OPERATOR+= to add the account, checks for null and returns true if successful
    bool addAccount(Account* newAccount) {
        if (newAccount == nullptr) {
            return false;
        }
        *this += newAccount;
        return true;
    }

    // Operator addition to add an account
    AccountManager& operator+=(Account* newAccount) {
        if (newAccount != nullptr) {
            this->accounts.add(newAccount);
        }
        return *this;
    }

    // Operator subtraction to remove an account || NOT TO BE USED YET, 
    AccountManager& operator-=(int index) {
        if (index < 0 || index >= accounts.size()) {
            throw BankException("Account removal failed: invalid index");
        }
        Account* accountToRemove = (*this)[index];
        delete accountToRemove;
        accounts.removeAt(index);
        return *this;
    }

    Account* operator[](int index) const {
        return accounts[index];
    }

    // Getter for account count
    int getCount() const {
        return accounts.size();
    }

    // Getter for accounts at a specific index, returns nullptr if index is out of bounds
    Account* getAt(int index) const {
        return (*this)[index];
    }

    // Simple wrapper so Bank can use the existing recursive print function.
    void printAllAccountsRecursive() const {
        accounts.printAllRecursive();
    }

    // Destructor to clean up memory, deletes all accounts and then the array itself
    ~AccountManager() {
        for (int i = 0; i < accounts.size(); i++) {
            delete accounts.getAt(i);
        }
    }

};


// Derived class, Savings adds interest which will be manipulated later on to add interest over time
class SavingsAccount : public Account {
private:
    double interestRate;
    TransactionHistory history;

public:
    // Constructors
    SavingsAccount() {
        interestRate = 0.0;
    }
    SavingsAccount(int accNum, const string& name, double bal, double rate) : Account(accNum, name, bal) {
        interestRate = rate;
    }

    // Getters
    double getInterestRate() const {
        return interestRate;
    }
    TransactionHistory& getHistory() {
        return history;
    }
    const TransactionHistory& getHistory() const {
        return history;
    }

    // Setters
    void setInterestRate(double rate) {
        interestRate = rate;
    }

    // Override print
    void print() const override {
        // Call base print first
        Account::print();
        cout << endl;
        // Add savings-specific info
        cout << left << setw(20) << "Account Type:"
            << "Savings" << endl;
        cout << setw(20) << "Interest Rate:"
            << fixed << setprecision(2) << interestRate * 100 << "%" << endl;
        // Add transaction history
        cout << setw(20) << "Total Transactions:"
            << history.getTotalTransactions() << endl;
        cout << setw(20) << "Total Deposited:"
            << "$" << fixed << setprecision(2) << history.getTotalDeposited() << endl;
        cout << setw(20) << "Total Withdrawn:"
            << "$" << fixed << setprecision(2) << history.getTotalWithdrawn() << endl;
    }

    void toStream(ostream& os) const override {
        os << "Savings #" << accountNumber
            << " | " << memberName
            << " | Balance: $" << fixed << setprecision(2) << balance
            << " | Rate: " << fixed << setprecision(2) << interestRate * 100 << "%";
    }

    // To be used later on, no utilization now but more in the future
    double calculateInterest() const {
        return balance * interestRate;
    }

    // Record transaction in history
    void recordTransaction(bool isDeposit, double amount) override {
        if (isDeposit)
            history.recordDeposit(amount);
        else
            history.recordWithdrawal(amount);
    }


    bool operator==(const SavingsAccount& other) const {
        return memberName == other.memberName;
    }
};






// Checking account Derived class, adds a monthly fee that will be manipulated later on
class CheckingAccount : public Account {
private:
    double monthlyFee;
    TransactionHistory history;

public:
    // Constructors
    CheckingAccount() {
        monthlyFee = 0.0;
    }
    CheckingAccount(int accNum, const string& name, double bal, double fee) : Account(accNum, name, bal) {
        monthlyFee = fee;
    }

    // Getters
    double getMonthlyFee() const {
        return monthlyFee;
    }
    TransactionHistory& getHistory() {
        return history;
    }
    const TransactionHistory& getHistory() const {
        return history;
    }

    // Setters
    void setMonthlyFee(double fee) {
        monthlyFee = fee;
    }

    // Override print
    void print() const override {
        // Call base print first
        Account::print();
        cout << endl;
        // Add checking-specific info
        cout << left << setw(20) << "Account Type:"
            << "Checking" << endl;
        cout << setw(20) << "Monthly Fee:"
            << "$" << fixed << setprecision(2) << monthlyFee << endl;
        // Add transaction history
        cout << setw(20) << "Total Transactions:"
            << history.getTotalTransactions() << endl;
        cout << setw(20) << "Total Deposited:"
            << "$" << fixed << setprecision(2) << history.getTotalDeposited() << endl;
        cout << setw(20) << "Total Withdrawn:"
            << "$" << fixed << setprecision(2) << history.getTotalWithdrawn() << endl;
    }

    void toStream(ostream& os) const override {
        os << "Checking #" << accountNumber
            << " | " << memberName
            << " | Balance: $" << fixed << setprecision(2) << balance
            << " | Fee: $" << fixed << setprecision(2) << monthlyFee;
    }

    // Helper method
    void applyMonthlyFee() {
        balance -= monthlyFee;
    }

    // Record transaction in history
    void recordTransaction(bool isDeposit, double amount) override {
        if (isDeposit)
            history.recordDeposit(amount);
        else
            history.recordWithdrawal(amount);
    }
};






// Class Bank encapsulates the actual functions of the program
class Bank {
private:
    // Replaces Account array with an AccountManager object, this is composition as Bank 'has a' AccountManager
    AccountManager manager;

    // All functions are public for Bank Manager usage
public:

    // Displays the menu every time, foundational for the program
    void displayMenu() {
        cout << "\n==================================================\n";
        cout << setw(32) << "SimpleBank Menu\n";
        cout << "==================================================\n";
        cout << "1. Add account\n";
        cout << "2. View account\n";
        cout << "3. Deposit or Withdraw\n";
        cout << "4. Save report\n";
        cout << "5. Delete account\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";
    }

    // We're just adding a new account to the list, the maximum being 999 Accounts
    void addAccount() {
        // Initializes holders and Error which will be utilized to stop bad inputs
        int accNum;
        string name;
        double bal;
        int typeChoice;
        bool error = true;

        while (error) {
            // We are checking for 'Error', we assume there is none at the start and merely ask for a normal account#
            error = false;
            cout << "\nEnter account number: ";
            cin >> accNum;

            // BUT, if there IS an Error it will clear out the line, output an Error Message, and also redo the whole thing
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: You can not use Alphabetic or Special Characters in an account number !\n";
                error = true;
            }

            for (int i = 0; i < manager.getCount(); i++) {
                // Search for if the account they entered was the same as one of our other accounts
                if (accNum == manager.getAt(i)->getAccountNumber()) {
                    error = true;
                    cout << "! Error: There was a duplicate account number found !\n";
                }
            }

            // Your account number may not be 0 or Negative as well
            if (accNum <= 0) {
                error = true;
                cout << "! Error: Account can not be negative or 0 !\n";
            }
        }

        error = true;
        // Clears the line to allow for an input of a name
        cin.ignore(10000, '\n');
        cout << "\nEnter Member Name: ";
        // Get Line to allow spaces in names
        getline(cin, name);

        while (error) {
            // We are checking for 'Error', we assume there is none at the start and ask the user for Dollars & Cents)
            error = false;
            cout << "\nEnter Savings Balance ($##.##): ";
            cin >> bal;

            // Rounds it via template helper
            bal = roundCurrency(bal);

            // If another failure happens we restart the question to get a proper number
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: You can not use Alphabetic or Special Characters for account balance !\n";
                error = true;
                continue;
            }

            // It also may not be negative or equal to 0, you need to start with something in there
            // Most banks have minimums (0 to 5 dollars), for this instance no minimum is required
            if (bal <= 0) {
                error = true;
                cout << "! Error: The Balance may not be equal to or less than $0.00 !\n";
            }
        }

        cout << "\nSelect account type (1 for Savings, 2 for Checking): ";
        cin >> typeChoice;
        if (cin.fail() || (typeChoice != 1 && typeChoice != 2)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "! Error: Invalid account type choice !\n";
            return;
        }

        Account* newAccount = nullptr;
        if (typeChoice == 1) {
            double interestRate = 0.05; // Example interest rate
            newAccount = new SavingsAccount(accNum, name, bal, interestRate);
        }
        else {
            double monthlyFee = 10.0; // Example monthly fee
            newAccount = new CheckingAccount(accNum, name, bal, monthlyFee);
        }

        if (!manager.addAccount(newAccount)) {
            delete newAccount;
            cout << "! Error: Could not store account !\n";
            return;
        }
        cout << "Account added successfully!\n";
    }

    void viewAccount(int accountNum) {
        // Searches for the Index
        int index = searchAccount(accountNum);

        // Sets up the actual view format for accounts
        cout << "\n==================================================\n";
        cout << setw(28) << "Account\n";
        cout << "==================================================\n";

        cout << left
            << setw(20) << "Account#"
            << setw(20) << "Member Name"
            << setw(20) << "Balance" << endl;

        cout << setfill('-') << setw(50) << "-" << endl;
        cout << setfill(' ');

        // If there is a -1 then there is no account
        if (index == -1) {
            cout << "! There was no account that was found !\n";
            cout << right << setfill(' ');
            return;
        }

        // Else it will output the actual index using the virtual print function
        manager.getAt(index)->print();

        cout << right << setfill(' ');
    }

    // View member is similar to view account but it will search for all accounts with that member name and display them
    void viewMember(const string& memberName) {
        bool found = false;

        cout << "\n==================================================\n";
        cout << setw(35) << "Member Accounts\n";
        cout << "==================================================\n";

        for (int i = 0; i < manager.getCount(); i++) {
            // Searches through all accounts and if the member name matches it will output the account
            Account* account = manager[i];
            // Uses Operator comparison and Operator print to output the account, this is to show operator overloading
            if (account->getMemberName() == memberName) {
                cout << *account << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "! There were no accounts found for that member !\n";
        }
    }


    int searchAccount(int accountNum) {
        // Account is searched for, once found it is indexed to be used for the transaction
        int index = -1;
        for (int i = 0; i < manager.getCount(); i++) {
            if (manager.getAt(i)->getAccountNumber() == accountNum) {
                index = i;
                break;
            }
        }
        // Returns what the actual array index is
        return index;
    }


    void transaction() {
        // Initializes needed varriables including Error again to stop bad inputs, and index to find the account
        int accountNum;
        int choice;
        double amount;
        bool error = true;
        int index;


        while (error) {
            error = false;
            cout << "\nEnter Account Number: ";
            cin >> accountNum;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: You can not use Alphabetic or Special Characters in an account number !\n";
                error = true;
                continue;
            }
            // Searches for the account number to see if it exists, if it doesn't it will output an error and return to the menu
            index = searchAccount(accountNum);
            if (index == -1) {
                error = true;
                cout << "! Error: No account found with that number, you may try again later !\n";
                return;
            }

        }

        // Looking for transaction type, this is their chance to get out if they don't want to do the transaction
        error = true;
        while (error) {
            error = false;
            cout << "\nSelect transaction type (1 for Deposit, 2 for Withdraw, 3 for Exit): ";
            cin >> choice;
            if (cin.fail() || (choice != 1 && choice != 2 && choice != 3)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: Invalid transaction type choice !\n";
                error = true;
            }
        }
        if (choice == 3)
        {
            return;
        }

        // They could also input 0 or negative amounts, and we don't want that, so we will check for that as well as for bad inputs
        error = true;
        while (error) {
            error = false;
            cout << "\nEnter amount ($##.##): ";
            cin >> amount;

            // Rounds it via template helper
            amount = roundCurrency(amount);

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: You can not use Alphabetic or Special Characters for amount !\n";
                error = true;
            }

            if (amount <= 0) {
                error = true;
                cout << "! Error: The Amount may not be equal to or less than $0.00 !\n";
            }
        }

        // Call deposit or withdrawl and record it in the transaction history, then show the account after the transaction, even if it fails, to show the user what happened
        bool success = false;
        if (choice == 1) {
            success = deposit(accountNum, amount);
            if (success)
                cout << "\nDeposited $" << fixed << setprecision(2) << amount << " to account #" << manager.getAt(index)->getAccountNumber() << ".\n";
            else
                cout << "\nDeposit failed.\n";
        }
        else if (choice == 2) {
            success = withdraw(accountNum, amount);
            if (success)
                cout << "\nWithdrew $" << fixed << setprecision(2) << amount << " from account #" << manager.getAt(index)->getAccountNumber() << ".\n";
            else
                cout << "\nInsufficient funds or invalid withdrawal.\n";
        }
        // Show the account after the actual transaction, even if it 'cancels'
        viewAccount(accountNum);
    }

    double deleteAccount(int accountNum) {
        int index = searchAccount(accountNum);
        if (index == -1) {
            return -1; // Account not found
        }
        double balance = manager.getAt(index)->getBalance();
        manager -= index;
        return balance;
    }

    // Saving the actual accounts all to a file for a report
    void saveReport(const string& filename) {
        ofstream report(filename);
        if (!report) {
            cout << "Error: Could not open file for writing.\n";
            return;
        }

        // All accounts are displayed in the same format as viewAccount
        report << "==================================================\n";
        report << setw(30) << "All Accounts\n";
        report << "==================================================\n";
        report << left << setw(20) << "Account#" << setw(20) << "Member Name" << setw(20) << "Balance" << endl;
        report << setfill('-') << setw(50) << "-" << endl;
        report << setfill(' ');

        for (int i = 0; i < manager.getCount(); i++) {
            Account* account = manager.getAt(i);
            report << left << setw(20) << account->getAccountNumber() <<
                setw(20) << account->getMemberName() << "$" << fixed << setprecision(2) << setw(19) << account->getBalance() << endl;
        }

        // Recursive implementation usage (prints same report rows to console).
        cout << "==================================================\n";
        cout << setw(30) << "All Accounts\n";
        cout << "==================================================\n";
        cout << left << setw(20) << "Account#" << setw(20) << "Member Name" << setw(20) << "Balance" << endl;
        manager.printAllAccountsRecursive();
        cout << right << setfill(' ') << defaultfloat << setprecision(6);

        // File Name is given for them to find the actual File
        cout << "Report saved to " << filename << endl;
    }

    // Test functions
    bool addAccountTest(int accountNumber, const string& memberName, double balance, Account::AccountType type) {
        Account* newAccount = nullptr;
        if (type == Account::Savings) {
            newAccount = new SavingsAccount(accountNumber, memberName, balance, 0.05); // Example interest rate
        }
        else if (type == Account::Checking) {
            newAccount = new CheckingAccount(accountNumber, memberName, balance, 10.0); // Example monthly fee
        }
        else {
            return false; // Invalid account type
        }

        return manager.addAccount(newAccount);
    }

    // Getter for account count for testing purposes
    int getAccountCount() const {
        return manager.getCount();
    }

    // Deposit directly for testing
    bool deposit(int accountNum, double amount) {
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) {
            return false;
        }
        // Deposit if it's clear
        Account* account = manager.getAt(index);
        account->setBalance(account->getBalance() + amount);
        account->recordTransaction(true, amount);
        return true;
    }

    // Withdraw directly for testing
    bool withdraw(int accountNum, double amount) {
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) {
            return false;
        }
        Account* account = manager.getAt(index);
        if (amount > account->getBalance()) {
            return false; // Insufficient funds
        }
        account->setBalance(account->getBalance() - amount);
        account->recordTransaction(false, amount);
        return true;
    }

    // SIMPLIFIED TESTING TRANSACTION, this is just to test my enumeration
    bool doTransaction(int accountNum, int type, double amount) {
        if (type == 1) {
            return deposit(accountNum, amount);
        }
        else if (type == 2) {
            return withdraw(accountNum, amount);
        }
        return false;
    }
};




#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// TEST CLASS METHODS for inheritance and composition
TEST_CASE("Testing Week 1 to 4 - Inheritance and Composition") {
    Bank b;

    // Test 1: Add Savings Account and verify it initializes correctly
    CHECK(b.addAccountTest(101, "Alice", 500.0, Account::Savings) == true);
    CHECK(b.doTransaction(101, 1, 100) == true);
    CHECK(b.getAccountCount() == 1);
    CHECK(b.searchAccount(101) == 0);

    // Test 2: Add Checking Account and verify it initializes correctly
    CHECK(b.addAccountTest(102, "Bob", 1000.0, Account::Checking) == true);
    CHECK(b.doTransaction(102, 2, 100) == true);
    CHECK(b.getAccountCount() == 2);
    CHECK(b.searchAccount(102) == 1);

    // Test 3: Deposit into Savings (tests transaction recording and composition)
    CHECK(b.deposit(101, 100.0) == true);

    // Test 4: Withdraw from Checking (tests transaction recording and composition)
    CHECK(b.withdraw(102, 200.0) == true);

    // Test 5: Test search functionality and reject non-existent accounts
    CHECK(b.searchAccount(999) == -1);
    CHECK(b.deposit(999, 50) == false);

    // Test 6: Test insufficient funds rejection
    CHECK(b.withdraw(101, 10000) == false);

    // Test 7-8: operator== equality and inequality
    SavingsAccount eqA(201, "Alex", 100.0, 0.05);
    SavingsAccount eqB(202, "Alex", 300.0, 0.03);
    SavingsAccount neqA(203, "Blake", 100.0, 0.05);
    CHECK(eqA == eqB);
    CHECK_FALSE(eqA == neqA);

    // Test 9-10: operator<< output using ostringstream
    ostringstream savingsOut;
    savingsOut << static_cast<const Account&>(eqA);
    CHECK(savingsOut.str() == "Savings #201 | Alex | Balance: $100.00 | Rate: 5.00%");

    CheckingAccount checkStream(204, "Casey", 250.0, 10.0);
    ostringstream checkingOut;
    checkingOut << static_cast<const Account&>(checkStream);
    CHECK(checkingOut.str() == "Checking #204 | Casey | Balance: $250.00 | Fee: $10.00");

    // Test 11-12: operator[] valid and invalid index behavior
    AccountManager indexManager;
    indexManager += new SavingsAccount(301, "Dana", 500.0, 0.05);
    CHECK(indexManager[0] != nullptr);
    CHECK(indexManager[0]->getAccountNumber() == 301);
    CHECK_THROWS_AS(indexManager[99], out_of_range);

    // Test 13-14: operator+= add and operator-= remove/shift
    AccountManager opManager;
    opManager += new SavingsAccount(401, "Erin", 600.0, 0.05);
    opManager += new CheckingAccount(402, "Flynn", 700.0, 10.0);
    CHECK(opManager.getCount() == 2);
    CHECK(opManager[1] != nullptr);
    CHECK(opManager[1]->getAccountNumber() == 402);

    opManager -= 0;
    CHECK(opManager.getCount() == 1);
    CHECK(opManager[0] != nullptr);
    CHECK(opManager[0]->getAccountNumber() == 402);
    CHECK_THROWS_AS(opManager -= 99, BankException);

    // Test 15-16: function template usage (int and double)
    CHECK(roundCurrency(42) == 42);
    CHECK(roundCurrency(12.346) == 12.35);

    // Test 17-18: class template usage (store/remove/resizing)
    DynamicArray<int> values;
    CHECK(values.add(10) == true);
    CHECK(values.add(20) == true);
    CHECK(values.add(30) == true);
    CHECK(values.size() == 3);
    CHECK(values[1] == 20);
    CHECK(values.removeAt(1) == true);
    CHECK(values.size() == 2);
    CHECK(values[1] == 30);
    CHECK_THROWS_AS(values[99], out_of_range);
    CHECK_THROWS_AS(values.removeAt(99), out_of_range);
}
#endif



#ifndef _DEBUG
int main()
{
    int choice = 0;
    string filename;
    string memberName;
    int searchChoice;
    Bank bankManager;
    double balance;

    while (choice != 6) {
        bankManager.displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: You can not use special characters in this menu.\n";
            choice = 0;
            continue;
        }

        switch (choice) {
        case 1:
            // Calls the add account function which will ask for all the needed info and add the account to the manager
            bankManager.addAccount();
            break;
        case 2:
            cout << "Would you like to search by account number or member name? (1 for Account Number, 2 for Member Name): ";
            cin >> searchChoice;
            if (cin.fail() || (searchChoice != 1 && searchChoice != 2)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "! Error: Invalid search type choice !\n";
                break;
            }
            if (searchChoice == 1) {
                cout << "Enter Account Number: ";
                cin >> searchChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: You can not use special characters for account number.\n";
                    break;
                }
                bankManager.viewAccount(searchChoice);
            }
            else {
                cin.ignore(10000, '\n');
                cout << "Enter Member Name: ";
                getline(cin, memberName);
                bankManager.viewMember(memberName);
            }
            break;
        case 3:
            // Calls the transaction function which will ask for the account number, transaction type, and amount to do the transaction, this will also show the account after the transaction to show the user what happened
            bankManager.transaction();
            break;
        case 4:
            // Calls the save report function which will ask for a file name and then save all the accounts to that file in a nice format, this is to show file output
            cout << "Enter filename to save report: ";
            cin >> filename;
            bankManager.saveReport(filename);
            break;
        case 5:
            // Calls the delete account function which will ask for the account number to delete, this will also return the final balance of the deleted account to show the user what they got back
            cout << "Enter Account Number to delete: ";
            cin >> searchChoice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Error: You can not use special characters for account number.\n";
                break;
            }

            // Deletes the account and returns the balance, if the balance is -1 then there was no account found with that number
            balance = bankManager.deleteAccount(searchChoice);
            if (balance != -1) {
                cout << "Account deleted successfully.\n";
                cout << "Final balance of deleted account returned: $" << fixed << setprecision(2) << balance << endl;
            }
            else {
                cout << "! Error: No account found with that number !\n";
            }
            break;
        case 6:
            // Exits the program, this is to show the user that they can exit when they want and that the program doesn't crash when they do
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
#endif