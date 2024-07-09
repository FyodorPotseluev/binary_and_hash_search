/* hash_sort_and_search.c */

#include "hash_sort_and_search.h"
#include "constants.h"
#include "error_handling.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* initialization in `transfer_with_possible_rebuilding` function;
used in `merge_hash_files_with_possible_rebuilding` => `traverse_file_and` =>
=> `execute_action` => `rebuild_if_needed_and_transfer_entry_to` =>
`do_we_rebuild_hash_file` */
static const char *module_global_var_file_name = NULL;

/* the next primes after the numbers 8, 16, 32, 64 etc. */
static const unsigned int hash_table_size[] = {
    11,        17,        37,        67,         131,        257,
    521,       1031,      2053,      4099,       8209,       16411,
    32771,     65537,     131101,    262147,     524309,     1048583,
    2097169,   4194319,   8388617,   16777259,   33554467,   67108879,
    134217757, 268435459, 536870923, 1073741827
};

typedef enum tag_find_idx_pos_action {
    write_entry, print_entry
} find_idx_pos_action;

typedef enum tag_traverse_action {
    rebuild_if_needed_and_transfer_entries_to_from,
    transfer_all_entries_to_from,
    print_all_entries
} traverse_action;

/* the flag indicating whether we have to increment the first 4 bytes of the
file containing the information about the current number of entries in the hash file */
typedef enum tag_byte_inc_flag {
    dont_inc_num_of_entries_byte, inc_num_of_entries_byte
} byte_inc_flag;

void initializing_hash_file(const char *file_name)
{
    /* sets the initial file size and fills it with 0 bytes */
    truncate_err_checked(
        file_name, init_bytes_num + hash_table_size[0]*sizeof(entry)
    );
}

static void idx_increment(unsigned int *idx, unsigned int table_size)
{
    (*idx)++;
    if (*idx == table_size)
        *idx = 0;
}

static void inc_curr_num_of_entries_in_file(FILE *file)
{
    unsigned int curr_num_of_entries;
    fseek_err_checked(file, 0, SEEK_SET);
    fread_err_checked(&curr_num_of_entries, sizeof(unsigned int), 1, file);
    curr_num_of_entries++;
    fseek_err_checked(file, 0, SEEK_SET);
    fwrite_err_checked(&curr_num_of_entries, sizeof(unsigned int), 1, file);
}

static void writing_entry(FILE *file, const entry *entry_to_write)
{
    fseek_err_checked(file, -sizeof(entry), SEEK_CUR);
    fwrite_err_checked(entry_to_write, sizeof(entry), 1, file);
}

static void handle_free_entry(
    find_idx_pos_action action, const entry *new_entry,
    FILE *file, byte_inc_flag inc_entries_byte,
    unsigned int *curr_num_of_entries
)
{
    switch (action) {
        case write_entry:
            writing_entry(file, new_entry);
            if (inc_entries_byte) {
                inc_curr_num_of_entries_in_file(file);
                if (curr_num_of_entries) (*curr_num_of_entries)++;
            }
            break;
        case print_entry:
            printf("%s - %d\n", new_entry->str, new_entry->data);
    }
}

static void handle_found_entry(
    find_idx_pos_action action, entry *found_entry, FILE *file
)
{
    switch (action) {
        case write_entry:
            (found_entry->data)++;
            writing_entry(file, found_entry);
            break;
        case print_entry:
            printf("%s - %d\n", found_entry->str, found_entry->data);
    }
}

static void find_idx_position_in_file_and(
    find_idx_pos_action action, FILE *file, const entry *new_entry,
    unsigned int idx, unsigned int *curr_num_of_entries, unsigned int size_idx,
    byte_inc_flag inc_entries_byte
)
{
    entry idx_entry;
    for (;;) {
        /* find idx entry in the file */
        fseek_err_checked(file, init_bytes_num + idx*sizeof(entry), SEEK_SET);
        fread_err_checked(&idx_entry, sizeof(entry), 1, file);
        /* is it free? */
        if (idx_entry.str[0] != '\0') {
            /* it is not free */
            /* is it the entry we're looking for? */
            if (0 == strcmp(new_entry->str, idx_entry.str)) {
                /* it is */
                handle_found_entry(action, &idx_entry, file);
                break;
            }
            idx_increment(&idx, hash_table_size[size_idx]);
            continue;
        }
        /* it is free */
        handle_free_entry(
            action, new_entry, file, inc_entries_byte, curr_num_of_entries
        );
        break;
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

static void get_init_bytes(
    FILE *file, unsigned int *curr_num_of_entries, unsigned int *idx
)
{
    fseek_err_checked(file, 0, SEEK_SET);
    fread_err_checked(curr_num_of_entries, sizeof(unsigned int), 1, file);
    fread_err_checked(idx, sizeof(unsigned int), 1, file);
}

static void prepare_dst_file(
    FILE *new_file, FILE *old_file, unsigned int *size_idx
)
{
    int old_file_pos = ftell_err_checked(old_file);
    unsigned int curr_num_of_entries;
    get_init_bytes(old_file, &curr_num_of_entries, size_idx);
    (*size_idx)++;
    /* increase `new_file` size */
    truncate_err_checked(
        "new_file", init_bytes_num + hash_table_size[*size_idx]*sizeof(entry)
    );
    /* write initial bytes */
    fwrite_err_checked(&curr_num_of_entries, sizeof(unsigned int), 1, new_file);
    fwrite_err_checked(size_idx, sizeof(unsigned int), 1, new_file);
    /* return old_file pointer to the initial positon */
    fseek_err_checked(old_file, old_file_pos, SEEK_SET);
}

static void get_idx_then_find_idx_position_in_file_and(
        find_idx_pos_action action, FILE *dst_file, const entry *src_file_entry,
        unsigned int *curr_num_of_entries, unsigned int size_idx,
        byte_inc_flag inc_entries_byte
)
{
    unsigned int idx = get_idx(src_file_entry->str, hash_table_size[size_idx]);
    find_idx_position_in_file_and(
        action, dst_file, src_file_entry, idx,
        curr_num_of_entries, size_idx, inc_entries_byte
    );
}

static void transfer_entry_to_from(
    FILE *dst_file, FILE *src_file,
    const entry *src_file_entry, bool first_iteration,
    byte_inc_flag inc_entries_byte, unsigned int *curr_num_of_entries
)
{
    static unsigned int size_idx;
    if (first_iteration)
        prepare_dst_file(dst_file, src_file, &size_idx);
    get_idx_then_find_idx_position_in_file_and(
        write_entry, dst_file, src_file_entry,
        curr_num_of_entries, size_idx, inc_entries_byte
    );
}

static void do_we_rebuild_hash_file(
    FILE **file, const char *file_name,
    unsigned int curr_num_of_entries, unsigned int *size_idx
);

static void rebuild_if_needed_and_transfer_entry_to(
    FILE **dst_file, const entry *src_file_entry, bool first_iteration
)
{
    static unsigned int curr_num_of_entries, size_idx;
    if (first_iteration)
        get_init_bytes(*dst_file, &curr_num_of_entries, &size_idx);
    do_we_rebuild_hash_file(
        dst_file, module_global_var_file_name, curr_num_of_entries, &size_idx
    );
    get_idx_then_find_idx_position_in_file_and(
        write_entry, *dst_file, src_file_entry,
        &curr_num_of_entries, size_idx, true
    );
}

static void execute_action(
    traverse_action action, FILE **dst_file, FILE *src_file,
    const entry *src_file_entry, bool first_iteration
)
{
    switch (action) {
        case rebuild_if_needed_and_transfer_entries_to_from:
            rebuild_if_needed_and_transfer_entry_to(
                dst_file, src_file_entry, first_iteration
            );
            break;
        case transfer_all_entries_to_from:
            transfer_entry_to_from(
                    *dst_file, src_file, src_file_entry, first_iteration,
                    dont_inc_num_of_entries_byte, NULL
            );
            break;
        case print_all_entries:
            printf("%s - %d\n", src_file_entry->str, src_file_entry->data);
    }
}

static void traverse_file_and(
    traverse_action action, FILE **dst_file, FILE *src_file
)
{
    bool first_iteration = true;
    unsigned int size_idx, i;
    fseek_err_checked(src_file, sizeof(unsigned int), SEEK_SET);
    fread_err_checked(&size_idx, sizeof(unsigned int), 1, src_file);
    entry src_file_entry;
    for (i=0; i < hash_table_size[size_idx]; i++) {
        /* read the next entry from the `src_file` */
        fread_err_checked(&src_file_entry, sizeof(entry), 1, src_file);
        /* if it's empty - continue */
        if (src_file_entry.str[0] == '\0') continue;
        execute_action(
            action, dst_file, src_file, &src_file_entry, first_iteration
        );
        first_iteration = false;
    }
}

static FILE *rebuild(FILE *old_file, const char *old_file_name)
{
    FILE *new_file = fopen_err_checked("new_file", "w+");
    traverse_file_and(transfer_all_entries_to_from, &new_file, old_file);
    /* remove old file */
    remove_err_checked(old_file_name);
    /* cange `new_file`'s name to `old_file`'s name */
    rename_err_checked("new_file", old_file_name);
    return new_file;
}

static void do_we_rebuild_hash_file(
    FILE **file, const char *file_name,
    unsigned int curr_num_of_entries, unsigned int *size_idx
)
{
    /* to rebuild, the table must be more than 2/3 full */
    if (curr_num_of_entries*3 > hash_table_size[*size_idx]*2) {
        *file = rebuild(*file, file_name);
        (*size_idx)++;
    }
}

void add_entry_to_hash_file(
    FILE **file, const char *file_name, const char *entry_name
)
{
    unsigned int idx, curr_num_of_entries, size_idx;
    get_init_bytes(*file, &curr_num_of_entries, &size_idx);
    do_we_rebuild_hash_file(file, file_name, curr_num_of_entries, &size_idx);
    /* create new entry with 1 value */
    entry new_entry;
    memset(&new_entry, 0, sizeof(entry));
    strcpy(new_entry.str, entry_name);
    new_entry.data = 1;
    idx = get_idx(entry_name, hash_table_size[size_idx]);
    find_idx_position_in_file_and(
        write_entry, *file, &new_entry, idx, NULL,
        size_idx, inc_num_of_entries_byte
    );
}

void print_hash_file(FILE *file)
{
    traverse_file_and(print_all_entries, NULL, file);
}

void print_hash_entry(FILE *file, const char *entry_name)
{
    unsigned int size_idx;
    entry entry_to_print;
    strcpy(entry_to_print.str, entry_name);
    entry_to_print.data = 0;
    fseek_err_checked(file, sizeof(unsigned int), SEEK_SET);
    fread_err_checked(&size_idx, sizeof(unsigned int), 1, file);
    unsigned int idx = get_idx(entry_to_print.str, hash_table_size[size_idx]);
    find_idx_position_in_file_and(
        print_entry, file, &entry_to_print, idx, NULL,
        size_idx, dont_inc_num_of_entries_byte
    );
}

void merge_hash_files_with_possible_rebuilding(
    FILE **dst_file, const char *dst_file_name, FILE *src_file
)
{
    module_global_var_file_name = dst_file_name;
    traverse_file_and(
        rebuild_if_needed_and_transfer_entries_to_from, dst_file, src_file
    );
}
