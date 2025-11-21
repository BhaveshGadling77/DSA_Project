#include <stdbool.h>
#include <stdio.h>
#ifndef LOGIN_REGISTRATION_H
#define LOGIN_REGISTRATION_H
typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[128];
} user;

typedef struct userStatus {
    int userId;
	char name[64];
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} userStatus;
int registerAsUser(int); /*register the organizer or Register the Attendee and save data in userAttendee.csv*/
int loginAsUser(int choice);
int giveUserDetails(char *email, int id, FILE *fp, char *givenName);
int userValidation(user *a, int choice);
long long giveValidMobileNumber();
userStatus getDetails();

#endif