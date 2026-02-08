/*
* Week 4 Assignment - Bank Manager with Inheritance
* By: Eldon Salman
* Date: February 8th 2026
*/

// All of the needed libraries
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;






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

    // Virtual method to record transactions
    virtual void recordTransaction(bool isDeposit, double amount) {
        // Base class does nothing - derived classes override
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
    // You can't Manipulate the actual account, you need to access it through the functions
    static const int max = 999;
    // 999 Is chosen as the 'max' but hypothetically we could place it at an extremely high number if we wanted to do so
    Account* accounts[max];
    int accountCount = 0;

    // All functions are public for Bank Manager usage
public:

    // Displays the menu every time, foundational for the program
    void displayMenu() {
        cout << "\n==================================================\n";
        cout << setw(32) << "SimpleBank Menu\n";
        cout << "==================================================\n";
        cout << "1. Add account\n";
        cout << "2. View accounts\n";
        cout << "3. Deposit or Withdraw\n";
        cout << "4. Save report\n";
        cout << "5. Exit\n";
        cout << "Choose an option: ";
    }

    // We're just adding a new account to the list, the maximum being 999 Accounts
    void addAccount() {
		// Returns if going over the max amount of accounts
        if (accountCount >= max) {
            cout << "Cannot add more accounts. Maximum reached.\n";
            return;
        }

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

            for (int i = 0; i < accountCount; i++) {
                // Search for if the account they entered was the same as one of our other accounts
                if (accNum == accounts[i]->getAccountNumber()) {
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

            // Rounds it to the nearest dollar via: Making it whole numbers, rounding it to the nearest dollar, turn it back into cents
            bal = round(bal * 100.0) / 100.0;

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

        accounts[accountCount++] = newAccount;
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
        accounts[index]->print();

        cout << right << setfill(' ');
    }


    int searchAccount(int accountNum) {
        // Account is searched for, once found it is indexed to be used for the transaction
        int index = -1;
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i]->getAccountNumber() == accountNum) {
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

            // Rounds it to the nearest dollar via: Making it whole numbers, rounding it to the nearest dollar, turn it back into cents
            amount = round(amount * 100.0) / 100.0;

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
                cout << "\nDeposited $" << fixed << setprecision(2) << amount << " to account #" << accounts[index]->getAccountNumber() << ".\n";
            else
                cout << "\nDeposit failed.\n";
        }
        else if (choice == 2) {
            success = withdraw(accountNum, amount);
            if (success)
                cout << "\nWithdrew $" << fixed << setprecision(2) << amount << " from account #" << accounts[index]->getAccountNumber() << ".\n";
            else
                cout << "\nInsufficient funds or invalid withdrawal.\n";
        }
        // Show the account after the actual transaction, even if it 'cancels'
        viewAccount(accountNum);
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

        for (int i = 0; i < accountCount; i++) {
            report << left << setw(20) << accounts[i]->getAccountNumber() << 
                setw(20) << accounts[i]->getMemberName() << "$" << fixed << setprecision(2) << setw(19) << accounts[i]->getBalance() << endl;
        }

        // File Name is given for them to find the actual File
        cout << "Report saved to " << filename << endl;
    }




    // Test functions
    bool addAccountTest(int accountNumber, const string& memberName, double balance, Account::AccountType type) {
        if (accountCount >= max) {
            return false; // Cannot add more accounts
        }

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

        accounts[accountCount++] = newAccount;
        return true;
    }

	// Getter for account count for testing purposes
    int getAccountCount() const {
        return accountCount;
    }

    // Deposit directly for testing
    bool deposit(int accountNum, double amount) { 
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) {
            return false;
        }
        // Deposit if it's clear
        accounts[index]->setBalance(accounts[index]->getBalance() + amount);
        accounts[index]->recordTransaction(true, amount);
        return true;
    }

    // Withdraw directly for testing
    bool withdraw(int accountNum, double amount) {
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) {
            return false;
        }
        if (amount > accounts[index]->getBalance()) {
            return false; // Insufficient funds
        }
        accounts[index]->setBalance(accounts[index]->getBalance() - amount);
        accounts[index]->recordTransaction(false, amount);
        return true;
    }

    // SIMPLIFIED TESTING TRANSACTION, this is just to test my enumeration
    bool doTransaction(int accountNum, int type, double amount) {
        int index = searchAccount(accountNum); // Search accounts
        if (index == -1) return false; // If there is no account found then it fails

        if (amount <= 0) return false; // If there is a negative or zero amount it fails

        if (type == 1) {
            accounts[index]->setBalance(accounts[index]->getBalance() + amount);
            accounts[index]->recordTransaction(true, amount);
            return true;
        }
        else if (type == 2) {
            if (amount > accounts[index]->getBalance()) return false; // Insufficient funds
            accounts[index]->setBalance(accounts[index]->getBalance() - amount);
            accounts[index]->recordTransaction(false, amount);
            return true;
        }
        return false; // fallback (shouldn't happen)
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
}
#endif



#ifndef _DEBUG
int main()
{
    int choice = 0;
    string filename;
    int accountNum;
    Bank bankManager;

    while (choice != 5) {
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
            bankManager.addAccount();
            break;
        case 2:
            cout << "Enter Account Number: ";
            cin >> accountNum;
            bankManager.viewAccount(accountNum);
            break;
        case 3:
            bankManager.transaction();
            break;
        case 4:
            cout << "Enter filename to save report: ";
            cin >> filename;
            bankManager.saveReport(filename);
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
#endif