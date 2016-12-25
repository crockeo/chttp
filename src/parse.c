#include "chttp.h"

#if defined(__APPLE__) || defined(__WIN32)
#include <stdlib.h>
// TODO: Implement core support for fmemopen implementations.
#endif

#ifdef __APPLE__
static FILE *fmemopen(const char *string, size_t len, const char *mode)
{
    // TODO: Implement.
    printf("fmemopen unimplemented on APPLE.\n");
    exit(1);
}
#elif __WIN32
static FILE *fmemopen(const char *string, size_t len, const char *mode)
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
    FILE *f = fmemopen(string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_request(r, f);
    fclose(f);
    return n;
}

// Pipes string to a FILE * and falls chttp_parse_response.
int chttp_sparse_response(chttp_response *r, const char *string, int len)
{
    FILE *f = fmemopen(string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_response(r, f);
    fclose(f);
    return n;
}
