// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include "InboxOutbox.h"
#include "SpamPriority.h"
#include "SearchRetrieval.h"

using namespace std;

// Function declarations
void login();
void signup();
void showMenu(InboxOutbox& inboxOutbox, SpamPriority& spamPriority, SearchRetrieval& searchRetrieval);
bool authenticateUser(const string& email, const string& password);

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

        // Initialize email management components
        InboxOutbox inboxOutbox;
        SpamPriority spamPriority;
        SearchRetrieval searchRetrieval;

        showMenu(inboxOutbox, spamPriority, searchRetrieval);
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

void showMenu(InboxOutbox& inboxOutbox, SpamPriority& spamPriority, SearchRetrieval& searchRetrieval) {
    while (true) {
        cout << "\n--- Email System Menu ---" << endl;
        cout << "1. Inbox" << endl;
        cout << "2. Send Mail" << endl;
        cout << "3. Outbox" << endl;
        cout << "4. Spam" << endl;
        cout << "5. Priority Mail" << endl;
        cout << "6. Search Emails" << endl;
        cout << "7. Logout" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                inboxOutbox.displayInbox();  // Inbox management
                break;
            case 2:
                inboxOutbox.sendMail();      // Outbox management
                break;
            case 3:
                inboxOutbox.displayOutbox(); // Outbox management
                break;
            case 4:
                spamPriority.displaySpam();  // Spam handling
                break;
            case 5:
                spamPriority.displayPriorityMails(); // Priority handling
                break;
            case 6:
                searchRetrieval.searchEmails(); // Search and Retrieval
                break;
            case 7:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid choice." << endl;
        }
    }
}
