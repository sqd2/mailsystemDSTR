//SALEM AHMED ABDULLAH BA SUHAI - TP073526
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "Email.hpp"
#include "UserAuth.hpp"
using namespace std;

//stack for inbox 
template <typename T>
class MailStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* top;
    int size;

public:
    MailStack() : top(nullptr), size(0) {}
    
    ~MailStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = top;
        top = newNode;
        size++;
    }

    T pop() {
        if (isEmpty()) throw runtime_error("Stack is empty");
        Node* temp = top;
        T data = temp->data;
        top = top->next;
        delete temp;
        size--;
        return data;
    }

    bool isEmpty() const { return top == nullptr; }
    int getSize() const { return size; }
};

//queue for outbox
template <typename T>
class MailQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* front;
    Node* rear;
    int size;

public:
    MailQueue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~MailQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(const T& data) {
        Node* newNode = new Node(data);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    T dequeue() {
        if (isEmpty()) throw runtime_error("Queue is empty");
        Node* temp = front;
        T data = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        size--;
        return data;
    }

    bool isEmpty() const { return front == nullptr; }
    int getSize() const { return size; }
};

class EmailSystem {
private:
    string currentUser;
    MailStack<Email> inbox;
    MailQueue<Email> outbox;

    time_t getCurrentTimestamp() {
        return time(nullptr);
    }

    string parseQuotedField(const string line, size_t& pos) {
        // Skip initial quote
        pos++;
        size_t endQuote = line.find('\"', pos);
        if (endQuote == string::npos) return "";
        
        string result = line.substr(pos, endQuote - pos);
        pos = endQuote + 2; // Skip quote and comma
        return result;
    }

    void clearMailboxes() {
        // Clear inbox
        while (!inbox.isEmpty()) {
            inbox.pop();
        }
        // Clear outbox
        while (!outbox.isEmpty()) {
            outbox.dequeue();
        }
    }

    void loadEmails() {
        // Clear existing emails before loading, to prevent outbox conflict when multiple users sign in within same instance.
        clearMailboxes();
        
        ifstream file("emails.csv");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            size_t pos = 0;
            
            //get sender
            size_t comma = line.find(',', pos);
            if (comma == string::npos) continue;
            string sender = line.substr(pos, comma - pos);
            pos = comma + 1;

            //get recipient
            comma = line.find(',', pos);
            if (comma == string::npos) continue;
            string recipient = line.substr(pos, comma - pos);
            pos = comma + 1;

            // get subject thro quotes
            string subject = parseQuotedField(line, pos);
            if (pos == string::npos) continue;

            // get content thro quotes
            string content = parseQuotedField(line, pos);
            if (pos == string::npos) continue;

            // get time
            comma = line.find(',', pos);
            if (comma == string::npos) continue;
            string timestampStr = line.substr(pos, comma - pos);
            time_t timestamp = stoll(timestampStr);
            pos = comma + 1;

            // get priority
            string priorityStr = line.substr(pos);
            int priority = stoi(priorityStr);

            Email email(sender, recipient, subject, content, timestamp, priority);

            if (recipient == currentUser) {// Only add to inbox if user is recipient
                inbox.push(email);
            } else if (sender == currentUser) {// Only add to outbox if user is sender
                outbox.enqueue(email);
            }
        }
        file.close();
    }

    void saveEmail(const Email email) {
        ofstream file("emails.csv", ios::app);
        if (!file.is_open()) return;

        file << email.getSender() << ","
             << email.getRecipient() << ",\""
             << email.getSubject() << "\",\""
             << email.getContent() << "\","
             << email.getTime() << ","
             << email.getPriority() << endl;

        file.close();
    }

public:
    EmailSystem() {}

     void setCurrentUser(const string user) {
        currentUser = user;
        loadEmails();
    }

void viewInbox() {
    if (inbox.isEmpty()) {
        cout << "Inbox is empty." << endl;
        return;
    }

    MailStack<Email> tempStack; // holds emails for redisplaying after preview
    MailStack<Email> displayStack; // holds emails temporarily for displaying
    
    int count = 1;
    cout << "\n=== INBOX ===" << endl;

    // transfer emails to displayStack for orderly preview
    while (!inbox.isEmpty()) {
        displayStack.push(inbox.pop());
    }

    // Display emails with numbering for user choice
    while (!displayStack.isEmpty()) {
        Email email = displayStack.pop();
        cout << "\n[" << count++ << "] From: " << email.getSender() 
             << "\n    Subject: " << email.getSubject()
             << "\n    Date: " << email.getFormattedTime() << endl;
        tempStack.push(email); // Save into tempStack to re-push to inbox later
    }

    // Restore inbox content from tempStack
    while (!tempStack.isEmpty()) {
        inbox.push(tempStack.pop());
    }

    cout << "\nEnter email number to view (0 to cancel): ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice > 0 && choice < count) {
        // Move emails back to display selected one
        MailStack<Email> finalDisplayStack;
        count = 1;

        // Pop emails from inbox and find the selected email by number
        while (!inbox.isEmpty()) {
            Email email = inbox.pop();
            if (count == choice) {
                cout << "\n=== EMAIL DETAILS ===" << endl;
                cout << "From: " << email.getSender() 
                     << "\nSubject: " << email.getSubject()
                     << "\nDate: " << email.getFormattedTime()
                     << "\nPriority: " << email.getPriority()
                     << "\nContent: " << email.getContent() << endl;

                cout << "\nWould you like to reply? (y/n): ";
                char reply;
                cin >> reply;
                cin.ignore();

                if (reply == 'y' || reply == 'Y') {
                    string content;
                    string replySubject = "RE: " + email.getSubject();

                    cout << "Enter your reply: ";
                    getline(cin, content);

                    Email replyEmail(currentUser, 
                                   email.getSender(),
                                   replySubject,
                                   content,
                                   getCurrentTimestamp(),
                                   email.getPriority());

                    outbox.enqueue(replyEmail);
                    saveEmail(replyEmail);
                    cout << "Reply sent successfully!" << endl;
                }
            }
            finalDisplayStack.push(email);
            count++;
        }

        // Restore inbox content
        while (!finalDisplayStack.isEmpty()) {
            inbox.push(finalDisplayStack.pop());
        }
    }
}


    void viewOutbox() {
        if (outbox.isEmpty()) {
            cout << "Outbox is empty." << endl;
            return;
        }

        MailQueue<Email> temp;
        int count = 1;
        cout << "\n=== OUTBOX ===" << endl;
        
        while (!outbox.isEmpty()) {
            Email email = outbox.dequeue();
            cout << "\n[" << count++ << "] To: " << email.getRecipient() 
                 << "\n    Subject: " << email.getSubject()
                 << "\n    Date: " << email.getFormattedTime() << endl;
            temp.enqueue(email);
        }

        while (!temp.isEmpty()) {
            outbox.enqueue(temp.dequeue());
        }

        cout << "\nEnter email number to view (0 to cancel): ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice > 0 && choice < count) {
            MailQueue<Email> temp2;
            int current = 1;
            
            while (!outbox.isEmpty()) {
                Email email = outbox.dequeue();
                if (current == choice) {
                    cout << "\n=== EMAIL DETAILS ===" << endl;
                    cout << "To: " << email.getRecipient() 
                         << "\nSubject: " << email.getSubject()
                         << "\nDate: " << email.getFormattedTime()
                         << "\nPriority: " << email.getPriority()
                         << "\nContent: " << email.getContent() << endl;
                }
                temp2.enqueue(email);
                current++;
            }
            
            while (!temp2.isEmpty()) {
                outbox.enqueue(temp2.dequeue());
            }
        }
    }

 void sendEmail() {
    string recipient, subject, content;
    int priority;

    cout << "\n=== COMPOSE NEW EMAIL ===" << endl;

    // Ensure recipient is not empty
    do {
        cout << "To: ";
        getline(cin, recipient);
        if (recipient.empty()) {
            cout << "Recipient cannot be empty. Please enter a valid recipient.\n";
        }
    } while (recipient.empty());

    // Ensure subject is not empty
    do {
        cout << "Subject: ";
        getline(cin, subject);
        if (subject.empty()) {
            cout << "Subject cannot be empty. Please enter a subject.\n";
        }
    } while (subject.empty());

    // Ensure content is not empty
    do {
        cout << "Content: ";
        getline(cin, content);
        if (content.empty()) {
            cout << "Content cannot be empty. Please enter the email content.\n";
        }
    } while (content.empty());

    // Validate priority
    cout << "Priority (1 for normal priority - 2 for high priority): ";
    cin >> priority;
    cin.ignore();
    if (priority < 1 || priority > 2) {
        cout << "Invalid priority. Setting to default (1)." << endl;
        priority = 1;
    }

    // create and enqueue the email
    Email email(currentUser, recipient, subject, content, getCurrentTimestamp(), priority);
    outbox.enqueue(email);
    saveEmail(email);
    cout << "Email sent successfully!" << endl;
}
};


