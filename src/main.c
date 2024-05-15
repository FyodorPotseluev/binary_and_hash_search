/* main.c */

#include "bin_sort_and_search.h"
#include "constants.h"
#include "error_handling.h"
#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_entry(char *const *argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    FILE *file = fopen(file_name, "r+");
    int file_pos = 0;
    entry_name_error_handling(entry_name);
    if (file) {
        file_pos = find_entry_with_binary_search(file, file_name, entry_name);
        if (file_pos != -1) {
            /* entry was found */
            increment_entry(file, file_pos);
            goto file_close;
        }
        /* entry was not found */
    } else {
        /* if there was an error trying to open existing file: */
        if (0 == access(file_name, F_OK))
            file_open_error_handling(file, file_name);
        file = fopen(file_name, "w");
        file_open_error_handling(file, file_name);
    }
    make_new_entry(file, entry_name);
    quick_sort_hoare(file);
    file_close: ;
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

void print_entry(char *const *argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    entry *read_res = malloc(sizeof(entry));
    FILE *file = fopen(file_name, "r");
    file_open_error_handling(file, file_name);
    entry_name_error_handling(entry_name);
    int file_pos = find_entry_with_binary_search(file, file_name, entry_name);
    if (file_pos != -1) {
        fseek(file, file_pos*sizeof(entry), SEEK_SET);
        fread(read_res, sizeof(entry), 1, file);
        printf("%s - %d\n", entry_name, read_res->data);
    } else
        printf("%s - 0\n", entry_name);
    free(read_res);
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

void print_all_entries(char *const *argv)
{
    const char *file_name = argv[file_position];
    entry *read_res = malloc(sizeof(entry));
    FILE *file = fopen(file_name, "r");
    file_open_error_handling(file, file_name);
    while (fread(read_res, 1, sizeof(entry), file)) {
        printf("%s - %d\n", read_res->str, read_res->data);
    }
    free(read_res);
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

int main(int argc, char **argv)
{
    incorr_num_of_args_error_handling(argc, &is_less_than_three);
    if (0 == strncmp(argv[cmd_position], "add", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_more_than_four);
        add_entry(argv);
    } else
    if (0 == strncmp(argv[cmd_position], "query", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_more_than_four);
        print_entry(argv);
    } else
    if (0 == strncmp(argv[cmd_position], "list", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_not_three_and_is_not_four);
        print_all_entries(argv);
    } else {
        printf("Error: Incorrect command\n");
        return 1;
    }
    return 0;
}
