#ifndef ATTENDEE_H
#define ATTENDEE_H

#include <stdbool.h>

#include <stdbool.h>
#include "../Login_Registration_system/login_registration.h"
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
typedef struct userStatus {
    int userId;
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} userStatus;

// Linked List Node
typedef struct Node {
    Attendee data;
    struct Node* next;
} Node;

// Function declarations

// Attendee Functions (only attendees can call)
void registerAttendeeForEvent(Node** head, int eventID, userStatus *user);
void unregisterAttendee(Node** head, userStatus* user);


// organizer functions
void markAttendance(Node* head);
void viewAllAttendees(Node* head, int eventID);
void viewStatistics(Node *head);

// common function
void getCurrentDateTime(char* buffer);
void searchAttendee(Node* head);
void saveToFile(Node* head, int eventID);
void loadFromFile(Node** head, int eventID);
void freeList(Node* head);


#endif
