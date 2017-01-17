#include "chttp.h"

#include <stdlib.h>
#include <string.h>

// Filling a request.
void chttp_request_fill(chttp_request *r)
{
    memset(r, 0, sizeof(chttp_request));
    r->headers = chttp_header_set_allocate();
}

// Allocating the space for a chttp_request.
chttp_request *chttp_request_allocate()
{
    chttp_request *r = (chttp_request *)malloc(sizeof(chttp_request));
    chttp_request_fill(r);
    return r;
}

// Freeing a chttp_request.
void chttp_request_free(chttp_request *r)
{
    chttp_header_set_free(r->headers);
    free(r);
}

// Filling a response.
void chttp_response_fill(chttp_response *r)
{
    memset(r, 0, sizeof(chttp_response));
    r->headers = chttp_header_set_allocate();
}

// Allocating the space for a chttp_response.
chttp_response *chttp_response_allocate()
{
    chttp_response *r = (chttp_response *)malloc(sizeof(chttp_response));
    chttp_response_fill(r);
    return r;
}

// Freeing a chttp_response.
void chttp_response_free(chttp_response *r)
{
    chttp_header_set_free(r->headers);
    free(r);
}
