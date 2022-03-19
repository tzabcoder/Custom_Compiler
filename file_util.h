/******************************************************************************
FILENAME:   file_util.h
DESCRIPTION:
    Header file that contains all program constants, and function protypes.
    main.c and file_util.c reference header file for constants and subroutines.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
    Jared Rohrbaugh   - roh2827@calu.edu
    Ryan Lemmon       - lem8289@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski
    Group 4

*******************************************************************************/

#ifndef FILE_UTIL
#define FILE_UTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Variables and Definitions
#define FSIZE 30
#define BUFFER 1024
#define READ_FILE_MODE "r"
#define WRITE_FILE_MODE "w"
#define INPUT_EXTENSION ".IN"
#define OUTPUT_EXTENSION ".OUT"
#define LISTING_EXTENSION ".LIS"
#define BACKUP_EXTENSION ".BAK"
#define TEMP_FILE "tmpfile.TMP"

typedef enum {
    false,       // 0 for false
    true         // 1 for true 
} bool;

//Input File Position
int input_pos;
//Holds the number of lexical errors
int lexical_errors;
int syntax_errors;
int line_count;
bool error_flag;

FILE *input_file;
FILE *output_file;
FILE *list_file;
FILE *temp_file;

//Function Prototypes
bool init_files(int, char**);                //File Start-Up Routine
bool process_input(char*);                   //Open the input file
bool process_output(char*);                  //Open the output file
bool process_tempfile();                     //Open temporary file
bool wrap_up();                              //File Wrap-Up Routine
void copy_file_contents(FILE*, FILE*);       //File copy routine
void write_output(FILE*, char*, char*);

#endif