// structure of attendee

typedef struct time {
    short unsigned int hour;        // [0, 23]
    short unsigned int minute;      // [0, 59]
    short unsigned int second;      // [0, 59]
} time;

typedef struct Attendee{
    char attendeeID[50];
    char name[50];
    char email[50];
    unsigned long phoneNo[15];
    int eventID;
    char status[15];
    char registrationDate[30];
    time attendance_time;
} Attendee;