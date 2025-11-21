#include <stdio.h>
#include "Organiser.h"
int main() {
	int choice, check = 0;
	printf("What you want to do ?\n");
	while (check != -1) {
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
				registerAsUser(1);
				registerAsUser(1);
			} else {
				registerAsUser(2);
				printf("Registering As Attendee.\n");
			}
			userStatus st = getDetails();
			printf("Is Logged In : %s\n", st.status ? " Yes it Logged In": "No it is not logged In");
			
		} else if (choice == 2) {
			/* This is the Choice for the Login.*/
			printf("1. Login as Organiser.\n");
			printf("2. Login as Attendee \n");
			scanf("%d", &choice);
			if (choice == 1) {
				fflush(stdout);
				loginAsUser(1);
			} else {
				loginAsUser(2);
				printf("Login As Attendee.\n");
			}
			userStatus st = getDetails();
			printf("User ID : %d\n",st.userId);
			printf("Is Logged In : %s\n", st.status ? " Yes it Logged In": "No it is not logged In");
			printf("Organiser Or Attendee : %s\n", st.isOrg ? "Organiser": "Attendee");
		}
	}
	return 0;
}
