#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../attendance_system/attendee.h"
#include "Organiser.h"
#include <time.h>
typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
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
			n = atol(num);
			// printf("%ld\n", n);
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
	int newOrgId;
	FILE *fp = fopen("../Data/userOrganizer.csv", "r+");
	if (fp == NULL) {
		printf("This is error.\n");
		perror("Error in opening userOrganizer.csv!\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) {
		printf("this is empty file.\n");
		org->organiserID = 0;
	} else {
		fseek(fp, 0, SEEK_SET);
		char buffer[2048];
		while (fgets(buffer, sizeof(buffer), fp) != NULL) {
			;
		}
		newOrgId = atoi(strtok(buffer, ","));
	}
	// printf("Name :- %s\n", org->name);
	// printf("Mobile Number :- %llu\n", org->mobileNumber);
	// printf("Email :- %s\n", org->email);
	// printf("Organiser ID :- %d\n", org->organiserID);
	// printf("noOfEventsOrganised :- %d\n", org->noOfEventsOrganised);
	// printf("Successfully Stored Your Data\n");
	char str[64];
	printf("Enter Your Name : ");
	char ch;
	getchar();
	int i = 0;
	org->organiserID = ++newOrgId;
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
	fprintf(fp, "%d,%s,%d,%lld,%s\n", org->organiserID, org->name, org->noOfEventsOrganised, org->mobileNumber, org->email);
	free(org);
}

int userValidation(user *a) {
	user comp;
	FILE *fp = fopen("../Data/userAttendee.csv", "r");
	if (fp == NULL) {
		printf("Error in file opening.\n");
		exit(1);
	}
	while(fscanf(fp,"%d,%s,%lld,%u,%s\n", &comp.userId, &comp.name, &comp.mobileNumber, &comp.noOfEventsAttended, comp.email) == 4) {
		if (strcmp(comp.email, a->email) == 0 || strcmp(comp.name, a->name) == 0) {
			continue;
		} else {
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return -1;
}
void RegisterAsAttendee() {
	srand(time(0));
	user *att = (user *)malloc(sizeof(user));
	att->userId = 0;
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
	printf("%lld", att->mobileNumber);
	printf("Enter Your Email :- ");
	while(scanf("%s", (att->email)) == 1) {
		// printf("%s", att->email);
		if (validateEmail(att->email)) {
			if (userValidation(att->email) == 1) {
				printf("Failed to save the because user already Exist.\n");
				break;
			} else {
				printf("user already exist please Login.\n");
				break;
			}
		} else {
			/*Continue the Loop */
			printf("Email Should be valid.\n");
			printf("Please Enter Your Email Again:- ");
		}
	}
	int newAttId;
	FILE *fp = fopen("../Data/userAttendee.csv", "r+");
	if (fp == NULL) {
		printf("This is error.\n");
		perror("Error in opening userAttendee.csv!\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) {
		printf("this is empty file.\n");
		att->userId = 0;
	} else {
		fseek(fp, 0, SEEK_SET);
		char buffer[2048];
		while (fgets(buffer, sizeof(buffer), fp) != NULL) {
			;
		}
		newAttId = atoi(strtok(buffer, ","));
	}
	att->userId = ++newAttId;
	fprintf(fp, "%d,%s,%lld,%u,%s\n", att->userId, att->name, att->mobileNumber, att->noOfEventsAttended, att->email);
	fclose(fp);
	free(att);
}

void loginAsOrganiser() {
	FILE *fp = fopen("../Data/userOrganizer.csv", "r");
	Organiser org;
	printf("You have to Enter Your OrganiserID and your email.\n");
	printf("Email is case sensitive.\n");
	if (fp == NULL) {
		printf("Error while opening file\n");
		exit(1);
	}

	while (fp != NULL) {
		fscanf(fp, "%d,%s,%lld,%u,%s\n", &org.organiserID, org.name, org.noOfEventsOrganised, org.mobileNumber, org.email);
	}
	printf("last entry :- %d,%s,%lld,%u,%s", org.organiserID, org.name, org.noOfEventsOrganised, org.mobileNumber, org.email);
}