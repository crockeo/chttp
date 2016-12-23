#include "chttp.h"

// Printing a chttp_request to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_parse_request.
int chttp_sprint_request(chttp_request *r, char *string, int len)
{
    // TODO
    return -1;
}

// Printing a chttp_response to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_arse_response.
int chttp_sprint_response(chttp_response *r, char *string, int len)
{
    // TODO
    return -1;
}

// Printing a chttp_request to FILE *f.
void chttp_fprint_request(FILE *f, chttp_request *r)
{
    // TODO
}

// Printing a chttp_response to FILE *f.
void chttp_fprint_response(FILE *f, chttp_response *r)
{
    // TODO
}

// Printing a chttp_request to stdout.
void chttp_print_request(chttp_request *r) { chttp_fprint_request(stdout, r); }

// Printing a chttp_response to stdout.
void chttp_print_response(chttp_response *r) { chttp_fprint_response(stdout, r); }
