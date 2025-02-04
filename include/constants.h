/* constants.h */

#ifndef CONSTANTS_INCLUDED
#define CONSTANTS_INCLUDED

enum constants{
    /* command line arguments - file name position */
    file_name_position      = 1,
    /* command line arguments - position of the command to be executed */
    cmd_position            = 2,
    /* maximum string size for the executed command */
    cmd_size                = 6,
    /* command line arguments - entry name (id) position */
    entry_name_position     = 3,
    /* command line arguments - if the command is "merge" - it has a special
    command position */
    merge_cmd_position      = 1,
    /* command line arguments - if the command is "merge" - destination file
    (the one that will receive all entries from a source file) name position */
    dst_file_name_position  = 2,
    /* command line arguments - if the command is "merge" - source file (the
    one that will transfer all its entries to a destination file) name position
    */
    src_file_name_position  = 3,
    /* command line arguments - if the command is "merge" - the position of the
    name the final file must have */
    final_file_name_position= 4,
    /* maximum string size for the entry name (id) */
    max_entry_name_size     = 60,
    /* maximum string size for the `file_ops_with_err_checks` module diagnostic
    message */
    max_err_msg_size        = 128,
    /* array size, for this size and smaller, selection sort is used intead of
    binary sort to increase the sort speed */
    selection_sort_arr_size = 30,
    /* the number of bytes at the beginning of a hash file containing metadata
    (information about the current entries number and the current size of the
    hash file) */
    num_of_metadata_bytes   = sizeof(unsigned int) * 2
};

typedef enum tag_to_do {
    to_write, to_print
} to_do;

typedef struct tag_entry {
    char str[max_entry_name_size];
    int data;
} entry;

typedef struct tag_file_data {
    int fd;
    const char *name;
    long size;
    void *map;
} file_data;

#endif
