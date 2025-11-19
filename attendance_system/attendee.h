#ifndef ATTENDEE_H
#define ATTENDEE_H
#include <stdbool.h>

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

// user status 
typedef struct UserStatus{
    int userId;
    bool status; // should be true
    bool isOrg; // should be false
    char userName[50]; 
}UserStatus;

// Linked List Node
typedef struct Node {
    Attendee data;
    struct Node* next;
} Node;

// Function declarations

// Attendee Functions (only attedees can call)
void registerAttendeeForEvent(Node** head, int eventID, UserStatus *user);
void unregisterAttendee(Node** head, UserStatus* user);


// organizer functions
void markAttendance(Node* head);
void viewAllAttendees(Node* head, int eventID);
void viewStatistics(Node* head);

void getCurrentDateTime(char* buffer);

// common function
void searchAttendee(Node* head);
void saveToFile(Node* head, int eventID);
void loadFromFile(Node** head, int eventID);
void freeList(Node* head);

#endif
