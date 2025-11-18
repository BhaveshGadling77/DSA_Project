#include <stdbool.h>
typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[128];
} user;

typedef struct status {
    int userId;
    bool status; // true = logged in, false = not logged in
    bool isOrg; //if org = true, else  false;
} status;
void registerAsUser(int); /*register the organizer or Register the Attendee and save data in userAttendee.csv*/
void loginAsUser(int choice);
int giveUserDetails(char *email, int id, FILE *fp, char *givenName);
int userValidation(user *a, int choice);
long long giveValidMobileNumber();
status getDetails();
