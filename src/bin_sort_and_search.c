/* bin_sort_and_search.c */

#include "bin_sort_and_search.h"
#include "constants.h"
#include "error_handling.h"
#include "file_operations.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int part_of_quick_sort_hoare(FILE *file, int left_pos, int right_pos)
{
    entry left, pivot, right;
    read_entry(&pivot, file, left_pos+(right_pos-left_pos)/2);
    while (left_pos <= right_pos) {
        /* while (left.str < pivot.str) left_pos++ */
        read_entry(&left, file, left_pos);
        while (strcmp(left.str, pivot.str) < 0) {
            left_pos++;
            read_entry(&left, file, left_pos);
        }
        /* while (pivot.str < right.str) right_pos-- */
        read_entry(&right, file, right_pos);
        while (strcmp(pivot.str, right.str) < 0) {
            right_pos--;
            read_entry(&right, file, right_pos);
        }
        if (left_pos <= right_pos) {
            swap_entries(file, left_pos, right_pos);
            left_pos++;
            right_pos--;
        }
    }
    return left_pos;
}

static void selection_sort(FILE *file, int start, int end)
{
    entry entry_min_idx, entry_i_idx, entry_j_idx;
    int i;
    for (i=start; i < end; i++) {
        int j, min = i;
        read_entry(&entry_i_idx, file, i);
        copy_entry(&entry_min_idx, &entry_i_idx);
        for (j=i+1; j < end + 1; j++) {
            read_entry(&entry_j_idx, file, j);
            /* if (entry_min_idx.str > entry_j_idx.str) */
            if (strcmp(entry_min_idx.str, entry_j_idx.str) > 0) {
                min = j;
                copy_entry(&entry_min_idx, &entry_j_idx);
            }
        }
        if (min != i)
            swap_entries(file, min, i);
    }
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

static void quick_sort_hoare(FILE *file)
{
    int entries_num = num_of_entries(file);
    quick_sort_hoare_reqursive_call(file, 0, entries_num -1);
}

int find_entry_with_binary_search(
    FILE *file, const char *entry_name
)
{
    int entries_num = num_of_entries(file);
    int left = 0, mid, right = entries_num-1;
    entry mid_entry;
    for(;;) {
        /* there is no such entry */
        if (left > right) return -1;
        mid = left + (right - left) / 2;
        read_entry(&mid_entry, file, mid);
        /* entry_name < mid_entry.str */
        if (strcmp(entry_name, mid_entry.str) < 0) right = mid-1;
        /* mid_entry.str < entry_name */
        if (strcmp(mid_entry.str, entry_name) < 0) left = mid+1;
        /* mid_entry.str == entry_name */
        /* entry found */
        if (strcmp(mid_entry.str, entry_name) == 0) return mid;
    }
}

bool add_existing_entry_to_bin_file(FILE *file, const char *entry_name)
{
    int file_pos = find_entry_with_binary_search(file, entry_name);
    if (file_pos != -1) {
        /* entry was found */
        increment_entry(file, file_pos);
        return true;
    } else
        /* entry was not found */
        return false;
}

void add_new_entry_to_bin_file(FILE *file, const char *entry_name)
{
    add_new_entry(file, entry_name);
    quick_sort_hoare(file);
}

void merge_bin_files(FILE *dst_file, FILE *src_file)
{
    int i;
    entry transfer_entry;
    for (i=0; i < num_of_entries(src_file); i++) {
        fread_err_checked(&transfer_entry, sizeof(entry), 1, src_file);
        bool found = add_existing_entry_to_bin_file(
            dst_file, transfer_entry.str
        );
        if (!found) add_new_entry_to_bin_file(dst_file, transfer_entry.str);
    }
    quick_sort_hoare(dst_file);
}
