#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "login_registration.h"
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
/*Convert Uppercase to lower case*/
void convertToLowerCase(char *s) {
	int n = strlen(s);
	for (int i = 0; i < strlen(s); i++) {
        s[i] = tolower(s[i]);
    }
}

/*User id of the person.*/
static userStatus st;
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
/* Validate and insert the mobile number in Organiser struct.*/
long long giveValidMobileNumber() {
	char num[32];
	long long n;
	while (scanf("%s", num) == 1) {
		if (strlen(num) == 10) {
			n = atol(num);
			// printf("%ld\n", n);
			break;
		} else {
	 		printf("Mobile Number Should be of 10 digits.\n");
	 		printf("Please Enter Your Mobile Number Again:- ");
		}
	}
	return n;
}
int userValidation(user *a, int choice) {
	int userId;
	FILE *fp;
	unsigned int noOfEventattended;
	char name[256], email[256];
	long long mobileNum;
	if (choice == 1) {
		fp = fopen("../Data/userOrganizer.csv", "r");
	} else {
		fp = fopen("../Data/userAttendee.csv", "r");
	}
	if (fp == NULL) {
		printf("Error in file opening.\n");
		exit(1);
	}
	char buffer[2048];
	/* Checking if the user already exist or not.*/
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		/* tokenize safely and check each token before using */
		char *token = strtok(buffer, ",");
		if (token == NULL) continue;
		userId = atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		strncpy(name, token, sizeof(name) - 1);
		name[sizeof(name) - 1] = '\0';

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		mobileNum = atol(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		noOfEventattended = (unsigned int)atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		/* remove any trailing newline from email token */
		strncpy(email, token, sizeof(email) - 1);
		email[sizeof(email) - 1] = '\0';
		size_t ln = strlen(email);
		if (ln > 0 && email[ln - 1] == '\n') email[ln - 1] = '\0';

		// Convert the name and email to the lower case 
		convertToLowerCase(email);
		convertToLowerCase(name);
		if (strcmp(email, a->email) == 0 || strcmp(name, a->name) == 0 || mobileNum == a->mobileNumber) {
			printf("User Already Exist please Try to login.\n");
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return -1;
}
/*Register as Both organiser and attendee based on the choice.*/
int registerAsUser(int choice) {
    user *att = (user *)malloc(sizeof(user));
    att->userId = 0;
    FILE *fp;
    att->noOfEventsAttended = 0;
    char str[64];
    int newAttId;
    bool isUserExist = false;
    printf("Enter Your Name : ");
    char ch;
    getchar();
    int i = 0;
    /* Taking the name input of person. */
    while ((scanf("%c", &ch) != -1) && ch != '\n') {
        str[i++] = ch;
    }
    str[i] = '\0';
    strcpy(att->name, str);
    strcpy(st.name, att->name);
    st.status = true;
    printf("Enter Your Phone Number:- ");
    att->mobileNumber = giveValidMobileNumber();
    // printf("%lld", att->mobileNumber);
    printf("Enter Your Email :- ");
    while(scanf("%s", (att->email)) == 1) {
        // printf("%s", att->email);
        if (validateEmail(att->email)) {
            if (userValidation(att, choice) == 1) {
                printf("Failed to save the because user already Exist.\n");
                isUserExist = true;
                return 0;
                break;
            } else {
                break;
            }
        } else {
            /*Continue the Loop */
            printf("Email Should be valid.\n");
            printf("Please Enter Your Email Again:- ");
        }
    }
    if (isUserExist) {
        printf("Please Try to login.\n");
        return 0;
    }
    if (choice == 2) {
        fp = fopen("../Data/userAttendee.csv", "r+");
        st.isOrg = false;
    } else {
        st.isOrg = true;
        fp = fopen("../Data/userOrganizer.csv", "r+");
    }
    if (fp == NULL) {
        printf("This is error.\n");
        perror("Error in opening userAttendee.csv!\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        printf("this is empty file.\n");
        att->userId = 0;
    } else {
        fseek(fp, 0, SEEK_SET);
        char buffer[2048];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            ;
        }
        newAttId = atoi(strtok(buffer, ","));
    }
    att->userId = ++newAttId;
    st.userId = att->userId;
    if (choice == 1) {
        char filename[128];
        /* fixed: properly build filename */
        sprintf(filename,"../Data/Organizer_%d.csv", att->userId);
        FILE *fp1 = fopen(filename, "w");
        if (fp1) fclose(fp1);
    }
    printf("Your UserId is :- %d\n", att->userId);
    fprintf(fp, "%d,%s,%u,%lld,%s\n", att->userId, att->name, att->noOfEventsAttended, att->mobileNumber,att->email);
    fclose(fp);
    free(att);
    return 1;
}
int giveUserDetails(char *email, int id, FILE *fp, char *givenName) {
	char buffer[2048];
	char name[128];
	int userId;
	char emailF[128];
	long long mobileNum;
	unsigned int noOfEventattended;
	/* Checking if the user already exist or not.*/
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		/* tokenize safely and check each token before using */
		char *token = strtok(buffer, ",");
		if (token == NULL) continue;
		userId = atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		strncpy(name, token, sizeof(name) - 1);
		name[sizeof(name) - 1] = '\0';

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		mobileNum = atol(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		noOfEventattended = (unsigned int)atoi(token);

		token = strtok(NULL, ",");
		if (token == NULL) continue;
		/* remove any trailing newline from email token */
		strncpy(emailF, token, sizeof(emailF) - 1);
		emailF[sizeof(emailF) - 1] = '\0';
		size_t ln = strlen(emailF);
		if (ln > 0 && emailF[ln - 1] == '\n') emailF[ln - 1] = '\0';

		// Convert the name and email to the lower case 
		convertToLowerCase(emailF);
		convertToLowerCase(name);
		convertToLowerCase(givenName);
		if (strcmp(emailF, email) == 0 && strcmp(name, givenName) == 0 && id == userId) {
			st.status = true;
			st.userId = userId;
			fclose(fp);
			return 1;
		} else if (strcmp(emailF, email) == 0 && strcmp(name, givenName) == 0) {
			printf("Names and Email Details are correct but userId is wrong.\n");
		}
		memset(buffer, 0, 2048);
	}
	printf("Failed to Logged in User Dosen't exist.\n");
	fclose(fp);
	return -1;
}
int loginAsUser(int choice) {
	FILE *fp;
	int id;
	char email[200], str[200];
	if (choice == 1) {
		// this is the choice for the organiser.
		fp = fopen("../Data/userOrganizer.csv", "r");
		st.isOrg = true;

	} else {
		// this is the choice for the attendee.
		st.isOrg = false;
		fp = fopen("../Data/userAttendee.csv", "r");
	}
	printf("For login you need to have the your Registration id and email\n");
	printf("Enter Your Name : ");
	char ch;
	getchar();
	int i = 0;
	/* Taking the name input of person. */
	while ((scanf("%c", &ch) != -1) && ch != '\n') {
		str[i++] = ch;
	}
	str[i] = '\0';
	strcpy(st.name, str);
	// printf("%s", st.name);
	printf("Enter Your Registration ID : ");
	if(scanf("%d", &id) == -1) {
		return 0;
	}
	printf("Enter Your Email Address : ");
	while(scanf("%s", (email)) == 1) {
		// printf("%s", att->email);
		if (validateEmail(email)) {
			if(giveUserDetails(email, id, fp, str) == 1) {
				//User exist and validation is successful.
				st.isOrg = (choice == 1 ? true : false);
				break;
			} else {
				printf("Please Check the user Details Correctly.\n");
				break;
			}
		} else {
			/* Continue the Loop */
			printf("Email Should be valid.\n");
			printf("Please Enter Your Email Again:- ");
		}
	}
	return 1;
}
userStatus getDetails() {
	
	return st; 
}

void logout() {
	st.userId = 0;
	st.isOrg = false;
	st.status = false;
}