<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> f2a6607 (Corrected format of header file)
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
<<<<<<< HEAD
typedef struct userStatus {
    int userId;
	char name[64];
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} userStatus;
=======
typedef struct status {
=======
typedef struct userStatus {
>>>>>>> a75df0e (made the successful changes in login_registration header file.)
    int userId;
	char name[64];
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
<<<<<<< HEAD
} status;
>>>>>>> 9fe6f18 (Finalized the functions.)
=======
} userStatus;
>>>>>>> a75df0e (made the successful changes in login_registration header file.)
void registerAsUser(int); /*register the organizer or Register the Attendee and save data in userAttendee.csv*/
void loginAsUser(int choice);
int giveUserDetails(char *email, int id, FILE *fp, char *givenName);
int userValidation(user *a, int choice);
long long giveValidMobileNumber();
<<<<<<< HEAD
<<<<<<< HEAD
userStatus getDetails();

#endif
<<<<<<< HEAD
=======
status getDetails();
>>>>>>> 9fe6f18 (Finalized the functions.)
=======
userStatus getDetails();
>>>>>>> a75df0e (made the successful changes in login_registration header file.)
=======
>>>>>>> f2a6607 (Corrected format of header file)
