/* bin_sort_and_search.h */

/* Contains functions that enable the use of binary search */

#ifndef BIN_SORT_AND_SEARCH_H_INCLUDED
#define BIN_SORT_AND_SEARCH_H_INCLUDED

#include "constants.h"
#include <stdio.h>
#include <stdbool.h>

long find_entry_with_bin_search(file_data *file, const char *entry_name);
/*
    Finds the file position containing a given entry name (id). The binary 
search is a divide and conquer algorithm and has the O=log(N) complexity.
RECEIVES:
    - `file` the `file_data` struct containing information about the file being
    searched;
    - `entry_name` the string containing the entry name;
RETURNES:
    - the number of the position in the file containing the requested entry
    name. If the entry name was not found returnes -1. */

bool add_existing_entry_to_bin_file(file_data *file, const char *entry_name);
/*
    Searches for an entry in a file with alphabetically sorted entries. If it's
found - increments the entry value and returns `true`. If it's not - returns
`false`. Has the O=log(N) complexity.
RECEIVES:
    - `file` the `file_data` struct containing information about the file being
    searched;
    - `entry_name` a string containing the entry name (id) to add;
RETURNES:
    - boolean value - the entry was found. */

void add_new_entry_to_bin_file(file_data *file, const char *entry_name);
/*
    Adds an entry to a file with alphabetically sorted entries. It's used when
we are already sure that there is no such entry in the current file
(`add_existing_entry_to_bin_file` has already returned `false`). Has the
O=N*log(N) complexity.
RECEIVES:
    - `file` the `file_data` struct containing information about the file we add
    the entry to;
    - `entry_name` a string containing the entry name (id) to add;
RETURNES:
    --- */

void merge_bin_files(file_data *dst, file_data *src);
/*
    Transfers all entries from the source file to the destination file, keeping
the entries sorted by entry name (id) in alphabetical order.
RECEIVES:
    - `dst_file` `file_data` struct containing information about the destination
    file;
    - `src_file` `file_data` struct containing information about the source
    file.
RETURNES:
    --- */

#endif
