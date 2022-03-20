/******************************************************************************
FILENAME:   scanner.h
DESCRIPTION:
    Contains all subroutiones prototypes and global variables for the scanner
    to be fully functional.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
    Jared Rohrbaugh   - roh2827@calu.edu
    Ryan Lemmon       - lem8289@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski
    Group 4

*******************************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_util.h"

//Enumeration of the tokens
typedef enum {
    BEGIN,
    END,
    READ,
    WRITE,
    IF,
    THEN,
    ELSE,
    ENDIF,
    WHILE,
    ENDWHILE,
    ID,
    INTLITERAL,
    FALSEOP,
    TRUEOP,
    NULLOP,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    ASSIGNOP,
    PLUSOP,
    MINUSOP,
    MULTOP,
    DIVOP,
    NOTOP,
    LESSOP,
    LESSEQUALOP,
    GREATEROP,
    GREATEREQUALOP,
    EQUALOP,
    NOTEQUALOP,
    SCANEOF,
    ERROR
} token;

//Function Prototype
void clear_buffer(char*, int);
char* match(token);
token check_reserved(char*);
token check_symbol(char*);
token check_number(char*);
char* next_token();
char* test_next_token();

#endif