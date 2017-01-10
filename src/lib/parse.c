#include "chttp.h"

#include <ctype.h>

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

static FILE *fmemopen(void* buffer, size_t len, const char *mode)
{
    mem_region_t *mem = (mem_region_t *)malloc(sizeof(mem_region_t));

    mem->position = 0;
    mem->size = len;
    mem->buffer = buffer;

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

// Filling a token (as defined by whitespace) for parsing.
static size_t fill_token(FILE *f, char *buf, size_t len)
{
    char c;
    while (isspace(c = fgetc(f)) && !feof(f)) { }
    ungetc(c, f);

    size_t i;
    for (i = 0; i < len; i++)
    {
        c = fgetc(f);
        if (c == '\0' || isspace(c))
        {
            buf[i] = '\0';
            if (isspace(c))
                ungetc(c, f);
            break;
        }

        buf[i] = c;
    }

    return i;
}

#define chttp_pmp(buf, method_var, method) \
    do { if (strcmp(buf, #method) == 0) method_var = method; } while (0);

// Parsing out a chttp_method from a FILE.
static size_t parse_method(FILE *f, chttp_method *method)
{
    const int l = CHTTP_METHOD_LENGTH + 1;
    char buf[CHTTP_METHOD_LENGTH];
    size_t len = fill_token(f, buf, l);

    *method = OTHER;
    chttp_pmp(buf, *method, OPTIONS);
    chttp_pmp(buf, *method, GET);
    chttp_pmp(buf, *method, HEAD);
    chttp_pmp(buf, *method, POST);
    chttp_pmp(buf, *method, PUT);
    chttp_pmp(buf, *method, DELETE);
    chttp_pmp(buf, *method, TRACE);
    chttp_pmp(buf, *method, CONNECT);

    return len;
}

// Parsing a chttp_request from a given string. Returns the number of characters
// read on success. Returns -1 on failure. Inverse of chttp_sprint_request.
size_t chttp_parse_request(chttp_request *r, FILE *f)
{
    size_t start = ftell(f);
    parse_method(f, &r->method);

    fill_token(f, r->uri, CHTTP_URI_LENGTH);
    fill_token(f, r->http_version, CHTTP_HTTP_VERSION_LENGTH);

    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
    size_t e = 0;
    while (!feof(f))
    {
        e = fill_token(f, header, CHTTP_HEADER_KEY_LENGTH);
        if (header[e - 1] != ':')
            break;
        header[e - 1] = '\0';
        fill_token(f, value, CHTTP_HEADER_VALUE_LENGTH);

        chttp_add_header(r->headers, header, value);
    }

    memcpy(r->body, header, e);
    size_t n = fread(r->body + e, sizeof(char), CHTTP_BODY_LENGTH - 1, f);
    (r->body + e)[n] = '\0';

    return ftell(f) - start;
}

// Parsing a chttp_response from a given string. Returns the number of
// characters read on success. Returns -1 on failure. Inverse of
// chttp_sprint_response.
size_t chttp_parse_response(chttp_response *r, FILE *f)
{
    size_t start = ftell(f);
    fill_token(f, r->http_version, CHTTP_HTTP_VERSION_LENGTH);
    fscanf(f, "%d", &r->code);
    fill_token(f, r->reason_phrase, CHTTP_REASON_PHRASE_LENGTH);

    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
    size_t e = 0;
    while (!feof(f))
    {
        e = fill_token(f, header, CHTTP_HEADER_KEY_LENGTH);
        if (header[e - 1] != ':')
            break;
        header[e - 1] = '\0';
        fill_token(f, value, CHTTP_HEADER_VALUE_LENGTH);

        chttp_add_header(r->headers, header, value);
    }

    memcpy(r->body, header, e);
    size_t n = fread(r->body + e, sizeof(char), CHTTP_BODY_LENGTH - 1, f);
    (r->body + e)[n] = '\0';

    return ftell(f) - start;
}

// Pipes string to a FILE * and calls chttp_parse_request.
size_t chttp_sparse_request(chttp_request *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_request(r, f);
    fclose(f);
    return n;
}

// Pipes string to a FILE * and falls chttp_parse_response.
size_t chttp_sparse_response(chttp_response *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_response(r, f);
    fclose(f);
    return n;
}