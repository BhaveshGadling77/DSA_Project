#ifndef ATTENDEE_H
#define ATTENDEE_H

typedef struct time {
    short unsigned int hour;        // [0, 23]
    short unsigned int minute;      // [0, 59]
    short unsigned int second;      // [0, 59]
} time;

typedef struct Attendee{
    char *attendee_id;
    char name[50];
    char email[50];
    long unsigned phone_no;
    int event_id;
    char status[15]; // registered or present or absent
    char registration_date[30];
    time attendance_time; // time when registered
} Attendee;

// function declarations
void viewAllAttendee();
void searchAttendee();
void updateAttendee();
void deleteAttendee();
int getNextAttendeeId(int event_id);
void displayAttendee(Attendee a);


#endif