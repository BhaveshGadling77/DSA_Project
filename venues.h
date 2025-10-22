#include <stdio.h>
typedef struct Venue {
    int venueID;
    char venueName[64];
    long long capacity;
    int rentalCostPerHour;
    char venueHandlerName[64];
    unsigned long long handlerContact;
    char venueHandlerEmail[64];
    int parkingCapacity;
    int indoor;             // 1 = Indoor, 0 = Outdoor
    char location[128];     // City or full address
} Venue;
