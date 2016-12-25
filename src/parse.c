#include "chttp.h"

#if defined(__APPLE__) || defined(__WIN32)
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __APPLE__
struct mem_region
{
    size_t position;
    size_t size;
    void *buffer;
};
typedef struct mem_region mem_region_t;

int read_fn(void *file, char *buf, int nbytes)
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

int write_fn(void *file, const char *buf, int nbytes)
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

off_t seek_fn(void *file, off_t offset, int whence)
{
    return 0;
}

int close_fn(void *file)
{
    return 0;
}

static FILE *fmemopen(void* buffer, size_t len, const char *mode)
{
    mem_region_t *mem = (mem_region_t *)malloc(sizeof(mem_region_t));
    return funopen(mem, &read_fn, &write_fn, &seek_fn, &close_fn);
}
#elif __WIN32
static FILE *fmemopen(void *buffer, size_t len, const char *mode)
{
    // TODO: Implement.
    printf("fmemopen unimplemented on WIN32.\n");
    exit(1);
}
#endif

// Parsing a chttp_request from a given string. Returns the number of characters
// read on success. Returns -1 on failure. Inverse of chttp_sprint_request.
int chttp_parse_request(chttp_request *r, FILE *f)
{
    // TODO
    return -1;
}

// Parsing a chttp_response from a given string. Returns the number of
// characters read on success. Returns -1 on failure. Inverse of
// chttp_sprint_response.
int chttp_parse_response(chttp_response *r, FILE *f)
{
    // TODO
    return -1;
}

// Pipes string to a FILE * and calls chttp_parse_request.
int chttp_sparse_request(chttp_request *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_request(r, f);
    fclose(f);
    return n;
}

// Pipes string to a FILE * and falls chttp_parse_response.
int chttp_sparse_response(chttp_response *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_response(r, f);
    fclose(f);
    return n;
}
