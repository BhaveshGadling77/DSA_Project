#include <stdio.h>
#include "events.h"
#include "venues.h"

int menu() {
    int choice;
    printf("Enter the operation to be performed:\n");
    printf("1.  Add Event\n");
    printf("2.  Delete Event\n");
    printf("3.  Modify Event\n");
    printf("4.  Search Event by ID\n");
    printf("5.  View All Events\n");
    printf("6.  Sort Events by Date\n");
    printf("7.  Sort Events Chronologically\n");
    printf("8.  Sort Events by ID\n");
    printf("9.  Clean Past Events\n");
    printf("10. Test Venue Availability (Manual)\n");
    printf("11. View All Venues\n");
    printf("12.  Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int main() {
    // Initialize data
    loadVenues();
    loadEvents();

    int choice;
    while (choice != 12) {
        choice = menu();
        switch (choice) {
            case 1: {
                printf("\n--- Add Event ---\n");
                addEvent();
                break;
            }
            case 2: {
                printf("\n--- Delete Event ---\n");
                deleteEvent();
                break;
            }
            case 3: {
                printf("\n--- Modify Event ---\n");
                modifyEvent();
                break;
            }
            case 4: {
                printf("\n--- Search Event by ID ---\n");
                event* e = searchEventID();
                if (e) {
                    printf("Found: %d | %s | Org: %d | Venue: %d | %02hd-%02hd-%04hd | %02hu:%02hu:%02hu\n",
                           e->eventID, e->eventName, e->organiserID, e->venueID,
                           e->eventDate.date, e->eventDate.month, e->eventDate.year,
                           e->startTime.hour, e->startTime.minute, e->startTime.second);
                }
                break;
            }
            case 5: {
                printf("\n--- View All Events ---\n");
                viewEvents();
                break;
            }
            case 6: {
                printf("\n--- Sort Events by Date ---\n");
                sortEventByTime();
                break;
            }
            case 7: {
                printf("\n--- Sort Events Chronologically ---\n");
                sortEventChronological();
                break;
            }
            case 8: {
                printf("\n--- Sort Events by ID ---\n");
                sortEventByID();
                break;
            }
            case 9: {
                printf("\n--- Cleaning Past Events ---\n");
                cleanPastEvents();
                printf("Past events removed and CSV updated.\n");
                break;
            }
            case 10: {
                printf("\n--- Test Venue Availability ---\n");
                int venueID, dd, mm, yyyy;
                char start[9], end[9];
                struct date d;
                struct Time s, e;

                printf("Enter Venue ID: ");
                scanf("%d", &venueID);
                if (!getVenueByID(venueID)) {
                    printf("Invalid Venue ID.\n");
                    break;
                }
                printf("Enter Date (DD-MM-YYYY): ");
                scanf("%d-%d-%d", &dd, &mm, &yyyy);
                d.date = dd; d.month = mm; d.year = yyyy;
                printf("Enter Start Time (HH:MM:SS): ");
                scanf("%8s", start);
                printf("Enter End Time (HH:MM:SS): ");
                scanf("%8s", end);
                sscanf(start, "%hu:%hu:%hu", &s.hour, &s.minute, &s.second);
                sscanf(end,   "%hu:%hu:%hu", &e.hour, &e.minute, &e.second);

                if (checkVenueAvailability(venueID, d, s, e)) {
                    printf("Venue is AVAILABLE on %02d-%02d-%04d from %s to %s\n", dd, mm, yyyy, start, end);
                } else {
                    printf("Venue is NOT AVAILABLE (conflict detected)\n");
                }
                break;
            }
            case 11: {
                viewVenues();
                break;
            }
            case 12: {
                printf("\nExiting program. Goodbye!\n");
                return 0;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    }
    return 0;
}
