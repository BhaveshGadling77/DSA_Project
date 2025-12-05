<p align="center">
  <img src="https://img.icons8.com/external-tal-revivo-regular-tal-revivo/96/external-readme-is-a-easy-to-build-a-developer-hub-that-adapts-to-the-user-logo-regular-tal-revivo.png" width="100" />
</p>
<p align="center">
    <h1 align="center">Event Management System</h1>
</p>
<p align="center">
	<img src="https://img.shields.io/github/last-commit/BhaveshGadling77/DSA_Project?style=flat&logo=git&logoColor=white&color=0080ff" alt="last-commit">
	<img src="https://img.shields.io/github/languages/top/BhaveshGadling77/DSA_Project?style=flat&color=0080ff" alt="repo-top-language">
	<img src="https://img.shields.io/github/languages/count/BhaveshGadling77/DSA_Project?style=flat&color=0080ff" alt="repo-language-count">
<p>
<p align="center">
		<em>Developed with: </em>
</p>
<p align="center">
	<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=flat&logo=C&logoColor=black" alt="C">
</p>
<hr>

### Let's Get Started

Hello everyone! If you're reading this, you're diving into our Event Management System project. We're excited to share our journey as a team in building this console based application in C. This README serves as a narrative guide, walking you through our thought process, design decisions, and implementation details, much like a behind-the-scenes (BTS) tour of how we brought this system to life.

### Why This Project?

As second-year Computer Science & Engineering students at COEP Technological University, we undertook this project as part of our Data Structures and Algorithms Lab. The goal was to apply theoretical concepts to a real-world problem, using at least three data structures effectively. Event management systems are perfect for this because they involve handling dynamic data like user registrations, event schedules, and venue bookings areas where efficient data structures shine.

We chose this domain because events are everywhere: from university fests to corporate conferences. Managing them manually can be chaotic, with risks of overbooking venues or losing track of attendees. Our system automates these processes, ensuring reliability and scalability. Plus, it allowed us to explore persistence with CSV files, user authentication, and conflict resolution...Skills that go beyond just coding.

### What We Learned?

Throughout development, we gained deep insights into how data structures impact performance. For instance, we learned that a plain Binary Search Tree (BST) can degrade to O(n) time in worst cases, like sequential insertions, which is why we opted for AVL Trees to guarantee O(log n) operations. We also understood the power of combining structures: a Singly Linked List for sequential access paired with an AVL Tree for fast lookups.

On the algorithmic side, we implemented QuickSort with custom comparators for flexible event sorting and an Interval Tree for efficient venue conflict detection avoiding brute force O(m²) checks that could slow down the system. Working as a team taught us about modular design: separating login, event, and attendee modules ensured clean integration. Finally, handling edge cases like leap years in date validation and midnight-crossing events built our problem-solving resilience.

### 8 A Bit About Our Project

Our Event Management System is a complete console application in C that supports organizers and attendees. It consists of three interconnected modules:

1. **Login Authentication Module** (Handled by Bhavesh Gadling): Manages user registration and login for organizers and attendees.
2. **Event Management Module** (Handled by Piyush Deshpande): Allows creating, modifying, deleting events, and booking venues without conflicts.
3. **Attendee Management Module** (Handled by Sumit Desai): Tracks registrations, attendance, and generates statistics.

All of us (Sumit, Piyush, & Bhavesh) collaborated on testing, documentation, and integration. The system uses CSV files for data persistence, ensuring data survives program restarts. Users interact via a menu-driven interface in `main.c`

### How Does It Work?

Let's break it down step by step, starting from user authentication to event handling and attendee tracking.Let's break it down step by step, starting from user authentication to event handling and attendee tracking.

#### Step 1: User Authentication

Everything begins with secure login and registration. We use a singly linked list to manage user data in memory, loaded from `userAttendee.csv` or `userOrganizer.csv.`

Here's the user structure defined in `login_registration.h:`

```
typedef struct user {
    int userId;
    char name[64];
    unsigned int noOfEventsAttended; // or noOfEventsOrganized for organizers
    long long mobileNumber;
    char email[128];
} user;
```

Registration generates a unique ID and validates inputs like email format. Login checks credentials and updates a global `userStatus` struct to track the current session. For example, the `registerUser` function scans the CSV to avoid duplicates, ensuring O(n) time but executed infrequently.

#### Step 2: Event Creation and Management

Once logged in as an organizer, you can manage events. We use an AVL Tree for fast operations on events (keyed by unique eventID) and a Singly Linked List for sequential listing. Venues are stored in a Hash Table with separate chaining for O(1) average lookups.

Key functions include:
- `createEvent`: Generates a unique ID, checks venue availability using an Interval Tree, and inserts into the AVL Tree.
- Venue conflict check: Builds a temporary Interval Tree from existing bookings on the same date, querying for overlaps in O(m log m) time.

Here's a snippet from the AVL Tree insertion:

```
EventBST* insert(EventBST* node, Event* event) {
    if (node == NULL) return createNode(event);
    
    if (event->eventID < node->event.eventID)
        node->left = insert(node->left, event);
    else if (event->eventID > node->event.eventID)
        node->right = insert(node->right, event);
    
    // Update height and balance
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    
    // Perform rotations if unbalanced
    if (balance > 1 && event->eventID < node->left->event.eventID)
        return rightRotate(node);
    // ... (other rotation cases: LL, RR, LR, RL)
    
    return node;
}
```

Sorting uses QuickSort on a dynamic array converted from the linked list, with comparators like `compareByDate` for chronological ordering.


#### Step 3: Attendee Registration and Tracking

Attendees register for events, fetching data from the user database to avoid redundancy. We use a Singly Linked List for attendees per event, stored in `event_<id>.csv`

Registration flow:
- Fetch user data with `fetchUserData` (parses CSV using strtok for robustness).
- Check for duplicates.
- Insert at head for O(1) addition.

Example from `registerAttendeeForEvent`:

```
void registerAttendeeForEvent(Node **head, int eventID, userStatus *user) {
    // Validation and duplicate checks...
    if (!user->status || user->isOrg) return; // Login and role checks
    
    Node *temp = *head;
    while (temp != NULL) {
        if (temp->data.attendeeID == user->userId) {
            printf("Already registered!\n");
            return;
        }
        temp = temp->next;
    }
    
    Attendee a;
    if (!fetchUserData(user->userId, &a)) return;
    
    a.attendeeID = user->userId;
    a.eventID = eventID;
    strcpy(a.status, "Registered");
    getCurrentDateTime(a.registrationDate);
    
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = a;
    newNode->next = *head;
    *head = newNode;
    
    saveToFile(*head, eventID);
    updateEventsAttended(user->userId);
    printf("Successfully registered for event %d!\n", eventID);
}
```

Other features include unregistration (O(n) search and delete), marking attendance (update status to "Present"), searching by ID, viewing all attendees, and statistics (e.g., attendance percentage: (present / total) * 100).


#### Data Persistence and Cleanup

All data is saved to CSV files on modifications. Past events are cleaned up via a linked list traversal and AVL deletions, ensuring the system stays efficient: O(n + k log n) where k is the number of past events.

### Core Data Structures and Why We Chose Them

Our selection was driven by performance and use cases, ensuring no degradation in critical paths.

- AVL Tree (`EventBST *eventTree`): Self-balancing BST for events by eventID. Guarantees O(log n) insert/search/delete via height tracking and rotations (LL, RR, LR, RL). Chosen over plain BST to handle sequential IDs without O(n) skew.

- Singly Linked List (`EventNode *eventList`, `Node *attendeeList`): Maintains insertion order for traversal and listing. O(1) prepend insertions, O(n) for searches ideal for dynamic attendee lists and event iteration.

- Hash Table (`VenueNode *venueHashTable[101]`): Venues hashed by `venueID % 101` with chaining. Average O(1) lookups for frequent venue checks.

- Interval Tree: Dynamic BST on time intervals (seconds since midnight) for overlap detection in `checkVenueAvailability`. O(m log m) build, O(log m + k) queries far better than O(m²) pairwise checks.

- Dynamic Array (via `listToArray`): Converts lists for QuickSort's random access, enabling O(n log n) sorting by date, time, or ID.

This combo provides logarithmic searches, constant lookups, and efficient scheduling, as seen in similar implementations like room booking systems.


### Time Complexity Analysis

- **Linked List Operations:** Insert O(1), Search/Delete O(n).
- **AVL Tree Operations:** Insert/Delete/Search O(log n).
- **Sorting:** QuickSort O(n log n) average.
- **File Operations:** O(n) for CSV reads/writes.
- **Module Specific:** Registration O(n), Conflict Check O(m logm), Cleanup O(n + k logn).

### Future Goals

**Short-Term Goals:** Integrate a simple GUI using GTK; add email notifications for registrations.

**Long-Term Goals:** Evolve to a web app with MySQL backend; support multi-user concurrency via threads.

**Limitations of the Current System:** Console only interface (CLI) limits usability; single user mode (no locking for shared CSVs); fixed hash table size (101 buckets) assumes limited venues.

### Appendix: Key Structures

- Event Structure (from Event Module):
```
typedef struct Event {
    int eventID;
    char eventName[100];
    char venueID[20];
    char date[20]; // YYYY-MM-DD
    char startTime[10], endTime[10];
    // ... other fields
} Event;
```

- Attendee Structure (From Attendee Module):
```
typedef struct Attendee {
    int attendeeID;
    char name[50], email[50];
    unsigned long long phoneNo;
    int eventID;
    char status[15]; // "Registered", "Present", "Absent"
    char registrationDate[30];
} Attendee;
```

- User structure (From Login_Registration Module):
```
typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[128];
} user;

```

*Full implementations in respective .h/.c files.*

### References

- GeeksforGeeks & Tutorials Point (Data Structures & Their implementations)
- The C Programming Language Book by kernighan and ritchie
- Fundamentals of Data Structures in C
- Draw.io (To draw a pointer diagram)
- Canva (Presentation Purpose)

### How to Build & Run the Project

The project is written in standard C and has no external dependencies other than the standard library.

#### Prerequisites

- GCC (or any C compiler)
- Make (optional, but we provide a simple Makefile)

#### Steps

1. Clone or download the repository
2. Create a Data folder in the project root (this is where all CSV files will be stored):
```
mkdir Data
```
3. Change directory to Wholesystem:
```
cd Wholesystem
```
4. Simple run following command:
```
make
```
5. Run the program:
```
./a.out
```
(on Windows: ./a.exe)

The first time you run it, the system will automatically create the required CSV files (`userAttendee.csv`, `userOrganizer.csv`, `events.csv`, etc.) if they don't exist.


### Project Structure
```
├── Attendee_system
│   ├── attendee.c        
│   ├── attendee.h  
│   ├── Document.md
│   └── main.c    
├── Data
│   ├── attendees
│   ├── events
│   ├── organizers
│   ├── events.csv
│   ├── temp.csv
│   ├── userAttendee.csv
│   ├── userOrganizer.csv
│   └── venues.csv
├── Docs
│   ├── Pointer Digrams
│   ├── Project Presentation
│   ├── Project Report
│   ├── References.odt
│   ├── References.txt
│   └── Report.tex (Latex Code)
├── Event_Module
│   ├── Document.md
│   ├── event.c
│   ├── events.h
│   ├── main.c
│   ├── venues.c
│   └── venues.h
├── Login_Registration_Module
│   ├── Document.md
│   ├── login_registration.c
│   ├── login_registration.h
│   ├── main.c
│   ├── Makefile
│   └── TechnicalDocument.md
├── Wholesystem (Integrated System)
│   ├── attendee.c
│   ├── attendee.h
│   ├── events.c
│   ├── events.h
│   ├── login_registration.h
│   ├── login_registration.c
│   ├── main.c
│   ├── Makefile
│   ├── venues.c
│   └── venues.h
└──README.md

```

### How to contribute?

1. **Fork the repository**
2. **Create a new branch**
```bash
git checkout -b new_branch
```
3. **Make changes**
4. **Commit your changes**
```bash
git commit -m "Message"
```
5. **Push your changes**
```bash
git push origin new_branch
```
6. **Create a pull request**

Contributions are highly appreciated. Please create a pull request if you have any suggestions or improvements.

### Conclusions

This project solidified our understanding of DSA in practice, from balancing trees to chaining hashes. It not only met lab requirements but also created a functional system we'd use for real events. We're proud of the result and open to feedback..Feel free to fork and improve!

Team: [Sumit Desai](https://github.com/SumitDesai-21), [Piyush Deshpande](https://github.com/Piyush-D24), & [Bhavesh Gadling](https://github.com/BhaveshGadling77)
COEP Technological University, Pune.
