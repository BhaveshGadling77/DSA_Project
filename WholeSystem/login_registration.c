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
            n = atoll(num);
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
    unsigned int noEvents;
    char name[256], email[256];
    long long mobileNum;

    if (choice == 1)
        fp = fopen("../Data/userOrganizer.csv", "r");
    else
        fp = fopen("../Data/userAttendee.csv", "r");

    if (fp == NULL) {
        printf("Error in file opening.\n");
        exit(1);
    }

    char buffer[2048];

    /* Checking if the user already exist or not.*/
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        char *token = strtok(buffer, ",");
        if (!token) continue;
        userId = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(name, token, sizeof(name));
        name[sizeof(name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        noEvents = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        mobileNum = atoll(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(email, token, sizeof(email));
        email[sizeof(email)-1] = '\0';
        size_t ln = strlen(email);
        if (ln > 0 && email[ln-1] == '\n') email[ln-1] = '\0';

        convertToLowerCase(name);
        convertToLowerCase(email);

        char aName[64], aEmail[128];
        strcpy(aName, a->name);
        strcpy(aEmail, a->email);
        convertToLowerCase(aName);
        convertToLowerCase(aEmail);

        if (strcmp(email, aEmail) == 0 || strcmp(name, aName) == 0 || mobileNum == a->mobileNumber) {
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
    user *att = malloc(sizeof(user));
    att->userId = 0;
    att->noOfEventsAttended = 0;
    FILE *fp;

    char str[64];
    int newAttId;
    bool isUserExist = false;

    printf("Enter Your Name : ");
    getchar();
    int i=0;
    char ch;
    while ((scanf("%c", &ch) != -1) && ch != '\n') {
        str[i++] = ch;
    }
    str[i] = '\0';
    strcpy(att->name, str);
    strcpy(st.name, att->name);
    st.status = true;

    printf("Enter Your Phone Number:- ");
    att->mobileNumber = giveValidMobileNumber();

    printf("Enter Your Email :- ");
    while (scanf("%s", att->email) == 1) {
        if (validateEmail(att->email)) {
            if (userValidation(att, choice) == 1) {
                printf("Failed to save because user already Exist.\n");
                isUserExist = true;
                return 0;
            }
            break;
        } else {
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
        fp = fopen("../Data/userOrganizer.csv", "r+");
        st.isOrg = true;
    }

    if (fp == NULL) {
        perror("Error in opening file!");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        newAttId = 0;
    } else {
        fseek(fp, 0, SEEK_SET);
        char buffer[2048];
        while (fgets(buffer, sizeof(buffer), fp)) {}
        newAttId = atoi(strtok(buffer, ","));
    }

    att->userId = ++newAttId;
    st.userId = att->userId;

    if (choice == 1) {
        char filename[128], num[16];
        sprintf(num,"%d.csv", att->userId);
        strcpy(filename, "../Data/Organizer_");
        strcat(filename, num);
        FILE *fp1 = fopen(filename, "w");
        fclose(fp1);
    }

    printf("Your UserId is :- %d\n", att->userId);
    fprintf(fp, "%d,%s,%u,%lld,%s\n", att->userId, att->name,
            att->noOfEventsAttended, att->mobileNumber, att->email);

    fclose(fp);
    free(att);
    return 1;
}

int giveUserDetails(char *email, int id, FILE *fp, char *givenName) {
    char buffer[2048];
    int userId;
    char name[128];
    char emailF[128];
    unsigned int noEvents;
    long long mobileNum;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        char *token = strtok(buffer, ",");
        if (!token) continue;
        userId = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(name, token, sizeof(name));
        name[sizeof(name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        noEvents = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        mobileNum = atoll(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(emailF, token, sizeof(emailF));
        emailF[sizeof(emailF)-1] = '\0';
        size_t ln = strlen(emailF);
        if (ln > 0 && emailF[ln-1] == '\n') emailF[ln-1] = '\0';

        convertToLowerCase(name);
        convertToLowerCase(emailF);

        char gname[128], gemail[128];
        strcpy(gname, givenName);
        strcpy(gemail, email);
        convertToLowerCase(gname);
        convertToLowerCase(gemail);

        if (strcmp(gemail, emailF) == 0 &&
            strcmp(gname, name) == 0 &&
            id == userId) {

            st.status = true;
            st.userId = userId;
            fclose(fp);
            return 1;
        }
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
        fp = fopen("../Data/userOrganizer.csv", "r");
        st.isOrg = true;
    } else {
        fp = fopen("../Data/userAttendee.csv", "r");
        st.isOrg = false;
    }

    printf("For login you need to have the your Registration id and email\n");
    printf("Enter Your Name : ");
    getchar();
    int i=0;
    char ch;
    while ((scanf("%c",&ch)!=-1) && ch!='\n') {
        str[i++] = ch;
    }
    str[i] = '\0';
    strcpy(st.name, str);

    printf("Enter Your Registration ID : ");
    if (scanf("%d", &id) == -1) return 0;

    printf("Enter Your Email Address : ");
    while (scanf("%s", email) == 1) {
        if (validateEmail(email)) {

            if (giveUserDetails(email, id, fp, str) == 1) {
                break;
            } else {
                printf("Please Check the user Details Correctly.\n");
                break;
            }

        } else {
            printf("Email Should be valid.\n");
            printf("Please Enter Your Email Again:- ");
        }
    }

    return 1;
}

userStatus getDetails() {
    return st;
}
