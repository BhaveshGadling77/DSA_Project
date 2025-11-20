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
    char status[15];           // present, absent or only registered
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

---

### Function Implementations:
#### 1.Register Attendee

Purpose: Add a new attendee to the event.

Algorithm:
1. Collect user input (name, email, phone)
2. Generate unique attendee ID
3. Set initial status as Registered
4. Store current timestamp
5. Allocate memory for new node
6. Insert at head of linked list
7. Save to the file

Time Complexity: O(1) for insertion at head

#### 2.Unregister Attendee

Purpose: Remove an attendee who wants to unregister.

Algorithm:
1. Search for an attendee by ID
2. Maintain previous pointer during traversal
3. delete node (prev->next = curr->next)
4. Free deleted node (demallocate it)
5. Update file

Time Complexity: O(n) due to linear search

#### 3.Mark Attencance

Purpose: Update attendance status when attendee arrives.

Algorithm:
1. Take attendee ID as input
2. Traverse list to find matching ID
3. If found, Update status field to "Present"
4. Save changes to file

Time Complexity: O(n) for linear search

#### 4. View All Attendees

Purpose: Display complete list of registered attendees.

Algorithm:
1. Traverse entire linked list
2. Print current attendee data i.e struct attributes
3. Continue till NULL is reached

Time Complexity: O(n) as every node must be visited.

#### 5. Search Attendee

Purpose: Find specific attendee by ID.

Algorithm:
1. Take search ID as input
2. Traverse through linked list 
3. Return & display details if found
4. else convey attendee doesn't exist

Time Complexity: O(n) worst case

#### 6. View Statistics

Purpose: Generate attendance report.

Algorithm:
1. Initialize counters 
2. Traverse through list
3. Count total registrations & present attendees
4. Calculate attendance percentage
5. Display formatted report

Time Complexity: O(n) with single traversal

#### 7. Save To A File

Purpose: To save data to the csv file.

```
void saveToFile(Node *head, int eventId){
    char filename[100];

    // sprintf function use to write to the file
    sprintf(filename, "data/event_%d.csv", eventId);

    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        printf("Couldn't create file.\n");
        return;
    }
    // header line
    fprintf(fp, "AttendeeID,Name,Email,Phone,EventId,Status,RegistrationDate"); 

    Node *temp = head;
    while(temp != NULL){
        fprintf(fp, "%d,%s,%s,%lu,%d,%s,%s\n",
            temp->data.attendeeID, temp->data.name, temp->data.email, temp->data.phoneNo, temp->data.eventID,
        temp->data.status, temp->data.registrationDate);
        temp = temp->next;
    }
    fclose(fp);
    printf("File 'data/event_%d.csv' has been saved.", eventId);
}
```

Data would be stored in CSV (Comma Seprated Value) format:
```
101,Rahul Sharma,rahul@email.com,9876543210,1,Present,26-11-2024 14:30
102,Priya Singh,priya@email.com,9123456789,1,Registered,26-11-2024 14:25
```

#### 8. Load From A File

Purpose: To load csv file & access attendees data to perform operation on them.

```
void loadFromFile(Node **head, int eventID){
    char filename[100];
    sprintf(filename, "data/event_%d.csv", eventID); //  loads file

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) return;

    char buffer[500]; 
    fgets(buffer, sizeof(buffer), fp); // to skip header line while reading data from file
    // as we've wrote header line before saving file.

    Attendee a;
    // delimiters are handled.
    while(fscanf(fp, "%d,%[^,],%[^,],%lu,%d,%[^,],%[^\n]\n", 
        &a.attendeeID, a.name, a.email, &a.phoneNo, &a.eventID, 
    a.status, a.registrationDate) == 7){
        Node *newNode = (Node *) malloc(sizeof(Node));
        newNode->data = a;
        newNode->next = *head;
        *head = newNode;
    }
    fclose(fp);
}
```

What is does: Reads file line by line, handles delimiters (comma here), and reconstructs the linked list in memory.

#### 9. FreeList

Purpose: Frees allocated memory to avoid memory leaks.

```
void freeList(Node *head){
    Node *temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}
```

---
### Attendee Managment System Architecture:
#### Module Organization
Files:
1. Main.c - Menu system, entry point of program
2. attendee.h - Structure definitions and function declarations
3. attendee.c - Function implementations

Header files define interfaces while implementation files provides the actual logic.

