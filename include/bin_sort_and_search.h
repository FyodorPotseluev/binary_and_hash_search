/* bin_sort_and_search.h */

/* Contains functions that enable the use of binary search */

#ifndef BIN_SORT_AND_SEARCH_H_INCLUDED
#define BIN_SORT_AND_SEARCH_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

int find_entry_with_binary_search(FILE *file, const char *entry_name);
/*
    Finds the file position containing a given entry name (id). The binary search
is a divide and conquer algorithm and has the O=log(N) complexity.
RECEIVES:
    - `file` file pointer;
    - `file_name` a string containing the file name to include it in possible
    error messages;
    - `entry_name` a string containing the entry name;
RETURNES:
    - the number of the position in the file containing the requested entry
    name. If the entry name was not found returnes -1. */

bool add_existing_entry_to_bin_file(FILE *file, const char *entry_name);
/*
    Searches for an entry in a file with alphabetically sorted entries. If it's
found - increments the entry value and returns `true`. If it's not - returns
`false`. Has the O=log(N) complexity.
RECEIVES:
    - `file` file pointer;
    - `entry_name` a string containing the entry name (id) to add;
RETURNES:
    --- */

void add_new_entry_to_bin_file(FILE *file, const char *entry_name);
/*
    Adds an entry to a file with alphabetically sorted entries. It's used when
we are already sure that there is no such entry in the current file
(`add_existing_entry_to_bin_file` has already returned `false`). Has the
O=log(N) complexity.
RECEIVES:
    - `file` file pointer;
    - `entry_name` a string containing the entry name (id) to add;
RETURNES:
    --- */

void merge_bin_files(FILE *dst_file, FILE *src_file);
/*
    Transfers all entries from the source file to the destination file, keeping
the entries sorted by entry name (id) in alphabetical order.
RECEIVES:
    - `dst_file` destination file pointer;
    - `src_file` source file pointer;
RETURNES:
    --- */

#endif
