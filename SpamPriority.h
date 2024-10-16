// SpamPriority.h
#ifndef SPAMPRIORITY_H
#define SPAMPRIORITY_H

#include <iostream>
#include <string>
#include "Email.h"

using namespace std;

class SpamPriority {
private:
    struct SpamNode {
        Email email;
        SpamNode* next;
    };

    struct PriorityNode {
        Email email;
        PriorityNode* next;
    };

    SpamNode* spamFront;       // Queue for spam (FIFO)
    SpamNode* spamRear;
    PriorityNode* priorityFront; // Priority Queue for important emails

public:
    SpamPriority()  // Constructor
    {
        // Logic here
    };

    ~SpamPriority() // Destructor
    {
        // Logic here
    };

    // Spam handling
    void markAsSpam(const Email& email)  // Enqueue to spam (queue)
    {
        // Logic here
    };

    void displaySpam()                   // Dequeue and display spam emails (queue)
    {
        // Logic here
    };
    

    // Priority mail handling
    void prioritizeEmail(const Email& email)   // Insert email in priority queue
    {
        // Logic here
    };
    
    void displayPriorityMails()                // Display and remove priority emails
    {
        // Logic here
    };
    
};

#endif
