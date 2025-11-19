#include <stdio.h>
#include "login_registration.h"
#include <stdlib.h>
void optionsAtOrganizer() {
	userStatus st = getDetails();
	printf("Organiser Id : %d\n", st.userId);
	int choice, check = 1;
	while (check != -1) {
		printf("1. Add Event.\n");
		printf("2. Delete Event.\n");
		printf("3. Modify Event.\n");
		printf("4. List the Attendees.\n");
		printf("5. Logout.\n");
		int check = scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			/* Add events */
			printf("\nAdded the Event.\n");
			break;
		
		case 2:
			printf("\nDelete the Event.\n");
			break;
		
		case 3:
			printf("\nSuccessfully Modified the Event.\n");
			break;

		case 4:
			printf("\nList of Attendees for a particular Event.\n");
			break;
		case 5:
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
	printf("Organiser Id : %d\n", st.userId);
	int choice, check = 1;
	while (check != -1) {
		printf("1. register for Event.\n");
		printf("2. unregister for Event.\n");
		printf("3. Mark the Attendence for Event.\n");
		printf("4. Sort Events by Chronological Order.\n");
		printf("5. Sort Events by There IDs.\n");
		printf("6. Sort Events by there Timings.\n");
		printf("7. List All the Events.\n");
		printf("8. Logout\n");
		int check = scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			/* Add events */
			printf("\nRegistered for Event.\n");
			break;
		
		case 2:
			printf("\nunRegistered for Event.\n");
			break;
		
		case 3:
			printf("\nMark the Attendence for the Event.\n");
			break;

		case 4:
			printf("\nSorted the Events by There chronological order.\n");
			break;

		case 5:
			printf("\nSorted the Events by There IDs.\n");
			break;
		
		case 6:
			printf("\nSorted the Events by there Timings.\n");
			break;
		
		case 7: 
			printf("\nList All the Events.\n");
			break;
		case 8:
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
			
			// printf("User ID : %d\n",st.userId);
			// printf("Is Logged In : %s\n", st.status ? " Yes it Logged In": "No it is not logged In");
			// printf("Organiser Or Attendee : %s\n", st.isOrg ? "Organiser": "Attendee");
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