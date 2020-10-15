//create using:
    //gcc --std=gnu99 -o movies test.c
//then run using
    //./movies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PREFIX "movies_"
#define POSTFIX ".csv"


int main()
{
    char result[50];
    int randomNumber = random() % 10000;
    sprintf(result, "%i", randomNumber);
    printf("%s", result);

    return EXIT_SUCCESS;
    }