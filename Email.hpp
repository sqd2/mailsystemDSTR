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

public:
    Email(const string& s = "", const string& r = "", const string& subj = "", 
          const string& cont = "", time_t time = 0, int p = 1)
        : sender(s), recipient(r), subject(subj), content(cont), 
          timestamp(time), priority(p) {}

    const string& getSender() const {
         return sender; 
    }
    const string& getRecipient() const {
        return recipient; 
    }
    const string& getSubject() const { 
        return subject; 
    }
    const string& getContent() const { 
        return content; 
    }
    time_t getTime() const { 
        return timestamp; 
    }
    int getPriority() const { 
        return priority; 
    }

    string getFormattedTime() const {
        char buffer[80];
        struct tm* timeinfo = localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
};

#endif