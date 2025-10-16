#include <stdio.h> 
#include <unistd.h>
int main() {
	while (1) {
		int choice;
		printf("Menu\n");
		/* Suppose a user somehow logged in*/
		printf("1. Add Event\n");
		printf("2. Modify Event\n");
		printf("3. Cancel Event\n");
		printf("4. View Events");
		printf("5. Sort Events on Date.\n");
		printf("6. Sort Events on basis of Chonological Order\n");
		printf("7. Exit\n\n");
		int n = scanf("%d", &choice);
		if (n < 1)
			break;

		int cnt = 3;
		switch(choice) {
			case 1:
				printf("Adding the Event Details...\n");
				break;
			case 2: 
				printf("Modifying Event...\n");
				break;

			case 3: 
				printf("Canceling Event...\n");
				break;

			case 4: 
				printf("Showing List Of Events...\n");
				break;

			case 5:
				printf("Sorting Events On Dates...\n");
				break;

			case 6:
				printf("Sorting Events on the Basis of Chronological Order...\n");
				break;

			case 7: 
				printf("Exiting the program");
				while (cnt--) {
					fflush(stdout);
					sleep(1);
					printf(".");
				}
				printf("\n");
				return 0;
				break;

			default:
				printf("Invalid Choice");
				break;
		}
	}
	return 0;
}
