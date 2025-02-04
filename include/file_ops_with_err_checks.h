/* file_ops_with_err_checks.h */

/* Contains functions that provide secure use of file system calls */

#ifndef FILE_OPS_WITH_ERR_CHECKS_H_INCLUDED
#define FILE_OPS_WITH_ERR_CHECKS_H_INCLUDED

#include "constants.h"
#include <stdbool.h>
#include <stdio.h>

void file_ops_error_handling(
    int res, const char *working_file_name,
    const char *src_module_file_name, int line
);
/*
    Checks if an error occurred while opening a file. If it did - prints the
corresponding error message and ends the program with the error code 1.
RECEIVES:
    - `res` the value returned by the `open` syscall;
    - `working_file_name` the name of the file we tried to open;
    - `src_module_file_name` the name of the current program module from which
    the function was called (should be passed as the `__FILE__` macro);
    - `line` line number where the function was called (should be passed as the
    `__LINE__` macro).
RETURNES:
    --- */

/*******************************************************************************
    The functions below are basically the standard functions with built-in
error handling.
    If an error takes place the corresponding error message is printed and the
program terminates with an exit code of 1.
    The extra function arguments are:
    - `src_module_file_name` the name of the current program module from which
    the function was called (should be passed as the `__FILE__` macro);
    - `line` line number where the function was called (should be passed as the
    `__LINE__` macro).
*******************************************************************************/

void close_err_checked(
    int fd, const char *file_name, const char *src_module_file_name, int line
);

void unlink_err_checked(
    const char *deleted_file_name,
    const char *src_module_file_name, int line
);

void rename_err_checked(
    const char *old_file_name, const char *new_file_name,
    const char *src_module_file_name, int line
);

void truncate_err_checked(
    const char *file_name, long length,
    const char *src_module_file_name, int line
);

void *mmap_err_checked(
    file_data *file, int prot, const char *src_module_file_name, int line
);

void msync_err_checked(
    void *addr, long length, const char *src_module_file_name, int line
);

void munmap_err_checked(
    void *addr, long length, const char *src_module_file_name, int line
);

/******************************************************************************/

long get_file_size_err_checked(
    const char *file_name, const char *src_module_file_name, int line
);
/*
    Returnes the file size.
RECEIVES:
    - `file_name` the name of the file;
    - `src_module_file_name` the name of the current program module from which
    the function was called (should be passed as the `__FILE__` macro);
    - `line` line number where the function was called (should be passed as the
    `__LINE__` macro).
RETURNES:
    - the file size */

#endif
