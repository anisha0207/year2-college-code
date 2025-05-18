//start
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> //directory entries 
#include <string.h>
#include <unistd.h> //access()
#include <sys/stat.h> //needed to find the bytes of the file for show_directory_long and show_directory_long_all

//Four functions are called here, each serving a purpose that is described in my_ls.h
//directory is the common parameter here when passing the argument into the main function
//new thing to add is dirent.h: helps for directing files adn folders within the directory while running this ls program

void show_directory(char* directory) {
    DIR *dirp; // type defined in dirent.h; directory stream, read the contents of the directory
    struct dirent *entry; // custom data type that holds info about directory entry; *entry is the pointer

    // Open the directory using the opendir() function
    dirp = opendir(directory);
    if (!dirp) {
        // If the directory can't be opened, print an error message
        perror("opendir");
        return;
    }

    while ((entry = readdir(dirp)) != NULL){//changed from do-while to while loop for efficency
        // If the entry name doesn't start with a dot (i.e., not hidden), print it
        if (entry->d_name[0] != '.') {
            printf("%s\n", entry->d_name);
        }
    }

    // Close the directory at the end
    closedir(dirp);
}

void show_directory_all(char *directory) {
    struct dirent *entry;
    DIR *dirp = opendir(directory);
    
    if (dirp == NULL) {
        perror("opendir");
        return;
    }

    // Initialize counts for files and directories
    int count_inFile = 0;
    int count_inDir = 0;

    // Loop through all files and directories in the provided directory
    entry = readdir(dirp);
    if (entry == NULL) {
        closedir(dirp);
        return;
    }
    while ((entry = readdir(dirp)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip . and .. entries
        }

        // Print only the file/directory name
        printf("%s\n", entry->d_name);
    }

    closedir(dirp);
}

// Function to display the contents of the directory with detailed information (non-hidden files only)
void show_directory_long(char* directory) {
    DIR *dirp;
    struct dirent *entry;
    struct stat file_size;
    int count_inFile = 0;  // Initialize count for files
    int count_inDir = 0;   // Initialize count for directories

    // Open the directory stream
    if ((dirp = opendir(directory)) == NULL) {
        perror("error");
        return;
    }

    // First, count the number of files and directories in the directory
    while ((entry = readdir(dirp)) != NULL) {
        // Skip hidden files (files that start with a dot)
        if (entry->d_name[0] == '.') {
            continue;  // Skip hidden files
        }

        // If the entry is a directory, increment the directory counter
        if (entry->d_type == DT_DIR) {//source cited adn followed under the struct dirent
            count_inDir++;  // Increment directory count
        } else {
            count_inFile++;  // Otherwise, it's a file, so increment the file count
        }
    }

    // Rewind the directory stream to read entries again
    rewinddir(dirp);

    // Print the total count of files and directories
    printf("Total files: %d\n", count_inFile);  // Display the count of files
    printf("Total directories: %d\n", count_inDir);  // Display the count of directories

    // Read and print the directory entries (excluding hidden files)
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_name[0] != '.') {  // Skip hidden files (files that start with a dot)
            char full_path[512];  // a buffer for storing the full path of the entry
            snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);  // Create the full path

            // Get file information using stat()
            if (stat(full_path, &file_size) == 0) {// this format followed using a source cited below
                printf("%s", entry->d_name);  // Print the name of the directory or file

                // Check if the entry is a directory or executable
                if (entry->d_type == DT_DIR) {
                    printf("/");  // Mark directories with '/'
                } else if (access(full_path, X_OK) == 0) {  // If the entry is executable
                    printf("*");  // Mark the executable with '*'
                }

                printf(" (%d bytes)\n", file_size.st_size);  // Print the file size
            } else {
                perror("error"); // Print error message if stat fails
                printf("%s\n", entry->d_name); // Just print the file name if stat fails
            }
        }
    }
    // Close the directory stream at the end
    closedir(dirp); 
}
 
void show_directory_long_all(char* directory){
    //alot of the code below is similar to the one above, except we are including hidden files 
    DIR *dirp;
    struct dirent *entry;
    struct stat file_size; //custom data type that holds info about the file, including the size, file_size is the pointer
    int count_inFile = 0;
    int count_inDir = 0;

    // Open the directory
    if ((dirp = opendir(directory)) == NULL) {
        perror("error");
        return;
    }

    // First, count the number of files and folders in the directory
    while ((entry = readdir(dirp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip current (.) and parent (..) directory entries
        }
    //When running the code, the output displayed . and .. to indicate such files existed but this portion is redundant in the process

        if (entry->d_type == DT_DIR) { // Directory
            count_inDir++;
        } else {  // Regular file
            count_inFile++;
        }
    }

    // Rewind the directory stream to read entries again
    rewinddir(dirp); // Needed to loop through it again to print the details

    // Print counts for files and directories
    printf("Total files: %d\n", count_inFile);   // Print total count of files
    printf("Total directories: %d\n", count_inDir); // Print total count of directories

    // Read and print the directory entries (including hidden files)
    while ((entry = readdir(dirp)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }//had to declare this since it didn't work the first time as it outputted twice in a row
        
        // Construct the full path to the entry by combining the directory path and entry name
        char full_path[512];//buffer
        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);  // Full path

        // Get file information using stat()
            if (stat(full_path, &file_size) == 0) {// this format followed using a source cited below
                printf("%s", entry->d_name);  // Print the name of the directory or file

                // Check if the entry is a directory or executable
                if (entry->d_type == DT_DIR) {
                    printf("/");  // Mark directories with '/'
                } else if (access(full_path, X_OK) == 0) {  // If the entry is executable
                    printf("*");  // Mark the executable with '*'
                }

                printf(" (%d bytes)\n", file_size.st_size);  // Print the file size
            } else {
                perror("error"); // Print error message if stat fails
                printf("%s\n", entry->d_name); // Just print the file name if stat fails
            }
    }
    // Close the directory stream at the end
    closedir(dirp);   
}

