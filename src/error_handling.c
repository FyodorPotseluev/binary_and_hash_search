/* error_handling.c */

#include "error_handling.h"
#include "constants.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void fwrite_err_checked(
    const void *buffer, size_t item_size, size_t items_num, FILE *file
)
{
    size_t items_written = fwrite(buffer, item_size, items_num, file);
    if (items_written != items_num) {
        perror("fwrite failed");
        exit(1);
    }
}

size_t fread_err_checked(
    void *buffer, size_t item_size, size_t items_num, FILE *file
)
{
    size_t items_read = fread(buffer, item_size, items_num, file);
    if (items_read != items_num &&
        (int)items_read != EOF &&
        0 != strcmp(strerror(errno), "Success"))
    {
        perror("fread failed");
        exit(1);
    }
    return items_read;
}

void fseek_err_checked(FILE *file, long offset, int whence)
{
    int res = fseek(file, offset, whence);
    if (res != 0) {
        perror("fseek failed");
        exit(1);
    }
}

long ftell_err_checked(FILE *file)
{
    long pos = ftell(file);
    if (pos == -1) {
        perror("ftell failed");
        exit(1);
    }
    return pos;
}

void *malloc_err_checked(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        perror("malloc failed");
        exit(1);
    }
    return ptr;
}

void *calloc_err_checked(size_t arr_size, size_t element_size)
{
    void *ptr = calloc(arr_size, element_size);
    if (!ptr) {
        perror("calloc failed");
        exit(1);
    }
    return ptr;
}

void truncate_err_checked(const char *file_name, off_t length)
{
    int trunc_res = truncate(file_name, length);
    if (trunc_res == -1) {
        perror("truncate failed");
        exit(1);
    }
}

void remove_err_checked(const char *file_name)
{
    int res = remove(file_name);
    if (res != 0) {
        perror(file_name);
        exit(1);
    }
}

void rename_err_checked(const char *old_file_name, const char *new_file_name)
{
    int res = rename(old_file_name, new_file_name);
    if (res != 0) {
        perror(old_file_name);
        exit(1);
    }
}

void file_open_error_handling(FILE *file, const char *file_name)
{
    if (!file) {
        perror(file_name);
        exit(1);
    }
}

FILE *fopen_err_checked(const char *path_name, const char *mode)
{
    FILE *file = fopen(path_name, mode);
    file_open_error_handling(file, path_name);
    return(file);
}

void fclose_err_checked(FILE *file)
{
    int fclose_res = fclose(file);
    if (fclose_res == -1) {
        perror("fclose failed");
        exit(1);
    }
}
