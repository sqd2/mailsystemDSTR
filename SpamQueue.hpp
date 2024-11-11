#ifndef SPAM_QUEUE_HPP
#define SPAM_QUEUE_HPP

#include "Email.hpp"
#include <fstream>
#include <sstream>

using namespace std;

struct SpamNode {
    Email email;
    SpamNode* next;

    SpamNode(const Email& e) : email(e), next(nullptr) {}
};

class SpamQueue {
private:
    SpamNode* front;
    SpamNode* rear;
    SpamNode* spamWordsHead; // Linked list to store spam words

public:
    SpamQueue() : front(nullptr), rear(nullptr), spamWordsHead(nullptr) {}

    ~SpamQueue() {
        // Clean up email queue
        while (front != nullptr) {
            SpamNode* temp = front;
            front = front->next;
            delete temp;
        }

        // Clean up spam words linked list
        while (spamWordsHead != nullptr) {
            SpamNode* temp = spamWordsHead;
            spamWordsHead = spamWordsHead->next;
            delete temp;
        }
    }

    void loadSpamWords(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Unable to open the spam words file.");
        }

        string word;
        while (getline(file, word)) {
            SpamNode* newNode = new SpamNode(Email("", "", word, "", 0, 0)); // Store word in `subject` field
            if (spamWordsHead == nullptr) {
                spamWordsHead = newNode;
            } else {
                SpamNode* current = spamWordsHead;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newNode;
            }
        }
        file.close();
    }

    void enqueue(const Email& email) {
        SpamNode* newNode = new SpamNode(email);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    Email dequeue() {
        if (front == nullptr) {
            throw runtime_error("SpamQueue is empty");
        }
        SpamNode* temp = front;
        Email email = front->email;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return email;
    }

    bool containsSpamWords(const string& text) const {
        SpamNode* current = spamWordsHead;
        while (current != nullptr) {
            // Check if the text contains the spam word stored in `subject` of SpamNode
            if (text.find(current->email.getSubject()) != string::npos) {
                return true;
            }
            current = current->next;
        }
        return false;
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

            // Check if the recipient matches and if the subject or content contains spam words
            if (recipient == currentRecipient && (containsSpamWords(subject) || containsSpamWords(content))) {
                enqueue(Email(sender, recipient, subject, content, timestamp, priority));
            }
        }
        file.close();
    }

    void display() const {
        SpamNode* current = front;
        while (current != nullptr) {
            const Email& email = current->email;
            cout << "From: " << email.getSender() << ", Subject: " << email.getSubject() << ", Time: " << email.getFormattedTime() << endl;
            current = current->next;
        }
    }
};

#endif
