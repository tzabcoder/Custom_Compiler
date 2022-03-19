#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "file_util.h"
#include "scanner.h"

//Production Functions
char* to_upper(char*);
bool system_goal();
bool program();

#endif