/* file_operations.h */

/* Contains functions that perform auxiliary (in addition to the basic `add`,
`query`, `list`) operations with files. */

#ifndef FILE_OPERATIONS_H_INCLUDED
#define FILE_OPERATIONS_H_INCLUDED

#include "constants.h"
#include <stdbool.h>
#include <stdio.h>

void swap_files_if_src_file_larger(
    FILE **dst_file, FILE **src_file,
    const char **dst_file_name, const char **src_file_name
);
/*
    Swaps the file pointers it receives when the `src_file` is larger than
the `dst_file.
    If the `src_file` is larger also swaps the file names. */

int num_of_entries(FILE *file);
/*
    Returns the number of entries the file currently contains.
RECEIVES:
    - `file` file pointer;
RETURNES:
    - the number of entries in the file */

void read_entry(entry *read_res, FILE *file, int file_pos);
/*
    Reads an entry (id + value) from the file.
RECEIVES:
    - `read_res` address of the entry structure where the result of the read
    operation will be saved;
    - `file` file pointer;
    - `file_pos` the number of the position in the file where the desired entry
    is stored.
RETURNES:
    --- */

void write_entry(const entry *to_write, FILE *file, int file_pos);
/*
    Writes an entry (id + value) to the file.
RECEIVES:
    - `to_write` address of the entry structure, information from which will be
    used to write to the file;
    - `file` file pointer;
    - `file_pos` the number of the position in the file where the desired entry
    is stored.
RETURNES:
    --- */

void swap_entries(FILE *file, int first_pos, int second_pos);
/*
    Swaps two entries (id + value) in the file.
RECIEVES:
    - `file` file pointer;
    - `first_pos` the number of the first position in the file;
    - `second_pos` the number of the second position in the file.
RETURNES:
    --- */

void copy_entry(entry *dst, const entry *src);
/*
    Copies information from one `entry` type structure to another.
RECIEVES:
    - `dst` where to copy;
    - `src` where to copy from.
RETURNES:
    --- */

void add_new_entry(FILE *file, const char *entry_name);
/*
    Creates a new entry in the end of the file and sets its value to 1.
RECIEVES:
    - `file` file pointer;
    - `entry_name` a string containing the entry name (id).
RETURNES:
    --- */

void increment_entry(FILE *file, int file_pos);
/*
    Finds an entry and increments its value.
RECIEVES:
    - `file` file pointer;
    - `file_pos` entry's position in the file;
RETURNES:
    --- */

#endif
