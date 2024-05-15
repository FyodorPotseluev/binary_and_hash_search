/* constants.h */

#ifndef CONSTANTS_INCLUDED
#define CONSTANTS_INCLUDED

enum constants{
    /* command line arguments - file name position */
    file_position       = 1,
    /* command line arguments - position of the command to be executed */
    cmd_position        = 2,
    /* maximum string size for the executed command */
    cmd_size            = 6,
    /* command line arguments - entry name (id) position */
    entry_name_position  = 3,
    /* maximum string size for the entry name (id) */
    max_entry_name_size  = 60,
    /* array size, for this size and smaller, selection sort is used intead of
    binary sort to increase the sort speed */
    selection_sort_arr_size = 30
};

typedef struct tag_entry {
    char str[max_entry_name_size];
    int data;
} entry;

#endif
