/* hash_sort_and_search.c */

#include "hash_sort_and_search.h"
#include "file_ops_with_err_checks.h"
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

/* the next primes after the numbers 8, 16, 32, 64 etc. */
static const unsigned int hash_table_size[] = {
    11,        17,        37,        67,         131,        257,
    521,       1031,      2053,      4099,       8209,       16411,
    32771,     65537,     131101,    262147,     524309,     1048583,
    2097169,   4194319,   8388617,   16777259,   33554467,   67108879,
    134217757, 268435459, 536870923, 1073741827
};

static void idx_increment(unsigned int *idx, unsigned int table_size)
{
    (*idx)++;
    if (*idx == table_size)
        *idx = 0;
}

static void increment_curr_num_of_entries_in_file(void *arr)
{
    unsigned int *curr_num_of_entries =
        (unsigned int *)((char *)arr - num_of_metadata_bytes);
    (*curr_num_of_entries)++;
}

static void handle_entry(
    to_do action, entry *arr, unsigned int idx, entry *search_entry
)
{
    switch (action) {
        case to_write:
            if (arr[idx].data == 0) {
                strcpy(arr[idx].str, search_entry->str);
                increment_curr_num_of_entries_in_file(arr);
            }
            arr[idx].data += search_entry->data;
            break;
        case to_print:
            printf("%s - %d\n", search_entry->str, arr[idx].data);
    }
}

static void find_entry_and_(
    to_do action, file_data *file, entry *search_entry,
    unsigned int idx, unsigned int size_idx
)
{
    entry *arr = (entry *)((char *)file->map + num_of_metadata_bytes);
    for (;;) {
        /* find idx entry in the file */
        /* is it free? */
        if (arr[idx].str[0] != '\0') {
            /* it is not free */
            /* is it the entry we're looking for? */
            if (0 == strcmp(search_entry->str, arr[idx].str)) {
                /* it is */
                handle_entry(action, arr, idx, search_entry);
                break;
            } else {
                /* it isn't */
                idx_increment(&idx, hash_table_size[size_idx]);
                continue;
            }
        } else {
            /* it is free */
            handle_entry(action, arr, idx, search_entry);
            break;
        }
    }
}

static unsigned int get_hash(const char *str)
{
    unsigned int hash = 0;
    int i, j, len = strlen(str);
    for (i=0, j=1; i < len; i++, j++)
        hash += (unsigned int)str[i] * (unsigned int)pow(31, len-j);
    return hash;
}

static unsigned int get_idx(const char *str, unsigned int hash_table_size)
{
    return get_hash(str) % hash_table_size;
}

void traverse_file_and_(to_do action, file_data *dst, file_data *src);

void initializing_hash_file(file_data *file, unsigned int idx)
{
    file->size = num_of_metadata_bytes + hash_table_size[idx]*sizeof(entry);
    /* sets the initial file size and fills it with 0 bytes */
    truncate_err_checked(file->name, file->size, __FILE__, __LINE__);
    file->map = mmap_err_checked(file, PROT_READ|PROT_WRITE, __FILE__,__LINE__);
    unsigned int *arr = file->map;
    arr[1] = idx;
}

static void rebuild(file_data *old_file, unsigned int size_idx)
{
    file_data new_file = { 0, "new_file", 0, NULL };
    new_file.fd = open(new_file.name, O_RDWR|O_CREAT, 0666);
    file_ops_error_handling(new_file.fd, new_file.name, __FILE__, __LINE__);
    initializing_hash_file(&new_file, size_idx);
    traverse_file_and_(to_write, &new_file, old_file);
    munmap_err_checked(old_file->map, old_file->size, __FILE__, __LINE__);
    close_err_checked(old_file->fd, old_file->name, __FILE__, __LINE__);
    unlink_err_checked(old_file->name, __FILE__, __LINE__);
    rename_err_checked("new_file", old_file->name, __FILE__, __LINE__);
    old_file->fd = new_file.fd;
    old_file->size = new_file.size;
    old_file->map = new_file.map;
}

static void do_we_rebuild_hash_file(
    file_data *file, unsigned int curr_num_of_entries, unsigned int *size_idx
)
{
    /* to rebuild, the table must be more than 2/3 full */
    if (curr_num_of_entries*3 > hash_table_size[*size_idx]*2) {
        (*size_idx)++;
        rebuild(file, *size_idx);
    }
}

static void get_init_bytes(
    file_data *file, unsigned int *curr_num_of_entries, unsigned int *idx
)
{
    unsigned int *arr = file->map;
    if (curr_num_of_entries)
        *curr_num_of_entries = arr[0];
    *idx = arr[1];
}

void entry_action(to_do action, file_data *file, entry *search_entry)
{
    unsigned int idx;
    unsigned int curr_num_of_entries, size_idx;
    get_init_bytes(file, &curr_num_of_entries, &size_idx);
    if (action == to_write)
        do_we_rebuild_hash_file(file, curr_num_of_entries, &size_idx);
    idx = get_idx(search_entry->str, hash_table_size[size_idx]);
    find_entry_and_(action, file, search_entry, idx, size_idx);
}

void traverse_file_and_(to_do action, file_data *dst, file_data *src)
{
    entry *arr = (entry *)((char *)src->map + num_of_metadata_bytes);
    unsigned int i;
    unsigned int size_idx;
    get_init_bytes(src, NULL, &size_idx);
    for (i=0; i < hash_table_size[size_idx]; i++) {
        /* read the next entry from the `src_file` */
        /* if it's empty - continue */
        if (arr[i].str[0] == '\0')
            continue;
        if (action == to_print)
            printf("%s - %d\n", arr[i].str, arr[i].data);
        else
        if (action == to_write)
            entry_action(action, dst, &arr[i]);
    }
}
