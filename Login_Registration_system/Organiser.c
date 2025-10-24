#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
    unsigned long long mobileNumber;
    char email[64];
} Organiser;

void RegisterAsOrganiser() {
	Organiser *org = (Organiser *)malloc(sizeof(Organiser));
	org->organiserID = rand();
	char str[64];
	printf("Enter Your Name : ");
	char ch;
	getchar();
	int i = 0;
	/* Taking the name input of person. */
	while ((scanf("%c", &ch) != -1) && ch != '\n') {
		str[i++] = ch;
	}
	str[i] = '\0';
	strcpy(org->name, str);
	org->noOfEventsOrganised = 0;
	printf("Enter Your Phone Number:- ");
	scanf("%llu", &(org->mobileNumber));
	printf("Enter Your Email :- ");
	scanf("%s", (org->email));

	/* Printing details for the Organiser*/
	printf("Name :- %s\n", org->name);
	printf("Mobile Number :- %llu\n", org->mobileNumber);
	printf("Email :- %s\n", org->email);
	printf("Organiser ID :- %d\n", org->organiserID);
	printf("noOfEventsOrganised :- %d\n", org->noOfEventsOrganised);
	printf("Successfully Stored Your Data\n");
    struct stat st = {0};
    char folder[64] = "../Data/";
    strcat(folder, org->name);
    /*Making the Directory of the with organiser name.*/
    if (stat(folder, &st) == -1) {
        mkdir(folder, 0700);
    } else {
        printf("Organiser Already Exist.\n");
    }
}
