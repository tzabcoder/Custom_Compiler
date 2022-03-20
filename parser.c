#include "parser.h"

//Use write to output file in the parser

char* to_upper(char* buffer) {
    int i = 0;
    int len = strlen(buffer);
    char b[TOKEN];
    char* arr;

    for (int i = 0; i < len; i++) {
        b[i] = toupper(buffer[i]);
    }

    strcpy(arr, b);
    return arr;
}

bool system_goal() {
    bool status = false;
    char* t = "";

    while (strcmp(t, "end") != 0) {
        t = next_token();
        printf("%s ", t);
    }

    return status;
}
