#ifndef PRIORITY_MAIL_HPP
#define PRIORITY_MAIL_HPP

#include "Email.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct PriorityNode {
    Email email;
    PriorityNode* next;

    PriorityNode(const Email& e) : email(e), next(nullptr) {}
};

class PriorityMail {
private:
    PriorityNode* head;

public:
    PriorityMail() : head(nullptr) {}

    ~PriorityMail() {
        while (head != nullptr) {
            PriorityNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Only enqueue emails with priority 2
    void enqueue(const Email& email) {
        if (email.getPriority() != 2) {
            return;  // Ignore emails with priority other than 2
        }

        PriorityNode* newNode = new PriorityNode(email);
        if (head == nullptr || email.getPriority() > head->email.getPriority()) {
            newNode->next = head;
            head = newNode;
        } else {
            PriorityNode* current = head;
            while (current->next != nullptr && current->next->email.getPriority() >= email.getPriority()) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        cout << "[DEBUG] Enqueued priority email from " << email.getSender() << " to " << email.getRecipient() << endl;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    Email dequeue() {
        if (head == nullptr) {
            throw runtime_error("PriorityMail is empty");
        }
        PriorityNode* temp = head;
        Email email = head->email;
        head = head->next;
        delete temp;
        return email;
    }

    void readAndFilterEmails(const string& filename, const string& currentRecipient) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Unable to open the file.");
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string sender, recipient, subject, content;
            time_t timestamp;
            int priority;

            getline(ss, sender, ',');
            getline(ss, recipient, ',');
            getline(ss, subject, ',');
            getline(ss, content, ',');
            ss >> timestamp;
            ss.ignore(1); // Skip the comma
            ss >> priority;

            // Only enqueue emails for the current recipient with priority 2
            if (recipient == currentRecipient && priority == 2) {
                enqueue(Email(sender, recipient, subject, content, timestamp, priority));
            }
        }
        file.close();
    }

    void display() const {
        if (isEmpty()) {
            cout << "No priority emails found." << endl;
            return;
        }
        
        PriorityNode* current = head;
        cout << "\n--- Priority Emails ---" << endl;
        while (current != nullptr) {
            const Email& email = current->email;
            cout << "From: " << email.getSender()
                 << ", Subject: " << email.getSubject()
                 << ", Priority: " << email.getPriority()
                 << ", Time: " << email.getFormattedTime() << endl;
            current = current->next;
        }
    }
};

#endif // PRIORITY_MAIL_HPP
