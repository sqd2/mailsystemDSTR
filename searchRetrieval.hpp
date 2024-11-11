//MOHAMMED HAFEEL RUSKI - TP070810
#ifndef SEARCHRETRIEVAL_H
#define SEARCHRETRIEVAL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "Email.hpp"

using namespace std;


// Stack implementation using linked list
class Stack {
private:
    struct Node {
        Email email;
        Node* next;
        Node(const Email& e) : email(e), next(nullptr) {}
    };

    Node* top;

public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const Email& email) {
        Node* newNode = new Node(email);
        newNode->next = top;
        top = newNode;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    Email pop() {
        if (isEmpty()) throw out_of_range("Stack is empty.");
        Node* temp = top;
        Email email = top->email;
        top = top->next;
        delete temp;
        return email;
    }

    Email peek() const {
        if (isEmpty()) throw out_of_range("Stack is empty.");
        return top->email;
    }
};

// Queue implementation using linked list
class Queue {
private:
    struct Node {
        Email email;
        Node* next;
        Node(const Email& e) : email(e), next(nullptr) {}
    };

    Node* front;
    Node* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(const Email& email) {
        Node* newNode = new Node(email);
        if (rear) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    Email dequeue() {
        if (isEmpty()) throw out_of_range("Queue is empty.");
        Node* temp = front;
        Email email = front->email;
        front = front->next;
        if (!front) rear = nullptr;  // Queue is empty now
        delete temp;
        return email;
    }

    Email peek() const {
        if (isEmpty()) throw out_of_range("Queue is empty.");
        return front->email;
    }
};



class SearchRetrieval {
private:
    Email* emailArray;       // Dynamic array of emails
    int emailCount;          // Number of emails in the array
    bool emailsLoaded;       // Flag to check if emails are loaded

public:
    SearchRetrieval() : emailArray(nullptr), emailCount(0), emailsLoaded(false) {}

    ~SearchRetrieval() {
        delete[] emailArray;
    }

    // Load emails from CSV file into an array
    void loadEmailsFromCSV(const string& filename = "emails.csv") {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        emailCount = 0;

        // Skip header row
        getline(file, line); // Read and discard the header row

        // Debug: Initial message
        cout << "Loading emails..." << endl;

        // Count remaining lines to determine email count
        while (getline(file, line)) emailCount++;
        file.clear();
        file.seekg(0);

        // Skip header again before actual loading
        getline(file, line); 

        // Allocate memory for the email array
        emailArray = new Email[emailCount];
        int index = 0;

        // Parse each line to create Email objects
        while (getline(file, line)) {
            istringstream ss(line);
            string sender, recipient, subject, content, timeStr, priorityStr;
            bool insideQuotes = false;
            string currentField;

            for (char ch : line) {
                if (ch == '"') {
                    insideQuotes = !insideQuotes;
                } else if (ch == ',' && !insideQuotes) {
                    if (sender.empty()) {
                        sender = currentField;
                    } else if (recipient.empty()) {
                        recipient = currentField;
                    } else if (subject.empty()) {
                        subject = currentField;
                    } else if (content.empty()) {
                        content = currentField;
                    } else if (timeStr.empty()) {
                        timeStr = currentField;
                    } else {
                        priorityStr = currentField;
                    }
                    currentField.clear();
                } else {
                    currentField += ch;
                }
            }
            if (priorityStr.empty()) {
                priorityStr = currentField;
            }

            try {
                time_t timestamp = stoi(timeStr);
                int priority = stoi(priorityStr);
                emailArray[index++] = Email(sender, recipient, subject, content, timestamp, priority);
            } catch (const exception& e) {
                cerr << "Error parsing line: " << line << endl;
                cerr << "Exception: " << e.what() << endl;
                continue;
            }
        }

        emailsLoaded = true;
        file.close();

    }



    // Display search menu to user and perform the selected search
    void searchMenu(const string& userEmail) {
        if (!emailsLoaded) {
            loadEmailsFromCSV();  // Automatically load emails from default file if not already loaded
        }
        while (true) {
            cout << "\n--- Email Search Menu ---" << endl;
            cout << "1. Display emails chronologically (oldest to latest)" << endl;
            cout << "2. Display emails in reverse chronological order (latest to oldest)" << endl;
            cout << "3. Search emails by sender" << endl;
            cout << "4. Search emails by recipient" << endl;
            cout << "5. Search emails by priority (0 for spam, 1 for normal, 2 for urgent)" << endl;
            cout << "6. Search emails by keyword in subject or content" << endl;
            cout << "7. Exit search menu" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    displayChronological(userEmail);
                    break;
                case 2:
                    displayReverseChronological(userEmail);
                    break;
                case 3: {
                    searchBySender(userEmail);
                    break;
                }
                case 4: {
                    cout << "Enter recipient email to search: ";
                    string recipient;
                    getline(cin, recipient);
                    searchByRecipient(recipient);
                    break;
                }
                case 5: {
                    cout << "Enter priority level (0 for spam, 1 for normal, 2 for urgent): ";
                    int priorityLevel;
                    cin >> priorityLevel;
                    searchByPriority(priorityLevel, userEmail);
                    break;
                }
                case 6: {
                    cout << "Enter keyword to search in subject/content: ";
                    string keyword;
                    getline(cin, keyword);
                    searchByKeyword(keyword, userEmail);
                    break;
                }
                case 7:
                    cout << "Exiting search menu." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }


    // Function to display emails in chronological order
    void displayChronological(const string& userEmail) {
        Queue chronologicalQueue;  // Create a queue to hold the emails for chronological order

        // Enqueue all emails from or to the logged-in user
        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].getSender() == userEmail || emailArray[i].getRecipient() == userEmail) {
                chronologicalQueue.enqueue(emailArray[i]);
            }
        }

        int count = 0;
        while (!chronologicalQueue.isEmpty()) {
            Email email = chronologicalQueue.dequeue();
            displayEmail(email);
            count++;

            if (count % 5 == 0) {
                char choice;
                cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
                cin >> choice;
                if (choice == 'X' || choice == 'x') break;
            }
        }
    }

    void displayReverseChronological(const string& userEmail) {
        Stack reverseChronologicalStack;  // Create a stack to hold the emails in reverse order
    // Push all emails from or to the logged-in user onto the stack
            for (int i = 0; i < emailCount; ++i) {
                if (emailArray[i].getSender() == userEmail || emailArray[i].getRecipient() == userEmail) {
                    reverseChronologicalStack.push(emailArray[i]);
                }
        }

    int count = 0;
    while (!reverseChronologicalStack.isEmpty()) {
        Email email = reverseChronologicalStack.pop();
        displayEmail(email);
        count++;

        if (count % 5 == 0) {
            char choice;
            cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
            cin >> choice;
            if (choice == 'X' || choice == 'x') break;
        }
    }
}

    // Search emails by sender
    void searchBySender(const string& userEmail) {
        int count = 0;

        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].getSender() == userEmail || emailArray[i].getRecipient() == userEmail) {
                displayEmail(emailArray[i]);
                count++;

                if (count % 5 == 0) {
                    char choice;
                    cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
                    cin >> choice;
                    if (choice == 'X' || choice == 'x') break;
                }
            }
        }
    }


    // Search emails by recipient
void searchByRecipient(const string& recipient) {
    int count = 0;

    for (int i = 0; i < emailCount; ++i) {
        if (emailArray[i].getRecipient() == recipient) {  
            displayEmail(emailArray[i]);
            count++;

            if (count % 5 == 0) {
                char choice;
                cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
                cin >> choice;
                if (choice == 'X' || choice == 'x') break;
            }
        }
    }
}

    // Search emails by priority (1 for urgent, 0 for spam)
    void searchByPriority(int priorityLevel, const string& userEmail) {
        int count = 0;

        for (int i = 0; i < emailCount; ++i) {
            // Only show emails that match the priority and are from or to the logged-in user
            if (emailArray[i].getPriority() == priorityLevel && 
                (emailArray[i].getSender() == userEmail || emailArray[i].getRecipient() == userEmail)) {
                displayEmail(emailArray[i]);
                count++;

                if (count % 5 == 0) {
                    char choice;
                    cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
                    cin >> choice;
                    if (choice == 'X' || choice == 'x') break;
                }
            }
        }
    }


    // Function to search for emails by keyword
    void searchByKeyword(const string& keyword, const string& userEmail) {
        int count = 0;

        for (int i = 0; i < emailCount; ++i) {
            // Only show emails that contain the keyword and are from or to the logged-in user
            if ((emailArray[i].getSubject().find(keyword) != string::npos ||
                emailArray[i].getContent().find(keyword) != string::npos) && 
                (emailArray[i].getSender() == userEmail || emailArray[i].getRecipient() == userEmail)) {
                displayEmail(emailArray[i]);
                count++;

                if (count % 5 == 0) {
                    char choice;
                    cout << "\nPress 'N' to view the next 5 emails or 'X' to exit: ";
                    cin >> choice;
                    if (choice == 'X' || choice == 'x') break;
                }
            }
        }
    }


private:
    // Display a single email's details
    void displayEmail(const Email& email) {
        cout << "---------------------------" << endl;
        cout << "Sender: " << email.getSender() << endl;
        cout << "Recipient: " << email.getRecipient() << endl;
        cout << "Subject: " << email.getSubject() << endl;
        cout << "Content: " << email.getContent() << endl;
        cout << "Timestamp: " << email.getFormattedTime() << endl;
        // Convert priority level to text
        string priorityText;
        switch (email.getPriority()) {
            case 0: priorityText = "Spam"; break;
            case 1: priorityText = "Normal"; break;
            case 2: priorityText = "Urgent"; break;
            default: priorityText = "Unknown"; break;
        }
        cout << "Priority: " << priorityText << endl;
        cout << "---------------------------" << endl;
    }
};

#endif