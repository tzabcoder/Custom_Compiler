/******************************************************************************
FILENAME:   file_util.c
DESCRIPTION:
    Contains all subroutiones to support the main process routine. File has all 
    file utility functions to handle and support file operations. 
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

void copy_file_contents(FILE *in, FILE *out) {
    char c;

    c = getc(in);
    while (c != EOF) {
        fputc(c, out);
        c = getc(in);
    }
}

bool process_tempfile() {
    if ((temp_file = fopen(TEMP_FILE, WRITE_FILE_MODE))) {
        return true;
    }
    else {
        return false;
    }
}

//Process the input file 
bool process_input(char *infile) {
    bool valid_flag = false;
    bool proc_status;
    char file[FSIZE];

    //Proc Status:
    //0. Valid and input file opened properly
    //1. User quits program

    //Check if there was an infile provided 
    if (strlen(infile) == 0) {
        printf("Enter an input file name: ");
        scanf("%s", file);
    }
    else {
        strcpy(file, infile);
    }

    do {
        //Check if the file has extension
        if (strstr(file, ".") == 0) {
            strcat(file, INPUT_EXTENSION);
        }

        input_file = fopen(file, READ_FILE_MODE);
        if (input_file == NULL) {
            printf("Input file does not exist...\n");
            printf("Enter a new input file name: ");
            scanf("%s", file);

            if (strcmp(file, "q") == 0) {
                valid_flag = true;
                proc_status = false;
            }
        }
        else {
            valid_flag = true;
            proc_status = true;
        }
    } while (valid_flag == false);

    return proc_status;
}

bool process_output(char *outfile) {
    FILE *temp;
    bool valid_flag = false;
    bool proc_status;
    int prompt = 1;
    int choice;
    char file[FSIZE];

    //Proc Status:
    //0. Valid and input file opened properly
    //1. User quits program

    if (strlen(outfile) == 0) {
        printf("Enter an output file name: ");
        scanf("%s", file);
    }
    else {
        strcpy(file, outfile);
    }

    do {
        //Check if there is a file extension
        if (strstr(file, ".") == 0) {
            strcat(file, OUTPUT_EXTENSION);
        }

        if ((output_file = fopen(file, READ_FILE_MODE))) {
            prompt = 1;
            printf("Output file already exists...\n");
            while (prompt) {
                //User prompt for an existing output file
                printf("Enter the number.\n");
                printf("1.) Overwrite Existing File\n");
                printf("2.) Enter New Filename\n");
                printf("3.) Quit Program\n");
                printf("->");
                scanf("%d", &choice);
                fflush(stdin);  

                switch (choice) {
                    case 1:
                        //Process output file
                        strcat(file, BACKUP_EXTENSION);
                        temp = fopen(file, WRITE_FILE_MODE);
                        copy_file_contents(output_file, temp);
                        fclose(temp);
                        fclose(output_file);
                        //Open output file in writing mode
                        file[strlen(file) - 4] = 0;
                        output_file = fopen(file, WRITE_FILE_MODE);

                        //Process listing file 
                        printf("%s\n", file);
                        file[strlen(file) - 4] = 0;
                        strcat(file, LISTING_EXTENSION);
                        list_file = fopen(file, READ_FILE_MODE);
                        strcat(file, BACKUP_EXTENSION);
                        temp = fopen(file, WRITE_FILE_MODE);
                        copy_file_contents(list_file, temp);
                        fclose(temp);
                        fclose(list_file);
                        //Open listing file in writing mode 
                        file[strlen(file) - 4] = 0;
                        list_file = fopen(file, WRITE_FILE_MODE);
                        
                        prompt = 0;
                        valid_flag = true;
                        proc_status = true;
                        break;
                    case 2:
                        printf("Enter a new file name: ");
                        scanf("%s", file);

                        prompt = 0;
                        break;
                    case 3:
                        printf("Goodbye.\n");
                        valid_flag = true;
                        proc_status = false;
                        prompt = 0;
                        break;
                    default:
                        printf("Invalid choice...\n");
                        break;
                }
            }
        }
        else {
            //open output file
            output_file = fopen(file, WRITE_FILE_MODE);

            //open listing file
            file[strlen(file) - 4] = 0;
            strcat(file, LISTING_EXTENSION);
            list_file = fopen(file, WRITE_FILE_MODE);
            valid_flag = true;
            proc_status = true;
        }
    } while(valid_flag == false);

    return proc_status;
}

bool init_files(int argc, char *argv[]) {
    //File Process Flags 
    bool input_flag = false;
    bool output_flag = false;

    input_pos = 0;
    line_count = 1;
    lexical_errors = 0;
    syntax_errors = 0;
    i = 0;
    j = 0;
    k = 0;
    error_flag = false;
    process_token = false;
    process_symbol = false;
    process_number = false;

    //Set Buffers
    memset(token_buffer, 0, TOKEN);
    memset(symbol_buffer, 0, TOKEN);
    memset(number_buffer, 0, TOKEN);
    memset(lex_error_sequence, 0, TOKEN);

    //Process the number of arguements 
    if (argc == 1) {
        input_flag = process_input("\0");
        output_flag = process_output("\0");
    }
    else if (argc == 2) {
        input_flag = process_input(argv[1]);
        output_flag = process_output("\0");
    }
    else if (argc == 3) {
        input_flag = process_input(argv[1]);
        output_flag = process_output(argv[2]);
    }
    else {
        input_flag = process_input(argv[1]);
        output_flag = process_output(argv[2]);
    }

    //Validate files were processed successfully
    if (input_flag == true && output_flag == true) {
        //If both files are processed, open temp file
        bool temp_flag = process_tempfile();

        //Write the first line number to the listing file 
        fprintf(list_file, "%d   ", line_count);
        line_count += 1;

        if (temp_flag == true) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

//Write to output file
void write_output(FILE *out_file, char* token_id, char* token_buffer) {
    int w[2] = {25, 25};

    //Write in formatted output 
    fprintf(out_file, "Expected Token:    %-*s", w[0], token_id);
    fprintf(out_file, "Actual Token:    %-*s\n", w[1], token_buffer);
}

bool wrap_up() {
    //Check if files closed properly
    if (fclose(input_file) && fclose(output_file) && fclose(list_file) && fclose(temp_file)) {
        return true;
    }
    else {
        return false;
    }
}