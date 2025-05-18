//start
#include <stdio.h>
#include <string.h>
#include "myls.h"  // including the header file into this program

//the main program will execute whether the inputs have an argument or not in order to output the files depending on if it is hidden or not
//the necessary flags below indicate whether all files are shown as hidden or not including hidden as the type of arguments
//using argc as integer passed in the command-line arguments, in this case, argc is the postion where the file name is mentioned
//argv is the array of strings within the command-line argument, it is always the name of the program. It checks for the neccessary flag position which is important here

int main(int argc, char *argv[]) {
    int all = 0;
    int long1 = 0;
    char *directory = ".";  // need this for the else statement if there is no flag; setting parameter variable

    // if no arguments are supplied in the current directory
    if (argc == 1) {
        show_directory(directory);  // calls this function to list the contents of the current directory; prints the files (excluding hidden files)
    } else {
        // this time, it needs to be able to print ./myls folder name without " " empty in show directory
        //strcmp () source is cited for understanding more about this function's use 
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {  // handles flags for -a and -all
                all = 1;  // lists all files, including hidden files
            } else if (strcmp(argv[i], "-l") == 0) {// handles flags for -l
                long1 = 1; //lists files with details, exlcuding hidden files
            } else if (strcmp(argv[i], "-al") == 0 || strcmp(argv[i], "-la") == 0) {//handles flags for -al and -la
                long1 = 1;//this function includes all files, including hidden ones where details are printed out
                all = 1;
            } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {//handles flags for -v and --version 
                printf("my_ls v.1.0 by Anisha Dasgupta\n");//prints this simply
                return 0;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {//handles flags for -h and --help
                printf("Program Usage: using the ls command where it lists the contents of directory with options to display hidden files and detailed info.\n");
                printf("Description: myls program prints the contents of the directory with flags to show the hidden files, details or display the version or the help info.\n");
                printf("-a, --all: displays all files, including hidden files.\n");
                printf("-l: shows details for each file.\n");
                printf("-al, -la: combination of -a and -l (shows hidden file and detailed info).\n");
                printf("-v, --version: prints version of the program.\n");
                printf("-h, --help: displays help message which describes the program and the options.\n");
                printf("Types of output values: \n");
                printf("myls : listing the current directory\n");
                printf("myls -a : showing the hidden files\n");
                printf("myls -l /home : shows details of the files that are in the /home\n");//prints this manual 
                return 0;
            } else if (argv[i][0] != '-') {//if no flag exists or simply types anything without '-', then it simply prints the files that are within the directory
                directory = argv[i];
            }
        }
    }

    // when executed as either all, or long1 or both, all of that is displayed below on the types of function to call for
    if (all && long1) {
        show_directory_long_all(directory);  // this function includes all files, including hidden ones where details are printed out
    } else if (long1) {
        show_directory_long(directory);  // excludes hidden files, but prints files with details
    } else if (all) {
        show_directory_all(directory);  // just prints files within directory, including hidden
    } else {
        show_directory(directory);  // prints files within directory, excluding hidden files
    }

    return 0;  // if there are arguments supplied
}


