/* database_binary_and_hash_search.c */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

enum constants{
    file_position       = 1,
    cmd_position        = 2,
    cmd_size            = 6,
    entry_name_position  = 3,
    max_entry_name_size  = 60,
    selection_sort_arr_size = 30
};

typedef struct tag_entry {
    char str[max_entry_name_size];
    int data;
} entry;

void file_open_error_handling(const FILE *file, const char *file_name)
{
    if (!file) {
        perror(file_name);
        exit(1);
    }
}

void entry_name_error_handling(const char *entry_name)
{
    if (!entry_name) {
        printf("Error: Item name has not been specified\n");
        exit(1);
    }
    if (strlen(entry_name) >= max_entry_name_size) {
        printf("Error: Too long entry name\n");
        exit(1);
    }
}

void read_error_handling(FILE *file, const char *file_name)
{
    if (ferror(file)) {
        perror(file_name);
        exit(1);
    }
}

void file_close_error_handling(int fclose_res, const char *file_name)
{
    if (fclose_res == -1) {
        printf(
            "Error: The \"%s\" file was not closed successfully",
            file_name
        );
        exit(1);
    }
}

int size_of_file(FILE *file)
{
    /* this function sets the file position at the end of file! */
    fseek(file, 0, SEEK_END);
    return(ftell(file) / sizeof(entry));
}

void get_entry(entry *source, FILE *file, int file_pos)
{
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fread(source, sizeof(entry), 1, file);
}

void swap_entries(
    FILE *file, const entry *left, const entry *right,
    int left_pos, int right_pos
)
{
    entry *tmp = malloc(sizeof(entry));

    /* tmp = left; */
    memcpy(tmp, left, sizeof(entry));
    /* left = right; */
    fseek(file, left_pos*sizeof(entry), SEEK_SET);
    fwrite(right, sizeof(entry), 1, file);
    /* right = tmp; */
    fseek(file, right_pos*sizeof(entry), SEEK_SET);
    fwrite(tmp, sizeof(entry), 1, file);

    free(tmp);
}

/* Variadic function. */
/* The last parameter in the parameter list must be equal to NULL - this */
/* allows the function to understand that the parameter list has ended.*/
void clean_up(entry *first, ...)
{
    va_list vl;
    entry *p;
    va_start(vl, first);
    for (p=first; p; p=va_arg(vl, entry *))
        free(p);
    va_end(vl);
}

int part_of_quick_sort_hoare(FILE *file, int left_pos, int right_pos)
{
    entry *left = malloc(sizeof(entry));
    entry *pivot = malloc(sizeof(entry));
    entry *right = malloc(sizeof(entry));
    get_entry(pivot, file, left_pos+(right_pos-left_pos)/2);
    while (left_pos <= right_pos) {
        /* while (left->str < pivot->str) left_pos++ */
        get_entry(left, file, left_pos);
        while (strcmp(left->str, pivot->str) < 0) {
            left_pos++;
            get_entry(left, file, left_pos);
        }
        /* while (pivot->str < right->str) right_pos-- */
        get_entry(right, file, right_pos);
        while (strcmp(pivot->str, right->str) < 0) {
            right_pos--;
            get_entry(right, file, right_pos);
        }
        if (left_pos <= right_pos) {
            swap_entries(file, left, right, left_pos, right_pos);
            left_pos++;
            right_pos--;
        }
    }
    clean_up(left, pivot, right, NULL);
    return left_pos;
}

void copy_entry(entry *dst, entry *src)
{
    memcpy(dst->str, src->str, max_entry_name_size);
    dst->data = src->data;
}

void selection_sort(FILE *file, int start, int end)
{
    entry *entry_min_idx = malloc(sizeof(entry));
    entry *entry_i_idx = malloc(sizeof(entry));
    entry *entry_j_idx = malloc(sizeof(entry));
    int i;
    for (i=start; i < end; i++) {
        int j, min = i;
        get_entry(entry_i_idx, file, i);
        copy_entry(entry_min_idx, entry_i_idx);
        for (j=i+1; j < end + 1; j++) {
            get_entry(entry_j_idx, file, j);
            /* if (entry_min_idx->str > entry_j_idx->str) */
            if (strcmp(entry_min_idx->str, entry_j_idx->str) > 0) {
                min = j;
                copy_entry(entry_min_idx, entry_j_idx);
            }
        }
        if (min != i)
            swap_entries(file, entry_min_idx, entry_i_idx, min, i);
    }
    clean_up(entry_min_idx, entry_i_idx, entry_j_idx, NULL);
}

void quick_sort_hoare_reqursive_call(FILE *file, int start, int end)
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
    FILE *file, const char *file_name, const char *entry_name
)
{
    int file_size = size_of_file(file);
    int left = 0, mid, right = file_size-1;
    entry *mid_entry = malloc(sizeof(entry));
    for(;;) {
        int read_res;
        if (left > right){
            /* there is no such entry */
            free(mid_entry);
            return -1;
        }
        mid = left + (right - left) / 2;
        fseek(file, mid*sizeof(entry), SEEK_SET);
        read_res = fread(mid_entry, sizeof(entry), 1, file);
        if (!read_res)
            read_error_handling(file, file_name);
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

void make_new_entry(FILE *file, const char *entry_name, entry *buffer)
{
    strcpy(buffer->str, entry_name);
    buffer->data = 1;
    fwrite(buffer, 1, sizeof(entry), file);
}

void edit_entry(FILE *file, int file_pos, entry *buffer)
{
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fread(buffer, sizeof(entry), 1, file);
    buffer->data += 1;
    fseek(file, file_pos*sizeof(entry), SEEK_SET);
    fwrite(buffer, sizeof(entry), 1, file);
}

void add_entry(char *const *argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    entry *buffer = calloc(sizeof(entry), 1);
    FILE *file = fopen(file_name, "r+");
    int file_pos = 0;
    entry_name_error_handling(entry_name);
    if (file) {
        file_pos = find_entry_with_binary_search(file, file_name, entry_name);
        if (file_pos != -1) {
            /* entry was found */
            edit_entry(file, file_pos, buffer);
            goto file_close;
        }
        /* entry was not found */
        fseek(file, 0, SEEK_END);
    } else {
        /* if there was an error trying to open existing file: */
        if (0 == access(file_name, F_OK))
            file_open_error_handling(file, file_name);
        file = fopen(file_name, "w");
        file_open_error_handling(file, file_name);
    }
    make_new_entry(file, entry_name, buffer);
    quick_sort_hoare(file);
    file_close:
    free(buffer);
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

void print_entry(char *const *argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    entry *read_res = malloc(sizeof(entry));
    FILE *file = fopen(file_name, "r");
    file_open_error_handling(file, file_name);
    entry_name_error_handling(entry_name);
    int file_pos = find_entry_with_binary_search(file, file_name, entry_name);
    if (file_pos != -1) {
        fseek(file, file_pos*sizeof(entry), SEEK_SET);
        fread(read_res, sizeof(entry), 1, file);
        printf("%s - %d\n", entry_name, read_res->data);
    } else
        printf("%s - 0\n", entry_name);
    free(read_res);
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

void print_all_entries(char *const *argv)
{
    const char *file_name = argv[file_position];
    entry *read_res = malloc(sizeof(entry));
    FILE *file = fopen(file_name, "r");
    file_open_error_handling(file, file_name);
    while (fread(read_res, 1, sizeof(entry), file)) {
        printf("%s - %d\n", read_res->str, read_res->data);
    }
    free(read_res);
    int fclose_res = fclose(file);
    file_close_error_handling(fclose_res, file_name);
}

bool is_less_than_three(int num)
{
    return (num < 3) ? true : false;
}

bool is_more_than_four(int num)
{
    return (num > 4) ? true : false;
}

bool is_not_three_and_is_not_four(int num)
{
    return ((num != 3) && (num != 4)) ? true : false;
}

void incorr_num_of_args_error_handling(int argc, bool (*condition)(int))
{
    if ((*condition)(argc)) {
        printf("Error: Incorrect number of arguments\n");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    incorr_num_of_args_error_handling(argc, &is_less_than_three);
    if (0 == strncmp(argv[cmd_position], "add", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_more_than_four);
        add_entry(argv);
    } else
    if (0 == strncmp(argv[cmd_position], "query", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_more_than_four);
        print_entry(argv);
    } else
    if (0 == strncmp(argv[cmd_position], "list", cmd_size)) {
        incorr_num_of_args_error_handling(argc, &is_not_three_and_is_not_four);
        print_all_entries(argv);
    } else {
        printf("Error: Incorrect command\n");
        return 1;
    }
    return 0;
}
