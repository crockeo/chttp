#include "chttp.h"

#include <stdarg.h>
#include <string.h>

// Printing a method to a string.
size_t chttp_sprint_method(chttp_method method, char *str, int len)
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

    return (size_t)(stpcpy(str, method_str) - str);
}

// Utility for printing iteratively into a string.
static int chttp_sprint(char *dst, int len, const char *format, size_t *n, ...)
{
    va_list arg_list;
    va_start(arg_list, n);
    size_t v = vsnprintf(dst + *n, len - *n, format, arg_list);
    va_end(arg_list);
    if (v > (len - *n))
        return 1;
    *n += v;
    return 0;
}

// Printing a chttp_request to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_parse_request.
size_t chttp_sprint_request(chttp_request *r, char *string, int len)
{
    size_t n = 0;

    char method[CHTTP_METHOD_LENGTH];
    chttp_sprint_method(r->method, method, CHTTP_METHOD_LENGTH);

    if (chttp_sprint(string, len, "%s %s %s\r\n", &n, method, r->uri, r->http_version))
        return -1;
    for (int i = 0; i < r->headers->len; i++)
        if (chttp_sprint(string, len, "%s: %s\r\n", &n, r->headers->headers[i].header, r->headers->headers[i].value))
            return -1;
    if (chttp_sprint(string, len, "\r\n", &n))
        return -1;
    if (chttp_sprint(string, len, "%s\r\n", &n, r->body))
        return -1;

    if (n == len)
        return -1;
    string[n] = '\0';
    return n + 1;
}

// Printing a chttp_response to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_arse_response.
size_t chttp_sprint_response(chttp_response *r, char *string, int len)
{
    size_t n = 0;

    if (chttp_sprint(string, len, "%s %d %s\r\n", &n, r->http_version, r->code, r->reason_phrase))
        return -1;
    for (int i = 0; i < r->headers->len; i++)
        if (chttp_sprint(string, len, "%s: %s\r\n", &n, r->headers->headers[i].header, r->headers->headers[i].value))
            return -1;
    if (chttp_sprint(string, len, "\r\n", &n))
        return -1;
    if (chttp_sprint(string, len, "%s\r\n", &n, r->body))
        return -1;

    if (n == len)
        return -1;
    string[n] = '\0';
    return n + 1;
}

// Printing a chttp_request to FILE *f.
size_t chttp_fprint_request(FILE *f, chttp_request *r)
{
    size_t n = 0;

    char method[CHTTP_METHOD_LENGTH];
    n += chttp_sprint_method(r->method, method, CHTTP_METHOD_LENGTH);

    n += fprintf(f, "%s %s %s\n", method, r->uri, r->http_version);
    for (int i = 0; i < r->headers->len; i++)
        n += fprintf(f, "%s: %s\n", r->headers->headers[i].header, r->headers->headers[i].value);
    n += fprintf(f, "%s\n", r->body);

    return n;
}

// Printing a chttp_response to FILE *f.
size_t chttp_fprint_response(FILE *f, chttp_response *r)
{
    size_t n = 0;

    n += fprintf(f, "%s %d %s\n", r->http_version, r->code, r->reason_phrase);
    for (int i = 0; i < r->headers->len; i++)
        n += fprintf(f, "%s: %s\n", r->headers->headers[i].header, r->headers->headers[i].value);
    n += fprintf(f, "%s\n", r->body);

    return n;
}

// Printing a chttp_request to stdout.
size_t chttp_print_request(chttp_request *r) { return chttp_fprint_request(stdout, r); }

// Printing a chttp_response to stdout.
size_t chttp_print_response(chttp_response *r) { return chttp_fprint_response(stdout, r); }
