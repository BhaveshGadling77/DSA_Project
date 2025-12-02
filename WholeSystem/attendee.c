#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "attendee.h"

void getCurrentDateTime(char *buffer)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(buffer, "%02d-%02d-%d %02d:%02d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
}

void updateEventsAttended(int userID)
{
    FILE *fp = fopen("../Data/userAttendee.csv", "r");
    FILE *temp = fopen("../Data/temp.csv", "w");

    int id, events;
    unsigned long long phone;
    char name[100], email[100];

    while (fscanf(fp, "%d,%[^,],%d,%llu,%s",
                  &id, name, &events, &phone, email) == 5)
    {
        if (id == userID) events++;

        fprintf(temp, "%d,%s,%d,%llu,%s\n",
                id, name, events, phone, email);
    }

    fclose(fp);
    fclose(temp);

    remove("../Data/userAttendee.csv");
    rename("../Data/temp.csv", "../Data/userAttendee.csv");
}


bool fetchUserData(int userID, Attendee *a)
{
    FILE *fp = fopen("../Data/userAttendee.csv", "r");
    if (!fp) return false;

    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (!token) continue;

        int id = atoi(token);
        if (id != userID) continue;

        // name
        token = strtok(NULL, ",");
        strcpy(a->name, token);

        // events attended (ignore if not needed)
        token = strtok(NULL, ",");
        a->eventID = atoi(token);

        // phone
        token = strtok(NULL, ",");
        a->phoneNo = strtoull(token, NULL, 10);

        // email
        token = strtok(NULL, "\n");
        strcpy(a->email, token);

        fclose(fp);
        return true;
    }

    fclose(fp);
    return false;
}


void registerAttendeeForEvent(Node **head, int eventID, userStatus *user)
{
    if (!user->status)
    {
        printf("\nError: Please login first!\n");
        return;
    }
    // Organizers cannot register as attendees
    if (user->isOrg)
    {
        printf("\nError: Organizers cannot register as attendees!\n");
        return;
    }

    // Check if already registered
    Node *temp = *head;
    while (temp != NULL)
    {
        if (temp->data.attendeeID == user->userId)
        {
            printf("\nYou're already registered for this event!\n");
            return;
        }
        temp = temp->next;
    }

    // Fetch user data
    Attendee a;
    printf("your userId is :- %d\n", user->userId);
    if (!fetchUserData(user->userId, &a))
    {
        printf("\nError: Couldn't fetch user data!\n");
        return;
    }

    // Set attendee data
    a.attendeeID = user->userId;
    a.eventID = eventID;
    strcpy(a.status, "Registered");
    getCurrentDateTime(a.registrationDate);

    // Create and insert node
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = a;
    newNode->next = *head;
    *head = newNode;

    // save to file too
    saveToFile(*head, eventID);

    // Update events attended by attendee
    updateEventsAttended(user->userId);

    printf("\nSuccessfully registered for event %d!\n", eventID);
}

void unregisterAttendee(Node **head, userStatus *user)
{
    if (*head == NULL)
    {
        printf("\nNo registrations found.\n");
        return;
    }

    Node *temp = *head, *prev = NULL;

    while (temp != NULL && temp->data.attendeeID != user->userId)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\nYou are not registered for any event.\n");
        return;
    }

    int eventID = temp->data.eventID;
    
    // Unlink node
    if (prev == NULL)
        *head = temp->next;
    else
        prev->next = temp->next;

    printf("\nUnregistered: %s from event %d\n", temp->data.name, eventID);
    
    // Free the node
    free(temp);
    
    // Save updated list
    saveToFile(*head, eventID);
}

void markAttendance(Node **head)
{
    if (*head == NULL)
    {
        printf("\nNo attendees registered.\n");
        return;
    }

    int id;
    printf("\nEnter Attendee ID: ");
    scanf("%d", &id);

    Node *temp = *head;
    while (temp != NULL) {
        if (temp->data.attendeeID == id)
        {
            if (strcmp(temp->data.status, "Present") == 0)
            {
                printf("\nAlready marked present: %s\n", temp->data.name);
                return;
            }
            
            strcpy(temp->data.status, "Present");
            saveToFile(*head, temp->data.eventID);
            printf("\nMarked Present: %s\n", temp->data.name);
            return;
        }
        temp = temp->next;
    }
    printf("\nAttendee ID not found.\n");
}

void viewAllAttendees(Node *head, int eventID)
{
    FILE *fp;
    char filePath[100];

    // write file path in filePath variable
    sprintf(filePath, "../Data/attendees/eventDetails_%d.txt", eventID);

    // Open file for writing
    fp = fopen(filePath, "w");
    if (fp == NULL)
    {
        printf("Error: Could not create file %s\n", filePath);
        return;
    }

    if (head == NULL)
    {
        printf("\nNo attendees!\n");
        fprintf(fp, "No attendees!\n"); // written in file as well
        fclose(fp);
        return;
    }

    // header to print on terminal
    printf("\n%-5s %-20s %-25s %-15s %-12s\n", "ID", "Name", "Email", "Phone", "Status");

    // also write in file
    fprintf(fp, "%-5s %-20s %-25s %-15s %-12s\n",
            "ID", "Name", "Email", "Phone", "Status");

    Node *temp = head;
    while (temp != NULL)
    {
        // Print to terminal
        printf("%-5d %-20s %-25s %-15llu %-12s\n",
               temp->data.attendeeID, temp->data.name, temp->data.email,
               temp->data.phoneNo, temp->data.status);

        // Write same line to file
        fprintf(fp, "%-5d %-20s %-25s %-15llu %-12s\n",
                temp->data.attendeeID, temp->data.name, temp->data.email,
                temp->data.phoneNo, temp->data.status);

        temp = temp->next;
    }

    fclose(fp);
    printf("\nFile created: %s\n", filePath);
}

void searchAttendee(Node *head)
{
    if (head == NULL)
    {
        printf("\nNo attendees to search.\n");
        return;
    }

    int id;
    printf("\nEnter Attendee ID: ");
    scanf("%d", &id);

    Node *temp = head;
    while (temp != NULL)
    {
        if (temp->data.attendeeID == id)
        {
            printf("\n--- Attendee Details ---\n");
            printf("ID: %d\n", temp->data.attendeeID);
            printf("Name: %s\n", temp->data.name);
            printf("Email: %s\n", temp->data.email);
            printf("Phone: %llu\n", temp->data.phoneNo);
            printf("Event ID: %d\n", temp->data.eventID);
            printf("Status: %s\n", temp->data.status);
            printf("Registered: %s\n", temp->data.registrationDate);
            return;
        }
        temp = temp->next;
    }
    printf("\nAttendee not found.\n");
}

void viewStatistics(Node *head)
{
    if (head == NULL)
    {
        printf("\nNo data to generate statistics.\n");
        return;
    }

    int total = 0, present = 0;
    Node *temp = head;

    while (temp != NULL)
    {
        total++;
        if (strcmp(temp->data.status, "Present") == 0)
            present++;
        temp = temp->next;
    }

    printf("\n--- Statistics ---\n");
    printf("Total Registered: %d\n", total);
    printf("Present: %d\n", present);
    printf("Absent: %d\n", total - present);
    
    if (total > 0)
    {
        printf("Attendance: %.2f%%\n", (float)present / total * 100);
    }
}

void saveToFile(Node *head, int eventId)
{
    char filename[100];
    sprintf(filename, "../Data/event_%d.csv", eventId);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error: Couldn't create file, check if folder exists.%s\n", filename);
        return;
    }

    // Write header
    fprintf(fp, "AttendeeID,Name,Email,Phone,EventID,Status,RegistrationDate\n");

    Node *temp = head;
    while (temp != NULL)
    {
        fprintf(fp, "%d,%s,%s,%llu,%d,%s,%s\n",
                temp->data.attendeeID, 
                temp->data.name, 
                temp->data.email, 
                temp->data.phoneNo, 
                temp->data.eventID,
                temp->data.status, 
                temp->data.registrationDate);
        temp = temp->next;
    }
    
    fclose(fp);
    printf("Data saved to %s\n", filename);
}

void loadFromFile(Node **head, int eventID)
{
    if (head) *head = NULL; // reset head always to avoid garbage value
    char filename[100];
    sprintf(filename, "../Data/event_%d.csv", eventID);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return;
    }

    char line[1024];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        char *token;

        Attendee a;
        memset(&a, 0, sizeof(a));

        // AttendeeID
        token = strtok(p, ",");
        if (!token) continue;
        a.attendeeID = atoi(token);

        // Name
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(a.name, token, sizeof(a.name)-1);

        // Email
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(a.email, token, sizeof(a.email)-1);

        // Phone
        token = strtok(NULL, ",");
        if (!token) continue;
        a.phoneNo = strtoul(token, NULL, 10);

        // EventID
        token = strtok(NULL, ",");
        if (!token) continue;
        a.eventID = atoi(token);

        // Status
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(a.status, token, sizeof(a.status)-1);

        // RegistrationDate (rest of line)
        token = strtok(NULL, "\n");
        if (token) {
            strncpy(a.registrationDate, token, sizeof(a.registrationDate)-1);
        }

        // create node
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (!newNode) {
            // handle malloc failure gracefully
            break;
        }
        newNode->data = a;
        newNode->next = *head;
        *head = newNode;
        memset(line, 0, sizeof(line));
    }

    fclose(fp);
}

void freeList(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
