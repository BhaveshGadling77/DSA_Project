typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
    unsigned long long mobileNumber;
    char email[64];
    char organisationName[64];
} Organiser;

void RegisterAsOrganiser();

