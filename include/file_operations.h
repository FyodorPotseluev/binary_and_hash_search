/* file_operations.h */

/* Contains functions that perform auxiliary (in addition to the basic `add`,
`query`, `list`) operations with files. */

#ifndef FILE_OPERATIONS_H_INCLUDED
#define FILE_OPERATIONS_H_INCLUDED

#include "constants.h"
#include <stdio.h>

int size_of_file(FILE *file);
/*
    Returnes file size.
SIDE EFFECT:
    - sets the file position at the end of file;
RECEIVES:
    - `file` file pointer;
RETURNES:
    - file size. */

void read_entry(entry *read_res, FILE *file, int file_pos);
/*
    Reades an entry (id + value) from the file.
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
    Write an entry (id + value) to the file.
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

void make_new_entry(FILE *file, const char *entry_name);
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
