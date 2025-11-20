#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "events.h"
#include "venues.h"
#include <errno.h>
#include "../WholeSystem/login_registration.h"

EventNode *eventList = NULL;                            // Linked list for sequential access
EventBST *eventTree = NULL;                             // BST for fast search/delete by ID

// Function to check if the provided time is valid or not
int checkValidTime(Time regEndtime, Time startTime, Time endTime) {
    if (startTime.hour >= 24 || startTime.minute >= 60 ||  startTime.second >= 60) {
            printf("Invalid start time of event\n");
            return 0;
        }
    if (endTime.hour >= 24 || endTime.minute >= 60 || endTime.second >= 60) {
            printf("Invalid end time of the event\n");
            return 0;
    }
    if (regEndtime.hour >= 24 || regEndtime.minute >= 60 || regEndtime.second >= 60) {
            printf("Invalid end time to register for the event\n");
            return 0;
    }
    int s = startTime.hour * 3600 + startTime.minute * 60 + startTime.second;
    int e = endTime.hour * 3600 + endTime.minute * 60 + endTime.second;
    int r = regEndtime.hour * 3600 + regEndtime.minute * 60 + regEndtime.second;
    if (s >= e) {
        printf("Start time must be before end time\n");
        return 0;
    }
    if (r >= s) {
        printf("Registration must end before the event begins\n");
        return 0;
    }
    return 1;
}

// Function to check if the provided date is valid or not
int checkValidDate(date d) {
    if (d.month > 12 || d.month <= 0 || d.date <= 0 || d.date > 31 || d.year <= 0 || d.year >= 2050) {
        printf("Invalid date of event\n");
        return 0;
    }
    if ((d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) && d.date > 30) {
        printf("Invalid date: This month has only 30 days\n");
        return 0;
    }
    if (d.month == 2) {
        // Check for leap year
        int isLeapYear = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
        if (isLeapYear && d.date > 29) {
            printf("Invalid date: February has only 29 days in leap year\n");
            return 0;
        }
        if (!isLeapYear && d.date > 28) {
            printf("Invalid date: February has only 28 days\n");
            return 0;
        }
    }
    date today = getCurrentDate();
    if (d.year < today.year) {
        printf("Invalid date: past date provided\n");
        return 0;
    }
    if (d.year == today.year && d.month < today.month) {
        printf("Invalid date: past date provided\n");
        return 0;
    }
    if (d.year == today.year && d.month == today.month && d.date < today.date) {
        printf("Invalid date: past date provided\n");
        return 0;
    }
    return 1;
}

// Function to create new BST node
EventBST* newBSTNode(event e) {
    EventBST* node = (EventBST*)malloc(sizeof(EventBST));
    node->evt = e;
    node->left = node->right = NULL;
    return node;
}

// Insert into BST by eventID
EventBST* insertBST(EventBST* root, event e) {
    if (root == NULL) return newBSTNode(e);
    if (e.eventID < root->evt.eventID)
        root->left = insertBST(root->left, e);
    else if (e.eventID > root->evt.eventID)
        root->right = insertBST(root->right, e);
    return root;
}

// Search in BST by eventID
EventBST* searchBST(EventBST* root, int eventID) {
    if (root == NULL || root->evt.eventID == eventID) return root;
    if (eventID < root->evt.eventID) return searchBST(root->left, eventID);
    return searchBST(root->right, eventID);
}

// Find min value node for delete
EventBST* minValueNode(EventBST* node) {
    EventBST* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete from BST
EventBST* deleteBST(EventBST* root, int eventID) {
    if (root == NULL) return root;
    if (eventID < root->evt.eventID)
        root->left = deleteBST(root->left, eventID);
    else if (eventID > root->evt.eventID)
        root->right = deleteBST(root->right, eventID);
    else {
        if (root->left == NULL) {
            EventBST* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            EventBST* temp = root->left;
            free(root);
            return temp;
        }
        EventBST* temp = minValueNode(root->right);
        root->evt = temp->evt;
        root->right = deleteBST(root->right, temp->evt.eventID);
    }
    return root;
}

// Add  event node to the end of the linked list
void addToList(event e) {
    EventNode* newNode = (EventNode*)malloc(sizeof(EventNode));
    newNode->evt = e;
    newNode->next = NULL;
    if (eventList == NULL) {
        eventList = newNode;
        return;
    }
    EventNode* p = eventList;
    while (p->next) {
        p = p->next;
    }
    p->next = newNode;
}

// Load events from CSV into linked list and BST
void loadEvents(void) {
    FILE *file = fopen("../Data/events.csv", "r");
    if (!file) {
        perror("Error opening events.csv");
        return;
    }
    char line[2048];
    fgets(line, sizeof(line), file);        // Skip header
    while (fgets(line, sizeof(line), file)) {
        struct event e;
        char desc[2048];
        char dateStr[11], startTimeStr[9], endTimeStr[9];
        sscanf(line, "%d,%31[^,],%d,%d,%10[^,],%8[^,],%8[^,],%2047[^\n]",
               &e.eventID, e.eventName, &e.organiserID, &e.venueID,
               dateStr, startTimeStr, endTimeStr, desc);
        sscanf(dateStr, "%hd-%hd-%hd", &e.eventDate.date, &e.eventDate.month, &e.eventDate.year);
        sscanf(startTimeStr, "%hu:%hu:%hu", &e.startTime.hour, &e.startTime.minute, &e.startTime.second);
        sscanf(endTimeStr, "%hu:%hu:%hu", &e.endTime.hour, &e.endTime.minute, &e.endTime.second);
        e.description = (char*)malloc(sizeof(char) * (strlen(desc) + 1));
        strcpy(e.description, desc);
        addToList(e);
        eventTree = insertBST(eventTree, e);
    }
    fclose(file);
}

// Function to get current date
date getCurrentDate(void) {
    date current;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    current.date = (short int)tm_info->tm_mday;                    // Find current date
    current.month = (short int)(tm_info->tm_mon + 1);              // Find current month
    current.year = (short int)(tm_info->tm_year + 1900);           // Find current year
    
    return current;
}

// Function to check if event is past
int isPastEvent(date eventDate) {
    date current = getCurrentDate();
    if (eventDate.year < current.year) return 1;
    if (eventDate.year == current.year && eventDate.month < current.month) return 1;
    if (eventDate.year == current.year && eventDate.month == current.month && eventDate.date < current.date) return 1;
    return 0;
}

// Clean past events using linked list and BST
void cleanPastEvents(void) {
    EventNode *curr = eventList, *prev = NULL;
    while (curr) {
        if (isPastEvent(curr->evt.eventDate)) {
            eventTree = deleteBST(eventTree, curr->evt.eventID);
            if (prev == NULL) {
                eventList = curr->next;
                free(curr);
                curr = eventList;
            } else {
                prev->next = curr->next;
                free(curr);
                curr = prev->next;
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    // Rewrite cleaned events to CSV
    FILE *file = fopen("../Data/events.csv", "w");
    if (!file) {
        perror("Error opening events.csv");
        return;
    }
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime,eventDescription\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu,%s\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second,
                e.description ? e.description : "");
        curr = curr->next;
    }
    fclose(file);
}

// Partition for quicksort
int partition(struct event arr[], int low, int high, int (*compare)(event, event)) {
    struct event pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (compare(arr[j], pivot) < 0) {
            i++;
            struct event temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    struct event temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

// Quicksort algorithm
void quickSort(struct event arr[], int low, int high, int (*compare)(event, event)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        quickSort(arr, low, pi - 1, compare);
        quickSort(arr, pi + 1, high, compare);
    }
}

// Compare by date
int compareByDate(event a, event b) {
    if (a.eventDate.year != b.eventDate.year) return a.eventDate.year - b.eventDate.year;
    if (a.eventDate.month != b.eventDate.month) return a.eventDate.month - b.eventDate.month;
    return a.eventDate.date - b.eventDate.date;
}

// Compare chronologically (date + time)
int compareChronological(event a, event b) {
    int dateCmp = compareByDate(a, b);
    if (dateCmp != 0) return dateCmp;
    int timeA = a.startTime.hour * 3600 + a.startTime.minute * 60 + a.startTime.second;
    int timeB = b.startTime.hour * 3600 + b.startTime.minute * 60 + b.startTime.second;
    return timeA - timeB;
}

// Compare by ID
int compareByID(event a, event b) {
    return a.eventID - b.eventID;
}

// Copy linked list to array, excluding past events
int listToArray(event arr[]) {
    int count = 0;
    EventNode *curr = eventList;
    while (curr) {
        if (!isPastEvent(curr->evt.eventDate)) {
            arr[count++] = curr->evt;
        }
        curr = curr->next;
    }
    return count;
}
void updateEventsOrganized(int userID)
{
    FILE *fp = fopen("../Data/userOrganizer.csv", "r"); // main file to read data
    FILE *temp = fopen("../Data/temp.csv", "w");       // temp file to write/update data

    if (!fp || !temp)
    {
        printf("Error opening user file!\n");
        return;
    }

    char buffer[500];
    fgets(buffer, sizeof(buffer), fp); // skip header & take into buffer
    fprintf(temp, "%s", buffer);       // paste buffer into temp

    int OrgId, eventsOrganized;
    unsigned long phone;
    char name[100], email[100], organisationName[100];

    while (fscanf(fp, "%d,%[^,],%d,%lu,%d,%[^,\n],%[^,]\n",
                  &OrgId, name, &eventsOrganized, &phone, email, organisationName) == 6)
    {
        if (OrgId == userID)
        {
            eventsOrganized++;
        }

        // curr orgaizer data into temp file
        fprintf(temp, "%d,%s,%d,%lu,%d,%s,%s\n",
                OrgId, name, eventsOrganized, phone, email, organisationName);
    }

    fclose(fp);
    fclose(temp);

    // replace old file with new file
    remove("../Data/userOrganizer.csv");
    rename("../Data/temp.csv", "../Data/userOrganizer.csv");
}
// Add event
void addEvent(void) {
    event newEvent;
    int i = 0;
    char dateStr[11], startTimeStr[9], endTimeStr[9], c, regDue[9];
    (newEvent.description) = (char*)malloc(sizeof(char) * 2048);
    userStatus user = getDetails();
    /*
    printf("Enter Event ID: ");
    scanf("%d", &newEvent.eventID);
    */
    newEvent.eventID = generateEventID();
    printf("Your event's alloted event ID: %d\n", newEvent.eventID);
    printf("Your user ID is %d\n", user.userId);
    printf("Enter Event Name: ");
    scanf(" %31[^\n]", newEvent.eventName);
    newEvent.organiserID = user.userId;
    printf("Enter Venue ID: ");
    scanf("%d", &newEvent.venueID);
    printf("Enter Event Date (DD-MM-YYYY): ");
    scanf("%10s", dateStr);
    printf("Enter Start Time (HH:MM:SS in 24 hour format with colons): ");
    scanf("%8s", startTimeStr);
    printf("Enter End Time (HH:MM:SS in 24 hour format with colons): ");
    scanf("%8s", endTimeStr);
    printf("Enter time upto which event can be registered (HH:MM:SS in 24 hour format with colons): ");
    scanf("%8s", regDue);
    getchar();
    printf("Enter the description of the event (Upto 2000 characters)\n");
    while(i <= 2047 && scanf("%c", &c) && c != '\n') {
        newEvent.description[i++] = c;
    }
    newEvent.description[i] = '\0';
    sscanf(dateStr, "%hd-%hd-%hd", &newEvent.eventDate.date, &newEvent.eventDate.month, &newEvent.eventDate.year);
    sscanf(startTimeStr, "%hu:%hu:%hu", &newEvent.startTime.hour, &newEvent.startTime.minute, &newEvent.startTime.second);
    sscanf(endTimeStr, "%hu:%hu:%hu", &newEvent.endTime.hour, &newEvent.endTime.minute, &newEvent.endTime.second);
    sscanf(regDue, "%hu:%hu:%hu", &newEvent.regDue.hour, &newEvent.regDue.minute, &newEvent.regDue.second);
    
    // Validate time and date
    if (checkValidDate(newEvent.eventDate) == 0) return;
    if (checkValidTime(newEvent.regDue, newEvent.startTime, newEvent.endTime) == 0) return;

    // Validate venue availability and existence
    if (!getVenueByID(newEvent.venueID)) {
        printf("Error: Invalid Venue ID.\n");
        return;
    }
    if (!checkVenueAvailability(newEvent.venueID, newEvent.eventDate, newEvent.startTime, newEvent.endTime)) {
        printf("Error: Venue not available.\n");
        return;
    }

    // Check for unique eventID
    if (searchBST(eventTree, newEvent.eventID)) {
        printf("Error: Event ID already exists.\n");
        return;
    }

    addToList(newEvent);
    eventTree = insertBST(eventTree, newEvent);

    // Append to CSV
    FILE *file = fopen("../Data/events.csv", "a");
    if (!file) {
        perror("Error opening events.csv");
        return;
    }
    fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu,%s\n",
        newEvent.eventID, newEvent.eventName, newEvent.organiserID, newEvent.venueID,
        newEvent.eventDate.date, newEvent.eventDate.month, newEvent.eventDate.year,
        newEvent.startTime.hour, newEvent.startTime.minute, newEvent.startTime.second,
        newEvent.endTime.hour, newEvent.endTime.minute, newEvent.endTime.second,
        newEvent.description ? newEvent.description : "");
    fclose(file);

    updateEventsOrganized(newEvent.organiserID);
    printf("Event added!\n");

    // Creation of a file to store the attendees of that event
    char filename[16];
    sprintf(filename, "../Data/event_%d.csv", newEvent.eventID);
    file = fopen(filename, "w");
    if (file)
        fclose(file);
    else
        printf("%s file could not be created\n", filename);

    // event details to be added in organiser_<userID>.csv
    sprintf(filename, "../Data/Organizer_%d.csv", user.userId);
    file = fopen(filename, "a");
    fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu,%s\n",
        newEvent.eventID, newEvent.eventName, newEvent.organiserID, newEvent.venueID,
        newEvent.eventDate.date, newEvent.eventDate.month, newEvent.eventDate.year,
        newEvent.startTime.hour, newEvent.startTime.minute, newEvent.startTime.second,
        newEvent.endTime.hour, newEvent.endTime.minute, newEvent.endTime.second,
        newEvent.description ? newEvent.description : "");
    fclose(file);
}

// Delete event (using BST)
void deleteEvent(void) {
    int eventID;
    printf("Enter Event ID to delete: ");
    scanf("%d", &eventID);

    EventBST* node = searchBST(eventTree, eventID);
    if (!node) {
        printf("Event ID not found.\n");
        return;
    }

    eventTree = deleteBST(eventTree, eventID);

    // Remove from list
    EventNode* curr = eventList, *prev = NULL;
    while (curr) {
        if (curr->evt.eventID == eventID) {
            if (prev == NULL) {
                eventList = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    // Rewrite CSV
    FILE* file = fopen("../Data/events.csv", "w");
    if (!file) {
        perror("Error opening events.csv");
        return;
    }
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime,eventDescription\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu,%s\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second,
                e.description ? e.description : "");
        curr = curr->next;
    }
    fclose(file);
    printf("Event deleted!\n");
}
void modifyEventDetailsInOrganizerFile(event e) {
    userStatus st = getDetails();
    char filename[64];
    sprintf(filename, "Organizer_%d.csv", st.userId);
    

}
// Modify event (using BST)
void modifyEvent(void) {
    int eventID, i = 0;
    char regDue[9], c;
    char *desc = (char*)malloc(sizeof(char) * 2048);
    printf("Enter Event ID to modify: ");
    scanf("%d", &eventID);
    userStatus user = getDetails();
    
    EventBST* node = searchBST(eventTree, eventID);
    if (!node) {
        printf("Event ID not found.\n");
        free(desc);
        return;
    }
    event originalEvent = node->evt;

    char dateStr[11], startTimeStr[9], endTimeStr[9];
    printf("Enter new Event Name: ");
    scanf(" %31[^\n]", node->evt.eventName);
    node->evt.organiserID = user.userId;
    scanf("%d", &node->evt.venueID);
    printf("Enter new Event Date (DD-MM-YYYY): ");
    scanf("%10s", dateStr);
    printf("Enter new Start Time (HH:MM:SS in 24 hour time format): ");
    scanf("%8s", startTimeStr);
    printf("Enter new End Time (HH:MM:SS in 24 hour time format): ");
    scanf("%8s", endTimeStr);
    printf("Enter new time upto which event can be registered (HH:MM:SS in 24 hour format with colons): ");
    scanf("%8s", regDue);
    getchar();
    printf("Enter the description of the event (Upto 2000 characters)\n");
    while(i <= 2047 && scanf("%c", &c) && c != '\n') {
        desc[i++] = c;
    }
    desc[i] = '\0';
    strcpy(node->evt.description, desc);
    sscanf(dateStr, "%hd-%hd-%hd", &node->evt.eventDate.date, &node->evt.eventDate.month, &node->evt.eventDate.year);
    sscanf(startTimeStr, "%hu:%hu:%hu", &node->evt.startTime.hour, &node->evt.startTime.minute, &node->evt.startTime.second);
    sscanf(endTimeStr, "%hu:%hu:%hu", &node->evt.endTime.hour, &node->evt.endTime.minute, &node->evt.endTime.second);
    sscanf(regDue, "%hu:%hu:%hu", &node->evt.regDue.hour, &node->evt.regDue.minute, &node->evt.regDue.second);
    
    // Validate proper date and time
    if (checkValidDate(node->evt.eventDate) == 0) {
        node->evt = originalEvent;
        free(desc);
        return;
    }
    if (checkValidTime(node->evt.regDue , node->evt.startTime, node->evt.endTime) == 0) {
        node->evt = originalEvent;
        free(desc);
        return;
    }
    
    // Validate venue
    if (!getVenueByID(node->evt.venueID)) {
        printf("Error: Invalid Venue ID.\n");
        node->evt = originalEvent;
        free(desc);
        return;
    }
    if (!checkVenueAvailability(node->evt.venueID, node->evt.eventDate, node->evt.startTime, node->evt.endTime)) {
        printf("Error: Venue not available.\n");
        node->evt = originalEvent;
        free(desc);
        return;
    }

    // Update linked list
    EventNode *curr = eventList;
    while (curr) {
        if (curr->evt.eventID == eventID) {
            curr->evt = node->evt;
            break;
        }
        curr = curr->next;
    }

    // Rewrite CSV
    FILE *file = fopen("../Data/events.csv", "w");
    if (!file) {
        perror("Error opening events.csv");
        free(desc);
        return;
    }
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime,eventDescription\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu,%s\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second,
                e.description ? e.description : "");
        curr = curr->next;
    }
    fclose(file);
    free(desc);
    //modify the event details in Organizer_<UserId>.csv
    modifyEventDetails(originalEvent);
    printf("Event modified!\n");
}

// Search event (using BST)
event* searchEventID(void) {
    int eventID;
    printf("Enter Event ID to search: ");
    scanf("%d", &eventID);

    EventBST* node = searchBST(eventTree, eventID);
    if (node) return &node->evt;
    printf("Event ID not found.\n");
    return NULL;
}

// View events (using linked list)
void viewEvents(void) {
    printf("\nEvents List:\n");
    printf("ID | Name | Organiser ID | Venue ID | Date | Start Time | End Time\n");
    EventNode *curr = eventList;
    while (curr) {
        event e = curr->evt;
        if (!isPastEvent(e.eventDate)) {
            printf("%d | %s | %d | %d | %02hd-%02hd-%04hd | %02hu:%02hu:%02hu | %02hu:%02hu:%02hu\n",
                   e.eventID, e.eventName, e.organiserID, e.venueID,
                   e.eventDate.date, e.eventDate.month, e.eventDate.year,
                   e.startTime.hour, e.startTime.minute, e.startTime.second,
                   e.endTime.hour, e.endTime.minute, e.endTime.second);
        }
        curr = curr->next;
    }
}

// Sort events by date (using quicksort)
void sortEventByTime(void) {
    event arr[200];
    int count = listToArray(arr);
    quickSort(arr, 0, count - 1, compareByDate);
    printf("\nEvents Sorted by Date:\n");
    printf("ID | Name | Organiser ID | Venue ID | Date | Start Time | End Time\n");
    for (int i = 0; i < count; i++) {
        event e = arr[i];
        printf("%d | %s | %d | %d | %02hd-%02hd-%04hd | %02hu:%02hu:%02hu | %02hu:%02hu:%02hu\n",
               e.eventID, e.eventName, e.organiserID, e.venueID,
               e.eventDate.date, e.eventDate.month, e.eventDate.year,
               e.startTime.hour, e.startTime.minute, e.startTime.second,
               e.endTime.hour, e.endTime.minute, e.endTime.second);
    }
}

// Function to sort the events chronologically
void sortEventChronological(void) {
    event arr[200];
    int count = listToArray(arr);
    quickSort(arr, 0, count - 1, compareChronological);
    printf("\nEvents Sorted Chronologically:\n");
    printf("ID | Name | Organiser ID | Venue ID | Date | Start Time | End Time\n");
    for (int i = 0; i < count; i++) {
        event e = arr[i];
        printf("%d | %s | %d | %d | %02hd-%02hd-%04hd | %02hu:%02hu:%02hu | %02hu:%02hu:%02hu\n",
               e.eventID, e.eventName, e.organiserID, e.venueID,
               e.eventDate.date, e.eventDate.month, e.eventDate.year,
               e.startTime.hour, e.startTime.minute, e.startTime.second,
               e.endTime.hour, e.endTime.minute, e.endTime.second);
    }
}

// Function to sort the events by ID
void sortEventByID(void) {
    event arr[200];
    int count = listToArray(arr);
    quickSort(arr, 0, count - 1, compareByID);
    printf("\nEvents Sorted by ID:\n");
    printf("ID | Name | Organiser ID | Venue ID | Date | Start Time | End Time\n");
    for (int i = 0; i < count; i++) {
        event e = arr[i];
        printf("%d | %s | %d | %d | %02hd-%02hd-%04hd | %02hu:%02hu:%02hu | %02hu:%02hu:%02hu\n",
               e.eventID, e.eventName, e.organiserID, e.venueID,
               e.eventDate.date, e.eventDate.month, e.eventDate.year,
               e.startTime.hour, e.startTime.minute, e.startTime.second,
               e.endTime.hour, e.endTime.minute, e.endTime.second);
    }
}

// Function to generate a unique event ID
int generateEventID() {
    FILE* file = fopen("../Data/events.csv", "r");
    if (!file) {
        perror("Error opening events.csv");
        return 2000;
    }
    int id = 0;
    char line[256];
    fgets(line, sizeof(line), file);        // Skipping the header line
    while (fgets(line, sizeof(line), file)) {
        int currID;
        sscanf(line, "%d,", &currID);
        if (currID > id) {
            id = currID;                    // Just in case file isn't sorted according to eventID
        }
    }
    fclose(file);
    return id + 1;
}