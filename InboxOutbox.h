// InboxOutbox.h
#ifndef INBOXOUTBOX_H
#define INBOXOUTBOX_H

#include <iostream>
#include <string>
#include "Email.h"

using namespace std;

class InboxOutbox {
private:
    struct Node {
        Email email;
        Node* next;
    };

    Node* inboxTop;   // Stack for inbox (LIFO)
    Node* outboxFront; // Queue for outbox (FIFO)
    Node* outboxRear;

public:
    InboxOutbox()  // Constructor
    {
        // Logic here
    };
    
    ~InboxOutbox() // Destructor
    {
        // Logic here
    };
    

    // Inbox management (LIFO)
    void receiveEmail(const Email& email) // Push to inbox (stack)
    {
        // Logic here
    };
    
    void displayInbox()                   // Pop and display inbox emails (stack)
    {
        // Logic here
    };
    

    // Outbox management (FIFO)
    void sendMail()         // Enqueue to outbox (queue)
    {
        // Logic here
    };
    
    void displayOutbox()     // Dequeue and display sent mails (queue)
    {
        // Logic here
    };
    
};

#endif
