//create using:
    //gcc --std=gnu99 -o movies main.c
//then run using
    //./movies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h> 
#include <stdbool.h>


#define PREFIX "movies_"
#define POSTFIX ".csv"

//FUNCTION DECLARATIONS
void fileProcessingMenu();
void largestFile();
void smallestFile();
bool specifyFile();
char* generateName();
void createMovieFiles(char* fileName);
struct movie *createMovie(char *currLine);
struct movie *processFile(char *filePath);
void freeMem(struct movie *list);


int main()
{
    int userChoice = 1; //initialize to 1 to enter while loop (later)

    //main user interface loop
    //will exit on user choice of 2 (or 0 as loop would evaluate to false and exit)
    while (userChoice)
    {
        printf("\n");

        //Show user main menu options
        printf("1. Select file to process.\n");
        printf("2. Exit the program.\n\n");

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



//movie struct
struct movie
{
    //variables within movie struct
    char *title;
    char *tempYear; //using as a temp holder, will change str to int year
    int year;
    //at most, 5 languages can be saved (0-4)
    //each string max of 20 characters (+null terminating)
    char languages[4][21];
    char *tempRating; //using as a temp holder, will change str to int rating
    double rating;
    struct movie *next; //pointer to next linked list item
};





//FUNCTIONS ------------------------------------------------------------

//Displays a menu with options for what to do with the file
//INPUT: nothing
//OUTPUT: (depends on which option selected)
void fileProcessingMenu()
{
    int fpmUserChoice = 1; //default user choice (so that it enters following loop)
    while (fpmUserChoice)
    {
        //display choices
        printf("Which file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");

        printf("\n");

        //Prompt user for menu choice
        printf("Enter a choice from 1 to 3: ");
        //Get user input
        scanf("%i", &fpmUserChoice);

        printf("\n");

        //if bad input (out of bounds), print error and ask again
        if (fpmUserChoice < 1 || fpmUserChoice > 3)
            printf("You entered an incorrect choice. Try again.\n\n");

        //pick largest file choice
        else if (fpmUserChoice == 1)
        {
            largestFile();
            break;
        }
        //pick smallest file choice
        else if (fpmUserChoice == 2)
        {
            smallestFile();
            break;
        }
        //pick specific name file choice
        else if (fpmUserChoice == 3)
        {
            if (specifyFile() == true) //Found the file, exit processing menu
                break;
            else //didn't find the file, re-loop through processing menu options
                fpmUserChoice = 1;
        }
    }
    return;
}



//Finds the largest file in the current directory, creates new directory and parses
// that info into year categories in the new directory. Returns to parent directory
//INPUT: nothing
//OUTPUT: creates new directory with movie info inside it
void largestFile()
{
    //open the current directory
    DIR* currDir = opendir(".");

    struct dirent *aDir; //used for current directory being examined
    struct stat dirStat;
    struct dirent *myDir; //used to save largest file
    struct stat myDirStat;

    int counter = 0; //used to see if it's first file or not (0 = first file, 1+ = not first file)

    //iterate through all files in directory
    while ((aDir = readdir(currDir)) != NULL)
    {
        //check if movies file starts with prefix ("movies_")
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0)
        {
            int size = strlen(POSTFIX); //get size of POSTFIX (4 characters ".csv")
            int tempSize = strlen(aDir->d_name) - size; //get size of file - size
            //make temp pointer to file starting at end - size address (which leaves just the last 4 characters)
            char *testName = &aDir->d_name[tempSize];

            //check if movies file ends with postfix (".csv")
            if (strcmp(testName, POSTFIX) == 0)
            {
                if (counter == 0)//if the counter = 0, then it's the firstfile being examined
                {
                    myDir = aDir;
                    counter = 1;
                }
                else // check if aDir (one being viewed currrently) is larger than saved myDir, using stat
                {
                    stat(aDir->d_name, &dirStat);
                    stat(myDir->d_name, &myDirStat);

                    //if new dir is larger than currently saved dir, copy over the old one with the new one
                    if (dirStat.st_size > myDirStat.st_size)
                    {
                        myDir = aDir;
                    } 
                }
            }
        }
    }
    //display which file it is processing
    printf("Now processing the chosen file named %s\n", myDir->d_name);

    //create a new directory name
    char* newDirName = generateName();

    //make the directory with permissions: rwxr-x--- (0750)
    mkdir(newDirName, 0750);

    printf("Created directory with name %s\n", newDirName);

    //copy current directory name into fileName
    char* fileName = myDir->d_name; 

    chdir(newDirName); //change to the child directory recently created

        //save the file path to get back to parent directory 
        //(used to reference file in parent directory while in createMovieFiles function, since we're in 
        // the newly created child directory)
        char* parentFilePath = "../"; //prefix for going to parent directory

        //get total size of new parent directory name
        int parentNewSize = strlen(parentFilePath)+ strlen(fileName);

        //allocate memory for new parent directory name size
        char* pFilePathBuffer = (char *)malloc(parentNewSize);

        //copy and concat the names into pFilePathBuffer
        strcpy(pFilePathBuffer,parentFilePath);
        strcat(pFilePathBuffer,fileName);

        // copy the new buffer
        parentFilePath = pFilePathBuffer;

        //release old buffer
        free(pFilePathBuffer);

        //create files within newDirName (the current directory we are in), one file for each year a movie was released (.txt files)
        createMovieFiles(parentFilePath);

    chdir(".."); //change back to parent directory

    //close directory and exit function
    closedir(currDir);
    return;
}



//Finds the smallest file in the current directory, creates new directory and parses
// that info into year categories in the new directory. Returns to parent directory
//INPUT: nothing
//OUTPUT: creates new directory with movie info inside it
void smallestFile()
{

}




bool specifyFile()
{
    char userFileName[50];
    //prompt for the name of the file the user wants to process
    printf("Enter the complete file name: ");
    //Get user input
    scanf("%s", userFileName);

    printf("\n");

    //open the current directory
    DIR* currDir = opendir(".");

    struct dirent *aDir; //used for current directory being examined
    struct stat dirStat;
    bool flag = false; //flag for whether file is found or not

    //iterate through all files in directory
    while ((aDir = readdir(currDir)) != NULL)
    {
        //check if movies file is same as user's file name
        if (strncmp(userFileName, aDir->d_name, strlen(aDir->d_name)) == 0)
        {
            //file found, set flag to true
            flag = true;
        }
    }

    //if going through all the files doesn't find anything matching, it will keep the flag as false
    if (flag == false)
    {
        printf("The file %s was not found. Try again.\n", userFileName);
        printf("\n");
        //close directory and exit function
        closedir(currDir);
        return false;
    }
    else //file name found a match, create a new directory and .txt contents
    {
        //display which file it is processing
        printf("Now processing the chosen file named %s\n", userFileName);

        //create a new directory name
        char* newDirName = generateName();

        //make the directory with permissions: rwxr-x--- (0750)
        mkdir(newDirName, 0750);

        printf("Created directory with name %s\n", newDirName);

        //copy current file name into fileName (uncessary, but keeping with code setup of largestFile() and smallestFile() )
        char* fileName = userFileName; 

        chdir(newDirName); //change to the child directory recently created

            //save the file path to get back to parent directory 
            //(used to reference file in parent directory while in createMovieFiles function, since we're in 
            // the newly created child directory)
            char* parentFilePath = "../"; //prefix for going to parent directory

            //get total size of new parent directory name
            int parentNewSize = strlen(parentFilePath)+ strlen(fileName);

            //allocate memory for new parent directory name size
            char* pFilePathBuffer = (char *)malloc(parentNewSize);

            //copy and concat the names into pFilePathBuffer
            strcpy(pFilePathBuffer,parentFilePath);
            strcat(pFilePathBuffer,fileName);

            // copy the new buffer
            parentFilePath = pFilePathBuffer;

            //release old buffer
            free(pFilePathBuffer);

            //create files within newDirName (the current directory we are in), one file for each year a movie was released (.txt files)
            createMovieFiles(parentFilePath);

        chdir(".."); //change back to parent directory     
    }

    //close directory and exit function
    closedir(currDir);
    
    return true;
}



//Creates a new char* name using "ONID" + ".movies." + "random number"
//INPUT: nothing
//OUTPUT: cstring
char* generateName()
{
    // Use current time as seed for random generator 
    srandom(time(0)); 
    int randomNumber = random() % 100000; //mod by 100000 to get numbers (remainders) under 99999

    //create spot for random number int to be converted to string
    char rNumStr[10];
    sprintf(rNumStr, "%i", randomNumber); // convert to string

    char* newDirName = "stumbaup";
    char* moviesName = ".movies.";
    char* ONID = "stumbuap";
    //get total size of new name
    int newSize = strlen(newDirName) + strlen(moviesName) + strlen(rNumStr);

    //allocate memory for new name size
    char* newBuffer = (char *)malloc(newSize);

    //copy and concat the names into newBuffer
    strcpy(newBuffer,newDirName); //add prefix (ONID)
    strcat(newBuffer,moviesName); //add ".movies."
    strcat(newBuffer,rNumStr); //add postfix (random number)

    // copy the new buffer
    newDirName = newBuffer;

   // release old buffer
    free(newBuffer);

    return newDirName;
}



//Creates files for each year, if a movie was released in that year. 
//creates files in the current directory from the calling function
//INPUT: fileName
//OUPUT: creates .txt files wihtin directoryName 
void createMovieFiles(char* fileName)
{
    //process the file (creates a linked list of movies with head pointer in *moviesLL)
    struct movie *moviesLL = processFile(fileName);

    //iterate through all the movies in our linked list of movies
    while (moviesLL != NULL)
    {
        int thisYear = moviesLL->year;
	    int file_descriptor;
        char yearStr[10];
        char* moviesPostfix = ".txt";
        sprintf(yearStr, "%i", thisYear); // convert to string

        //get total size of new name
        int newSize = strlen(moviesPostfix) + strlen(yearStr);

        //allocate memory for new name size
        char* newBuffer = (char *)malloc(newSize);

        //copy and concat the names into newBuffer
        strcpy(newBuffer,yearStr);
        strcat(newBuffer,moviesPostfix);

        // copy the new buffer
        char* newFilePath = newBuffer;

        free(newBuffer);

        //open file and do work
	    file_descriptor = open(newFilePath, O_RDWR | O_CREAT | O_APPEND, 0640); //permissions: read-write, read, none
	    if (file_descriptor == -1)
        {
		    printf("open() failed on \"%s\"\n", newFilePath);
		    perror("Error");
		    exit(1);
	    }
        //write our string to the file
        int messageSize = strlen(moviesLL->title) + 1;
        char message[messageSize]; //create cstring of the size of the title about to be copied
        strcpy(message, moviesLL->title);

        //temp open a file to read (checking the size if it's empty or not)
        FILE* myFile = fopen(newFilePath, "r"); 
        fseek (myFile, 0, SEEK_END);
        int size = ftell(myFile);
        fclose(myFile);
        if (size == 0) //empty file
        {
            write(file_descriptor, message, strlen(message)); //write message(title) to file
        }
        else //not empty file
        {
            char newLine[] = "\n"; //create new line and write to file
            write(file_descriptor, newLine, strlen(newLine)); //write message(title) to file
            write(file_descriptor, message, strlen(message)); //write message(title) to file
        }

        // Close the file descriptor
        close(file_descriptor);

        //go to next movie in list
        moviesLL = moviesLL->next;
    }

    //free up memory from movies linked list
    freeMem(moviesLL);

    return;
}



//**taken and edited from my HW1**
//creator for movie struct. Will parse through current line to gather info
//INPUT: currLine cstring pointer
//OUTPUT: movie struct node
struct movie *createMovie(char *currLine)
{
    //Variables
    //allocate memory for new movie struct (will free at end of function)
    struct movie *currMovie = malloc(sizeof(struct movie));
    // For use with strtok_r
    char *saveptr;
    char *langSaveptr;
    //token placeholders for info
    char *token;
    char *langToken;

    // Get movie title
    token = strtok_r(currLine, ",", &saveptr); //from first token
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    
    //get movie year
    token = strtok_r(NULL, ",", &saveptr); //from second token
    currMovie->tempYear = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempYear, token);
    //convert from string to int
    currMovie->year = atoi(currMovie->tempYear);

    //get movie language(s)
    //token includes all languages
    token = strtok_r(NULL, ",", &saveptr); //from third token
    langToken = strtok_r(token, "[", &langSaveptr); //get rid of [ from start
    int langCounter = 0;
    while (langCounter < 5)
    {
        if (langCounter == 0) //first item
        {
            token = strtok_r(langToken, "; ]", &langSaveptr); //from first token
            strcpy(currMovie->languages[langCounter], token);
            langCounter++;
        }
        else
        {
            token = strtok_r(NULL, "; ]", &langSaveptr); //from first token
            if (token == NULL) //nothing left
                break;
            strcpy(currMovie->languages[langCounter], token);
            langCounter++;
        }
    }

    //get movie rating
    //delimted by new line \n
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->tempRating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempRating, token);
    //convert from string to double
    char* end;
    currMovie->rating = strtod(currMovie->tempRating, &end);
    
    //assume this node will be at end of list, set next pointer to NULL
    currMovie->next = NULL;

    return currMovie;
}



//**taken and edited from my HW1**
//read through input file and parse information into a linked list of movies
//INPUT: cstring pointer filePath
//OUTPUT: head pointer to linked list
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading
    FILE *moviesFile = fopen(filePath, "r");

    //variables to use while making new linked list of movies
    char *currLine = NULL; //assume starting with no data, null
    size_t len = 0;
    ssize_t nread;
    //setup head/tail pointers for linked list to NULL (empty list)
    //head will point to head of linked list.
    //tail will point to last item in linked list
    struct movie *head = NULL;
    struct movie *tail = NULL;

    //get first line and do nothing with it
    //first line is just column titles
    getline(&currLine, &len, moviesFile);

    // Read the file line by line (starting at line 2)
    //loop until out of data
    while ((nread = getline(&currLine, &len, moviesFile)) != -1)
    {
        //if in loop, assume there is a new movie to get
        //create a new movie node, call createMovie function to get info
        // from line
        struct movie *newNode = createMovie(currLine);

        //check if it's the first line. If yes, set head and tail pointers
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else //there are movies already in data, add to end of linked list
        {
            tail->next = newNode;
            tail = newNode; //reset tail pointer to current node
        }
    }
    
    free(currLine); //free up memory
    fclose(moviesFile); //close file
    return head;
}




//frees memory from heap from movies linked list
//INPUT: movie list head pointer
//OUTPUT: nothing
void freeMem(struct movie *list)
{

    struct movie *temp = list;
    while (temp != NULL)
    {
        struct movie *temp2 = temp->next;
        free(temp->title);
        free(temp->tempYear);
        free(temp->tempRating);
        free(temp);
        temp = temp2;
    }

    return;
}



