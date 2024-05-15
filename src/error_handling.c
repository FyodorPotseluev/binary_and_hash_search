/* error_handling.c */

#include "error_handling.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

void file_open_error_handling(const FILE *file, const char *file_name)
{
    if (!file) {
        perror(file_name);
        exit(1);
    }
}

void entry_name_error_handling(const char *entry_name)
{
    if (!entry_name) {
        printf("Error: Item name has not been specified\n");
        exit(1);
    }
    if (strlen(entry_name) >= max_entry_name_size) {
        printf("Error: Too long entry name\n");
        exit(1);
    }
}

void read_error_handling(FILE *file, const char *file_name)
{
    if (ferror(file)) {
        perror(file_name);
        exit(1);
    }
}

void file_close_error_handling(int fclose_res, const char *file_name)
{
    if (fclose_res == -1) {
        printf(
            "Error: The \"%s\" file was not closed successfully",
            file_name
        );
        exit(1);
    }
}

bool is_less_than_three(int num)
{
    return (num < 3) ? true : false;
}

bool is_more_than_four(int num)
{
    return (num > 4) ? true : false;
}

bool is_not_three_and_is_not_four(int num)
{
    return ((num != 3) && (num != 4)) ? true : false;
}

void incorr_num_of_args_error_handling(int argc, bool (*condition)(int))
{
    if ((*condition)(argc)) {
        printf("Error: Incorrect number of arguments\n");
        exit(1);
    }
}
