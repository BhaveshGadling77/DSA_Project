#include <stdio.h>

typedef struct venue {
    char venueName[32];
    int venueID;
    long long int capacity;
    int rentalCost;                     // Cost of booking the venue per hour
    char venuHandler[32];               // Name of the person handling the venue
    unsigned long int handlerContact;   // Handler Mobile Contact information
    char venueHandlerEmail[48];         // Handler emailID
    int parkingCapacity;
    int indoor;                         // 1 → Indoor, 0 → Outdoor
} venue;