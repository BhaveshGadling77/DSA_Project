#include <stdio.h>

typedef struct time {
    short unsigned int hour;        // [0, 23]
    short unsigned int minute;      // [0, 59]
    short unsigned int second;      // [0, 59]
} time;

typedef struct date {
    short int date;
    short int month;
    short int year;
} date;

typedef struct Event {
    int eventID;
    char eventName[64];
    int organiserID;        // Linked to Organiser
    int venueID;            // Linked to Venue
    struct date eventDate;
    struct time startTime;
    struct time endTime;
} Event;