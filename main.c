//create using:
// gcc --std=gnu99 -o movies main.c
//then run using
// ./movies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//FUNCTION DECLARATIONS
void fileProcessingMenu();
void largestFile();
void smallestFile();
void specifyFile();

int main()
{
    int userChoice = 1; //initialize to 1 to enter while loop (later)

    //main user interface loop
    while (userChoice)
    {
        //Show user main menu options
        printf("1. Select file to process.\n");
        printf("2. Exit the program.\n");

        //Prompt user for menu choice
        printf("Enter a choice from 1 to 2: ");
        //Get user input
        scanf("%i", &userChoice);

        printf("\n");

        //if bad input (out of bounds)
        if (userChoice < 1 || userChoice > 2)
            printf("You entered an incorrect choice. Try again.\n\n");

        else if (userChoice == 1)
        {
            fileProcessingMenu();
        }
        else if (userChoice == 2)
        {
            break;
        }
    }


    return EXIT_SUCCESS;
}



//FUNCTIONS ------------------------------------------------------------
void fileProcessingMenu()
{
    int fpmUserChoice = 1;
    while (fpmUserChoice)
    {
        printf("Which file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");

        //Prompt user for menu choice
        printf("Enter a choice from 1 to 3: ");
        //Get user input
        scanf("%i", &fpmUserChoice);

        printf("\n");

        //if bad input (out of bounds)
        if (fpmUserChoice < 1 || fpmUserChoice > 3)
            printf("You entered an incorrect choice. Try again.\n\n");

        else if (fpmUserChoice == 1)
        {
            largestFile();
            break;
        }
        else if (fpmUserChoice == 2)
        {
            smallestFile();
            break;
        }
        else if (fpmUserChoice == 3)
        {
            specifyFile();
            break;
        }
    }

    return;
}

void largestFile()
{

    return;
}

void smallestFile()
{

    return;
}

void specifyFile()
{


    return;
}
