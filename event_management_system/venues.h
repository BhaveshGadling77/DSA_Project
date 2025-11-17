#ifndef VENUES_H
#define VENUES_H
#include <stdio.h>
#include "events.h"

typedef struct Venue {
    int venueID;
    char venueName[64];
    long long capacity;
    int rentalCostPerHour;
    char venueHandlerName[64];
    unsigned long long handlerContact;
    char venueHandlerEmail[64];
    int parkingCapacity;
    int indoor;                         // 1 = Indoor, 0 = Outdoor
    char location[128];                 // City or full address
} Venue;

// Interval structure for booked times
typedef struct Interval {
    int startSeconds;       // Start time of event converted to seconds since 00:00
    int endSeconds;         // End time of event converted to seconds since 00:00
    struct Interval *left;
    struct Interval *right;
} Interval;

// Hash table node for venues
typedef struct VenueNode {
    Venue venue;
    struct VenueNode *next;
} VenueNode;

#define HASH_SIZE 101

extern VenueNode* venueHashTable[HASH_SIZE];

// Function prototypes
void loadVenues(void);
Venue* getVenueByID(int venueID);
int checkVenueAvailability(int venueID, date eventDate, Time startTime, Time endTime);
unsigned int hash(int key);
Interval* newInterval(int start, int end);
Interval* insertInterval(Interval* root, int start, int end);
int doOverlap(int start1, int end1, int start2, int end2);
int checkOverlap(Interval* root, int start, int end);
void freeIntervalTree(Interval* root);

#endif