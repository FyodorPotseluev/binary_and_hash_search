/* bin_sort_and_search.h */

/* Contains functions that enable the use of binary search */

#ifndef BIN_SORT_AND_SEARCH_H_INCLUDED
#define BIN_SORT_AND_SEARCH_H_INCLUDED

#include <stdio.h>

void quick_sort_hoare(FILE *file);
/*
    Sorts the entries in the file. Entries are sorted by entry name (id) in
alpahabetical order. This enables to use the binary search algorithm later. The sort uses a divide and conquer algorithm and has the O=log(N) complexity.
RECEIVES:
    - `file` file pointer;
RETURNES:
    --- */

int find_entry_with_binary_search(
    FILE *file, const char *file_name, const char *entry_name
);
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

#endif
