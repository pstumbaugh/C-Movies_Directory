HW2 movies

This program will parse through a data file, create a new directory (using my ONID, ".movies.", and a random number), and store the movie names in .txt files within that new directory. Those files will be named based on the year the movie is associated with. 

The main menu will give the user 3 different options. Use the largest file, use the smallest file or use a user specified file. The largest/smallest files must start with the prefix "movies_" and end with the postfix ".csv". The user specified file must be entered exactly as the file is named, including the extension. 


To make the file:
Create an executable in the command line of the same directory that main.c is in by typing in:
    gcc --std=gnu99 -o movies main.c

This will create an executable file named "movies" (as per the -o flag command above)

To run the file, type "./movies" in the command line. This will start the executable file and you will follow the options from there. 

*Note - the data files you will want to parse from must be in the same directory as the main.c and movies executable file. 
