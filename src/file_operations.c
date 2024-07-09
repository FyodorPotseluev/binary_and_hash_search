/* file_operations.c */

#include "file_operations.h"
#include "error_handling.h"
#include <stdlib.h>
#include <string.h>

int num_of_entries(FILE *file)
{
    int cur_file_pos, file_size_in_bytes;
    cur_file_pos = ftell_err_checked(file);
    fseek_err_checked(file, 0, SEEK_END);
    file_size_in_bytes = ftell_err_checked(file);
    fseek_err_checked(file, cur_file_pos, SEEK_SET);
    return(file_size_in_bytes / sizeof(entry));
}

static bool src_file_is_bigger(FILE *dst_file, FILE *src_file)
{
    if (num_of_entries(src_file) > num_of_entries(dst_file))
        return true;
    else
        return false;
}

void swap_files_if_src_file_larger(
    FILE **dst_file, FILE **src_file,
    const char **dst_file_name, const char **src_file_name
)
{
    if (src_file_is_bigger(*dst_file, *src_file)) {
        FILE *tmp = *dst_file;
        *dst_file = *src_file;
        *src_file = tmp;
        const char *tmp_str = *dst_file_name;
        *dst_file_name = *src_file_name;
        *src_file_name = tmp_str;
    }
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
    entry first, second;
    read_entry(&first, file, first_pos);
    read_entry(&second, file, second_pos);
    write_entry(&second, file, first_pos);
    write_entry(&first, file, second_pos);
}

void copy_entry(entry *dst, const entry *src)
{
    memcpy(dst->str, src->str, max_entry_name_size);
    dst->data = src->data;
}

void add_new_entry(FILE *file, const char *entry_name)
{
    entry buffer;
    memset(&buffer, 0, sizeof(entry));
    fseek_err_checked(file, 0, SEEK_END);
    strcpy(buffer.str, entry_name);
    buffer.data = 1;
    fwrite_err_checked(&buffer, 1, sizeof(entry), file);
}

void increment_entry(FILE *file, int file_pos)
{
    entry buffer;
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fread_err_checked(&buffer, sizeof(entry), 1, file);
    buffer.data++;
    fseek_err_checked(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite_err_checked(&buffer, sizeof(entry), 1, file);
}
