#ifndef SPAM_QUEUE_HPP
#define SPAM_QUEUE_HPP

#include "Email.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

struct SpamWordNode {
    string spamWord;
    SpamWordNode* next;

    SpamWordNode(const string& word) : spamWord(word), next(nullptr) {}
};

struct SpamNode {
    Email email;
    SpamNode* next;

    SpamNode(const Email& e) : email(e), next(nullptr) {}
};

class SpamQueue {
private:
    SpamNode* front;
    SpamNode* rear;
    SpamWordNode* spamWordsHead; // Linked list to store spam words

public:
    SpamQueue() : front(nullptr), rear(nullptr), spamWordsHead(nullptr) {}

    ~SpamQueue() {
        while (front != nullptr) {
            SpamNode* temp = front;
            front = front->next;
            delete temp;
        }

        while (spamWordsHead != nullptr) {
            SpamWordNode* temp = spamWordsHead;
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
            SpamWordNode* newNode = new SpamWordNode(word);
            if (spamWordsHead == nullptr) {
                spamWordsHead = newNode;
            } else {
                SpamWordNode* current = spamWordsHead;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newNode;
            }
            cout << "[DEBUG] Loaded spam word: " << word << endl;
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
        cout << "[DEBUG] Enqueued spam email from " << email.getSender() << " to " << email.getRecipient() << endl;
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
        SpamWordNode* current = spamWordsHead;
        while (current != nullptr) {
            if (text.find(current->spamWord) != string::npos) {
                cout << "[DEBUG] Found spam word in text: " << current->spamWord << endl;
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

            if (recipient == currentRecipient && (containsSpamWords(subject) || containsSpamWords(content))) {
                enqueue(Email(sender, recipient, subject, content, timestamp, priority));
            }
        }
        file.close();
    }

    void display() const {
        if (isEmpty()) {
            cout << "No spam emails found." << endl;
            return;
        }

        SpamNode* current = front;
        cout << "\n--- Spam Emails ---" << endl;
        while (current != nullptr) {
            const Email& email = current->email;
            cout << "From: " << email.getSender()
                 << ", Subject: " << email.getSubject()
                 << ", Time: " << email.getFormattedTime() << endl;
            current = current->next;
        }
    }
};

#endif // SPAM_QUEUE_HPP
