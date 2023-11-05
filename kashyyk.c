// KASHYYYK HOTEL
// just planning a bit not doing anything major, more details on the push commit description thing
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



// declaring subroutines for later
int checkIn(int currentGuestNo);
int bookDinner();
int checkOut();
int isNameValid(char name[]);

//global variables
int MAIN[12][2] = {{0, 0}, {0,0}, {0,0}, {0,0},
                   {0,0},{0,0},{0,0},{0,0},
                   {0,0},{0,0},{0,0}, {0, 0}};  //empty 2Darray of 11x2
                   // [0] = BookingID, [1] = FirstName, [2] = Surname, [3] = AdultsStaying, [4] = ChildrenStaying,
                   // [5] = BoardType, [6] = LengthOfStay, [7] = DailyNewspaper, [8] = RoomBooking, [9] = TableBooking
                   // [10] = hasBookedTable, [11] = age
int ROOMS[6] = {0, 0, 0, 0, 0, 0}; //normal array for room availability
                    // [0] = Room1, [1] = Room2, etc....
                    // values stored in table: 0 = available, 1 = booked
                    // all rooms start available
int TABLES[6] = {0, 0, 0, 0, 0, 0}; //another normal array for table availability
                   // [0] = Endor19:00, [1] = Endor21:00, [2] = Naboo19:00, [3] = Naboo21:00
                   // [4] = Tattooine19:00, [5] = Tattooine21:00
                   // 0 = available, 1 = booked         all tables start available


// main program called when code is ran
int main() {
    int currentGuestNo = -1; //keeps track of what column we read and write from in MAIN array

    checkIn(currentGuestNo);



}






int checkIn(int currentGuestNo) {
    //        CHECK IN
    //variables
    char fName[150];
    char lName[150];
    char BookingID[154]; //max surname = 150 and max num = 100, overall 154 max characters including null terminator
    int lengthOfStay;
    int adultsStaying;
    int childrenStaying;
    int maxStaying;
    int userAge;
    char boardTypeChoice[3]; //0 for BB, 1 for HB, 2 for FB
    currentGuestNo++; //first check in guest column = 0
    char confirm;
    char newspaperChoice;



    // INPUTS (+ validation)
    //first name
    do {
        printf("\n\nPlease enter your first name: "); //input
        scanf("%s", fName);
        fflush(stdin);
    } while (isNameValid(fName) != 0);
    int ASCIIfirstName = atoi(fName); //convert to ascii
    MAIN[1][currentGuestNo] = ASCIIfirstName; // firstname stored

    //last name
    do {
        printf("\n\nPlease enter your last name: ");
        scanf("%s", lName);
        fflush(stdin);
    } while (isNameValid(lName) != 0);
    int ASCIIlastName = atoi(lName); //convert to ascii
    MAIN[2][currentGuestNo] = ASCIIlastName; //store

    //age
    do {
        printf("\n\nEnter your age: ");
        scanf("%d", &userAge);
        fflush(stdin);
        if (userAge < 16) {
            printf("You are too young to book, try again.");
        } else if (userAge > 123) {
            printf("Invalid age choice, try again.");
        } else {
            //confirmation
            printf("\nYou are %d years old, type '/' to proceed: ", userAge);
            scanf("%c", &confirm);
            fflush(stdin);
            if (confirm == '/') {
                printf("Thank you for your booking");
            } else {
                printf("Input cancelled, try again");
            }
        }
    } while (userAge < 16 || userAge > 123 || confirm != '/');
    MAIN[11][currentGuestNo] = userAge;

    // adults staying
    printf("\n\n\t NOTE: the Kashyyyk Hotel can only support 4 guests in one room");
    do {
        do {
            printf("\n\nHow many guests are adults (over 16): ");
            scanf("%d", &adultsStaying); //input adults staying
            fflush(stdin);
            if (adultsStaying > 4) {//too many guests
                printf("You have entered too many guests, try again.");
            } else if (adultsStaying < 0) {//too little guests
                printf("Your choice is invalid, try again.");
            } else if (adultsStaying == 0) {//no adults
                printf("There must be at least 1 adult in order to book a room, try again");
            } else {
                //confirmation
                if (adultsStaying == 1) {
                    printf("You are booking for 1 adult, type '/' to proceed: ");
                } else {
                    printf("You are booking for %d adults, type '/' to proceed: ", adultsStaying); }
                scanf("%c", &confirm);
                fflush(stdin);
                if (confirm == '/') {
                    printf("Thank you for your booking.");
                } else {
                    printf("Input cancelled, try again");
                }
            }
        } while (adultsStaying > 4 || adultsStaying <= 0 || confirm != '/');

        //children staying
        do {
            printf("\n\nHow many guests are children (under 16): ");
            scanf("%d", &childrenStaying); //input adults staying
            fflush(stdin);
            if (childrenStaying > 4) {//too many guests
                printf("You have entered too many guests, try again.");
            } else if (childrenStaying < 0) {//too little guests
                printf("Your choice in invalid, try again.");
            } else {
                //confirmation
                if (childrenStaying == 1) {
                    printf("You are booking for 1 child, type '/' to proceed: ");
                } else {
                    printf("You are booking for %d children, type '/' to proceed: ", childrenStaying); }
                scanf("%c", &confirm);
                fflush(stdin);
                if (confirm == '/') {
                    printf("Thank you for your booking");
                } else {
                    printf("Input cancelled, try again");
                }
            }
        } while (childrenStaying > 4 || childrenStaying < 0 || confirm != '/');
        maxStaying = adultsStaying + childrenStaying;
        if (maxStaying > 4) {
            printf("\n\nYou have booked for too many guests, try again.");
        }


    } while (maxStaying > 4 || maxStaying <= 0 || confirm != '/');
    MAIN[4][currentGuestNo] = childrenStaying; //store
    MAIN[3][currentGuestNo] = adultsStaying;

    //board type
    do {
        printf("\n\nWhat board type would you like? \nType BB for Bed & Breakfast, HB for Half Board and FB for Full Board: ");
        scanf("%s", boardTypeChoice);
        fflush(stdin);
        boardTypeChoice[0] = toupper(boardTypeChoice[0]); boardTypeChoice[1] = toupper(boardTypeChoice[1]);
        if (strcmp(boardTypeChoice, "BB") != 0 && strcmp(boardTypeChoice, "HB") != 0 &&
            strcmp(boardTypeChoice, "FB") != 0) {
            printf("\nInvalid board type, try again.");
        } else if (strlen(boardTypeChoice) > 2 || strlen(boardTypeChoice) <= 1) {
            printf("\nInvalid board type, try again");
        } else {
            // confirmation
            printf("You are booking for Board Option: %s, type '/' to proceed: ", boardTypeChoice);
            scanf("%c", &confirm);
            fflush(stdin);
            if (confirm == '/') {
                printf("Thank you for your booking");
            } else {
                printf("Input cancelled, try again");
            } //Boardtypechoice will equal BB, HB, or FB
        }
    } while (strlen(boardTypeChoice) > 2 || strlen(boardTypeChoice) <= 1 || confirm != '/' ||
            (strcmp(boardTypeChoice, "BB") != 0 && strcmp(boardTypeChoice, "HB") != 0 && strcmp(boardTypeChoice, "FB") != 0));
    MAIN[5][currentGuestNo] = (strcmp(boardTypeChoice, "BB") == 0) ? 3 : (strcmp(boardTypeChoice, "HB") == 0) ? 2 :
            (strcmp(boardTypeChoice, "FB") == 0) ? 1 : 0;
    // this is an easier way of writing a massive if-else statement
    // if BB, array stores 3, if HB, array stores 2, if FB, array stores 1

    //lengthOfStay
    do {
        printf("\n\nPlease enter the length of your stay in days: ");
        scanf("%d", &lengthOfStay); //input length of stay, no need to check if it is digits, as scanf already handles it
        fflush(stdin);
        if (lengthOfStay <= 0) {
            printf("Your choice is invalid, Try again.");
        } else if (lengthOfStay > 14) {
            printf("The maximum time a room can be booked is 14 days. Try again.");
        } else {
            if (lengthOfStay == 1) {
                printf("You are booking for 1 day, type '/' to proceed: ");
            } else {
                printf("You are booking for %d days, type '/' to proceed: ", lengthOfStay); }
            scanf("%c", &confirm);
            fflush(stdin);
            if (confirm == '/')
                printf("Thank you for your booking, we hope your enjoy your stay.");
            else
                printf("Input cancelled, try again");
        }
    } while (lengthOfStay > 14 || lengthOfStay <= 0 ||
             confirm != '/'); //while lengthofstay is in range and user has confirmed
    MAIN[6][currentGuestNo] = lengthOfStay;

    //daily newspaper

    do {
        //also we do not need a second while loop as the do-while can handle looping
        // char newspaperChoice; //instead of a string we can use characters (easier to deal with)
        printf("\n\nWould you like to opt for our daily newspaper, this service has a one-off charge of £5.50.");
        printf("Enter 'Y' for yes and 'N' for no: ");
        scanf("%c", &newspaperChoice);
        fflush(stdin);
        newspaperChoice = toupper(newspaperChoice);
        if (newspaperChoice == 'Y') {
            printf("Your newspapers will be delivered daily, thank you for choosing our service");
        } else if (newspaperChoice == 'N') {
            printf("You will not be charged with newspapers, enjoy your stay");
        } else {
            printf("Invalid choice, try again");
        }
    } while (newspaperChoice != 'Y' && newspaperChoice != 'N'); //while newspaper choice isn't valid
    MAIN[7][currentGuestNo] = newspaperChoice == 'Y' ? 1 : 0; //1 = yes, 0 = no

    for (int i = 0; i < 11; i++) {
        printf("%s", MAIN[i][currentGuestNo]);
    }

    //BOOKING ID
    srand(time(NULL));
    int BookingIDRandom = rand() % 100;
    sprintf(BookingID, "%s%d", lName, BookingIDRandom);
    printf("\nYour Booking ID is: %s", BookingID);

    //CALCULATION (boardRate and roomRate)

    return 0;
}


int bookDinner() {
    //book dinner
}
int checkOut() {
    //check out
}

int calculateAge(char *dateOfBirth) { //for example  calculateAge(3/12/2003)

}

int isNameValid(char name[]) {  //returns 0 if string is letters, returns -1 if string has a number in it
    if (strlen(name) <= 150 && strlen(name) > 0) { //if string < 150 and bigger than 0
        int validCheck = 0;
        for (int i = 0; i < strlen(name); i++) {
            if (isalpha(name[i]) == 0)
                validCheck++; //valid check isn't 0 if there is a number
        }
        if (validCheck == 0)
            return 0; //Name is Valid
        else
            return -1; //name not valid
    } else {
        return -1;
    }
}
