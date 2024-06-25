/* bin_sort_and_search.c */

#include "bin_sort_and_search.h"
#include "constants.h"
#include "error_handling.h"
#include "file_operations.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Variadic function. */
/* The last parameter in the parameter list must be equal to NULL - this */
/* allows the function to understand that the parameter list has ended.*/
static void clean_up(entry *first, ...)
{
    va_list vl;
    entry *p;
    va_start(vl, first);
    for (p=first; p; p=va_arg(vl, entry *))
        free(p);
    va_end(vl);
}

static int part_of_quick_sort_hoare(FILE *file, int left_pos, int right_pos)
{
    entry *left = malloc_err_checked(sizeof(entry));
    entry *pivot = malloc_err_checked(sizeof(entry));
    entry *right = malloc_err_checked(sizeof(entry));
    read_entry(pivot, file, left_pos+(right_pos-left_pos)/2);
    while (left_pos <= right_pos) {
        /* while (left->str < pivot->str) left_pos++ */
        read_entry(left, file, left_pos);
        while (strcmp(left->str, pivot->str) < 0) {
            left_pos++;
            read_entry(left, file, left_pos);
        }
        /* while (pivot->str < right->str) right_pos-- */
        read_entry(right, file, right_pos);
        while (strcmp(pivot->str, right->str) < 0) {
            right_pos--;
            read_entry(right, file, right_pos);
        }
        if (left_pos <= right_pos) {
            swap_entries(file, left_pos, right_pos);
            left_pos++;
            right_pos--;
        }
    }
    clean_up(left, pivot, right, NULL);
    return left_pos;
}

static void selection_sort(FILE *file, int start, int end)
{
    entry *entry_min_idx = malloc_err_checked(sizeof(entry));
    entry *entry_i_idx = malloc_err_checked(sizeof(entry));
    entry *entry_j_idx = malloc_err_checked(sizeof(entry));
    int i;
    for (i=start; i < end; i++) {
        int j, min = i;
        read_entry(entry_i_idx, file, i);
        copy_entry(entry_min_idx, entry_i_idx);
        for (j=i+1; j < end + 1; j++) {
            read_entry(entry_j_idx, file, j);
            /* if (entry_min_idx->str > entry_j_idx->str) */
            if (strcmp(entry_min_idx->str, entry_j_idx->str) > 0) {
                min = j;
                copy_entry(entry_min_idx, entry_j_idx);
            }
        }
        if (min != i)
            swap_entries(file, min, i);
    }
    clean_up(entry_min_idx, entry_i_idx, entry_j_idx, NULL);
}

static void quick_sort_hoare_reqursive_call(FILE *file, int start, int end)
{
    if (start >= end) return;
    if (end - start < selection_sort_arr_size) {
        selection_sort(file, start, end);
        return;
    }
    int right_start_position = part_of_quick_sort_hoare(file, start, end);
    quick_sort_hoare_reqursive_call(file, start, right_start_position-1);
    quick_sort_hoare_reqursive_call(file, right_start_position, end);
}

void quick_sort_hoare(FILE *file)
{
    int file_size = size_of_file(file);
    quick_sort_hoare_reqursive_call(file, 0, file_size-1);
}

int find_entry_with_binary_search(
    FILE *file, const char *entry_name
)
{
    int file_size = size_of_file(file);
    int left = 0, mid, right = file_size-1;
    entry *mid_entry = malloc_err_checked(sizeof(entry));
    for(;;) {
        if (left > right) {
            /* there is no such entry */
            free(mid_entry);
            return -1;
        }
        mid = left + (right - left) / 2;
        fseek_err_checked(file, mid*sizeof(entry), SEEK_SET);
        fread_err_checked(mid_entry, sizeof(entry), 1, file);
        /* entry_name < mid_entry->str */
        if (strcmp(entry_name, mid_entry->str) < 0) right = mid-1;
        /* mid_entry->str < entry_name */
        if (strcmp(mid_entry->str, entry_name) < 0) left = mid+1;
        /* mid_entry->str == entry_name */
        if (strcmp(mid_entry->str, entry_name) == 0) {
            /* entry found */
            free(mid_entry);
            return mid;
        }
    }
}
