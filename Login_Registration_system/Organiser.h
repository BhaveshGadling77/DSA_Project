typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
    long long mobileNumber;
    char email[128];
} Organiser;

typedef struct user {
	int userId;
	char name[64];
	unsigned int noOfEventsAttended;
	long long mobileNumber;
	char email[128];
} user;

void RegisterAsOrganiser();

