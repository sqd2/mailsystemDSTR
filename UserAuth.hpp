//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#ifndef USERAUTH_HPP
#define USERAUTH_HPP

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// User Authentication class
class UserAuth {
private:
    string email;
    string password;

    bool checkCredentials(const string email, const string password) {
        ifstream file("users.txt");
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos == string::npos) continue;
            
            string storedEmail = line.substr(0, pos);
            string storedPass = line.substr(pos + 1);
            
            if (email == storedEmail && password == storedPass) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

public:
    bool login() {
        string emailInput, passInput;
        
        cout << "Enter email: ";
        getline(cin, emailInput);
        cout << "Enter password: ";
        getline(cin, passInput);

        if (checkCredentials(emailInput, passInput)) {
            email = emailInput;
            password = passInput;
            return true;
        }
        return false;
    }

    bool registerUser() {
        string emailInput, passInput;
        
        cout << "Enter new email: ";
        getline(cin, emailInput);
        cout << "Enter new password: ";
        getline(cin, passInput);

        ofstream file("users.txt", ios::app);
        if (!file.is_open()) return false;

        file << emailInput << "," << passInput << endl;
        file.close();

        email = emailInput;
        password = passInput;
        return true;
    }

    string getEmail() const {
        return email; 
    }
};
#endif