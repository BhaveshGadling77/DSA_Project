#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../attendance_system/attendee.h"
typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
    long long mobileNumber;
    long long mobileNumber;
    char email[64];
} Organiser;

typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[64];
} user;


typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[64];
} user;

/* Validate the email*/
int validateEmail(char *email) {
 	int n = strlen(email);
	int cntat = 0;
	for (int i = 0; i < n; i++) {
		if (email[i] == '@') {
			cntat++;
		}
		if (email[i] == ' ' || email[i] == '/' || email[i] == ':' || email[i] == ';'
				|| email[i] == '<' || email[i] == '['|| email[i] == '>' || email[i] == ']' || email[i] == ':') {
			return 0;
		}
 	}
	if (cntat == 1) {
		if (email[0] != '@') {
			char *dot = strchr(email, '.');
			if (dot != NULL && dot > strchr(email, '@')) {
				return 1;
			}
		}
	}
	return 0;
}
/* Validate and insert the mobile number in Organiser struct.*/
long long giveValidMobileNumber() {
	char num[32];
	long long n;
	while (scanf("%s", num) == 1) {
		if (strlen(num) == 10) {
			n = atoi(num);
			break;
		} else {
	 		printf("Mobile Number Should be of 10 digits.\n");
	 		printf("Please Enter Your Mobile Number Again:- ");
		}
	}
	return n;
}
/*Register as organiser.*/
void RegisterAsOrganiser() {
	Organiser *org = (Organiser *)malloc(sizeof(Organiser));
	srand(time(0));
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
	org->mobileNumber = giveValidMobileNumber();
	printf("Enter Your Email :- ");
	while(scanf("%s", (org->email)) == 1) {
		if (validateEmail(org->email)) {
			break;
		} else {
			/*Continue the Loop */
			printf("Email Should be valid.\n");
			printf("Please Enter Your Email Again:- ");
		}
	}
	fflush(stdin);
	/* Printing details for the Organiser*/
	printf("Name :- %s\n", org->name);
	printf("Mobile Number :- %llu\n", org->mobileNumber);
	printf("Email :- %s\n", org->email);
	printf("Organiser ID :- %d\n", org->organiserID);
	printf("noOfEventsOrganised :- %d\n", org->noOfEventsOrganised);
	printf("Successfully Stored Your Data\n");
	struct stat st = {0};
	char folder[64] = "../Data/";
	struct stat st = {0};
	char folder[64] = "../Data/";
    strcat(folder, org->name);
	printf("folder = %s\n", folder);
    /* Making the Directory of the with organiser name.*/
    if (stat(folder, &st) == -1) {
    	mkdir(folder, 0700);
    	mkdir(folder, 0700);
    } else {
        printf("Organiser Already Exist.\n");
		return;
    }
	char cwd[1024]; /* Buffer to store the directory path */
	chdir(folder);
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
   	   printf("Current working directory: %s\n", cwd);
   	} else {
   	   perror("getcwd() error"); /* Prints error if getcwd() fails*/
   	}
}

void RegisterAsAttendee() {
	user *att = (user *)malloc(sizeof(user));
	att->userId = rand();
	att->noOfEventsAttended = 0;
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
	strcpy(att->name, str);
	printf("Enter Your Phone Number:- ");
	att->mobileNumber = giveValidMobileNumber();
	printf("Enter Your Email :- ");
	while(scanf("%s", (att->email)) == 1) {
		// printf("%s", att->email);
		if (validateEmail(att->email)) {
			break;
		} else {
			/*Continue the Loop */
			printf("Email Should be valid.\n");
			printf("Please Enter Your Email Again:- ");
		}
	}
	chdir("../Data");
	FILE *fp = fopen("./userAttendee.csv", "a");
	if (fp == -1) {
		printf("Error in Opening User File");
		exit(1);
	}
	fprintf(fp, "%d,%s,%ld,%u,%s\n", att->userId, att->name, att->mobileNumber, att->noOfEventsAttended, att->email);
	fclose(fp);
}