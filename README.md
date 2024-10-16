List of files(divided based on the roles we chose):

1. main.cpp
Handles user interface, login/signup, and the main menu to navigate between Inbox, Send Mail, Spam, Priority Mail, Search, and Logout.
2. InboxOutbox.h
Manages the Inbox (using a stack) and Outbox (using a queue).
Contains:
receiveEmail() (Push to inbox)
displayInbox() (Pop and display inbox emails)
sendMail() (Enqueue to outbox)
displayOutbox() (Dequeue and display outbox emails)
3. SpamPriority.h
Manages Spam (using a queue) and Priority Emails (using a priority queue).
Contains:
markAsSpam() (Enqueue to spam)
displaySpam() (Dequeue and display spam emails)
prioritizeEmail() (Insert email in priority queue)
displayPriorityMails() (Display and remove priority emails)
4. SearchRetrieval.h
Handles search and retrieval of emails from Inbox, Outbox, and Spam.
Contains:
searchEmails() (Search emails in inbox, outbox, and spam)
5. Email.h
Defines the Email structure with fields like sender, subject, content, and priority.

For now, the login function kinda works, storing the credentials in a text file users.txt

