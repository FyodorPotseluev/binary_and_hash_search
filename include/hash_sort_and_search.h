/* hash_sort_and_search.h */

#ifndef HASH_SORT_AND_SEARCH_H_INCLUDED
#define HASH_SORT_AND_SEARCH_H_INCLUDED

#include <stdio.h>

void initializing_hash_file(const char *file_name);
/*
    The function purpose is to set a newly created hash file to its initial
size:
    - two 4-byte numbers of unsigned int type at the beginning of the file,
    containing information about:
        -- the current total number of entries in the file;
        -- the current index of the array of possible hash file sizes;
    - the initial number of 64-byte entries (array of hash file sizes, index 0),
    which is 11;
All file byte values are 0.
RECEIVES:
    - `file_name` a string containing the file name;
RETURNES:
    --- */

void add_entry_to_hash_file(
    FILE **file, const char *file_name, const char *entry_name
);
/*
    Adds the next user entry to a hash file. Has the O(1) complexity.
Exception - the cases when the hash file is already 2/3 full of entries and
needs to be rebuilt in order to keep the hash search efficient.
The rebuilding process can take a considerable amount of time because the
algorithm is linearly copies all the existing entries from the old file and adds
them to a new file of larger size.
RECEIVES:
    - `file` file pointer to pointer (the file and the pointer to it will be
    replaced in case of rebuilding);
    - `file_name` a string containing the file name;
    - `entry_name` a string containing the entry name (id) to add;
RETURNES:
    --- */

void print_hash_file(FILE *file);
/*
    Prints all the entries (id + value) contained in the file.
RECEIVES:
    - `file` file pointer;
RETURNES:
    --- */

void print_hash_entry(FILE *file, const char *entry_name);
/*
    Prints the value of a given entry id. The id search takes O=log(1) time.
If there is no such entry, prints `id - 0`.
RECEIVES:
    - `file` file pointer;
    - `entry_name` a string containing the entry id which value has to be
    printed;
RETURNES:
    --- */

void merge_hash_files_with_possible_rebuilding(
    FILE **dst_file, const char *dst_file_name, FILE *src_file
);
/*
    Transfers all entries from the source file to the destination file. Every
time during the transfering the destination file is full more than 2/3 it is
rebuilt.
RECEIVES:
    - `dst_file` pointer to destination file pointer;
    - `dst_file_name` a string containing the destination file name;
    - `src_file` source file pointer;
RETURNES:
    --- */

#endif
