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
3. Attendee Management Module - Tracks atendee registration and their attendance (my responsibility)

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
    unsigned long long phoneNo; // to support 10 digit numbers
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

**Why Linked List Over Array?**

Arrays require fixed size allocation, waste memory, and have expensive insertion/deletion operations. Linked lists grow dynamically and handle these operations more efficiently. For our use case where the number of attendees is unpredictable, linked list is the optimal choice.

---
In attendee.c file I've implemented functions to:

1. Fetch User Data
2. Register for Event.
3. Unregister from Event.
4. Mark Attendance.
5. View All Attendees.
6. Search Attendee.
7. View Statistics.
8. Save data to file.
9. Load data from a file.


---

### Function Implementations:

#### 9. Fetch User Data

**Purpose:** Retrieve user information from the user database (userAttendee.csv) to populate attendee details during event registration.

**Algorithm:**
1. Open userAttendee.csv file in read mode
2. Read file line by line using `fgets()`
3. Parse each line using `strtok()` with comma delimiter
4. Extract fields: userId, name, eventsAttended, phone, email
5. Compare userId with the requested userID
6. If match found, populate Attendee structure with user data
7. Return true if found, false otherwise

**Implementation Details:**
```c
bool fetchUserData(int userID, Attendee *a)
{
    FILE *fp = fopen("../Data/userAttendee.csv", "r");
    if (!fp) {
        printf("Error: User database not found!\n");
        return false;
    }

    int id, eventsAttended;
    char name[100], email[100];
    unsigned long long phone;
    char line[2048];

    // Read line by line
    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        char *token;
        
        // Parse userId
        token = strtok(p, ",");
        if (!token) continue;
        id = atoi(token);
        
        // Check if this is the user we're looking for
        if (id == userID) {
            // Parse name
            token = strtok(NULL, ",");
            if (!token) continue;
            strcpy(a->name, token);

            // Skip eventsAttended field
            token = strtok(NULL, ",");
            
            // Parse phone
            token = strtok(NULL, ",");
            if (!token) continue;
            a->phoneNo = strtoul(token, NULL, 10);

            // Parse email
            token = strtok(NULL, "\n");
            if (!token) continue;
            strcpy(a->email, token);
            
            fclose(fp);
            return true;
        }
        memset(line, 0, sizeof(line));
    }

    fclose(fp);
    return false;  // User not found
}
```

**Why strtok()?**
The `strtok()` function is used instead of `fscanf()` because CSV files can have irregular formatting, empty fields, or fields containing unexpected characters. Using `strtok()` provides more robust parsing by handling these edge cases gracefully.

**Time Complexity:** O(n) where n is the number of users in the database

---

#### 2.Register Attendee

Purpose: Register a logged-in attendee for a specific event by fetching their data from the user database and adding them to the event's attendee list.

**Algorithm:**
1. Validate user is logged in (check status)
2. Verify user is not an organizer (organizers cannot register as attendees)
3. Check if user is already registered for this event (traverse linked list)
4. **Fetch user data** from userAttendee.csv using `fetchUserData()`
5. Set event-specific data (attendeeID, eventID, status, registration date)
6. Create new node and insert at head of linked list
7. Save updated list to event CSV file
8. Update the user's events attended count using `updateEventsAttended()`

**Implementation Details:**
```c
void registerAttendeeForEvent(Node **head, int eventID, userStatus *user)
{
    // Validation checks
    if (!user->status) {
        printf("\nError: Please login first!\n");
        return;
    }
    
    if (user->isOrg) {
        printf("\nError: Organizers cannot register as attendees!\n");
        return;
    }

    // Check for duplicate registration
    Node *temp = *head;
    while (temp != NULL) {
        if (temp->data.attendeeID == user->userId) {
            printf("\nYou're already registered for this event!\n");
            return;
        }
        temp = temp->next;
    }

    // Fetch user data from database
    Attendee a;
    if (!fetchUserData(user->userId, &a)) {
        printf("\nError: Couldn't fetch user data!\n");
        return;
    }

    // Populate event-specific fields
    a.attendeeID = user->userId;
    a.eventID = eventID;
    strcpy(a.status, "Registered");
    getCurrentDateTime(a.registrationDate);

    // Create node and insert at head
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = a;
    newNode->next = *head;
    *head = newNode;

    // Persist to file
    saveToFile(*head, eventID);

    // Update user's event count
    updateEventsAttended(user->userId);

    printf("\nSuccessfully registered for event %d!\n", eventID);
}
```

**Key Design Decision:** 
Instead of asking the attendee to re-enter their name, email, and phone number during registration, we fetch this information from the central user database. This provides:
- **Data Consistency:** All modules use the same user information
- **Better User Experience:** No redundant data entry
- **Single Source of Truth:** User information maintained in one place
- **Integration with Login Module:** Seamless connection between authentication and attendance

**Time Complexity:** O(n) due to duplicate check and fetchUserData()

---

#### 3.Unregister Attendee

Purpose: Remove an attendee who wants to unregister.

**Algorithm:**
1. Check if list is empty
2. Search for attendee by userID
3. Maintain previous pointer during traversal
4. Delete node using pointer manipulation (prev->next = curr->next)
5. Free deleted node memory
6. Update file

**Time Complexity:** O(n) due to linear search

---

#### 4. Mark Attendance

**Purpose:** Update attendance status when attendee arrives at the event.

**Algorithm:**
1. Check if list is empty
2. Take attendee ID as input
3. Traverse list to find matching ID
4. Check if already marked present (avoid duplicates)
5. If found, update status field to "Present"
6. Save changes to file

**Time Complexity:** O(n) for linear search

---

#### 5. View All Attendees

**Purpose:** Display complete list of registered attendees for an event.

**Algorithm:**
1. Create output file for storing attendee details
2. Check if list is empty
3. Print header row (ID, Name, Email, Phone, Status)
4. Traverse entire linked list
5. Print and write each attendee's data to file
6. Continue till NULL is reached

**Time Complexity:** O(n) as every node must be visited

---

#### 6. Search Attendee

**Purpose:** Find specific attendee by ID and display their complete details.

**Algorithm:**
1. Check if list is empty
2. Take search ID as input
3. Traverse through linked list
4. Compare each attendee ID with search ID
5. Return and display complete details if found
6. Otherwise convey attendee doesn't exist

**Time Complexity:** O(n) worst case

---


#### 7. View Statistics

**Purpose:** Generate attendance report with percentage calculations.

**Algorithm:**
1. Check if list is empty
2. Initialize counters (total, present)
3. Traverse through list once
4. Count total registrations and present attendees
5. Calculate attendance percentage: (present/total) × 100
6. Display formatted report

**Output Format:**
```
--- Statistics ---
Total Registered: 25
Present: 18
Absent: 7
Attendance: 72.00%
```

**Time Complexity:** O(n) with single traversal

---

#### 8. Save To A File

Purpose: To save data to the csv file.

```
void saveToFile(Node *head, int eventId)
{
    char filename[100];
    sprintf(filename, "../Data/event_%d.csv", eventId);

    FILE *fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("Error: Couldn't create file\n");
        return;
    }
    
    // Write header
    fprintf(fp, "AttendeeID,Name,Email,Phone,EventID,Status,RegistrationDate\n");

    Node *temp = head;
    while(temp != NULL) {
        fprintf(fp, "%d,%s,%s,%llu,%d,%s,%s\n",
                temp->data.attendeeID, temp->data.name, 
                temp->data.email, temp->data.phoneNo, 
                temp->data.eventID, temp->data.status, 
                temp->data.registrationDate);
        temp = temp->next;
    }
    fclose(fp);
}
```

Data would be stored in CSV (Comma Seprated Value) format:
```
101,Rahul Sharma,rahul@email.com,9876543210,1,Present,26-11-2024 14:30
102,Priya Singh,priya@email.com,9123456789,1,Registered,26-11-2024 14:25
```

#### 9. Load From A File

Purpose: To load csv file & access attendees data to perform operation on them.

```
void loadFromFile(Node **head, int eventID)
{
    if (head) *head = NULL; // Reset to avoid garbage
    
    char filename[100];
    sprintf(filename, "../Data/event_%d.csv", eventID);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return; // File doesn't exist yet

    char line[1024];
    fgets(line, sizeof(line), fp); // Skip header

    while (fgets(line, sizeof(line), fp)) {
        Attendee a;
        char *token;
        
        // Parse AttendeeID
        token = strtok(line, ",");
        if (!token) continue;
        a.attendeeID = atoi(token);
        
        // Parse Name
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(a.name, token, sizeof(a.name)-1);
        
        // ... (parse remaining fields)
        
        // Create node
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->data = a;
        newNode->next = *head;
        *head = newNode;
    }
    fclose(fp);
}
```

What is does: Reads file line by line, handles delimiters (comma here), and reconstructs the linked list in memory.

---

#### 10. Update Events Attended

**Purpose:** Increment the event count for a user after successful registration.

**Algorithm:**
1. Open userAttendee.csv in read mode
2. Create temporary file for writing updates
3. Read each line and parse user data
4. If userId matches, increment eventsAttended counter
5. Write all records (updated and unchanged) to temp file
6. Replace original file with updated temp file

**Implementation:**
```c
void updateEventsAttended(int userID)
{
    FILE *fp = fopen("../Data/userAttendee.csv", "r+");
    FILE *temp = fopen("../Data/temp.csv", "w");

    if (!fp || !temp) {
        printf("Error opening user file!\n");
        return;
    }

    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        // Parse line
        int id, eventsAttended;
        // ... parse other fields
        
        if (id == userID) {
            eventsAttended++;  // Increment counter
        }
        
        // Write to temp file
        fprintf(temp, "%d,%s,%d,%llu,%s\n",
                id, name, eventsAttended, phone, email);
    }

    fclose(fp);
    fclose(temp);

    // Atomic update
    remove("../Data/userAttendee.csv");
    rename("../Data/temp.csv", "../Data/userAttendee.csv");
}
```

**Why Temp File?**
Using a temporary file ensures atomic updates. If the program crashes mid-write, the original file remains intact.

---

#### 10. FreeList

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

