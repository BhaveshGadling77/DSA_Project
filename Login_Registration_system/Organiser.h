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
void registeAsUser(int); /*register the o; /*Register the Attendee and save data in userAttendee.csv*/
void loginAsOrganiser(); 
void loginAsAttendee();

