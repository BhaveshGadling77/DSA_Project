#ifndef EVENTS_H
#define EVENTS_H
#include <stdio.h>

typedef struct Time {
    short unsigned int hour;                // [0, 23]
    short unsigned int minute;              // [0, 59]
    short unsigned int second;              // [0, 59]
} Time;

typedef struct date {
    short int date;
    short int month;
    short int year;
} date;

typedef struct event {
    int eventID;                            // Provided by us if adding an event
    char eventName[32];
    int organiserID;                        // Linked to Organiser
    int venueID;                            // Linked to Venue
    struct date eventDate;
    Time startTime;
    Time endTime;
    Time regDue;
    char* description;
} event;

// Linked list node for events
typedef struct EventNode {
    struct event evt;
    struct EventNode *next;
} EventNode;

// BST node for events by eventID
typedef struct EventBST {
    struct event evt;
    struct EventBST *left;
    struct EventBST *right;
} EventBST;

// Function prototypes
void loadEvents(void);                                  // Load existing events from csv to structures
void cleanPastEvents(void);                             // Delete events whose time has elapsed
void viewEvents(void);                                  // Display all events using linked lists
void sortEventByTime(void);                             // Sorting using quicksort
void sortEventChronological(void);                      // Sort events chronologically
void sortEventByID(void);                               // Sort events by ID
event* searchEventID(void);                             // Search event by ID
EventBST* newBSTNode(struct event e);                   // Creates a new BST node with the given event data
EventBST* insertBST(EventBST* root, struct event e);    // Inserts an event into the BST sorted by EventID ussing recursion
EventBST* searchBST(EventBST* root, int eventID);       // Search in BST by eventID
EventBST* minValueNode(EventBST* node);                 // Returns node with minimum eventID for deletion, used for deleting a node with two children
EventBST* deleteBST(EventBST* root, int eventID);       // Delete event from BST
date getCurrentDate(void);                              // Get current date
int isPastEvent(struct date eventDate);                 // Check if event date has elapsed
void quickSort(struct event arr[], int low, int high, int (*compare)(event, event)); // Quicksort implementation
int partition(struct event arr[], int low, int high, int (*compare)(event, event)); // Used for quicksort implementation
int compareChronological(event a, event b);             // Start comparing times of two events
int compareByDate(event a, event b);                    // Continue comparing dates of two events
int compareByID(event a, event b);                      // Compare event IDs
int listToArray(event arr[]);                           // Convert events that have not happened in array, and return count of the events
void addEvent(void);                                    // Add a new event
void deleteEvent(void);                                 // Delete event using BST
void modifyEvent(void);                                 // Modify event
void addToList(event e);                                // Add event node to BST
int checkValidTime(Time startTime, Time endTime);       // Return 1 if valid, 0 if invalid
int checkValidDate(date d);                             // Return 1 if valid, 0 if invalid
int generateEventID(void);                              // Generates a unique event ID
#endif