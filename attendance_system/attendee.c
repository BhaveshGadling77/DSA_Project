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