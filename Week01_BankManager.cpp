/*
* Week 2 Assignment - Bank Manager
* By: Eldon Salman
* Date: January 25th 2026
*/

#include <iostream> // For Strings
#include <iomanip> // SetW & Formatting
#include <cmath> // Rounding
#include <fstream> // File output
#include <string> // Getline
using namespace std;

// Bank Account will always have 3 of the following
struct BankAccount {
    int accountNumber;
    string memberName;
    double savingsAccountBalance;
};

// Class Bank encapsulates the actual functions of the program
class Bank {
    // You can't Manipulate the actual account, you need to access it through the functions
private:
    const static int max = 999;
    // 999 Is chosen as the 'max' but hypothetically we could place it at an extremely high number if we wanted to do so
    BankAccount accounts[max];
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
        bool error = true;

        // If the account is equal to 999, then it won't allow you to add a new account
        if (accountCount >= max) {
            cout << "Cannot add more accounts. Maximum reached.\n";
            return;
        }
        else { // Else the program will continue to the actual inputs
            while (error) {
                // We are checking for 'Error', we assume there is none at the start and merely ask for a normal account#
                error = false;
                cout << "Enter account number: ";
                cin >> accounts[accountCount].accountNumber;

                // BUT, if there IS an Error it will clear out the line, output an Error Message, and also redo the whole thing
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "! Error: You can not use Alphabetic or Special Characters in an account number !\n";
                    error = true;
                    continue;
                }

                for (int i = 0; i < accountCount; i++) {
                    // Search for if the account they entered was the same as one of our other accounts
                    if (accounts[accountCount].accountNumber == accounts[i].accountNumber) {
                        error = true;
                        cout << "! Error: There was a duplicate account number found !\n";
                    }
                }

                // Your account number may not be 0 or Negative as well
                if (accounts[accountCount].accountNumber <= 0) {
                    error = true;
                    cout << "! Error: Account can not be negative or 0 !\n";
                }
            }

            error = true;
            // Clears the line to allow for an input of a name
            cin.ignore(10000, '\n');
            cout << "\nEnter Member Name: ";
            // Get Line to allow spaces in names
            getline(cin, accounts[accountCount].memberName);


            while (error) {
                // We are checking for 'Error', we assume there is none at the start and ask the user for Dollars & Cents)
                error = false;
                cout << "\nEnter Savings Balance ($##.##): ";
                cin >> accounts[accountCount].savingsAccountBalance;

                // VERY COMPLICATED LOOKING! But all it does is round it to the nearest dollar via: First making it whole numbers, then rounding it to the nearest dollar, then turn it back into cents
                accounts[accountCount].savingsAccountBalance = round(accounts[accountCount].savingsAccountBalance * 100.0) / 100.0;

                // Again, if another failure happens we restart the question to get a proper number)
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "! Error: You can not use Alphabetic or Special Characters for account balance !\n";
                    error = true;
                    continue;
                }

                // It also may not be negative or equal to 0, you need to start with something in there
                // Most banks have minimums (0 to 5 dollars), for this instance no minimum is required
                if (accounts[accountCount].savingsAccountBalance <= 0) {
                    error = true;
                    cout << "! Error: The Balance may not be equal to or less than $0.00 !\n";
                }
            }
        }
        // Success, and our 'total' was increased in accountCount
        cout << "Account added successfully!\n";
        accountCount += 1;
        return;
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

        // Else it will output the actual index
        cout << left
            << setw(20) << accounts[index].accountNumber
            << setw(20) << accounts[index].memberName
            << "$" << fixed << setprecision(2)
            << setw(20) << accounts[index].savingsAccountBalance
            << endl;

        cout << right << setfill(' ');
    }


    int searchAccount(int accountNum) {
        // Account is searched for, once found it is indexxed to be used for the transaction
        int index = -1;
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNumber == accountNum) {
                index = i;
                break;
            }
        }
        // Returns what the actual array index is
        return index;
    }

    // Enumeration for Deposit/Withdraws
    enum TransactionType { Deposit = 1, Withdraw = 2 };

    void transaction() {
        // Account Number is needed for it to occur
        int accountNum;
        cout << "Enter the account number for this transaction: ";
        cin >> accountNum;

        // Account is searched for, once found it is indexxed to be used for the transaction
        int index = searchAccount(accountNum);

        // Transaction cancels if Index is not found
        if (index == -1) {
            cout << "Transaction failed: Account does not exist.\n";
            return;
        }

        // Transaction has to choose 1 or 2 for Deposit or Withdraw
        int choice;
        cout << "Select transaction type: Deposit (1) or Withdraw (2): ";
        cin >> choice;

        // If it's neither it cancels out
        TransactionType type;
        if (choice == 1) type = Deposit;
        else if (choice == 2) type = Withdraw;
        else {
            cout << "Invalid transaction type.\n";
            return;
        }

        // Ammount is entered and rounded to the nearest cent
        double amount;
        cout << "Enter the amount: ";
        cin >> amount;
        amount = round(amount * 100) / 100;

        // Will not work if it's not a valid number
        if (cin.fail() || amount <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid amount.\n";
            return;
        }

        bool success = false;
        if (type == Deposit) {
            success = deposit(accountNum, amount);
            if (success)
                cout << "Deposited $" << fixed << setprecision(2) << amount << " to account #" << accounts[index].accountNumber << ".\n";
            else
                cout << "Deposit failed.\n";
        }
        else if (type == Withdraw) {
            success = withdraw(accountNum, amount);
            if (success)
                cout << "Withdrew $" << fixed << setprecision(2) << amount << " from account #" << accounts[index].accountNumber << ".\n";
            else
                cout << "Insufficient funds or invalid withdrawal.\n";
        }
        // Show the account after the actual transaction, even if it 'cancels'
        viewAccount(accountNum);
    }

    // Deposit directly for testing
    bool deposit(int accountNum, double amount) {
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) {
            return false; // No account found or the amount is less than 0
        }
        // Deposit if it's clear
        accounts[index].savingsAccountBalance += round(amount * 100.0) / 100.0;
        return true;
    }

    // Withdraw directly for testing
    bool withdraw(int accountNum, double amount) {
        int index = searchAccount(accountNum); // Search for the actual account
        if (index == -1 || amount <= 0) return false;

        // Can't overdraw the account
        if (amount > accounts[index].savingsAccountBalance) {
            return false;
        }

        // Rounds the amount
        accounts[index].savingsAccountBalance -= round(amount * 100.0) / 100.0;
        return true;
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
            report << left << setw(20) << accounts[i].accountNumber << setw(20) << accounts[i].memberName << "$" << fixed << setprecision(2) << setw(19) << accounts[i].savingsAccountBalance << endl;
        }

        // File Name is given for them to find the actual File
        report.close();
        cout << "Report saved successfully to \"" << filename << "\".\n";
    }

    // The Most direct way to add an account, this will only be utilized for testing in #DEBUG
    bool addAccountTest(int accountNumber, const string& memberName, double balance) {
        if (accountCount >= max) {
            return false;  // No going over the max
        }
        if (balance <= 0) {
            return false; // No balance can be equal to or less than 0
        }

        // All we're doing is setting everything here
        accounts[accountCount].accountNumber = accountNumber;
        accounts[accountCount].memberName = memberName;
        accounts[accountCount].savingsAccountBalance = balance;
        accountCount++;
        return true;
    }

    // Public getter for testing
    int getAccountCount() const {
        return accountCount;
    }

    // SIMPLIFIED TESTING TRANSACTION, this is just to test my enumeration
    bool doTransaction(int accountNum, TransactionType type, double amount) {
        int index = searchAccount(accountNum); // Search accounts
        if (index == -1) return false; // If there is no account found then it fails

        if (amount <= 0) return false; // If there is a negative balance it fails

        if (type == Deposit) {
            accounts[index].savingsAccountBalance += amount;
            return true;
        }
        else if (type == Withdraw) {
            if (amount > accounts[index].savingsAccountBalance) return false;
            accounts[index].savingsAccountBalance -= amount;
            return true;
        }
        return false; // fallback (shouldn't happen)
    }


};

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// TEST CLASS METHODS: searchAccount & addAccountTest
TEST_CASE("Testing for Week 2") {
    Bank b;

    // Normal addAccountTest - Test 1
    CHECK(b.addAccountTest(10, "Alice", 100.0) == true); // Array Test 1
    CHECK(b.getAccountCount() == 1); // Calculation Test 1
    CHECK(b.searchAccount(10) == 0); // Calculation Test 2
    CHECK(b.deposit(10, 50) == true); // Calculation Test 3

    // Adding another account - Test 2
    CHECK(b.addAccountTest(25, "Bob", 50.0) == true); // Array Test 2
    CHECK(b.getAccountCount() == 2); // Calculation Test 4
    CHECK(b.searchAccount(25) == 1); // Calculation Test 5
    CHECK(b.withdraw(25, 25) == true); // Calculation test 6

    CHECK(b.searchAccount(26) == -1); // Calculation Test 7

    // Max Reached - Test 3
    for (int i = 0; i < 997; i++) {
        b.addAccountTest(100 + i, "Test", 1.0);
    }
    CHECK(b.addAccountTest(9999, "Overflow", 1.0) == false); // Array Test 3

    // Enumeration tests 1-3 utilizing the doTransaction function for Deposits & Withdraws
    CHECK(b.doTransaction(10, Bank::Deposit, 5) == true);
    CHECK(b.doTransaction(25, Bank::Withdraw, 5) == true);
    CHECK(b.doTransaction(10, Bank::Withdraw, 500) == false);
}
#endif


#ifndef _DEBUG
int main()
{
    // Declaring our 4 main inputs, choice for the menu, filename for the actual file, accountNum for the viewing accounts, bankManager to access the class
    int choice = 0;
    string filename;
    int accountNum;
    Bank bankManager;

    // If the choice isn't 5 it will continue to run until they enter 5
    while (choice != 5) {
        bankManager.displayMenu();
        cin >> choice;

        // No special characters are allowed, redo if such occurs
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: You can not use Special characters in this Menu\n";
            choice = 0;
            continue;
        }

        // Based on their choice, check through switch case what they want to do
        switch (choice)
        {
        case 1:
            // Call addAccount to make an account
            bankManager.addAccount();
            break;
        case 2:
            // Calls viewAccount to see account info
            cout << "Enter Account Number: ";
            cin >> accountNum;
            bankManager.viewAccount(accountNum);
            break;
        case 3:
            // Calls transaction to deposit/withdraw
            bankManager.transaction();
            break;
        case 4:
            // Saving an actual report of our accounts to a file
            cout << "Enter filename to save report: ";
            cin >> filename;
            bankManager.saveReport(filename);
            break;
        default:
            // If none of the above, try again
            cout << "Invalid choice. Try again.\n";
        }

    }
    // Exit Program
    cout << "Exiting program...\n";
}
#endif