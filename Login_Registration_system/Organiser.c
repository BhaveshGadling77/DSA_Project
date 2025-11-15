#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Organiser {
    int organiserID;
    char name[64];
    unsigned int noOfEventsOrganised; /*No. of events organised.*/
    unsigned long long mobileNumber;
    char email[64];
} Organiser;
/* Validate the email*/
 int validateEmail(char *email) {
 	int n = strlen(email);
	int cntat = 0;
	for (int i = 0; i < n; i++) {
		if (email[i] == '@') {
			cntat++;
		}
		if (email[i] == ' ' || email[i] == '/' || email[i] == ':' || email[i] == ';'
				|| email[i] == '<' || email[i] == '['|| email[i] == '>' || email[i] == ']' || email[i] == ':') {
			return 0;
		}
 	}
	if (cntat == 1) {
		if (email[0] != '@') {
			char *dot = strchr(email, '.');
			if (dot != NULL && dot > strchr(email, '@')) {
				return 1;
			}
		}
	}
	return 0;
 }
void RegisterAsOrganiser() {
	Organiser *org = (Organiser *)malloc(sizeof(Organiser));
	org->organiserID = rand();
	char str[64];
	char num[32];
	printf("Enter Your Name : ");
	char ch;
	getchar();
	int i = 0;
	/* Taking the name input of person. */
	while ((scanf("%c", &ch) != -1) && ch != '\n') {
		str[i++] = ch;
	}
	str[i] = '\0';
	strcpy(org->name, str);
	org->noOfEventsOrganised = 0;
	printf("Enter Your Phone Number:- ");
	while (scanf("%s", num) == 1) {
		if (strlen(num) == 10) {
			org->mobileNumber = stoi(num);
		} else {
	 		printf("Mobile Number Should be of 10 digits.\n");
	 		printf("Please Enter Your Email Again:- ");
		}
	}
	printf("Enter Your Email :- ");
	while(scanf("%s", (org->email)) == 1) {
		if (validateEmail(org->email)) {
			break;
	 	} else {
	 		/*Continue the Loop */
	 		printf("Email Should be valid.\n");
	 		printf("Please Enter Your Email Again:- ");
	 	}
	}
	fflush(stdin);
	/* Printing details for the Organiser*/
	printf("Name :- %s\n", org->name);
	printf("Mobile Number :- %llu\n", org->mobileNumber);
	printf("Email :- %s\n", org->email);
	printf("Organiser ID :- %d\n", org->organiserID);
	printf("noOfEventsOrganised :- %d\n", org->noOfEventsOrganised);
	printf("Successfully Stored Your Data\n");
    struct stat st = {0};
    char folder[64] = "../Data/";
    strcat(folder, org->name);
	printf("folder = %s\n", folder);
    /* Making the Directory of the with organiser name.*/
    if (stat(folder, &st) == -1) {
        mkdir(folder, 0700);
    } else {
        printf("Organiser Already Exist.\n");
		return;
    }
	char cwd[1024]; /* Buffer to store the directory path */
	chdir(folder);
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
   	   printf("Current working directory: %s\n", cwd);
   	} else {
   	   perror("getcwd() error"); /* Prints error if getcwd() fails*/
   	}
}
