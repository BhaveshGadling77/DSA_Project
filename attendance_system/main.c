#include <stdio.h>
#include <stdlib.h>
#include "attendee.h"
#include "../Login_Registration_system/login_registration.h"

int main(void)
{
    Node *head = NULL;
    int eventID = 1;                    
    userStatus currUser;

    system("mkdir -p data/attendees");

    // fetch curr logged in user
    currUser = getDetails();

    if (!currUser.status) {
        printf("Please login first\n");
        return 1;
    }

    // Load data for the default event
    loadFromFile(&head, eventID);

    printf("ATTENDANCE MANAGEMENT SYSTEM\n");
    printf("Logged in as: %-30s \n", currUser.isOrg ? "Organizer" : "Attendee");

    int choice;

    while (1)
    {
        if (currUser.isOrg)
        {
            // Organizer Menu
            printf("\n----- Organizer Menu -----\n");
            printf("1. View All Attendees\n");
            printf("2. Mark Attendance\n");
            printf("3. View Statistics\n");
            printf("4. Search Attendee\n");
            printf("5. Exit\n");

            printf("Enter choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
                case 1:
                    viewAllAttendees(head, eventID);
                    break;

                case 2:
                    markAttendance(head);
                    break;

                case 3:
                    viewStatistics(head);
                    break;

                case 4:
                    searchAttendee(head);
                    break;

                case 5:
                    saveToFile(head, eventID);
                    freeList(head);
                    return 0;

                default:
                    printf("\nInvalid choice!\n");
            }
        }
        else
        {
            // Attendee Menu
            printf("\n----- Attendee Menu -----\n");
            printf("1. Register for Event\n");
            printf("2. Unregister from Event\n");
            printf("3. Search Attendee\n");
            printf("4. Exit\n");

            printf("Enter choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
                case 1: {
                    int chosenEvent;
                    printf("Enter Event ID: ");
                    scanf("%d", &chosenEvent);
                    registerAttendeeForEvent(&head, chosenEvent, &currUser);
                    break;
                }

                case 2:
                    unregisterAttendee(&head, &currUser);
                    break;

                case 3:
                    searchAttendee(head);
                    break;

                case 4:
                    saveToFile(head, eventID);
                    freeList(head);
                    return 0;

                default:
                    printf("\nInvalid choice!\n");
            }
        }
    }

    return 0;
}