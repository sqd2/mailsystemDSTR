//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#include "UserAuth.hpp"
#include "InboxOutbox.hpp"
#include "Email.hpp"
#include "SpamQueue.hpp"
#include "PriorityMail.hpp"

using namespace std;


int main() {
    UserAuth auth;
    EmailSystem emailSystem;
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
            cout << "4. Exit to Menu\n";
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
                case 4: {
                    SpamQueue spamQueue;
                    spamQueue.loadSpamWords("spam_words.txt"); // Load spam words from a file
                    spamQueue.readAndFilterEmails("emails.csv", auth.getEmail());
                    cout << "\n--- Spam Emails ---" << endl;
                    spamQueue.display();
                    break; }
                case 5: {
                    PriorityMail priorityMail;
                    priorityMail.readAndFilterEmails("emails.csv", auth.getEmail());
                    cout << "\n--- Priority Emails ---" << endl;
                    priorityMail.display();
                    break; }
                case '4':
                    emailMenuRunning = false;  //return to login
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        }
    }

    return 0;
}
