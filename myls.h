//start
#ifndef MYLS_H
#define MYLS_H
#include <stdio.h>
#include <dirent.h>

void show_directory(char* directory);
//This function lists the files and folders that are only visible
//it only goes through the contents wihtout including the hidden files (starting with '.')
//directory is the parameter which is the directory to the path

void show_directory_all(char* directory);
//Lists all the files and folders, including the hidden files in the directory
//includes '.' (hidden files)
//directory is the parameter where the path of the directory

void show_directory_long(char* directory);
//lists files and folders that have details in it
//displays the number of files and directories in the given folder
//lists each item with information --whether it was executable
//parameter: directory that's listed

void show_directory_long_all(char* directory);
//Lists all the files and folders which includes the hidden ones that hold extra details
//Prints total number of files and folders, provides details on each item 
//parameter: directory that's listed

#endif //the source for this format is cited