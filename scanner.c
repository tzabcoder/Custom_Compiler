/******************************************************************************
FILENAME:   scanner.c
DESCRIPTION:
    Contains all subroutiones to support the scanner's functionality. The scanner
    process characters from a file one character at a time, to recognize tokens,
    and detect errors. Once the scanner is complete. It writes to output files.
AUTHORS:
    Trevor Zabilowicz - zab5682@calu.edu
COURSE:
    Language Translations - CSC 460
    Dr. Pyzdrowski

*******************************************************************************/

#include "scanner.h"
#include "file_util.h"

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
char* token_ident(token t) {
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

//Return the symbol identity 
char* symbol_ident(token t) {
    switch (t) {
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
        case ERROR: return "ERROR";
        case LESSOP: return "LESSOP";
        case LESSEQUALOP: return "LESSEQUALOP";
        case GREATEROP: return "GREATEROP";
        case GREATEREQUALOP: return "GREATEREQUALOP";
        case EQUALOP: return "EQUALOP";
        case NOTEQUALOP: return "NOTEQUALOP";
        default: return "ERROR"; 
    };
}

//Return the number identity
char* number_ident(token t) {
    switch (t) {
        case INTLITERAL: return "INTLITERAL";
        default: return "ERROR";
    };
}

//Write to output file
void write_output(FILE *out_file, token token_num, char *token_id, char *token_buffer) {
    int w[3] = {2, 15, 15};

    //Formatted Output File
    fprintf(out_file, "token number:  %-*d", w[0], token_num);
    fprintf(out_file, "     token type:  %-*s", w[1], token_id);
    fprintf(out_file, "     actual token:   %-*s\n", w[2], token_buffer);
}

bool scanner(FILE *input_file, FILE *output_file, FILE *list_file) {
    //Scanner Variables
    int lexical_errors;
    int token_num;
    int  i, j, k; 
    int line_count = 1;
    char lexeme;
    char next_lexeme;
    char symbol_buffer[TOKEN];
    char token_buffer[TOKEN];
    char number_buffer[TOKEN];
    char lex_error_sequence[TOKEN];
    char *token_id;
    bool comment_flag = false;
    bool error_flag = false;
    bool process_symbol = false;
    bool process_token = false;
    bool process_number = false;

    //Variable Initialization
    i = 0;
    j = 0;
    k = 0;
    lexical_errors = 0;

    memset(symbol_buffer, 0, TOKEN);
    memset(token_buffer, 0, TOKEN);
    memset(number_buffer, 0, TOKEN);

    //Write the first line number to the listing file 
    fprintf(list_file, "%d   ", line_count);
    line_count += 1;

    while (lexeme != EOF) {
        lexeme = fgetc(input_file); 

        //Try to process comment
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

                //Space Lexeme  
                if (isspace(lexeme) != 0) {
                    //check the sumbol buffer
                    if (strlen(symbol_buffer) > 0) {
                        process_symbol = true;
                    }
                    //check the number buffer
                    if (strlen(number_buffer) > 0) {
                        process_number = true;
                    }
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
                }
            }
        }

        //Recognize the token
        if (process_token == true) {
            //Check Reserved word 
            token_num = check_reserved(token_buffer);
            token_id = token_ident(token_num);

            //Write to output file
            write_output(output_file, token_num, token_id, token_buffer);
            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, token_buffer);
                lexical_errors += 1;
                error_flag = true;
            }

            //Clear token buffer    
            memset(token_buffer, 0, TOKEN);
            i = 0;
            process_token = false;
        }
        //Recognize the number 
        if (process_number == true) {
            //Check Valid number 
            token_num = check_number(number_buffer);
            token_id = number_ident(token_num);

            //Write to output file 
            write_output(output_file, token_num, token_id, number_buffer);
            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, number_buffer);
                lexical_errors += 1;
                error_flag = true;
            }

            //Clear number buffer
            memset(number_buffer, 0, TOKEN);
            k = 0;
            process_number = false;
        }
        //Recognize the symbol
        if (process_symbol == true) {
            //Check Reserved Symbol
            token_num = check_symbol(symbol_buffer);
            token_id = symbol_ident(token_num);

            //Write to output file 
            write_output(output_file, token_num, token_id, symbol_buffer);
            //Check Error
            if (token_num == ERROR) {
                strcpy(lex_error_sequence, symbol_buffer);
                lexical_errors += 1;
                error_flag = true;
            }
            
            //Clear symbol buffer
            memset(symbol_buffer, 0, TOKEN);
            j = 0;
            process_symbol = false;
        }

        //Check if EOF 
        if (lexeme == EOF) {
            //Process End of file
            token_num = SCANEOF;
            token_id = token_ident(token_num);
            write_output(output_file, token_num, "SCANEOF", token_id);

            //Write Lexical Ettor line
            fprintf(list_file, "\n\n%d   Lexical Errors.", lexical_errors);
        }
        else {
            //Write to listing file 
            if (lexeme == '\n') {
                fputc(lexeme, list_file);

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
            else {
                fputc(lexeme, list_file);
            }
        }
    }

    return true;
}