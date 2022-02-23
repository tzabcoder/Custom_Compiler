/******************************************************************************
FILENAME:   main.c
DESCRIPTION:
    Driver of the compiler. Calls utility functions to process the files and the 
    scanner.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski

*******************************************************************************/

#include "file_util.h"
#include "scanner.h"

int main (int argc, char *argv[]) {
    //Process Flags
    bool file_util_flag;
    bool scanner_flag;

    file_util_flag = process_files(argc, argv);
    if (file_util_flag == true) {
        scanner_flag = scanner(input_file, output_file, list_file);

        if (scanner_flag == true) {
            printf("Scanner Processed Successfully...\n");
        }
    }
    file_util_flag = close_files();

    return 0;
}