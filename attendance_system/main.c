#include <stdio.h>
#include <stdlib.h>
#include "attendee.h"

int menu()
{
    printf("-------- MENU --------\n");
    printf("1. Register for Event \n");
    printf("2. Unregister from Event\n");
    printf("3. Mark Attendance\n");
    printf("4. View All Attendees\n");
    printf("5. Search Attendee\n");
    printf("6. View Statistics\n");
    printf("7. Exit program\n");

    int choice;
    scanf("%d", &choice);
}
int main(void)
{
    Node *head = NULL;
    int eventID = 1;    

    system("mkdir -p data/attendance");
    // Load existing data
    loadFromFile(&head, eventID);

    printf("ATTENDANCE MANAGEMENT SYSTEM\n");
    printf("Event ID: %-3d", eventID);

    while (1)
    {
        int choice = menu();
        switch(choice){
            case 1:
                registerAttendee(&head, eventID);
                saveToFile(head, eventID);
                break;
            case 2:
                printf("\nAttende ID to unregister: ");
                int id;
                scanf("%d", &id);
                unregisterAttendee(&head, id);
                saveToFile(head, eventID);
                break;
            case 3:
                markAttendance(head);
                saveToFile(head, eventID);
                break;
            case 4:
                viewAttendees(head);
                break;
            case 5:
                searchAttendee(head);
                break;
            case 6:
                viewStatistics(head);
                break;
            case 7:
                saveToFile(head, eventID);
                freeList(head);
                printf("\nSaved & exited successfully.\n\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}