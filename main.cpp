//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#include "UserAuth.hpp"
#include "InboxOutbox.hpp"
#include "Email.hpp"
#include "SpamQueue.hpp"
#include "PriorityMail.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



int main() {
    UserAuth auth;
    EmailSystem emailSystem;
    PriorityMail priorityMail;
    SpamQueue spamQueue;


    char choice;
    bool programRunning = true;
    
    while (programRunning) {
        bool authenticated = false;
        
        //login menu
        while (!authenticated) {
            cout << "=== Welcome to Email System ===" << endl;
            cout << "1. Login\n2. Register\n3. Exit\nChoice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case '1':
                    authenticated = auth.login();
                    break;
                case '2':
                    authenticated = auth.registerUser();
                    break;
                case '3':
                     cout << "terminate" << endl;
                    return 0; //terminate
                default:
                    cout << "Invalid choice!" << endl;
                    continue;
            }

            if (!authenticated) {
                cout << "Authentication failed!" << endl;
            }
        }
        
        cout << "\nWelcome, " << auth.getEmail() << "!" << endl;
        emailSystem.setCurrentUser(auth.getEmail());

        //email system menu
        bool emailMenuRunning = true;
        while (emailMenuRunning) {
            cout << "\n=== Email System Menu ===" << endl;
            cout << "1. View Inbox\n";
            cout << "2. View Outbox\n";
            cout << "3. Send Email\n";
            cout << "4. Spam Inbox\n";
            cout << "5. Priority Mail\n";
            cout << "6. Exit to Menu\n";
            cout << "Choice: ";
            
            cin >> choice;
            cin.ignore();

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
                    cout << "\n--- Spam Emails ---" << endl;
                    if (spamQueue.isEmpty()) {
                        cout << "No spam emails found." << endl;
                    } else {
                        spamQueue.display();
                    }
                    break; 
                case '5': 
                    cout << "\n--- Priority Emails ---" << endl;
                    if (priorityMail.isEmpty()) {
                        cout << "No priority emails found." << endl;
                    } else {
                        priorityMail.display();
                    }
                    break;
                case '6':
                    emailMenuRunning = false;  //return to login
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        }
    }

    return 0;
}
