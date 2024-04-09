/* database_binary_and_hash_search.c */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

enum constants{
    file_position       = 1,
    cmd_position        = 2,
    cmd_size            = 6,
    entry_name_position  = 3,
    max_entry_name_size  = 60
};

typedef struct tag_entry {
    char search_key[max_entry_name_size + 1];
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
    if (strlen(entry_name) > max_entry_name_size) {
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

void add_entry(char *const *argv)
{
    const char *file_name = argv[file_position];
    const char *entry_name = argv[entry_name_position];
    entry *buffer = calloc(sizeof(entry), 1);
    FILE *file = fopen(file_name, "r+");
    int file_pos = 0;
    entry_name_error_handling(entry_name);
    if (file) {
        /* find existing entry then edit it */
        while (fread(buffer, 1, sizeof(entry), file)) {
            if (0 == strcmp(buffer->search_key, entry_name)) {
                buffer->data += 1;
                fseek(file, file_pos*sizeof(entry), SEEK_SET);
                fwrite(buffer, 1, sizeof(entry), file);
                /* if entry was found and edited - close the file */
                goto file_close;
            }
            file_pos++;
        }
        read_error_handling(file, file_name);
        fseek(file, file_pos*sizeof(entry), SEEK_SET);
    } else {
        /* if there was an error trying to open existing file */
        if (0 == access(file_name, F_OK))
            file_open_error_handling(file, file_name);
        file = fopen(file_name, "w");
        file_open_error_handling(file, file_name);
    }
    /* make a new entry */
    strcpy(buffer->search_key, entry_name);
    buffer->data = 1;
    fwrite(buffer, 1, sizeof(entry), file);
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
    while (fread(read_res, 1, sizeof(entry), file)) {
        if (0 == strcmp(entry_name, read_res->search_key)) {
            printf("%s - %d\n", entry_name, read_res->data);
            goto file_close;
        }
    }
    read_error_handling(file, file_name);
    printf("%s - 0\n", entry_name);
    file_close:
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
        printf("%s - %d\n", read_res->search_key, read_res->data);
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
