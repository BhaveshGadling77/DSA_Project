#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "venues.h"
#include "events.h"

#define HASH_SIZE 101               // Hash table size for venues

VenueNode *venueHashTable[HASH_SIZE];

// Hash function for venueID
unsigned int hash(int key) {
    return key % HASH_SIZE;
}

// Function to load venues from CSV into hash table
void loadVenues(void) {
    FILE *file = fopen("../Data/venues.csv", "r");
    if (!file) {
        printf("Error: Unable to open venues.csv\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file);         // Skip header
    while (fgets(line, sizeof(line), file)) {
        Venue v;
        sscanf(line, "%d,%63[^,],%lld,%d,%63[^,],%llu,%63[^,],%d,%d,%127[^,]",
               &v.venueID, v.venueName, &v.capacity, &v.rentalCostPerHour,
               v.venueHandlerName, &v.handlerContact, v.venueHandlerEmail,
               &v.parkingCapacity, &v.indoor, v.location);

        // Insert into hash table
        unsigned int index = hash(v.venueID);
        VenueNode *newNode = (VenueNode *)malloc(sizeof(VenueNode));
        newNode->venue = v;
        newNode->next = venueHashTable[index];
        venueHashTable[index] = newNode;
    }
    fclose(file);
}

// Function to get venue by ID using hash table
Venue* getVenueByID(int venueID) {
    unsigned int index = hash(venueID);
    VenueNode *node = venueHashTable[index];
    while (node) {
        if (node->venue.venueID == venueID) {
            return &node->venue;
        }
        node = node->next;
    }
    return NULL;
}

// Function to create new interval node
Interval* newInterval(int start, int end) {
    Interval* node = (Interval*)malloc(sizeof(Interval));
    node->startSeconds = start;
    node->endSeconds = end;
    node->left = node->right = NULL;
    return node;
}

// Function to insert interval into BST (sorted by start time)
Interval* insertInterval(Interval* root, int start, int end) {
    if (root == NULL) return newInterval(start, end);
    if (start < root->startSeconds)
        root->left = insertInterval(root->left, start, end);
    else
        root->right = insertInterval(root->right, start, end);
    return root;
}

// Function to check if intervals overlap, return 1 if overlap, 0 if not
int doOverlap(int start1, int end1, int start2, int end2) {
    if (end1 < start1) end1 += 24 * 3600;               // Handle midnight crossing
    if (end2 < start2) end2 += 24 * 3600;
    return !(end1 <= start2 || start1 >= end2);
}

// Traversing to check overlap of events
int checkOverlap(Interval* root, int start, int end) {
    if (root == NULL) return 0;
    if (doOverlap(start, end, root->startSeconds, root->endSeconds)) return 1;
    if (checkOverlap(root->left, start, end)) return 1;
    return checkOverlap(root->right, start, end);
}

void freeIntervalTree(Interval* root) {
    if (root == NULL) return;
    freeIntervalTree(root->left);
    freeIntervalTree(root->right);
    free(root);
}

// Function to check if venue is available using interval BST
int checkVenueAvailability(int venueID, struct date eventDate, struct Time startTime, struct Time endTime) {
    FILE *file = fopen("../Data/events.csv", "r");
    if (!file) {
        printf("Error: Unable to open events.csv\n");
        return 0;                        // Assume available if file cannot be opened
    }

    Interval* intervalTree = NULL;
    char line[256];
    fgets(line, sizeof(line), file);         // Skip header
    while (fgets(line, sizeof(line), file)) {
        event evt;
        char dateStr[11], startTimeStr[9], endTimeStr[9];
        sscanf(line, "%d,%31[^,],%d,%d,%10[^,],%8[^,],%8[^,]",
               &evt.eventID, evt.eventName, &evt.organiserID, &evt.venueID,
               dateStr, startTimeStr, endTimeStr);

               // Convert date and time appropriately according to the definition
        sscanf(dateStr, "%hd-%hd-%hd", &evt.eventDate.date, &evt.eventDate.month, &evt.eventDate.year);
        sscanf(startTimeStr, "%hu:%hu:%hu", &evt.startTime.hour, &evt.startTime.minute, &evt.startTime.second);
        sscanf(endTimeStr, "%hu:%hu:%hu", &evt.endTime.hour, &evt.endTime.minute, &evt.endTime.second);

        if (evt.venueID == venueID &&
            evt.eventDate.year == eventDate.year &&
            evt.eventDate.month == eventDate.month &&
            evt.eventDate.date == eventDate.date) {
            int start2 = evt.startTime.hour * 3600 + evt.startTime.minute * 60 + evt.startTime.second;
            int end2 = evt.endTime.hour * 3600 + evt.endTime.minute * 60 + evt.endTime.second;
            intervalTree = insertInterval(intervalTree, start2, end2);
        }
    }
    fclose(file);

    int start1 = startTime.hour * 3600 + startTime.minute * 60 + startTime.second;
    int end1 = endTime.hour * 3600 + endTime.minute * 60 + endTime.second;

    int overlap = checkOverlap(intervalTree, start1, end1);

    // Free the memory and return overlap situation
    freeIntervalTree(intervalTree);
    return !overlap;
}