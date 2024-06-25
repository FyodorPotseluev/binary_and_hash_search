/* main.c */

#if !defined(BIN) && !defined(HASH)
#error Please define either BIN or HASH
#endif

#if defined(BIN)
#include "bin_sort_and_search.h"
#elif defined(HASH)
#include "hash_sort_and_search.h"
#endif

#include "constants.h"
#include "error_handling.h"
#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_entry(const char **argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    FILE *file = fopen(file_name, "r+");
    entry_name_error_handling(entry_name);
    if (file) {
#if defined(BIN)
        int file_pos = find_entry_with_binary_search(file, entry_name);
        if (file_pos != -1) {
            /* entry was found */
            increment_entry(file, file_pos);
            goto file_close;
        }
        /* entry was not found */
#endif
    } else {
        /* if there was an error trying to open existing file: */
        if (0 == access(file_name, F_OK))
            file_open_error_handling(file, file_name);
        file = fopen_err_checked(file_name, "w+");
#if defined(HASH)
        initializing_hash_file(file_name);
    }
    add_entry_to_hash_file(&file, file_name, entry_name);
#elif defined(BIN)
    }
    make_new_entry(file, entry_name);
    quick_sort_hoare(file);
    file_close: ;
#endif
    fclose_err_checked(file);
}

void print_entry(const char **argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    FILE *file = fopen_err_checked(file_name, "r");
    entry_name_error_handling(entry_name);
#if defined(HASH)
    print_hash_entry(file, entry_name);
#elif defined(BIN)
    entry *read_res = malloc_err_checked(sizeof(entry));
    int file_pos = find_entry_with_binary_search(file, entry_name);
    if (file_pos != -1) {
        fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
        fread_err_checked(read_res, sizeof(entry), 1, file);
        printf("%s - %d\n", entry_name, read_res->data);
    } else
        printf("%s - 0\n", entry_name);
    free(read_res);
#endif
    fclose_err_checked(file);
}

void print_all_entries(const char **argv)
{
    const char *file_name = argv[file_position];
    FILE *file = fopen_err_checked(file_name, "r");
#if defined(HASH)
    print_hash_file(file);
#else
    entry *read_res = malloc_err_checked(sizeof(entry));
    while (
        fread_err_checked(read_res, 1, sizeof(entry), file)
    )
    {
        printf("%s - %d\n", read_res->str, read_res->data);
    }
    free(read_res);
#endif
    fclose_err_checked(file);
}

int main(int argc, const char **argv)
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
