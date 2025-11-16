#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "events.h"
#include "venues.h"
#include <errno.h>

EventNode *eventList = NULL; // Linked list for sequential access
EventBST *eventTree = NULL; // BST for fast search/delete by ID

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

// Add to linked list
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

    /*
    newNode->next = eventList;
    eventList = newNode;
    */
}

// Load events from CSV into linked list and BST
void loadEvents(void) {
    FILE *file = fopen("../Data/events.csv", "r");
    if (!file) {
        perror("Error opening events.csv");
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);        // Skip header
    while (fgets(line, sizeof(line), file)) {
        struct event e;
        char dateStr[11], startTimeStr[9], endTimeStr[9];
        sscanf(line, "%d,%31[^,],%d,%d,%10[^,],%8[^,],%8[^,]",
               &e.eventID, e.eventName, &e.organiserID, &e.venueID,
               dateStr, startTimeStr, endTimeStr);
        sscanf(dateStr, "%hd-%hd-%hd", &e.eventDate.date, &e.eventDate.month, &e.eventDate.year);
        sscanf(startTimeStr, "%hu:%hu:%hu", &e.startTime.hour, &e.startTime.minute, &e.startTime.second);
        sscanf(endTimeStr, "%hu:%hu:%hu", &e.endTime.hour, &e.endTime.minute, &e.endTime.second);

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
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second);
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

// Add event
void addEvent(void) {
    event newEvent;
    char dateStr[11], startTimeStr[9], endTimeStr[9];

    printf("Enter Event ID: ");
    scanf("%d", &newEvent.eventID);
    printf("Enter Event Name: ");
    scanf(" %31[^\n]", newEvent.eventName);
    printf("Enter Organiser ID: ");
    scanf("%d", &newEvent.organiserID);
    printf("Enter Venue ID: ");
    scanf("%d", &newEvent.venueID);
    printf("Enter Event Date (DD-MM-YYYY): ");
    scanf("%10s", dateStr);
    printf("Enter Start Time (HH:MM:SS): ");
    scanf("%8s", startTimeStr);
    printf("Enter End Time (HH:MM:SS): ");
    scanf("%8s", endTimeStr);

    sscanf(dateStr, "%hd-%hd-%hd", &newEvent.eventDate.date, &newEvent.eventDate.month, &newEvent.eventDate.year);
    sscanf(startTimeStr, "%hu:%hu:%hu", &newEvent.startTime.hour, &newEvent.startTime.minute, &newEvent.startTime.second);
    sscanf(endTimeStr, "%hu:%hu:%hu", &newEvent.endTime.hour, &newEvent.endTime.minute, &newEvent.endTime.second);

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
    fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu\n",
            newEvent.eventID, newEvent.eventName, newEvent.organiserID, newEvent.venueID,
            newEvent.eventDate.date, newEvent.eventDate.month, newEvent.eventDate.year,
            newEvent.startTime.hour, newEvent.startTime.minute, newEvent.startTime.second,
            newEvent.endTime.hour, newEvent.endTime.minute, newEvent.endTime.second);
    fclose(file);
    printf("Event added!\n");
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
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second);
        curr = curr->next;
    }
    fclose(file);
    printf("Event deleted!\n");
}

// Modify event (using BST)
void modifyEvent(void) {
    int eventID;
    printf("Enter Event ID to modify: ");
    scanf("%d", &eventID);

    EventBST* node = searchBST(eventTree, eventID);
    if (!node) {
        printf("Event ID not found.\n");
        return;
    }

    char dateStr[11], startTimeStr[9], endTimeStr[9];
    printf("Enter new Event Name: ");
    scanf(" %63[^\n]", node->evt.eventName);
    printf("Enter new Organiser ID: ");
    scanf("%d", &node->evt.organiserID);
    printf("Enter new Venue ID: ");
    scanf("%d", &node->evt.venueID);
    printf("Enter new Event Date (DD-MM-YYYY): ");
    scanf("%10s", dateStr);
    printf("Enter new Start Time (HH:MM:SS in 24 hour time format): ");
    scanf("%8s", startTimeStr);
    printf("Enter new End Time (HH:MM:SS in 24 hour time format): ");
    scanf("%8s", endTimeStr);

    sscanf(dateStr, "%hd-%hd-%hd", &node->evt.eventDate.date, &node->evt.eventDate.month, &node->evt.eventDate.year);
    sscanf(startTimeStr, "%hu:%hu:%hu", &node->evt.startTime.hour, &node->evt.startTime.minute, &node->evt.startTime.second);
    sscanf(endTimeStr, "%hu:%hu:%hu", &node->evt.endTime.hour, &node->evt.endTime.minute, &node->evt.endTime.second);

    // Validate venue
    if (!getVenueByID(node->evt.venueID)) {
        printf("Error: Invalid Venue ID.\n");
        return;
    }
    if (!checkVenueAvailability(node->evt.venueID, node->evt.eventDate, node->evt.startTime, node->evt.endTime)) {
        printf("Error: Venue not available.\n");
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
        return;
    }
    fprintf(file, "eventID,eventName,organiserID,venueID,eventDate,startTime,endTime\n");
    curr = eventList;
    while (curr) {
        event e = curr->evt;
        fprintf(file, "%d,%s,%d,%d,%02hd-%02hd-%04hd,%02hu:%02hu:%02hu,%02hu:%02hu:%02hu\n",
                e.eventID, e.eventName, e.organiserID, e.venueID,
                e.eventDate.date, e.eventDate.month, e.eventDate.year,
                e.startTime.hour, e.startTime.minute, e.startTime.second,
                e.endTime.hour, e.endTime.minute, e.endTime.second);
        curr = curr->next;
    }
    fclose(file);
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