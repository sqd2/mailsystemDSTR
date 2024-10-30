#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <iostream>
#include <ctime>
using namespace std;

class Email {
private:
    string sender;
    string recipient;
    string subject;
    string content;
    time_t timestamp;
    int priority;
    bool isSpam;

public:
    Email(string snd, string rcp, string sub, string cnt, int prio):
        sender(snd), recipient(rcp), subject(sub), content(cnt), priority(prio), isSpam(false) {
        timestamp = time(nullptr); // current time
    }

    void markSpam(){
        isSpam = true; 
    }
    bool getSpam() const {
        return isSpam; 
    }
    int getPriority() const {
        return priority; 
    }
    time_t getTime() const { 
        return timestamp; 
    }
    string getSender() const {
        return sender;
    }
    string getRecipient() const {
        return recipient;
    }
    string getSubject() const {
        return subject;
    }
    string getContent() const {
        return content;
    }
};

#endif
