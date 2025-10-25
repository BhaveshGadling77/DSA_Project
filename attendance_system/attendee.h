#ifndef ATTENDEE_H
#define ATTENDEE_H

// Attendee structure
typedef struct Attendee {
    int attendeeID;
    char name[50];
    char email[50];
    unsigned long phoneNo;
    int eventID;
    char status[15];           // present, absent or only registered
    char registrationDate[30];
} Attendee;

// Linked List Node
typedef struct Node {
    Attendee data;
    struct Node* next;
} Node;

// Function declarations
void registerAttendee(Node** head, int eventID);
void unregisterAttendee(Node** head, int attendeeID);
void markAttendance(Node* head);
void viewAttendees(Node* head);
void searchAttendee(Node* head);
void viewStatistics(Node* head);
void saveToFile(Node* head, int eventID);
void loadFromFile(Node** head, int eventID);
void freeList(Node* head);
void getCurrentDateTime(char* buffer);

#endif