<<<<<<< HEAD
#ifndef LOGIN_REGISTRATION_H
#define LOGIN_REGISTRATION_H

#include <stdbool.h>
#include <stdio.h>
=======
#include <stdbool.h>
>>>>>>> 9fe6f18 (Finalized the functions.)
typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[128];
} user;

<<<<<<< HEAD
typedef struct userStatus {
    int userId;
	char name[64];
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} userStatus;
=======
typedef struct status {
    int userId;
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} status;
>>>>>>> 9fe6f18 (Finalized the functions.)
void registerAsUser(int); /*register the organizer or Register the Attendee and save data in userAttendee.csv*/
void loginAsUser(int choice);
int giveUserDetails(char *email, int id, FILE *fp, char *givenName);
int userValidation(user *a, int choice);
long long giveValidMobileNumber();
<<<<<<< HEAD
userStatus getDetails();

#endif
=======
status getDetails();
>>>>>>> 9fe6f18 (Finalized the functions.)
