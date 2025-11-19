/* Only Logic of the functions should be here.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "attendee.h"
#include "../Login_Registration_system/login_registration.h"

// get current date & time function
void getCurrentDateTime(char *buffer)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(buffer, "%02d-%02d-%d %02d:%02d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    // example format
    // 26-10-2024 14:30
}
void updateEventsAttended(int userID)
{
    FILE *fp = fopen("../Data/userAttendee.csv", "r"); // main file to read data
    FILE *temp = fopen("../Data/temp.csv", "w");       // temp file to write/update data

    if (!fp || !temp)
    {
        printf("Error opening user file!\n");
        return;
    }

    char buffer[500];
    fgets(buffer, sizeof(buffer), fp); // skip header & take into buffer
    fprintf(temp, "%s", buffer);       // paste buffer into temp

    int id, eventsAttended;
    unsigned long phone;
    char name[100], email[100];

    while (fscanf(fp, "%d,%[^,],%lu,%d,%[^,\n]\n",
                  &id, name, &phone, &eventsAttended, email) == 5)
    {
        if (id == userID)
        {
            eventsAttended++;
        }

        // curr user data into temp file
        fprintf(temp, "%d,%s,%lu,%d,%s\n",
                id, name, phone, eventsAttended, email);
    }

    fclose(fp);
    fclose(temp);

    // replace old file with new file
    remove("../Data/userAttendee.csv");
    rename("../Data/temp.csv", "../Data/userAttendee.csv");
}
bool fetchUserData(int userID, Attendee *a)
{
    FILE *fp;
    fp = fopen("../Data/userAttendee.csv", "r"); // open in read mode
    if (!fp)
    {
        printf("Error, User databases not found!\n");
        return false;
    }

    // to skip header i.e userId, name, phone no., noOfEventsAttended, email
    char buffer[500];
    fgets(buffer, sizeof(buffer), fp);

    // search for user
    int id;
    char name[100];
    char email[100];
    int eventsAttended;
    unsigned long phone;

    while (fscanf(fp, "%d,%[^,],%lu,%d,%[^,]\n", &id, name, &phone, &eventsAttended, email) == 5)
    {
        if (id == userID)
        {
            // found user, now into event registration details
            strcpy(a->name, name);
            strcpy(a->email, email);
            a->phoneNo = phone;
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false; // user not found
}
void registerAttendeeForEvent(Node **head, int eventID, userStatus *user)
{
    if (!user->status)
    { // logged in or not status
        printf("\nError, Please login first !\n");
        return;
    }

    // check if user is organizer (cuz organizers shouldn't register as attendee)
    if (user->isOrg)
    {
        printf("\nError, Organizer cannot register as attendees!\n");
        return;
    }
    // check if already registered
    Node *temp = *head;
    while (temp != NULL)
    {
        if (temp->data.attendeeID == user->userId)
        {
            printf("\n You're already registered for this event!\n");
            return;
        }
        temp = temp->next;
    }

    // else register new attendee
    // fetch user data from userAttendee.csv
    Attendee a;
    if (!fetchUserData(user->userId, &a))
    {
        printf("\n Error, Couldn't fetch user data!\n");
        return;
    }

    // set attendee specific data
    a.attendeeID = user->userId;
    a.eventID = eventID;
    strcpy(a.status, "Registered");
    getCurrentDateTime(a.registrationDate);

    // create node & insert in linked list
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = a;
    node->next = *head;
    *head = node;

    // save attendee to event file
    saveToFile(*head, eventID);

    // update no. of events attended
    updateEventsAttended(user->userId);

    printf("\nSuccessfully registered for event %d!\n", eventID);
}

void unregisterAttendee(Node **head, userStatus *user)
{
    Node *temp = *head, *prev = NULL;

    while (temp != NULL && temp->data.attendeeID != user->userId)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("\n Attendee Not Found.\n");
        return;
    }

    if (prev == NULL)
        *head = temp->next; // temp is not null
    else
        prev->next = temp->next;

    saveToFile(*head, temp->data.eventID);
    printf("\n Unregistered: %s\n", temp->data.name);
    free(temp);
}

void markAttendance(Node *head)
{
    int id;
    printf("\nAttendee ID: ");
    scanf("%d", &id);

    Node *temp = head;

    while (temp != NULL)
    {
        if (temp->data.attendeeID == id)
        {
            strcpy(temp->data.status, "Present");
            saveToFile(head, temp->data.eventID);
            printf("\n Marked Present: %s\n", temp->data.name);
            return;
        }
        temp = temp->next;
    }
    printf("\n Id Not Found.\n");
}

void viewAllAttendees(Node *head, int eventID)
{
    FILE *fp;
    char filePath[100];

    // write file path in filePath variable
    sprintf(filePath, "data/attendees/eventDetails_%d.txt", eventID);

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
        printf("%-5d %-20s %-25s %-15lu %-12s\n",
               temp->data.attendeeID, temp->data.name, temp->data.email,
               temp->data.phoneNo, temp->data.status);

        // Write same line to file
        fprintf(fp, "%-5d %-20s %-25s %-15lu %-12s\n",
                temp->data.attendeeID, temp->data.name, temp->data.email,
                temp->data.phoneNo, temp->data.status);

        temp = temp->next;
    }

    fclose(fp);
    printf("\nFile created: %s\n", filePath);
}

void searchAttendee(Node *head)
{
    int id;
    printf("\n Attendee ID: ");
    scanf("%d", &id);

    Node *temp = head;
    while (temp != NULL)
    {
        if (temp->data.attendeeID == id)
        {
            printf("\n--- Attendee Details ---");
            printf("ID: %d\n", temp->data.attendeeID);
            printf("Name: %s\n", temp->data.name);
            printf("Email: %s\n", temp->data.email);
            printf("Phone: %lu\n", temp->data.phoneNo);
            printf("Status: %s\n", temp->data.status);
            printf("Registered on: %s\n", temp->data.registrationDate);
            return;
        }
        temp = temp->next;
    }
    printf("\n Not Found\n");
}
void viewStatistics(Node *head)
{
    int total = 0, present = 0;
    Node *temp = head;

    while (temp != NULL)
    {
        total++;
        if (strcmp(temp->data.status, "Present") == 0)
            present++;
        temp = temp->next;
    }
    printf("\n--- Statistics---");
    printf("Total Registered: %d\n", total);
    printf("Present: %d\n", present);
    printf("Absent: %d\n", total - present);
    if (total > 0)
    {
        // attendance percentage
        // %% to show % symbol
        printf("Attendance: %.2f%%\n", (float)present / total * 100);
    }
}
void saveToFile(Node *head, int eventId)
{
    char filename[100];

    // sprintf function use to write to the file
    sprintf(filename, "data/attendees/event_%d.csv", eventId);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Couldn't create file.\n");
        return;
    }
    // header line
    fprintf(fp, "AttendeeID,Name,Email,Phone,EventId,Status,RegistrationDate\n");

    Node *temp = head;
    while (temp != NULL)
    {
        fprintf(fp, "%d,%s,%s,%lu,%d,%s,%s\n",
                temp->data.attendeeID, temp->data.name, temp->data.email, temp->data.phoneNo, temp->data.eventID,
                temp->data.status, temp->data.registrationDate);
        temp = temp->next;
    }
    fclose(fp);
    printf("File 'data/attendees/event_%d.csv' has been saved.", eventId);
}
void loadFromFile(Node **head, int eventID)
{
    char filename[100];
    sprintf(filename, "data/attendees/event_%d.csv", eventID); //  loads file

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return;

    char buffer[500];
    fgets(buffer, sizeof(buffer), fp); // to skip header line while reading data from file
    // as we've wrote header line before saving file.

    Attendee a;
    // delimiters are handled.
    while (fscanf(fp, "%d,%[^,],%[^,],%lu,%d,%[^,],%[^\n]\n",
                  &a.attendeeID, a.name, a.email, &a.phoneNo, &a.eventID,
                  a.status, a.registrationDate) == 7)
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->data = a;
        newNode->next = *head;
        *head = newNode;
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
