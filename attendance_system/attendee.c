/* Only Logic of the functions should be here.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "attendee.h"

// get current date & time function
void getCurrentDateTime(char *buffer){
    time_t now = time(NULL);
    struct tm *t = localtime(&now); 
    sprintf(buffer, "%02d-%02d-%d %02d:%02d", t->tm_mday, t->tm_mon+1, t->tm_year+1900, t->tm_hour, t->tm_min);
    // example format
    // 26-10-2024 14:30
}

void RegisterAsAttendee(Node **head, int eventID){
    Attendee a;
    a.eventID = eventID;
    a.attendeeID = (*head == NULL) ? 1 : (*head)->data.attendeeID + 1; // increment id 

    printf("Enter you details: \n");
    printf("Name: "); scanf(" %[^\n]", a.name); 
    printf("Email: "); scanf("%s", a.email);
    printf("Phone: "); scanf("%lu", &a.phoneNo);


    strcpy(a.status, "Registered");
    getCurrentDateTime(a.registrationDate);

    Node *t = (Node *)malloc(sizeof(Node));
    t->data = a;
    t->next = *head;
    *head = t;


    printf("\n Registered! ID: %d\n", a.attendeeID);
}

void unregisterAttendee(Node **head, int attendeeId){
    Node *temp = *head, *prev = NULL;

    while(temp != NULL && temp->data.attendeeID != attendeeId){
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL){
        printf("\n Attendee Not Found.\n");
        return;
    }

    if(prev == NULL) *head = temp->next; // temp is not null
    else prev->next = temp->next;

    printf("\n Unregistered: %s\n", temp->data.name);
    free(temp);
}

void markAttendance(Node *head){
    int id;
    printf("\nAttendee ID: "); scanf("%d", &id);

    Node* temp = head;

    while(temp != NULL){
        if(temp->data.attendeeID == id){
            strcpy(temp->data.status, "Present");
            printf("\n Marked Present: %s\n", temp->data.name);
            return;
        }
        temp = temp->next;
    }
    printf("\n Id Not Found.\n");
}

void viewAttendees(Node* head){
    if(head == NULL){
        printf("\n No attendees.!\n");
        return;
    }
    printf("\n%-5s %-20s %-25s %-15s %-12s\n", "ID", "Name", "Email", "Phone", "Status");
    
    Node *temp = head;
    while(temp != NULL){
        printf("%-5d %-20s %-25s %-15lu %-12s\n", 
               temp->data.attendeeID, temp->data.name, temp->data.email, 
               temp->data.phoneNo, temp->data.status);

        temp = temp->next;
    }
}
void searchAttendee(Node *head){
    int id;
    printf("\n Attendee ID: "); scanf("%d", &id);


    Node *temp = head;
    while(temp != NULL){
        if(temp->data.attendeeID == id){
            printf("\n--- Attendee Details ---");
            printf("ID: %d\n", temp->data.attendeeID);
            printf("Name: %s\n", temp->data.name);
            printf("Email: %s\n", temp->data.email);
            printf("Phone: %lu\n", temp->data.phoneNo);
            printf("Status: %s\n", temp->data.status);
            printf("Registered: %s\n", temp->data.registrationDate);
            return;
       }
       temp = temp->next;
    }
    printf("\n Not Found\n");
}
void viewStatistics(Node *head){
    int total = 0, present = 0;
    Node *temp = head;

    while(temp != NULL){
        total++;
        if(strcmp(temp->data.status, "Present") == 0) present++;
        temp = temp->next;
    }
    printf("\n--- Statistics---");
    printf("Total Registered: %d\n", total);
    printf("Present: %d\n", present);
    printf("Absent: %d\n", total-present);
    if(total > 0){
        // attendance percentage
        // %% to show % symbol
        printf("Attendance: %.2f%%\n", (float)present/total*100);
    }
}
void saveToFile(Node *head, int eventId){
    char filename[100];
    // to create file which is downloadable

    // sprintf function use to write to the file
    sprintf(filename, "data/attendance/event_%d.txt", eventId);

    FILE *fp = fopen(filename, "w");
    if(fp == NULL) return;

    Node *temp = head;
    while(temp != NULL){
        fprintf(fp, "%d|%s|%s|%lu|%d|%s|%s\n",
            temp->data.attendeeID, temp->data.name, temp->data.email, temp->data.phoneNo, temp->data.eventID,
        temp->data.status, temp->data.registrationDate);
        temp = temp->next;
    }
    fclose(fp);
    printf("File 'data/attendance/event_%d.txt' has been saved.", eventId);
}

