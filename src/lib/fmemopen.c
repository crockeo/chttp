#include "chttp_fmemopen.h"

#if defined(__APPLE__) || defined(__WIN32)
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __APPLE__
#include <sys/types.h>

struct mem_region
{
    size_t position;
    size_t size;
    void *buffer;
};
typedef struct mem_region mem_region_t;

static int read_fn(void *file, char *buf, int nbytes)
{
    mem_region_t *mem = (mem_region_t *)file;
    size_t available = mem->size - mem->position;
    if (nbytes > available)
        nbytes = available;

    memcpy(buf, mem->buffer + mem->position, nbytes);
    size_t pos = mem->position;
    mem->position += nbytes;

    return mem->position - pos;
}

static int write_fn(void *file, const char *buf, int nbytes)
{
    mem_region_t *mem = (mem_region_t *)file;
    size_t available = mem->size - mem->position;
    if (nbytes > available)
        nbytes = available;

    memcpy(mem->buffer + mem->position, buf, available);
    size_t pos = mem->position;
    mem->position += nbytes;

    return mem->position - pos;
}

static off_t seek_fn(void *file, off_t offset, int whence)
{
    mem_region_t *mem = (mem_region_t *)file;
    size_t position = 0;

    switch (whence)
    {
    case SEEK_SET:
        position = offset;
        break;
    case SEEK_CUR:
        position = mem->position + offset;
        break;
    case SEEK_END:
        position = mem->size + offset;
        break;
    default:
        return -1;
    }

    if (position >= mem->size)
        return -1;
    mem->position = position;
    return position;
}

static int close_fn(void *file)
{
    free(file);
    return 0;
}

FILE *fmemopen(void* buffer, size_t len, const char *mode)
{
    mem_region_t *mem = (mem_region_t *)malloc(sizeof(mem_region_t));

    mem->position = 0;
    mem->size = len;
    mem->buffer = buffer;

    return funopen(mem, &read_fn, &write_fn, &seek_fn, &close_fn);
}
#elif __WIN32
FILE *fmemopen(void *buffer, size_t len, const char *mode)
{
    // TODO: Implement.
    printf("fmemopen unimplemented on WIN32.\n");
    exit(1);
}
#endif
