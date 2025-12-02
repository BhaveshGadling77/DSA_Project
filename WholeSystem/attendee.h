#ifndef ATTENDEE_H
#define ATTENDEE_H

#include <stdbool.h>
#include "login_registration.h"
// Attendee structure
typedef struct Attendee {
    int attendeeID;
    char name[50];
    char email[50];
    unsigned long long phoneNo;
    int eventID;
    int eventsRegistered;
    char status[15];           // present, absent or only registered
    char registrationDate[30];
} Attendee;

// Linked List Node
typedef struct Node {
    Attendee data;
    struct Node* next;
} Node;

#define DECREASE 1
#define INCREASE 2
// Attendee Functions (only attendees can call)
void registerAttendeeForEvent(Node** head, int eventID, userStatus *user);
bool unregisterAttendee(Node** head, userStatus* user);
bool fetchUserData(int userID, Attendee *a);
void updateEventsAttended(int userID, int choice);

// organizer functions
void markAttendance(Node** head);
void viewAllAttendees(Node* head, int eventID);
void viewStatistics(Node *head);

// common function
void getCurrentDateTime(char* buffer);
void searchAttendee(Node* head);
void saveToFile(Node* head, int eventID);
void loadFromFile(Node** head, int eventID);
void freeList(Node* head);

#endif