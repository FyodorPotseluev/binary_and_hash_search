/* hash_sort_and_search.h */

#ifndef HASH_SORT_AND_SEARCH_H_INCLUDED
#define HASH_SORT_AND_SEARCH_H_INCLUDED

#include "constants.h"
#include <stdio.h>

void initializing_hash_file(file_data *file, unsigned int idx);
/*
    Sets a newly created hash file to a specified size. All the file byte values
are 0 except the second 4 bytes of the file containing the current index of the
array of hash table sizes.
RECEIVES:
    - `file` the `file_data` struct containing information about the file we
    initialize;
    - `idx` the index of the `hash_table_size` pointing to the number of entries
    the current hash table can hold.
RETURNES:
    --- */

void entry_action(to_do action, file_data *file, entry *search_entry);
/*
    Finds a given entry id in the specified file and either prints its current
value or adds the given entry to the file. The id search takes O=log(1) time. 
RECEIVES:
    - `action`:
        -- `to_print`: prints the `search_entry` value stored in the file;
        -- `to_write`: adds the `search_entry` value to the file;
    - `file` the `file_data` struct containing information about the file we're
    working with;
    - `search_entry` the entry which id we're looking for in the file.
RETURNES:
    --- */

void traverse_file_and_(to_do action, file_data *dst, file_data *src);
/*
    Finds all entries in the `src` file and either prints them or adds their
values to the `dst` file (every time the destination file is more than 2/3 full,
it is rebuilt).
RECEIVES:
    - `action`:
        -- `to_print`: prints all `src` file entries;
        -- `to_write`: adds all `src` file entries to the `dst` file;
    - `dst` the `file_data` struct containing information about the file we're
    transferring data to (if `action` is `to_print` should be passed as NULL);
    - `src` the `file_data` struct containing information about the file we're
    traversing and getting entries one by one.
RETURNES:
    --- */

#endif
