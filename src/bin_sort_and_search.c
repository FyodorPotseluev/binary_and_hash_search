/* bin_sort_and_search.c */

#include "bin_sort_and_search.h"
#include "constants.h"
#include "file_ops_with_err_checks.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

static void swap_entries(entry *arr, long first, long second)
{
    if (first == second)
        return;
    entry tmp;
    strcpy(tmp.str, arr[first].str);
    tmp.data = arr[first].data;
    strcpy(arr[first].str, arr[second].str);
    arr[first].data = arr[second].data;
    strcpy(arr[second].str, tmp.str);
    arr[second].data = tmp.data;
}

static long part_of_quick_sort_hoare(entry *arr, long left, long right)
{
    entry pivot = arr[left + (right - left) / 2];
    while (left <= right) {
        /* while (arr[left].str < pivot.str) left++ */
        while (strcmp(arr[left].str, pivot.str) < 0)
            left++;
        /* while (pivot.str < arr[right].str) right-- */
        while (strcmp(pivot.str, arr[right].str) < 0)
            right--;
        if (left <= right) {
            swap_entries(arr, left, right);
            left++;
            right--;
        }
    }
    return left;
}

static void selection_sort(entry *arr, long start, long end)
{
    long i;
    for (i=start; i < end; i++) {
        long j, min_idx = i;
        for (j=i+1; j <= end; j++) {
            /* if (arr[min_idx].str > arr[j].str) */
            if (strcmp(arr[min_idx].str, arr[j].str) > 0)
                min_idx = j;
        }
        if (min_idx != i)
            swap_entries(arr, min_idx, i);
    }
}

static void quick_sort_hoare_reqursive_call(entry *arr, long start, long end)
{
    if (start >= end) return;
    if (end - start < selection_sort_arr_size) {
        selection_sort(arr, start, end);
        return;
    }
    long right_start = part_of_quick_sort_hoare(arr, start, end);
    quick_sort_hoare_reqursive_call(arr, start, right_start-1);
    quick_sort_hoare_reqursive_call(arr, right_start, end);
}

static void quick_sort_hoare(entry *arr, long size)
{
    quick_sort_hoare_reqursive_call(arr, 0, size-1);
}

long find_entry_with_bin_search(file_data *file, const char *entry_name)
{
    const entry *arr = file->map;
    long entries_num = file->size / sizeof(entry);
    long left = 0, mid, right = entries_num-1;
    for (;;) {
        /* there is no such entry */
        if (left > right) return -1;
        mid = left + (right - left) / 2;
        if (strcmp(entry_name, arr[mid].str) < 0) right = mid-1;
        if (strcmp(arr[mid].str, entry_name) < 0) left = mid+1;
        /* entry found */
        if (strcmp(arr[mid].str, entry_name) == 0) return mid;
    }
}

bool add_existing_entry_to_bin_file(file_data *file, const char *entry_name)
{
    entry *arr = file->map;
    long entry_pos = find_entry_with_bin_search(file, entry_name);
    if (entry_pos != -1) {
        /* entry was found */
        arr[entry_pos].data += 1;
        return true;
    } else
        /* entry was not found */
        return false;
}

static void increment_mapped_file_size(file_data *file)
{
    if (file->size > 0)
        munmap_err_checked(file->map, file->size, __FILE__, __LINE__);
    file->size = file->size + sizeof(entry);
    truncate_err_checked(file->name, file->size, __FILE__, __LINE__);
    file->map = mmap_err_checked(file, PROT_READ|PROT_WRITE, __FILE__,__LINE__);
}

void add_new_entry_to_bin_file(file_data *file, const char *entry_name)
{
    entry *arr;
    long arr_size;
    increment_mapped_file_size(file);
    arr_size = file->size / sizeof(entry);
    arr = file->map;
    strcpy(arr[arr_size-1].str, entry_name);
    arr[arr_size-1].data = 1;
    quick_sort_hoare(arr, arr_size);
}

void merge_bin_files(file_data *dst, file_data *src)
{
    entry *src_arr = src->map;
    unsigned long i;
    for (i=0; i < src->size / sizeof(entry); i++) {
        bool found = add_existing_entry_to_bin_file(dst, src_arr[i].str);
        if (!found)
            add_new_entry_to_bin_file(dst, src_arr[i].str);
    }
}
