## Attendee Management System - Technical Documentation

### Let's get started
So, hey guys! This document provides a comprehensive overview of the Login and Registration System (LRS), developed as part of a Event Management System project. The focus here is on the technical implementation, functional descriptions, and the  logic behind choosing specific data structures.

---
### Why Event Management System
Management systems provides an ideal opportunity to work with multiple data structures meaningfully. The project requirement was to use at least three data structures, but more importantly, to use them where they actually solve problems rather than forcing them in artificially.

---
### A bit of description of our project
Our team developed an Event Management System that allows organizers to create and manage events while enabling attendees to register and track their participation. The system consists of three modules:

1. Login Authentication Module - Handles user authentication and role management (organizer/attendee)(my responsibility).
2. Event Management Module - Creates, modifies, and manages events.
3. Attendee Management Module - Tracks registrations and atendees & thier attendance.

---
Now let's move to Login and Registration System...

### Why Login and Registration System
The Login Registration System needed to:

1. Allow Users To register in Our System.
2. Login as Organizer and as Attendee functionality.
3. Register as Organiser and as Attendee functionality.
4. provide details of User to the other functions.
5. Handle data in the csv format.
6. Maintain data flow across other files.
7. Minimize memory wastage.

---
### Techincal Approach:
1. Data Structure Selection
2. The Attendee Structure

#### This is a struct used for user.
I've defined structure of user and userStatus in login_registration.h header file.

```
typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended; // noOfEventsAttended or noOfEventsOrganized
	long long mobileNumber;
	char email[128];
} user;

```

---
#### Data Structure used to give information about currently logged in user.
The primary data structure is a singly linked list, defined as:

```
typedef struct userStatus {
    int userId; //default 0 if no user logged in.
	char name[64];
    bool status; // true = logged in, false = not logged in
    bool isOrg;  //if org = true, else  false;
} userStatus;
```

---
In login_registration.c file I've implemented functions to:

1. Register User.
2. Login User.
3. Give details of user.
4. Load Data from file.
5. validate the User details.
6. Save Data to File.

---

### Function Implementations:
#### 1.Register User

Purpose: Add a new User To our Database.

Algorithm:
1. Collect user input (name, email, phone)
2. Generate unique User ID
3. Validate the email and other details.
4. Allocate memory for new struct.
5. check if the user already exist.
6. Save to the file.
7. free the struct memory.

Time Complexity: O(n) for checking user data into File.

#### 2.Login User

Purpose: Login the user.

Algorithm:
1. collect user Input (name, email, userId)
2. open the file for reading.
3. find the user according to there details.
4. if users exist then update the userStatus struct.
5. close file

Time Complexity: O(n) due to linear search

#### 3. Logout

Purpose: Logout the Current user.

Algorithm:
1. just change the value in struct userStatus.
2. name should be empty string.
3. userId set to 0.
4. isOrg and isLoggedIn set to false.

Time Complexity: O(1) for changin values in userStatus struct.

### 4. getDetails
purpose : provide the current login user Details.
Algorithm:
1. return the global struct declared in login_registration.c file.

---
### Login and Registration System Architecture:
#### Module Organization
Files:
1. Main.c - Menu system, entry point of program
2. login_registration.h - Structure definitions and function declarations
3. login_registration.c - Function implementations

Header files define interfaces while implementation files provides the actual logic.

