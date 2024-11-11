//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#include "UserAuth.hpp"
#include "InboxOutbox.hpp"
#include "Email.hpp"
#include "searchRetrieval.hpp"
#include "SpamQueue.hpp"
#include "PriorityMail.hpp"
#include "Email.hpp"

using namespace std;

char validChoice() {
    char input[256];
    char ch;
    int i = 0;
    while ((ch = cin.get()) != '\n' && i < 255) { //read characters until newline
        input[i++] = ch;
    }
    input[i] = '\0';  //null-terminate
    //check for input being a char
    if (i != 1) {
        return '\0';  //return null for invalid
    }
    return input[0];
}

int main() {
    UserAuth auth;
    EmailSystem emailSystem;
    PriorityMail priorityMail;
    SpamQueue spamQueue;
    SearchRetrieval search;
    char choice;
    bool programRunning = true;

    spamQueue.loadSpamWords("spam_words.txt");
    
    while (programRunning) {
        bool authenticated = false;
        
        //login menu
        while (!authenticated) {
            cout << "=== Welcome to Email System ===" << endl;
            cout << "1. Login\n2. Register\n3. Exit\nChoice: ";
            
            choice = validChoice();

            if (choice == '\0') {
                cout << "Invalid input! Please enter a single character (1-3)." << endl;
                continue;
            }

            switch (choice) {
                case '1':
                    authenticated = auth.login();
                    break;
                case '2':
                    authenticated = auth.registerUser();
                    break;
                case '3':
                    cout << "terminating..." << endl;
                    return 0; //terminate
                default:
                    cout << "Invalid choice! Please enter 1, 2, or 3." << endl;
                    continue;
            }

            if (!authenticated) {
                cout << "Authentication failed!" << endl;
            }
        }
        
        cout << "\nWelcome, " << auth.getEmail() << "!" << endl;
        emailSystem.setCurrentUser(auth.getEmail());
        spamQueue.readAndFilterEmails("emails.csv", auth.getEmail());
        priorityMail.readAndFilterEmails("emails.csv", auth.getEmail());


        //email system menu
        bool emailMenuRunning = true;
        while (emailMenuRunning) {
            cout << "\n=== Email System Menu ===" << endl;
            cout << "1. View Inbox\n";
            cout << "2. View Outbox\n";
            cout << "3. Send Email\n";
            cout << "4. View Spam\n";
            cout << "5. View Priority\n";
            cout << "6. Search and retreive Email\n";
            cout << "7. Exit to Menu\n";
            cout << "Choice: ";
            
            choice = validChoice();

            if (choice == '\0') {
                cout << "Invalid input! Please enter a single character (1-4)." << endl;
                continue;
            }

            switch (choice) {
                case '1':
                    emailSystem.viewInbox();
                    break;
                case '2':
                    emailSystem.viewOutbox();
                    break;
                case '3':
                    emailSystem.sendEmail();
                    break;
                case '4': 
                    spamQueue.display();
                    break;
                case '5': 
                    priorityMail.display();
                    break;
                case '6':
                    search.searchMenu(auth.getEmail());
                    break;
                case '7':
                    emailMenuRunning = false;  //return to login
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }

    return 0;
}