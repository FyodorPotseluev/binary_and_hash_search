/* error_handling.h */

/* Contains functions that check and handle possible errors */

#ifndef ERROR_HANDLING_H_INCLUDED
#define ERROR_HANDLING_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

void incorr_num_of_args_error_handling(int argc, bool (*condition)(int));
/*
    Checks whether the number of command line arguments is correct. If it's not
prints error message and ends the program with the error code 1.
RECEIVES:
    - `argc` the number of command-line arguments passed by the user including
    the name of the program;
    - `condition` function pointer. Function calculates whether the number of
    command-line arguments is correct.
    RECEIVES:
        - a number;
    RETURNES:
        - boolean value depending on whether the argument matches the function
        condition.
RETURNES:
    --- */

bool is_less_than_three(int num);
/*
    Checks if a number is less than three. Returns the corresponding boolean
value. The function pointer is used by `incorr_num_of_args_error_handling`
function.
RECEIVES:
    - `num` integer value;
RETURNES:
    - boolean value.*/

bool is_more_than_four(int num);
/*
    Checks if a number is more than four. Returns the corresponding boolean
value. The function pointer is used by `incorr_num_of_args_error_handling`
function.
RECEIVES:
    - `num` integer value;
RETURNES:
    - boolean value.*/

bool is_not_three_and_is_not_four(int num);
/*
    Checks if a number is not three and is not four. Returns the corresponding
boolean value. The function pointer is used by
`incorr_num_of_args_error_handling` function.
RECEIVES:
    - `num` integer value;
RETURNES:
    - boolean value.*/

void file_open_error_handling(const FILE *file, const char *file_name);
/*
    Checks if an error occurred while opening the file. If it did - prints the
corresponding error message and ends the program with the error code 1.
RECEIVES:
    - `file` file pointer;
    - `file_name` a string containing the file name to include it in an error
    message.
RETURNES:
    --- */

void entry_name_error_handling(const char *entry_name);
/*
    Checks whether the entry name (id) is correct (not empty and not exceeding
the maximum string size for the entry name). If it's not - prints the
corresponding error message and ends the program with the error code 1.
RECEIVES:
    - `entry_name` a string containing the entry name.
RETURNES:
    --- */

void read_error_handling(FILE *file, const char *file_name);
/*
    Determines, if we can no longer read information from the file, whether an
error has occured. If it did - prints the corresponding error message and ends
the program with the error code 1.
RECEIVES:
    - `file` file pointer;
    - `file_name` a string containing the file name to include it in an error
    message.
RETURNES:
    --- */

void file_close_error_handling(int fclose_res, const char *file_name);
/*
    Checks if an error occurred while closing the file. If it did - prints the
corresponding error message and ends the program with the error code 1.
RECEIVES:
    - `fclose_res` file pointer;
    - `file_name` a string containing the file name to include it in an error
    message.
RETURNES:
    --- */

#endif
