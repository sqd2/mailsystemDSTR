// SearchRetrieval.h
#ifndef SEARCHRETRIEVAL_H
#define SEARCHRETRIEVAL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

// Email structure to hold each email's data
struct Email {
    string sender;
    string recipient;
    string subject;
    string content;
    time_t timestamp;
    int priority;

    // Default constructor
    Email() : sender(""), recipient(""), subject(""), content(""), timestamp(0), priority(0) {}


    Email(string snd, string rcp, string sub, string cnt, time_t ts, int prio) 
        : sender(snd), recipient(rcp), subject(sub), content(cnt), timestamp(ts), priority(prio) {}
};

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

public:
    SearchRetrieval() : emailArray(nullptr), emailCount(0) {}

    ~SearchRetrieval() {
        delete[] emailArray;
    }

    // Load emails from CSV file into an array
    void loadEmailsFromCSV(const string& filename) {
        ifstream file(filename);
        string line;
        emailCount = 0;

        // Count lines to determine array size
        while (getline(file, line)) emailCount++;
        file.clear();
        file.seekg(0);

        // Allocate email array
        emailArray = new Email[emailCount];
        int index = 0;

        // Read and parse each line from CSV
        while (getline(file, line)) {
            istringstream ss(line);
            string sender, recipient, subject, content, timeStr, priorityStr;
            bool insideQuotes = false;
            string currentField;

            // Read each character from the line manually
            for (char ch : line) {
                if (ch == '"') {
                    // Toggle insideQuotes flag when encountering a quote
                    insideQuotes = !insideQuotes;
                } else if (ch == ',' && !insideQuotes) {
                    // If not inside quotes, we have a field delimiter
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
                    currentField.clear(); // Reset for the next field
                } else {
                    currentField += ch; // Add character to the current field
                }
            }
            // Add the last field (priority)
            if (priorityStr.empty()) {
                priorityStr = currentField;
            }

            // Attempt to convert time and priority, with error handling
            try {
                time_t timestamp = stoi(timeStr);  // Convert timestamp to time_t
                int priority = stoi(priorityStr);  // Convert priority to integer

                // Add the email to the array
                emailArray[index++] = Email(sender, recipient, subject, content, timestamp, priority);
            } catch (const invalid_argument& e) {
                // Handle the invalid argument error (e.g., empty or malformed input)
                cerr << "Error parsing line: " << line << endl;
                cerr << "Invalid argument: " << e.what() << endl;
                continue;  // Skip this invalid line and continue processing the next one
            } catch (const out_of_range& e) {
                // Handle the out-of-range error if the number is too large
                cerr << "Error parsing line: " << line << endl;
                cerr << "Out of range error: " << e.what() << endl;
                continue;  // Skip this invalid line and continue processing the next one
            }
        }
        file.close();
    }


    // Display search menu to user and perform the selected search
    void searchMenu() {
        while (true) {
            cout << "\n--- Email Search Menu ---" << endl;
            cout << "1. Display emails chronologically (oldest to latest)" << endl;
            cout << "2. Display emails in reverse chronological order (latest to oldest)" << endl;
            cout << "3. Search emails by sender" << endl;
            cout << "4. Search emails by recipient" << endl;
            cout << "5. Search emails by priority (1 for urgent, 0 for spam)" << endl;
            cout << "6. Search emails by keyword in subject or content" << endl;
            cout << "7. Exit search menu" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cin.ignore(); // Ignore leftover newline character

            switch (choice) {
                case 1:
                    displayChronological();
                    break;
                case 2:
                    displayReverseChronological();
                    break;
                case 3: {
                    cout << "Enter sender email to search: ";
                    string sender;
                    getline(cin, sender);
                    searchBySender(sender);
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
                    cout << "Enter priority level (1 for urgent, 0 for spam): ";
                    int priorityLevel;
                    cin >> priorityLevel;
                    searchByPriority(priorityLevel);
                    break;
                }
                case 6: {
                    cout << "Enter keyword to search in subject/content: ";
                    string keyword;
                    getline(cin, keyword);
                    searchByKeyword(keyword);
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

    // Display emails in a queue (chronological order: oldest to latest)
    void displayChronological() {
        Queue chronologicalQueue;

        for (int i = 0; i < emailCount; ++i) {
            chronologicalQueue.enqueue(emailArray[i]);
        }

        while (!chronologicalQueue.isEmpty()) {
            displayEmail(chronologicalQueue.dequeue());
        }
    }

    // Display emails in reverse chronological order (latest to oldest)
    void displayReverseChronological() {
        Stack reverseChronologicalStack;

        for (int i = 0; i < emailCount; ++i) {
            reverseChronologicalStack.push(emailArray[i]);
        }

        while (!reverseChronologicalStack.isEmpty()) {
            displayEmail(reverseChronologicalStack.pop());
        }
    }

    // Search emails by sender
    void searchBySender(const string& sender) {
        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].sender == sender) {
                displayEmail(emailArray[i]);
            }
        }
    }

    // Search emails by recipient
    void searchByRecipient(const string& recipient) {
        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].recipient == recipient) {
                displayEmail(emailArray[i]);
            }
        }
    }

    // Search emails by priority (1 for urgent, 0 for spam)
    void searchByPriority(int priorityLevel) {
        Stack priorityStack;

        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].priority == priorityLevel) {
                priorityStack.push(emailArray[i]);
            }
        }

        while (!priorityStack.isEmpty()) {
            displayEmail(priorityStack.pop());
        }
    }

    // Search emails by a keyword in the subject or content
    void searchByKeyword(const string& keyword) {
        for (int i = 0; i < emailCount; ++i) {
            if (emailArray[i].subject.find(keyword) != string::npos || 
                emailArray[i].content.find(keyword) != string::npos) {
                displayEmail(emailArray[i]);
            }
        }
    }

private:
    // Display a single email's details
    void displayEmail(const Email& email) {
        cout << "Sender: " << email.sender << endl;
        cout << "Recipient: " << email.recipient << endl;
        cout << "Subject: " << email.subject << endl;
        cout << "Content: " << email.content << endl;
        cout << "Timestamp: " << ctime(&email.timestamp);
        cout << "Priority: " << (email.priority == 1 ? "Urgent" : "Spam") << endl;
        cout << "---------------------------" << endl;
    }
};

#endif
