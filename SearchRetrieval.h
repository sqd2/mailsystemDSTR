// SearchRetrieval.h
#ifndef SEARCHRETRIEVAL_H
#define SEARCHRETRIEVAL_H

#include <iostream>
#include <string>
#include "Email.h"

using namespace std;

class SearchRetrieval {
public:
    SearchRetrieval();  // Constructor
    ~SearchRetrieval(); // Destructor

    void searchEmails(); // Search functionality for inbox, outbox, and spam
};

#endif
