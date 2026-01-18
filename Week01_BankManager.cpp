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
                // Most banks have minimums (0 to 5 dollars), for this instance you need 1 Dollar
                if (accounts[accountCount].savingsAccountBalance <= 1) {
                    error = true;
                    cout << "! Error: The Balance may not be equal to or less than $1.00 Dollar !\n";
                }
            }
        }
        // Success, and our 'total' was increased in accountCount
        cout << "Account added successfully!\n";
        accountCount += 1;
        return;
    }

    void viewAccount(int accountNum) {
        // Account display
        bool found = false;
        cout << "\n==================================================\n";
        cout << setw(28) << "Account\n";
        cout << "==================================================\n";

        cout << left
            << setw(20) << "Account#"
            << setw(20) << "Member Name"
            << setw(20) << "Balance" << endl;

        cout << setfill('-') << setw(50) << "-" << endl;
        cout << setfill(' ');

        // Go through the accounts, find the same account number and display it's information
        for (int i = 0; i < accountCount; i++) {
            if (accountNum == accounts[i].accountNumber) {
                cout << left
                    << setw(20) << accounts[i].accountNumber
                    << setw(20) << accounts[i].memberName
                    << "$" << fixed << setprecision(2)
                    << setw(20) << accounts[i].savingsAccountBalance
                    << endl;
                cout << right << setfill(' ');
                bool found = true;
            }
        }

        // If it's not found, output such
        if (!found) {
            cout << "! There was no account that was found !\n";
            cout << right << setfill(' ');
        }
    }

    // Enumeration for Deposit/Withdraws
    enum TransactionType { Deposit = 1, Withdraw = 2 };

    void transaction() {
        // Account Number is needed for it to occur
        int accountNum;
        cout << "Enter the account number for this transaction: ";
        cin >> accountNum;

        // Account is searched for, once found it is indexxed to be used for the transaction
        int index = -1;
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].accountNumber == accountNum) {
                index = i;
                break;
            }
        }

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

        // Adds the ammount to the account as a Deposit
        if (type == Deposit) {
            accounts[index].savingsAccountBalance += amount;
            cout << "Deposited $" << fixed << setprecision(2) << amount << " to account #" << accounts[index].accountNumber << ".\n";
        }
        else if (type == Withdraw) {
            if (amount > accounts[index].savingsAccountBalance) {
                // We can not overdraw funds at this bank, thus a Withdrawal can not occur
                cout << "Insufficient funds for withdrawal.\n";
            }
            else {
                // Withdrawal goes through and 
                accounts[index].savingsAccountBalance -= amount;
                cout << "Withdrew $" << fixed << setprecision(2) << amount << " from account #" << accounts[index].accountNumber << ".\n";
            }
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
            report << left << setw(20) << accounts[i].accountNumber << setw(20) << accounts[i].memberName << "$" << fixed << setprecision(2) << setw(19) << accounts[i].savingsAccountBalance << endl;
        }

        // File Name is given for them to find the actual File
        report.close();
        cout << "Report saved successfully to \"" << filename << "\".\n";
    }
};


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