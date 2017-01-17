#include "chttp.h"

#include <stdlib.h>
#include <string.h>

// Filling a header set.
void chttp_header_set_fill(chttp_header_set *s)
{
    s->size = 1;
    s->len = 0;
    s->headers = (chttp_header *)malloc(sizeof(chttp_header));
}

// Allocating the space for a chttp_header_set.
chttp_header_set *chttp_header_set_allocate()
{
    chttp_header_set *hs = (chttp_header_set *)malloc(sizeof(chttp_header_set));
    chttp_header_set_fill(hs);
    return hs;
}

// Freeing a chttp_header_set.
void chttp_header_set_free(chttp_header_set *s)
{
    free(s->headers);
    free(s);
}

// Adding a header to the header set.
void chttp_add_header(chttp_header_set *set, const char *header, const char *value)
{
    if (set->len >= set->size)
    {
        set->size *= 2;
        chttp_header *new_headers = (chttp_header *)malloc(sizeof(chttp_header) * set->size);
        memcpy(new_headers, set->headers, set->len * sizeof(chttp_header));
        free(set->headers);
        set->headers = new_headers;
    }

    memset(&set->headers[set->len], 0, sizeof(chttp_header));
    strncpy(set->headers[set->len].header, header, CHTTP_HEADER_KEY_LENGTH);
    strncpy(set->headers[set->len].value, value, CHTTP_HEADER_VALUE_LENGTH);
    set->len++;
}

// Getting a header from the header set.
char *chttp_get_header(chttp_header_set *set, const char *header)
{
    for (int i = 0; i < set->len; i++)
        if (strcmp(header, set->headers[i].header) == 0)
            return set->headers[i].value;
    return NULL;
}
