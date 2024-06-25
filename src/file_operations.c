/* file_operations.c */

#include "file_operations.h"
#include "error_handling.h"
#include <stdlib.h>
#include <string.h>

int size_of_file(FILE *file)
{
    int cur_file_pos, file_size_in_bytes;
    cur_file_pos = ftell_err_checked(file);
    fseek_err_checked(file, 0, SEEK_END);
    file_size_in_bytes = ftell_err_checked(file);
    fseek_err_checked(file, cur_file_pos, SEEK_SET);
    return(file_size_in_bytes / sizeof(entry));
}

void read_entry(entry *read_res, FILE *file, int file_pos)
{
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fread_err_checked(read_res, sizeof(entry), 1, file);
}

void write_entry(const entry *to_write, FILE *file, int file_pos)
{
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite_err_checked(to_write, sizeof(entry), 1, file);
}

void swap_entries(FILE *file, int first_pos, int second_pos)
{
    entry *first = malloc_err_checked(sizeof(entry));
    entry *second = malloc_err_checked(sizeof(entry));
    read_entry(first, file, first_pos);
    read_entry(second, file, second_pos);
    write_entry(second, file, first_pos);
    write_entry(first, file, second_pos);
    free(first);
    free(second);
}

void copy_entry(entry *dst, const entry *src)
{
    memcpy(dst->str, src->str, max_entry_name_size);
    dst->data = src->data;
}

void make_new_entry(FILE *file, const char *entry_name)
{
    entry *buffer = calloc_err_checked(sizeof(entry), 1);
    fseek_err_checked(file, 0, SEEK_END);
    strcpy(buffer->str, entry_name);
    buffer->data = 1;
    fwrite_err_checked(buffer, 1, sizeof(entry), file);
    free(buffer);
}

void increment_entry(FILE *file, int file_pos)
{
    entry *buffer = malloc_err_checked(sizeof(entry));
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fread_err_checked(buffer, sizeof(entry), 1, file);
    buffer->data += 1;
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite_err_checked(buffer, sizeof(entry), 1, file);
    free(buffer);
}
