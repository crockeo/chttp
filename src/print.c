#include "chttp.h"

#include <string.h>

// Printing a method to a string.
int chttp_sprint_method(chttp_method method, char *str, int len)
{
    char *method_str;
    switch (method)
    {
    case OPTIONS: method_str = "OPTIONS"; break;
    case GET:     method_str = "GET";     break;
    case HEAD:    method_str = "HEAD";    break;
    case POST:    method_str = "POST";    break;
    case PUT:     method_str = "PUT";     break;
    case DELETE:  method_str = "DELETE";  break;
    case TRACE:   method_str = "TRACE";   break;
    case CONNECT: method_str = "CONNECT"; break;
    case OTHER:   method_str = "OTHER";   break;
    }

    return (int)(stpcpy(str, method_str) - str);
}

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
    char method[chttp_method_strlen];
    chttp_sprint_method(r->method, method, chttp_method_strlen);

    fprintf(f, "%s %s %s\n", method, r->uri, r->http_version);
    for (int i = 0; i < r->headers->len; i++)
        fprintf(f, "%s: %s\n", r->headers->headers[i]->header, r->headers->headers[i]->value);
    fprintf(f, "%s\n", r->body);
}

// Printing a chttp_response to FILE *f.
void chttp_fprint_response(FILE *f, chttp_response *r)
{
    fprintf(f, "%s %d %s\n", r->http_version, r->code, r->reason_phrase);
    for (int i = 0; i < r->headers->len; i++)
        fprintf(f, "%s: %s\n", r->headers->headers[i]->header, r->headers->headers[i]->value);
    fprintf(f, "%s\n", r->body);
}

// Printing a chttp_request to stdout.
void chttp_print_request(chttp_request *r) { chttp_fprint_request(stdout, r); }

// Printing a chttp_response to stdout.
void chttp_print_response(chttp_response *r) { chttp_fprint_response(stdout, r); }
