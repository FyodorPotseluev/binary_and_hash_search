/* file_operations.c */

#include "file_operations.h"
#include <stdlib.h>
#include <string.h>

int size_of_file(FILE *file)
{
    /* this function sets the file position at the end of file! */
    fseek(file, 0, SEEK_END);
    return(ftell(file) / sizeof(entry));
}

void read_entry(entry *read_res, FILE *file, int file_pos)
{
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fread(read_res, sizeof(entry), 1, file);
}

void write_entry(const entry *to_write, FILE *file, int file_pos)
{
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite(to_write, sizeof(entry), 1, file);
}

void swap_entries(FILE *file, int first_pos, int second_pos)
{
    entry *first = malloc(sizeof(entry));
    entry *second = malloc(sizeof(entry));
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
    entry *buffer = calloc(sizeof(entry), 1);
    fseek(file, 0, SEEK_END);
    strcpy(buffer->str, entry_name);
    buffer->data = 1;
    fwrite(buffer, 1, sizeof(entry), file);
    free(buffer);
}

void increment_entry(FILE *file, int file_pos)
{
    entry *buffer = malloc(sizeof(entry));
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fread(buffer, sizeof(entry), 1, file);
    buffer->data += 1;
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite(buffer, sizeof(entry), 1, file);
    free(buffer);
}
