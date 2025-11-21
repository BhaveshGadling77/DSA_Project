#include <stdio.h>
#include "attendee.h"
#include "login_registration.h"
#include "events.h"
#include "venues.h"
#include <stdlib.h>
void optionsAtOrganizer() {
	userStatus st = getDetails();
	printf("Organiser Id : %d\n", st.userId);
	loadEvents();
	loadVenues();
	cleanPastEvents();
	int choice, check = 1;
	while (check != -1) {
		printf("1. Add Event.\n");
		printf("2. Delete Event.\n");
		printf("3. Modify Event.\n");
		printf("4. View All Attendees.\n");
		printf("5. Mark Attendees.\n");
		printf("6. View Statistics.\n");
		printf("7. List Your Events\n");
		printf("8. List All Available veunues.\n");
		printf("9. Logout.\n");
		int check = scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			/* Add events */
			addEvent();
			break;
		
		case 2:
			deleteEvent();
			break;
		
		case 3:
			modifyEvent();
			break;

		case 4:
			printf("\n View All Attendees.\n");
			int eventId;
			printf("Enter the Event Id which you want to view.\n");
			scanf("%d", &eventId);
			Node *head;
			loadFromFile(&head, eventId);
			viewAllAttendees(head, eventId);
			freeList(head);
			break;

		case 5:
			printf("\nMark All Attendees.\n");
			printf("Enter the Event Id which you want to mark Attendee.\n");
			scanf("%d", &eventId);
			loadFromFile(&head, eventId);
			markAttendance(head);
			freeList(head);
			break;

		case 6:
			printf("Enter the Event Id which you want to View Statistics.\n");
			scanf("%d", &eventId);
			loadFromFile(&head, eventId);
			viewStatistics(head);
			freeList(head);
			printf("\nView Statistics.\n");
			break;
		
		case 7:
			listEventsOfOrganizer();
			break;

		case 8:
			viewVenues();
			printf("List of Venues\n");
			break;
		case 9:
			printf("\nLogging out");
			for (int i = 0; i < 3; i++) {
				printf(".");
				fflush(stdout);
				sleep(1);
			}
			printf("\n");
			return;

		default:
			printf("\nInvalid Options.\n");
			break;
		}
	}
}
void optionsAtAttendee() {
	userStatus st = getDetails();
	printf("attendee Id : %d\n", st.userId);
	int choice, check = 1;
	
	while (check != -1) {
		printf("1. register for Event.\n");
		printf("2. unregister for Event.\n");
		printf("3. Sort Events by Chronological Order.\n");
		printf("4. Sort Events by There IDs.\n");
		printf("5. Sort Events by there Timings.\n");
		printf("6. List All the Events.\n");
		printf("7. Logout\n");
		int check = scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			/* register for event */
			printf("\nRegistered for Event.\n");
			int eventId;
			Node *head;
			printf("Enter The Event ID: ");
			scanf("%d", &eventId);
			loadFromFile(&head, eventId);
			registerAttendeeForEvent(&head, eventId, &st);
			saveToFile(head, eventId);
			freeList(head);
			break;
		
		case 2:
			printf("Enter the Event ID: ");
			scanf("%d", &eventId);
			loadFromFile(&head, eventId);
			unregisterAttendee(&head, &st);
			saveToFile(head, eventId);
			freeList(head);
			printf("\nunRegistered for Event.\n");
			break;

		case 3:
			sortEventChronological();
			printf("\nSorted the Events by There chronological order.\n");
			break;

		case 4:
			sortEventByID();
			printf("\nSorted the Events by There IDs.\n");
			break;
		
		case 5:
			sortEventByTime();
			printf("\nSorted the Events by there Timings.\n");
			break;
		
		case 6:
			viewEvents();
			printf("\nList All the Events.\n");
			break;
		case 7:
			printf("\nLogging out");
			for (int i = 0; i < 3; i++) {
				printf(".");
				fflush(stdout);
				sleep(1);
			}
			printf("\n");
			return;

		default:
			printf("\nInvalid Options.\n");
			break;
		}
	}
}

int main() {
	int choice, check = 0;
	printf("What you want to do ?\n");
	while (check != -1) {
		printf("1. Register\n");
		printf("2. Login\n");
		printf("3. Exit the Program.\n");
		check = scanf("%d", &choice);
		if (choice == 1) {
			/*This is the choice for the Register */
			printf("1. Register as Organiser.\n");
			printf("2. Register as Attendee \n");
			scanf("%d", &choice);
			if (choice == 1) {
				fflush(stdout);
				registerAsUser(1);
				printf("Logged in As Organiser.\n");
				optionsAtOrganizer();
			} else {
				registerAsUser(2);
				printf("Logged in As Attendee.\n");
				optionsAtAttendee();
			}
			printf("Please Login now.\n");
			
		} else if (choice == 2) {
			/* This is the Choice for the Login.*/
			printf("1. Login as Organiser.\n");
			printf("2. Login as Attendee \n");
			scanf("%d", &choice);
			if (choice == 1) {
				fflush(stdout);
				loginAsUser(1);
				printf("Logged in As Organizer.\n");
				optionsAtOrganizer();
			} else {
				loginAsUser(2);
				printf("Logged in As Attendee.\n");
				optionsAtAttendee();
			}
			
			
		} else {
			printf("Thank You for using our system.\n");
			printf("Exiting The Program");
			for (int i = 0; i < 3; i++) {
				printf(".");
				sleep(1);
				fflush(stdout);
			}
			printf("\n");
			exit(0);
		}
	}
}