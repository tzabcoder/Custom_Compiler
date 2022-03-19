/******************************************************************************
FILENAME:   main.c
DESCRIPTION:
    Driver of the compiler. Calls utility functions to process the files and the 
    scanner.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
    Jared Rohrbaugh   - roh2827@calu.edu
    Ryan Lemmon       - lem8289@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski
    Group 4

*******************************************************************************/

#include "file_util.h"
#include "parser.h"

int main (int argc, char *argv[]) {
    //Process Flags
    bool file_util_flag;
    bool system_goal_flag;

    file_util_flag = init_files(argc, argv);
    if (file_util_flag == true) {
        system_goal_flag = system_goal();

        if (system_goal_flag == true) {
            printf("Paser Complete...\n");
        }
    }
    else {
        perror("Error: processing parser...\n");
    }
    file_util_flag = wrap_up();

    return 0;
}