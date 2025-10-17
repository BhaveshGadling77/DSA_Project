#include <stdio.h>

typedef struct time {
    short unsigned int hour;        // [0, 23]
    short unsigned int minute;      // [0, 59]
    short unsigned int second;      // [0, 59]
} time;

typedef struct date {
    short unsigned int date;        // [1, 28, 29, 30, 31]
    short unsigned int month;       // [1, 12]
    short unsigned int year;
} date;

typedef struct event {
    char eventName[24];
    int eventID;
    int guests;             // Number of guests
    char organiser;
    date eventDate;
    time inTime;
    time outTime;
} event;