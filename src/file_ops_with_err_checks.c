/* file_ops_with_err_checks.c */

#include "file_ops_with_err_checks.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* void fwrite_err_checked(
    const void *buffer, size_t item_size, size_t items_num, FILE *file
)
{
    size_t items_written = fwrite(buffer, item_size, items_num, file);
    if (items_written != items_num) {
        perror("fwrite failed");
        exit(1);
    }
}

long ftell_err_checked(FILE *file)
{
    long pos = ftell(file);
    if (pos == -1) {
        perror("ftell failed");
        exit(1);
    }
    return pos;
} */

void file_ops_error_handling(
    int res, const char *working_file_name,
    const char *src_module_file_name, int line
)
{
    char error_data[max_err_msg_size];
    if (res == -1) {
        snprintf(
            error_data, max_err_msg_size, "%s: line %d\n%s",
            src_module_file_name, line, working_file_name
        );
        perror(error_data);
        exit(1);
    }
}

void close_err_checked(
    int fd, const char *file_name, const char *src_module_file_name, int line
)
{
    int res = close(fd);
    file_ops_error_handling(res, file_name, src_module_file_name, line);
}

void unlink_err_checked(
    const char *deleted_file_name,
    const char *src_module_file_name, int line
)
{
    int res = unlink(deleted_file_name);
    file_ops_error_handling(res, deleted_file_name, src_module_file_name, line);
}

void rename_err_checked(
    const char *old_file_name, const char *new_file_name,
    const char *src_module_file_name, int line
)
{
    int res = rename(old_file_name, new_file_name);
    file_ops_error_handling(res, new_file_name, src_module_file_name, line);
}

long get_file_size_err_checked(
    const char *file_name, const char *src_module_file_name, int line
)
{
    struct stat buf;
    int res = stat(file_name, &buf);
    file_ops_error_handling(res, file_name, src_module_file_name, line);
    return buf.st_size;
}

void truncate_err_checked(
    const char *file_name, long length,
    const char *src_module_file_name, int line
)
{
    int res = truncate(file_name, length);
    file_ops_error_handling(res, file_name, src_module_file_name, line);
}

void *mmap_err_checked(
    file_data *file, int prot, const char *src_module_file_name, int line
)
{
    void *res = NULL;
    int page_size = getpagesize();
    /* minimal number greater than or equal to initial block size
    and multiple of page size */
    long aligned_size = ((file->size - 1) / page_size + 1) * page_size;
    res = mmap(NULL, aligned_size, prot, MAP_SHARED, file->fd, 0);
    file_ops_error_handling(
        (int)((long)res), file->name, src_module_file_name, line
    );
    return res;
}

void msync_err_checked(
    void *addr, long length, const char *src_module_file_name, int line
)
{
    int res = msync(addr, length, MS_ASYNC);
    file_ops_error_handling(res, "msync fail", src_module_file_name, line);
}

void munmap_err_checked(
    void *addr, long length, const char *src_module_file_name, int line
)
{
    int res = munmap(addr, length);
    file_ops_error_handling(res, "munmap fail", src_module_file_name, line);
}

/* void lseek_err_checked(
    int fd, long offset, int whence,
    const char *src_module_file_name, int line
)
{
    long res = lseek(fd, offset, whence);
    file_ops_error_handling((int)res, "lseek fail", src_module_file_name, line);
} */
