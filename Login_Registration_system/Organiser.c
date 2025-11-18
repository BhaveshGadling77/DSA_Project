#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../attendance_system/attendee.h"
#include "Organiser.h"
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
/*Convert Uppercase to lower case*/
void convertToLowerCase(char *s) {
	int n = strlen(s);
	for (int i = 0; i < strlen(s); i++) {
        s[i] = tolower(s[i]);
    }
}
/*User id of the person.*/
static status;
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
int userValidation(user *a, int choice) {
	int userId;
	FILE *fp;
	unsigned int noOfEventattended;
	char name[256], email[256];
	long long mobileNum;
	if (choice == 1) {
		fp = fopen("../Data/userOrganizer.csv", "r");
	} else {
		fp = fopen("../Data/userAttendee.csv", "r");
	}
	if (fp == NULL) {
		printf("Error in file opening.\n");
		exit(1);
	}
	char buffer[2048];
	/* Checking if the user already exist or not.*/
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		/* tokenize safely and check each token before using */
		char *token = strtok(buffer, ",");
		if (token == NULL) continue;
		userId = atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		strncpy(name, token, sizeof(name) - 1);
		name[sizeof(name) - 1] = '\0';

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		mobileNum = atol(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		noOfEventattended = (unsigned int)atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		/* remove any trailing newline from email token */
		strncpy(email, token, sizeof(email) - 1);
		email[sizeof(email) - 1] = '\0';
		size_t ln = strlen(email);
		if (ln > 0 && email[ln - 1] == '\n') email[ln - 1] = '\0';

		// Convert the name and email to the lower case 
		convertToLowerCase(email);
		convertToLowerCase(name);
		if (strcmp(email, a->email) == 0 || strcmp(name, a->name) == 0 || mobileNum == a->mobileNumber) {
			printf("User Already Exist please Try to login.\n");
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return -1;
}
/*Register as Both organiser and attendee based on the choice.*/
void registerAsUser(int choice) {
	user *att = (user *)malloc(sizeof(user));
	att->userId = 0;
	FILE *fp;
	att->noOfEventsAttended = 0;
	char str[64];
	int newAttId;
	bool isUserExist = false;
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
			if (userValidation(att, choice) == 1) {
				printf("Failed to save the because user already Exist.\n");
				isUserExist = true;
				break;
			} else {
				break;
			}
		} else {
			/*Continue the Loop */
			printf("Email Should be valid.\n");
			printf("Please Enter Your Email Again:- ");
		}
	}
	if (isUserExist) {
		printf("Please Try to login.\n");
		return ;
	}
	if (choice == 2) {
		fp = fopen("../Data/userAttendee.csv", "r+");
	} else {
		fp = fopen("../Data/userOrganizer.csv", "r+");
	}
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
	printf("Your UserId is :- %d", att->userId);
	fprintf(fp, "%d,%s,%lld,%u,%s\n", att->userId, att->name, att->mobileNumber, att->noOfEventsAttended, att->email);
	fclose(fp);
	free(att);
}

void loginAsUser(int choice) {
	if (choice == 1) {
		// this is the choice for the organiser.
		
	} else {
		// this is the choice for the attendee.
	}
}
unsigned int getId() {

}