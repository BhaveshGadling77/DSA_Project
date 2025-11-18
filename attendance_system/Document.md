## Attendee Management System - Technical Documentation

### Let's get started
So, hey guys! This document provides a comprehensive overview of the Attendee Management System (AMS), developed as part of a Event Management System project. The focus here is on the technical implementation, functional descriptions, and the  logic behind choosing specific data structures.

---
### Why Event Management System
Management systems provides an ideal opportunity to work with multiple data structures meaningfully. The project requirement was to use at least three data structures, but more importantly, to use them where they actually solve problems rather than forcing them in artificially.

---
### A bit of description of our project
Our team developed an Event Management System that allows organizers to create and manage events while enabling attendees to register and track their participation. The system consists of three modules:

1. Login Authentication Module - Handles user authentication and role management (organizer/attendee).
2. Event Management Module - Creates, modifies, and manages events.
3. Attendee Management Module - Tracks registrations and atendees & thier attendance (my responsibility)

---
Now let's move to Attendee Management System...

### Why attendee management system
The Attendee Management System needed to:

1. Allow attendees to register for events dynamically
2. Support unregistration when plans change
3. Track attendance status (Present/Absent/Registered)
4. Provide statistical insights to organizers
5. Handle an unknown number of attendees
6. Persist data across program sessions
7. Minimize memory wastage
8. Enable efficient search and updates

---
### Techincal Approach:
1. Data Structure Selection
2. The Attendee Structure

#### This is a struct used for attendees.
I've defined structure of attendee data & list of attendees in attendee.h header file.

```
typedef struct Attendee {
    int attendeeID;
    char name[50];
    char email[50];
    unsigned long phoneNo;
    int eventID;
    char status[15];           // present, absent or    only registered
    char registrationDate[30];
} Attendee;
```

---
#### Data Structure used to store attendee data:
The primary data structure is a singly linked list, defined as:

```
typedef struct Node {
    Attendee data;
    struct Node* next;
} Node;
```

---
In attendee.c file I've implemented functions to:

1. Register for Event.
2. Unregister from Event.
3. Mark Attendance.
4. View All Attendees.
5. Search Attendee.
6. View Statistics.
7. Save data to file.
8. Load data from a file.

#### Function Implementations:
1.Register Attendee

Purpose: Add a new attendee to the event

Algorithm:
1. Collect user input (name, email, phone)
2. Generate unique attendee ID
3. Set initial status as Registered
4. Capture current timestamp
5. Allocate memory for new node
6. Insert at head of linked list
7. Persist to file

Time Complexity: O(1) for insertion at head