/******************************************************************************
FILENAME:   scanner.c
DESCRIPTION:
    Contains all subroutiones to support the scanner's functionality. The scanner
    process characters from a file one character at a time, to recognize tokens,
    and detect errors. Once the scanner is complete. It writes to output files.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
    Jared Rohrbaugh   - roh2827@calu.edu
    Ryan Lemmon       - lem8289@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski
    Group 4

*******************************************************************************/

#include "scanner.h"

void clear_buffer(char *buffer, int length) {
    memset(buffer, 0, length);
}

//Checks the TEXT keywords
token check_reserved(char *buffer) {
    //Check if reserved
    if (strcmp(buffer, "begin") == 0) {
        return BEGIN;
    }
    else if (strcmp(buffer, "end") == 0) {
        return END;
    }   
    else if (strcmp(buffer, "read") == 0) {
        return READ;
    } 
    else if (strcmp(buffer, "write") == 0) {
        return WRITE;
    } 
    else if (strcmp(buffer, "if") == 0) {
        return IF;
    } 
    else if (strcmp(buffer, "then") == 0) {
        return THEN;
    } 
    else if (strcmp(buffer, "else") == 0) {
        return ELSE;
    } 
    else if (strcmp(buffer, "endif") == 0) {
        return ENDIF;
    } 
    else if (strcmp(buffer, "while") == 0) {
        return WHILE;
    } 
    else if (strcmp(buffer, "endwhile") == 0) {
        return ENDWHILE;
    } 
    else if (strcmp(buffer, "false") == 0) {
        return FALSEOP;
    } 
    else if (strcmp(buffer, "true") == 0) {
        return TRUEOP;
    } 
    else if (strcmp(buffer, "null") == 0) {
        return NULLOP;
    } 
    //Else ID
    else {
        return ID;
    }
}

//Check Symbols
token check_symbol(char *buffer) {
    if (strcmp(buffer, "(") == 0) {
        return LPAREN;
    }
    if (strcmp(buffer, ")") == 0) {
        return RPAREN;
    }
    if (strcmp(buffer, ";") == 0) {
        return SEMICOLON;
    }
    if (strcmp(buffer, ",") == 0) {
        return COMMA;
    }
    if (strcmp(buffer, ":=") == 0) {
        return EQUALOP;
    }
    if (strcmp(buffer, "+") == 0) {
        return PLUSOP;
    }
    if (strcmp(buffer, "-") == 0) {
        return MINUSOP;
    }
    if (strcmp(buffer, "*") == 0) {
        return MULTOP;
    }
    if (strcmp(buffer, "/") == 0) {
        return DIVOP;
    }
    if (strcmp(buffer, "!") == 0) {
        return NOTOP;
    }
    if (strcmp(buffer, "<") == 0) {
        return LESSOP;
    }
    if (strcmp(buffer, "<=") == 0) {
        return LESSEQUALOP;
    }
    if (strcmp(buffer, ">") == 0) {
        return GREATEROP;
    }
    if (strcmp(buffer, ">=") == 0) {
        return GREATEREQUALOP;
    }
    if (strcmp(buffer, "=") == 0) {
        return EQUALOP;
    }
    if (strcmp(buffer, "<>") == 0) {
        return NOTEQUALOP;
    }
    //Else Error
    else {
        return ERROR;
    }
}

//Check if the token is a number
token check_number(char *buffer) {
    if (atoi(buffer) == 0) {
        return ERROR;
    }
    else {
        return INTLITERAL;
    }
}

//Return the name of the token
char* match(token t) {
    switch (t) {
        case BEGIN: return "BEGIN";
        case END: return "END";
        case READ: return "READ";
        case WRITE: return "WRITE";
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case ENDIF: return "ENDIF";
        case WHILE: return "WHILE";
        case ENDWHILE: return "ENDWHILE";
        case ID: return "ID";
        case INTLITERAL: return "INTLITERAL";
        case FALSEOP: return "FALSEOP";
        case TRUEOP: return "TRUEOP";
        case NULLOP: return "NULLOP";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case ASSIGNOP: return "ASSIGNOP";
        case PLUSOP: return "PLUSOP";
        case MINUSOP: return "MINUSOP";
        case MULTOP: return "MULTOP";
        case DIVOP: return "DIVOP";
        case NOTOP: return "NOTOP";
        case LESSOP: return "LESSOP";
        case LESSEQUALOP: return "LESSEQUALOP";
        case GREATEROP: return "GREATEROP";
        case GREATEREQUALOP: return "GREATEREQUALOP";
        case EQUALOP: return "EQUALOP";
        case NOTEQUALOP: return "NOTEQUALOP";
        case SCANEOF: return "EOF";
        default: return "ERROR";
    };
}

//Writes the errors to the listing file
void write_errors() {
    //Write Lexical Error line
    fprintf(list_file, "\n\n%d   Lexical Errors.", lexical_errors);
}

char* next_token() {
    //Scanner Variables
    bool read_flag = true;
    bool comment_flag = false;
    int token_num;
    char lexeme;
    char next_lexeme;
    char* token_id;
    static char input_token[TOKEN];
    memset(input_token, 0, TOKEN);    

    while (read_flag == true) {
        lexeme = fgetc(input_file); 

        //Process comment
        if (lexeme == '-' && comment_flag == false) {
            next_lexeme = fgetc(input_file);
            if (next_lexeme == '-') {
                fseek(input_file, -1, SEEK_CUR);
                comment_flag = true;
            }
            else {
                fseek(input_file, -1, SEEK_CUR);
                symbol_buffer[j] = lexeme;
                process_symbol = true;
            }
        }
        
        if (comment_flag == false) {
            //Process alpha character
            if (isalpha(lexeme) != 0) {
                token_buffer[i] = lexeme;
                i += 1;

                //Check the symbol buffer
                if (strlen(symbol_buffer) > 0) {
                    process_symbol = true;
                }
                //Check the number buffer 
                if (strlen(number_buffer) > 0) {
                    process_number = true;
                }
            }
            else {
                //If a token was found
                if (strlen(token_buffer) > 0) {
                    process_token = true;
                }

                //Number lexeme 
                if (isalnum(lexeme) != 0) {
                    //Check the symbol buffer
                    if (strlen(symbol_buffer) > 0) {
                        process_symbol = true;
                    }

                    number_buffer[k] = lexeme;
                    k += 1;
                }

                //Process Symbol character 
                 if (isspace(lexeme) == 0 && isalnum(lexeme) == 0 && lexeme != EOF) {
                    symbol_buffer[j] = lexeme;
                    j += 1;

                    //Check for certian SINGLE Symbols
                    if (lexeme == '(' || lexeme == ')' || lexeme == ';' || lexeme == ',' || lexeme == '+' || lexeme == '*' || lexeme == '/' || lexeme == '!') {
                        process_symbol = true;
                    }

                    //Check the number buffer
                    if (strlen(number_buffer) > 0) {
                        process_number = true;
                    }
                    if (strlen(token_buffer) > 0) {
                        process_token = true;
                    }
                }
                else {
                    if (strlen(number_buffer) > 0) {
                        process_number = true;
                    }
                    if (strlen(symbol_buffer) > 0) {
                        process_symbol = true;
                    }
                    if (strlen(token_buffer) > 0) {
                        process_token = true;
                    }
                }
            }
        }

        //Recognize the token
        if (process_token == true) {
            read_flag = false;

            strcpy(input_token, token_buffer);

            //Check Reserved word 
            token_num = check_reserved(token_buffer);
            token_id = match(token_num);

            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, token_buffer);
                lexical_errors += 1;
                error_flag = true;
            }

            memset(token_buffer, 0, TOKEN);
            i = 0;
            process_token = false;
        }
        //Recognize the number 
        if (process_number == true) {
            read_flag = false;

            strcpy(input_token, number_buffer);

            //Check Valid number 
            token_num = check_number(number_buffer);
            token_id = match(token_num);

            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, number_buffer);
                lexical_errors += 1;
                error_flag = true;
            }

            memset(number_buffer, 0, TOKEN);
            k = 0;
            process_number = false;
        }
        //Recognize the symbol
        if (process_symbol == true) {
            read_flag = false;

            strcpy(input_token, symbol_buffer);

            //Check Reserved Symbol
            token_num = check_symbol(symbol_buffer);
            token_id = match(token_num);

            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, symbol_buffer);
                lexical_errors += 1;
                error_flag = true;
            }

            memset(symbol_buffer, 0, TOKEN);
            j = 0;
            process_symbol = false;
        }

        //Check if EOF 
        if (lexeme == EOF) {
            read_flag = false;

            //Process End of file
            token_num = SCANEOF;
            token_id = match(token_num);

            //Write Lexical Ettor line
            fprintf(list_file, "\n\n%d   Lexical Errors.", lexical_errors);
        }
        else {
            fputc(lexeme, list_file);

            //Write to listing file 
            if (lexeme == '\n') {
                //Check for errors
                if (error_flag == true) {
                    fprintf(list_file, "Error.  %s not recognized.\n", lex_error_sequence);
                    error_flag = false;
                    memset(lex_error_sequence, 0, TOKEN);
                }

                fprintf(list_file, "%d   ", line_count);
                //Reset comment flag
                comment_flag = false;
                line_count += 1;
            } 
        }
    }

    return input_token;
}