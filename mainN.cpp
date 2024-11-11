// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include "SpamQueue.hpp"
#include "PriorityMail.hpp"

// #include "InboxOutbox.h"
// #include "SpamPriority.h"
// #include "SearchRetrieval.h"

using namespace std;

// Function declarations
void login();
void signup();
void showMenu();
bool authenticateUser(const string& email, const string& password);
string currentRecipient;

int main() {
    cout << "=== MQ Email System ===" << endl;
    cout << "1. Login" << endl;
    cout << "2. Sign Up" << endl;
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            signup();
            break;
        default:
            cout << "Invalid choice, exiting." << endl;
            return 0;
    }

    return 0;
}

void login() {
    string email, password;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    if (authenticateUser(email, password)) {
        cout << "Login successful!" << endl;

        currentRecipient = email;
        // return currentRecipient;

        // Initialize email management components
        // InboxOutbox inboxOutbox;
        // SpamPriority spamPriority;
        // SearchRetrieval searchRetrieval;

        showMenu();
    } else {
        cout << "Invalid credentials. Try again." << endl;
    }
}

void signup() {
    string email, password;
    cout << "Enter new email: ";
    cin >> email;
    cout << "Enter new password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    file << email << " " << password << endl;
    file.close();

    cout << "Sign up successful. Please login now." << endl;
    login();
}

bool authenticateUser(const string& email, const string& password) {
    ifstream file("users.txt");
    string storedEmail, storedPassword;

    while (file >> storedEmail >> storedPassword) {
        if (email == storedEmail && password == storedPassword) {
            return true;
        }
    }

    return false;
}

void showMenu() {
    while (true) {

        cout << "\n--- Email System Menu ---" << endl;
        cout << currentRecipient << endl;

        cout << "1. Inbox" << endl;
        cout << "2. Send Mail" << endl;
        cout << "3. Outbox" << endl;
        cout << "4. Spam" << endl;
        cout << "5. Priority Mail" << endl;
        cout << "6. Search Emails" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";


        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                // TODO
                break;
            case 2:
                // TODO
                break;
            case 3:
                // TODO
                break;
            case 4: {
                SpamQueue spamQueue;
                spamQueue.loadSpamWords("spam_words.txt"); // Load spam words from a file
                spamQueue.readAndFilterEmails("emails.csv", currentRecipient);
                cout << "\n--- Spam Emails ---" << endl;
                spamQueue.display();
                break;
            }
            case 5: {
                PriorityMail priorityMail;
                priorityMail.readAndFilterEmails("emails.csv", currentRecipient);
                cout << "\n--- Priority Emails ---" << endl;
                priorityMail.display();
                break;
            }
            case 6:
                // TODO
                break;
            case 7:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice." << endl;
        }
    }
}
