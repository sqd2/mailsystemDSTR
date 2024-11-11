//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#include "UserAuth.hpp"
//#include "InboxOutbox.hpp"
#include "Email.hpp"
#include "SearchRetrieval.h"


using namespace std;


int main() {
    UserAuth auth;
    
    //EmailSystem emailSystem;
    SearchRetrieval search;
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
        //emailSystem.setCurrentUser(auth.getEmail());

        //email system menu
        bool emailMenuRunning = true;
        while (emailMenuRunning) {
            cout << "\n=== Email System Menu ===" << endl;
            cout << "1. View Inbox\n";
            cout << "2. View Outbox\n";
            cout << "3. Send Email\n";
            cout << "4. Search and retreive Email\n";
            cout << "5. Exit to Menu\n";
            cout << "Choice: ";
            
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case '1':
                    //emailSystem.viewInbox();
                    break;
                case '2':
                    //emailSystem.viewOutbox();
                    break;
                case '3':
                    //emailSystem.sendEmail();
                    break;
                case '4':
                    search.searchMenu(auth.getEmail());
                    break;
                case '5':
                    emailMenuRunning = false;  //return to login
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        }
    }

    return 0;
}
