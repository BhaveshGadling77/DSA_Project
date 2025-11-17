#include <stdio.h>
#include "Organiser.h"
int main() {
	int choice, check = 0;
	printf("What you want to do ?\n");
	while (check != 1 && (choice != 1 && choice != 2)) {
		printf("1. Register\n");
		printf("2. Login\n");
		check = scanf("%d", &choice);
		if (choice == 1) {
			/*This is the choice for the Register */
			printf("1. Register as Organiser.\n");
			printf("2. Register as Attendee \n");
			scanf("%d", &choice);
			if (choice == 1) {
				fflush(stdout);
				RegisterAsOrganiser();
			} else {
				RegisterAsAttendee();
				printf("Registering As Attendee.\n");
			}
		} else if (choice == 2) {
			/* This is the Choice for the Login.*/
			printf("THis is the choice for the Login");
		} 
	}
	return 0;
}
