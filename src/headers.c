#include "chttp.h"

#include <stdlib.h>
#include <string.h>

// Allocating space for a chttp_header.
chttp_header *chttp_header_allocate()
{
    chttp_header *h = (chttp_header *)malloc(sizeof(chttp_header));
    return h;
}

// Allocating the space for a chttp_header_set.
chttp_header_set *chttp_header_set_allocate()
{
    chttp_header_set *hs = (chttp_header_set *)malloc(sizeof(chttp_header_set));

    hs->size = 1;
    hs->len = 0;
    hs->headers = (chttp_header **)malloc(sizeof(chttp_header));

    return hs;
}

// Adding a header to the header set.
void chttp_add_header(chttp_header_set *set, char *header, char *value)
{
    if (set->len >= set->size)
    {
        set->size *= 2;
        chttp_header **new_headers = (chttp_header **)malloc(sizeof(chttp_header) * set->size);
        memcpy(new_headers, set->headers, set->len * sizeof(chttp_header *));
        free(set->headers);
        set->headers = new_headers;
    }

    chttp_header *h = chttp_header_allocate();
    strncpy(h->header, header, CHTTP_HEADER_KEY_LENGTH);
    strncpy(h->value, value, CHTTP_HEADER_VALUE_LENGTH);
    set->headers[set->len++] = h;
}

// Getting a header from the header set.
char *chttp_get_header(chttp_header_set *set, char *header)
{
    for (int i = 0; i < set->len; i++)
        if (strcmp(header, set->headers[i]->header) == 0)
            return set->headers[i]->value;
    return NULL;
}
