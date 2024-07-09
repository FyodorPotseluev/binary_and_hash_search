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
        bool found = add_existing_entry_to_bin_file(file, entry_name);
        if (found) goto file_close;
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
    add_new_entry_to_bin_file(file, entry_name);
    file_close:
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
    entry read_res;
    int file_pos = find_entry_with_binary_search(file, entry_name);
    if (file_pos != -1) {
        read_entry(&read_res, file, file_pos);
        printf("%s - %d\n", entry_name, read_res.data);
    } else
        printf("%s - 0\n", entry_name);
#endif
    fclose_err_checked(file);
}

void print_all_entries(const char **argv)
{
    const char *file_name = argv[file_position];
    FILE *file = fopen_err_checked(file_name, "r");
#if defined(HASH)
    print_hash_file(file);
#elif defined(BIN)
    entry read_res;
    while (
        fread_err_checked(&read_res, 1, sizeof(entry), file)
    )
    {
        printf("%s - %d\n", read_res.str, read_res.data);
    }
#endif
    fclose_err_checked(file);
}

void merge_files(const char **argv)
{
    const char *dst_file_name = argv[dst_file_position];
    const char *src_file_name = argv[src_file_position];
    const char *final_file_name = argv[final_file_position];
    FILE *dst_file = fopen_err_checked(dst_file_name, "r+");
    FILE *src_file = fopen_err_checked(src_file_name, "r+");
    swap_files_if_src_file_larger(
        &dst_file, &src_file, &dst_file_name, &src_file_name
    );
#if defined(HASH)
    merge_hash_files_with_possible_rebuilding(
        &dst_file, dst_file_name, src_file
    );
#elif defined(BIN)
    merge_bin_files(dst_file, src_file);
#endif
    fclose_err_checked(dst_file);
    fclose_err_checked(src_file);
    remove_err_checked(src_file_name);
    rename_err_checked(dst_file_name, final_file_name);
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
    } else
    if (0 == strncmp(argv[merge_cmd_position], "merge", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_not_five);
        merge_files(argv);
    } else {
        printf("Error: Incorrect command\n");
        return 1;
    }
    return 0;
}
