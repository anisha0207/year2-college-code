#include <stdio.h>
#include <stdlib.h> //getenv() and malloc() functions
#include <string.h> //string manipulation like strtok() and strcmp()
#include <unistd.h> //access function checking file permission

//compile using: gcc -o main main.c
//program using: ./main -a
//retrieve and split path varaible into directory (printed it out for testing)
//construct file path and check
//prints the matching file path based on the flag

// Function prototype-- executes and checks if file exist
int file_exists(char* path);

int main(int argc, char** argv){
    //read path variable
   char* path_e = getenv("PATH");//using the path variable
   if (path_e == NULL){
       printf("Path not found.\n");
       return 1;
   } 
   //copy path variable 
   char* path_c = strdup(path_e);//copies path, allocates memory into the path string
   if (path_c == NULL){
       printf("Fail\n");
       return 1;
   }
   //split path varaible into directories
    char* direct = strtok(path_c, ":");//splits path into directories using delimiter
    while(direct != NULL){
        // printf("Directory: %s\n", direct);//print each directory; testing purposes
        direct = strtok(NULL, ":");//move to next directory
    }

    //handle -a flag-- checks if command-line argument is valid
    if (argc< 2){
        printf("%s [-a] filename \n", argv[0]);
        return 2;//when invalid from the options
    }
    //check -a flag
    int flag = 0;
    int file = 1;
    if (strcmp(argv[1], "-a") == 0){// check if first argument is -a
        flag = 1;
        file = 2;
    }//when -a is found, set flag to 1 and look for file from argv[2]

    //starts iterating through filenames within the command line 
    for (int i = file; i < argc; i++) {
        char* filename = argv[i];

        // Create another copy of PATH for tokenization
        char* path_c2 = strdup(path_e);
        if (path_c2 == NULL) {
            printf("Failed.\n");
            return 1;
        }
        //tokenize copied path varaible to receive directories one at a time
        char* current_direct = strtok(path_c2, ":");

        while (current_direct != NULL) {
            // Construct full file path
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", current_direct, filename);

            // Check if the file exists and is executable
            if (file_exists(filepath)) {
                printf("%s\n", filepath);
                if (!flag) {
                    break;  // Stop searching if -a flag is not set
                }
            }
            current_direct = strtok(NULL, ":");
        }
        free(path_c2);//prevent memory leaks for allocated memory
    }
    free(path_c);//prevent memory leaks for original duplicated path variable
    return 0; 
}
int file_exists(char* path){
    if (access(path, X_OK) == 0){
        return 1;//file exists and executable
    }
    return 0; //file not exist or not executable
}

