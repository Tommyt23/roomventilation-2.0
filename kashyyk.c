// KASHYYYK HOTEL
// just planning a bit not doing anything major, more details on the push commit description thing
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>



// declaring subroutines for later
int mainMenu(int currentGuestNo);
int checkIn(int currentGuestNo);
int bookDinner(int currentGuestNo);
int checkOut(int currentGuestNo);
int isNameValid(char name[]);
int calculate_age(struct tm dob);

//declaring variables
bool mainMenuValid;

//global variables
int MAIN[12][6] = {{0,0},{0,0},{0,0},{0,0},
                   {0,0},{0,0},{0,0},{0,0},
                   {0,0},{0,0},{0,0},{0,0}};  //empty 2Darray of 11x2
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


// New thread that will enable the program to play music while the main thread is used in order to run the main program
DWORD WINAPI PlaySoundThread(LPVOID lpParam) {
    // Use SND_FILENAME to indicate that the first parameter is a file name
    // Use SND_ASYNC to play the sound asynchronously
    PlaySound(TEXT(/* --- PUT MUSIC FILE LOCATION BETWEEN THE STRING MARKERS --- */""), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    return 0;
}

bool hasQuit = false;
// main program called when code is ran
int main() {
    // calls the soundthread at the beginning of the program in order to let it run at the same time as the rest of the program
    HANDLE soundThread = CreateThread(NULL, 0, PlaySoundThread, NULL, 0, NULL);

    int currentGuestNo = -1; //keeps track of what column we read and write from in MAIN array
    hasQuit = false;
    mainMenuValid = true;

    mainMenu(currentGuestNo);

    // Optionally, wait for the sound thread to finish
    WaitForSingleObject(soundThread, INFINITE);

    // Close the handle to the thread
    CloseHandle(soundThread);
}


int mainMenuChoice;
bool mainMenuValid = true;

int mainMenu(int currentGuestNo) {
    printf("\n\n\n  WELCOME TO THE KASHYYYK HOTEL\n");
    // colourful ribbon because why not \/
    for (int i = 0; i < 33; i++) {
        if (i % 3 == 0) {
            printf("\033[0;31m"); printf("~"); //red text
        } else if (i % 3 == 1) {
            printf("\033[38;5;208m"); printf("~"); // orange text
        } else {
            printf("\033[93m"); printf("~"); //yellow text
        }
    }
    printf("\033[0m\n"); //reset colour

    //main menu
    //hasquit is true if the user has returned to the main menu by quitting
    printf("\n1) Check-In\t\t2) Book Table\t\t3) Check-Out\n");
    do {
        mainMenuValid = true; //set to false if their choice didn't make sense
        printf("\nEnter choice: ");
        scanf("%d", &mainMenuChoice);
        fflush(stdin);

        if (mainMenuChoice == 1) {
            if (hasQuit) {
                checkIn(currentGuestNo); // if player has quit, check-in with new guest number
            } else {
                if (currentGuestNo == -1) {
                    checkIn(currentGuestNo);   //allow check-in if program first started
                } else {
                    printf("You have already checked-in, pick another option.");
                    mainMenuValid = false;
                }
            }
        }
        else if (mainMenuChoice == 2) {
            if (!hasQuit) {
                if (currentGuestNo == -1) { //if program has just started
                    printf("You haven't checked in yet, please Check-In and enter option 1.");
                    mainMenuValid = false;
                } else {
                    bookDinner(currentGuestNo); //if player hasn't quit (has checked in) go ahead
                }
            } else {
                printf("You haven't checked in yet, please Check-In and enter option 1.");
                mainMenuValid = false;
            }
        }
        else if (mainMenuChoice == 3) {
            if (!hasQuit) {
                if (currentGuestNo == -1) { //if program has just started
                    printf("You haven't checked in yet, please Check-In and enter option 1.");
                    mainMenuValid = false;
                } else {
                    checkOut(currentGuestNo); //if player hasn't quit (has checked in) you may check-out
                }
            } else {
                printf("You haven't checked in yet, please Check-In and enter option 1.");
                mainMenuValid = false;
            }
        }
        else {
            printf("Option typed is invalid, try again.");
            mainMenuValid = false;
        }
    } while (mainMenuChoice < 1 || mainMenuChoice > 3 || !mainMenuValid);

    //check in

    return 0;
}

//variables for check-in
char fName[150];
char lName[150];
char BookingID[154]; //max surname = 150 and max num = 100, overall 154 max characters including null terminator
int lengthOfStay;
int adultsStaying;
int childrenStaying;
int maxStaying;
int userAge;
char boardTypeChoice[3]; //0 for BB, 1 for HB, 2 for FB
int roomChoice;
char newspaperChoice;

int checkIn(int currentGuestNo) {
    //        CHECK IN
    //variables
    currentGuestNo++; //first check in guest column = 0
    char confirm;

    // INPUTS (+ validation)
    //first name
    printf("\nNOTE: Remove any hyphens in your name (e.g Edgar Allan-Poe --> Edgar AllanPoe)");
    do {
        do {
            printf("\n\nPlease enter your first name: "); //input
            scanf("%s", fName);
            fflush(stdin);
            if (isNameValid(fName) != 0) { //error message
                printf("Name typed isn't valid.");
            }
        } while (isNameValid(fName) != 0);

        //last name
        do {
            printf("\n\nPlease enter your last name: ");
            scanf("%s", lName);
            fflush(stdin);
            if (isNameValid(lName) != 0) { //error message
                printf("Name typed isn't valid.");
            }
        } while (isNameValid(lName) != 0);

        //confirmation
        printf("\nYour name is: %s %s, type '/' to proceed: ", fName, lName);
        scanf("%c", &confirm);
        fflush(stdin);
        if (confirm == '/') {
            printf("Thank you for your booking");
        } else {
            printf("Input cancelled, try again");
        }

    } while (confirm != '/'); //redo if name is mistyped

    //age
    do {
        struct tm dob1 = {0};
        do {
            printf("\nEnter the year you were born:");
            scanf("%d", &dob1.tm_year);
            fflush(stdin);
            if (dob1.tm_year <= 0 || dob1.tm_year >= 2023) {
                printf("Invalid year, try again");
            }
        } while (dob1.tm_year <= 0 || dob1.tm_year >= 2023);
        do {
            printf("\nEnter the month you were born:");
            scanf("%d", &dob1.tm_mon);
            fflush(stdin);
            if (dob1.tm_mon > 12 || dob1.tm_mon <= 0) {
                printf("Invalid month, try again");
            }
        } while(dob1.tm_mon > 12 || dob1.tm_mon <= 0);
        do {
            printf("\nEnter the day you were born:");
            scanf("%d", &dob1.tm_mday);
            fflush(stdin);
            if (dob1.tm_mday <= 0 || dob1.tm_mday > 31) {
                printf("Invalid date, try again");
            }
        } while (dob1.tm_mday <= 0 || dob1.tm_mday > 31);
        userAge = calculate_age(dob1);
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
    printf("\n\nNOTE: the Kashyyyk Hotel can only support 4 guests in one room");
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
                } else if (childrenStaying == 0) {
                    printf("You are booking for no children, type '/' to proceed: ");
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
            printf("Invalid board type, try again.");
        } else if (strlen(boardTypeChoice) > 2 || strlen(boardTypeChoice) <= 1) {
            printf("Invalid board type, try again");
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
                printf("You are booking for %d days, type '/' to proceed: ", lengthOfStay);
            }
            scanf("%c", &confirm);
            fflush(stdin);
            if (confirm == '/') {
                printf("Thank you for your booking, we hope your enjoy your stay.");
            } else {
                printf("Input cancelled, try again");
            }
        }
    } while (lengthOfStay > 14 || lengthOfStay <= 0 ||
             confirm != '/'); //while lengthofstay is in range and user has confirmed
    MAIN[6][currentGuestNo] = lengthOfStay;

    //ROOM BOOKING

    //we don't need roomConfirm, instead use the global confirm variable
    //available rooms weren't printing, there was an error
    // moved RoomChoice to top
    printf("\n\nRATES PER DAY- \nRooms 1 & 2- £100\tRoom 3- £85\tRooms 4 & 5- £75\tRoom 6- £50\n"); //print prices
    for (int i = 1; i < 7; i++) {
        if (ROOMS[i] == 0) {      //printing what rooms are available
            printf("Room %d is available. \t", i);
        }
    }
    do {
        printf("\nEnter room choice: ");
        scanf("%d", &roomChoice);
        fflush(stdin);

        if (roomChoice < 1 || roomChoice > 6) {
            printf("Room Choice is invalid, try again.");
        } else if (ROOMS[roomChoice] != 0) {
            printf("Room is not available, try again.");
        } else {
            //confirmation
            printf("You are booking Room %d, type '/' to proceed: ", roomChoice);
            scanf("%c", &confirm);
            fflush(stdin);
            if (confirm == '/') {
                printf("Thank you for your booking, we hope your enjoy your stay.");
            } else {
                printf("Input cancelled, try again");
            }
        }
    } while (roomChoice < 1 || roomChoice > 6 || ROOMS[roomChoice] != 0 || confirm != '/'); //while choice is in range, isn't booked and isn't confirmed
    //store outside the while loop
    ROOMS[roomChoice] = 1; //mark room as booked
    MAIN[8][currentGuestNo] = roomChoice;

    //daily newspaper
    do {
        //also we do not need a second while loop as the do-while can handle looping
        // char newspaperChoice; //instead of a string we can use characters (easier to deal with)
        printf("\n\nWould you like to opt for our daily newspaper, this service has a one-off charge of £5.50.");
        printf("\nEnter 'Y' for yes and 'N' for no: ");
        scanf("%c", &newspaperChoice);
        fflush(stdin);
        newspaperChoice = toupper(newspaperChoice);
        if (newspaperChoice == 'Y') {
            printf("Your newspapers will be delivered daily, thank you for choosing our service");
        } else if (newspaperChoice == 'N') {
            printf("You will not be charged for daily newspapers, enjoy your stay\n");
        } else {
            printf("Invalid choice, try again");
        }
    } while (newspaperChoice != 'Y' && newspaperChoice != 'N'); //while newspaper choice isn't valid
    MAIN[7][currentGuestNo] = newspaperChoice == 'Y' ? 1 : 0; //1 = yes, 0 = no

    //BOOKING ID
    srand(time(NULL));
    int BookingIDRandom = rand() % 100;
    sprintf(BookingID, "%s%d", lName, BookingIDRandom);
    printf("\nYour Booking ID is: %s", BookingID);

    //check for quit
    printf("\n(type '!' to quit this booking and re-book or type any to continue.) : ");
    scanf("%c", &confirm); //using confirm to check for quitting
    fflush(stdin);
    if (confirm == '!') {
        hasQuit = true;
        mainMenu(currentGuestNo);
    } else {
        mainMenu(currentGuestNo);
    }
    // return to main menu, if quit, set hasQuit to true

    return 0;
}

int bookDinner(int currentGuestNo) {
    //book dinner
    //show availablity
    //ask for table (validation)
    //store table
    char confirm;
    int tableSum;
    int tableChoice = 0;
    int tableInvalid = 0;
    tableSum = TABLES[0] + TABLES[1] + TABLES[2] + TABLES[3] + TABLES[4] + TABLES[5];
    do {
        printf("\n\nThe Kashyyk hotel has 3 dining tables: Endor, Naboo and Tatooine.\nEach table has two available bookings, at 7pm and 9pm.\nHere are the available times: ");
        if (tableSum == 6) {
            printf("\nAll tables are currently booked");
            hasQuit = true;
        }
        while (tableSum != 6) {
            if (TABLES[0] == 1) {
                printf("\n1. Endor 19:00 is booked.");
            } else {
                printf("\n1. Endor 19:00 is available for booking.");
            }
            if (TABLES[1] == 1) {
                printf("\n2. Endor 21:00 is booked.");
            } else {
                printf("\n2. Endor 21:00 is available for booking.");
            }
            if (TABLES[2] == 1) {
                printf("\n3. Naboo 19:00 is booked.");
            } else {
                printf("\n3. Naboo 19:00 is available for booking.");
            }
            if (TABLES[3] == 1) {
                printf("\n4. Naboo 21:00 is booked.");
            } else {
                printf("\n4. Naboo 21:00 is available for booking.");
            }
            if (TABLES[4] == 1) {
                printf("\n5. Tattooine 19:00 is booked.");
            } else {
                printf("\n5. Tattooine 19:00 is available for booking.");
            }
            if (TABLES[5] == 1) {
                printf("\n6. Tattooine 21:00 is booked.");
            } else {
                printf("\n6. Tattooine 21:00 is available for booking.");
            }
            printf("\n\nPlease make your choice of table:");
            scanf("%d", &tableChoice);
            fflush(stdin);
            if (tableChoice > 6 || tableChoice <= 0) {
                printf("This is not a valid table choice, please try again.");
            }
            else if (TABLES[tableChoice - 1] == 1) {
                printf("Sorry, this table is already booked.");
                tableInvalid = 1;
            }
            else {
                printf("You are booking for table %d, please type '/' to confirm:", &tableChoice);
                scanf("%c", &confirm);
                fflush(stdin);
            }
        }
    } while (confirm != '/' || tableChoice > 6 || tableChoice <= 0 || tableInvalid == 1 );
    MAIN[10][currentGuestNo] = 1; //hasbookedtable is true
    MAIN[9][currentGuestNo] = tableChoice - 1;
    TABLES[tableChoice - 1] = 1;

    return 0;
}

int checkOut(int currentGuestNo) {
    //CHECK OUT

    //room rate per day
    int roomPrice = MAIN[8][currentGuestNo] == 1 || MAIN[8][currentGuestNo] == 2 ? 100 : MAIN[8][currentGuestNo] == 3 ? 85
            : MAIN[8][currentGuestNo] == 4 || MAIN[8][currentGuestNo] == 5 ? 75 : 50;
    float roomRate = roomPrice * MAIN[6][currentGuestNo];

    //senior discount (10% off room rate)
    float seniorDiscount = 0;
    if (MAIN[11][currentGuestNo] > 65) {
        seniorDiscount = roomRate * 0.1;
    }

    //board rate per day (adults)
    float boardPrice = MAIN[5][currentGuestNo] == 3 ? 5 : MAIN[5][currentGuestNo] == 2 ? 15 : 20;
    float adultBoardRate = boardPrice * MAIN[3][currentGuestNo];

    //board rate per day (kids)
    float childBoardRateRaw = boardPrice * MAIN[4][currentGuestNo];
    float childBoardRate = boardPrice * MAIN[4][currentGuestNo] * 0.5;
    float childDiscount = childBoardRateRaw - childBoardRate;

    float totalBoardRateRaw = adultBoardRate + childBoardRateRaw;

    //daily newspaper
    float newspaperBill = 0;
    if (MAIN[7][currentGuestNo] == 1) {
        newspaperBill = 5.5;
    }

    //overall
    float totalBill = (roomRate - seniorDiscount) + adultBoardRate + childBoardRate + newspaperBill;

    //print
    printf("\nFINAL BILL- \n");
    printf("\t-ROOM RATE- \n\t +£%f", roomRate);
    if (seniorDiscount > 0) {
        printf("\t-SENIOR DISCOUNT -£%f", seniorDiscount);
    }
    printf("\t-BOARD RATE- \n\t +£%f", totalBoardRateRaw);
    if (childBoardRate > 0) {
        printf("\t-CHILD DISCOUNT -£%f", childDiscount);
    }
    if (newspaperBill > 0) {
        printf("\t-OTHER- \n\t-NEWSPAPER +£%f", newspaperBill);
    }
    printf("\nTOTAL- £%f", totalBill);


    //free room
    int room = MAIN[8][currentGuestNo];
    ROOMS[room] = 0;
    //free table if booked table
    if (MAIN[10][currentGuestNo] = 1) {
        int tableChoice = MAIN[9][currentGuestNo];
        TABLES[tableChoice] = 0;
    }
    //clear currentguestno array data
    for (int i = 0; i < 12; i++) {
        MAIN[i][currentGuestNo] = 0;
    }
    //clear currentguestno
    currentGuestNo--;

    return 0;
}

int calculate_age(struct tm dob) {
 // needs to add variables like this with year month date to calculate prtoperly
//struct tm dob1 ={0};
//dob1.tm_year = xxxx;
//dob1.tm_mon = xx;
//dob1.tm_mday = xx;


    time_t now;
    time(&now);
    struct tm* today = localtime(&now);

    if (today->tm_mon > dob.tm_mon || (today->tm_mon == dob.tm_mon && today->tm_mday >= dob.tm_mday)){
        return today->tm_year + 1900 - dob.tm_year;
    } else {
        return today->tm_year +1900 - dob.tm_year - 1;
    }

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
