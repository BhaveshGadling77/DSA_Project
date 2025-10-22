// to create header files use this:
#ifndef ATTENDANCE_H
#define ATTENDANCE_H

typedef struct AttendanceCount{
    int total_registered; // total attendees registered
    int total_present;
    int total_absent;
    float attedance_percentage;
} AttendanceCount;


// Function declarations
void markAttendance();
void viewAttendance();
AttendanceCount getAttendanceStats(int event_id);

#endif

