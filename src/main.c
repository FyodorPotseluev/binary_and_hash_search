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
#include "file_ops_with_err_checks.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void entry_name_error_handling(
    const char *entry_name, const char *file, int line
)
{
    if (!entry_name) {
        printf(
            "%s:line %d\nError: Item name has not been specified\n", file, line
        );
        exit(1);
    }
    if (strlen(entry_name) >= max_entry_name_size) {
        printf("%s:line %d\nError: Too long entry name\n", file, line);
        exit(1);
    }
}

bool is_not_five(int num)
{
    return (num != 5) ? true : false;
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

void swap_files(file_data *dst, file_data *src)
{
    file_data tmp = { dst->fd, dst->name, dst->size, NULL };
    dst->fd = src->fd;
    src->fd = tmp.fd;
    dst->name = src->name;
    src->name = tmp.name;
    dst->size = src->size;
    src->size = tmp.size;
}

void add_entry(const char **argv)
{
    file_data file = { 0, argv[file_name_position], 0, NULL };
    const char *entry_name = argv[entry_name_position];
    entry_name_error_handling(entry_name, __FILE__, __LINE__);
    file.fd = open(file.name, O_RDWR);
    if (file.fd != -1) {
        /* file opening: success */
        file.size = get_file_size_err_checked(file.name, __FILE__, __LINE__);
        file.map =
            mmap_err_checked(&file, PROT_READ|PROT_WRITE, __FILE__, __LINE__);
#if defined(BIN)
        bool found = add_existing_entry_to_bin_file(&file, entry_name);
        if (found) goto file_close;
#endif
    } else {
        /* file opening: error */
        /* if the file we're trying to open exists */
        if (0 == access(file.name, F_OK))
            file_ops_error_handling(file.fd, file.name, __FILE__, __LINE__);
        file.fd = open(file.name, O_RDWR|O_CREAT, 0666);
        file_ops_error_handling(file.fd, file.name, __FILE__, __LINE__);
#if defined(HASH)
        /* truncate file to initial hash file size */
        /* mmap file */
        initializing_hash_file(&file, 0);
    }
    entry entry_to_add;
    strcpy(entry_to_add.str, entry_name);
    entry_to_add.data = 1;
    entry_action(to_write, &file, &entry_to_add);
#elif defined(BIN)
    }
    add_new_entry_to_bin_file(&file, entry_name);
    file_close:
#endif
    munmap_err_checked(file.map, file.size, __FILE__, __LINE__);
    close_err_checked(file.fd, file.name, __FILE__, __LINE__);
}

void print_entry(const char **argv)
{
    file_data file = { 0, argv[file_name_position], 0, NULL };
    const char *entry_name = argv[entry_name_position];
    file.fd = open(file.name, O_RDONLY);
    file_ops_error_handling(file.fd, file.name, __FILE__, __LINE__);
    entry_name_error_handling(entry_name, __FILE__, __LINE__);
    file.size = get_file_size_err_checked(file.name, __FILE__, __LINE__);
    file.map = mmap_err_checked(&file, PROT_READ, __FILE__, __LINE__);
#if defined(HASH)
    entry entry_to_print;
    strcpy(entry_to_print.str, entry_name);
    entry_action(to_print, &file, &entry_to_print);
#elif defined(BIN)
    entry *arr = file.map;
    long idx = find_entry_with_bin_search(&file, entry_name);
    if (idx != -1)
        printf("%s - %d\n", arr[idx].str, arr[idx].data);
    else
        printf("%s - 0\n", entry_name);
#endif
    munmap_err_checked(file.map, file.size, __FILE__, __LINE__);
    close_err_checked(file.fd, file.name, __FILE__, __LINE__);
}

void print_all_entries(const char **argv)
{
    file_data file = { 0, argv[file_name_position], 0, NULL };
    file.fd = open(file.name, O_RDONLY);
    file_ops_error_handling(file.fd, file.name, __FILE__, __LINE__);
    file.size = get_file_size_err_checked(file.name, __FILE__, __LINE__);
    file.map = mmap_err_checked(&file, PROT_READ, __FILE__, __LINE__);
#if defined(HASH)
    traverse_file_and_(to_print, NULL, &file);
#elif defined(BIN)
    entry *arr = file.map;
    unsigned long i;
    for (i=0; i < file.size / sizeof(entry); i++)
        printf("%s - %d\n", arr[i].str, arr[i].data);
#endif
    munmap_err_checked(file.map, file.size, __FILE__, __LINE__);
    close_err_checked(file.fd, file.name, __FILE__, __LINE__);
}

void merge_files(const char **argv)
{
    file_data dst = { 0, argv[dst_file_name_position], 0, NULL };
    file_data src = { 0, argv[src_file_name_position], 0, NULL };
    const char *final_file_name = argv[final_file_name_position];
    dst.fd = open(dst.name, O_RDWR);
    src.fd = open(src.name, O_RDWR);
    file_ops_error_handling(dst.fd, dst.name, __FILE__, __LINE__);
    file_ops_error_handling(src.fd, src.name, __FILE__, __LINE__);
    dst.size = get_file_size_err_checked(dst.name, __FILE__, __LINE__);
    src.size = get_file_size_err_checked(src.name, __FILE__, __LINE__);
    if (src.size > dst.size)
        swap_files(&dst, &src);
    dst.map = mmap_err_checked(&dst, PROT_READ|PROT_WRITE, __FILE__, __LINE__);
    src.map = mmap_err_checked(&src, PROT_READ|PROT_WRITE, __FILE__, __LINE__);
#if defined(HASH)
    traverse_file_and_(to_write, &dst, &src);
#elif defined(BIN)
    merge_bin_files(&dst, &src);
#endif
    munmap_err_checked(dst.map, dst.size, __FILE__, __LINE__);
    munmap_err_checked(src.map, src.size, __FILE__, __LINE__);
    close_err_checked(dst.fd, dst.name, __FILE__, __LINE__);
    close_err_checked(src.fd, src.name, __FILE__, __LINE__);
    unlink_err_checked(src.name, __FILE__, __LINE__);
    rename_err_checked(dst.name, final_file_name, __FILE__, __LINE__);
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
