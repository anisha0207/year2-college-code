# CSE109 - Systems Software - Spring 2025

# Programming Assignment 1 - my_which

**Due Date: 2/10/2025 EOD**

## Assignment

For this assignment you will write your own version of the the `which` program called `my_which`. This program will search through your PATH variable to find the directory or directories in which a target program is found. For example:

```
$ my_which gcc
```

may have the output: `/usr/local/bin/gcc`. Here is the man page for which:

```
WHICH(1)                                                        General Commands Manual                                                        WHICH(1)

NAME
       which - locate a command

SYNOPSIS
       which [-a] filename ...

DESCRIPTION
       which returns the pathnames of the files (or links) which would be executed in the current environment, had its arguments been given as commands
       in a strictly POSIX-conformant shell.  It does this by searching the PATH for executable files matching the names of the arguments. It does  not
       canonicalize path names.

OPTIONS
       -a     print all matching pathnames of each argument

EXIT STATUS
       0      if all specified commands are found and executable

       1      if one or more specified commands is nonexistent or not executable

       2      if an invalid option is specified
```

Your program should behave exactly the same way as `which`, including the flags and the output values. 

Make sure your program can do the following. You must make one commit per bullet item.

### `main()`

In the `int main(int argc, char** argv)` function:

- Read in the `PATH` variable. Save it to a variable local to your program.
       - Split the `PATH` variable into a list of directories. 

- Read and parse command line inputs.
       - Recognize all flags passed as inputs, separate them from the file names
       - Determine if the `-a` flag is passed, and if unknown flags are passed.

- Iterate through the `PATH` directories
  - Iterate through the file names
    - Concatenate the file name to the path directory.
    - Determine whether or not this is a valid executable file
       - If it is then print the concatenated path and the filename to `stdout`.
    - Move on to the next filename if the `-a` flag is not passed into the program. 
    - If `-a` is passed into the program, continue looping through the remaining directories in the PATH to find duplicate files.

- Return the appropriate value from the program according to the man page.

### `file_exists()`

In main.c, implement the following function. Make at least 1 commit:

- `int file_exists(char* path)` - Takes as input a path to a file, and determines if the file exists and is executable (the [`access()`](https://linux.die.net/man/2/access) function might help you here). Returns 0 if the file doesn't exist or is exetuable. Return 1 if it exsists and is executable.

## Instructions

The process of working on assignments will proceed as follows:

1. Fork the relevant repository into your CSE 109 group namespace. [Instructions](https://docs.gitlab.com/ee/user/project/repository/forking_workflow.html)

2. Clone your newly forked repository to your computer. Your repository should be hosted at 
```
https://gitlab.com/<your user name>-cse109/<assignment name>
```
You can use the following git command with the appropriate values substituted to customize it for you:
```
git clone https://gitlab.com/<your user name>-cse109/<assignment name>
```
[Instructions](https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html#clone-a-repository) 

3. Write the necessary code to get your project to pass the tests.

4. Push the changes you made locally to your gitlab repository. Follow the instructions [here](https://githowto.com/staging_and_committing) (read sections 6, 7 and 8) about staging and committing changes.
