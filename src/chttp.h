#ifndef _CHTTP_HTTP_H_
#define _CHTTP_HTTP_H_

#include <stdio.h>

// Representing an HTTP header.
typedef struct
{
    char *header;
    char *value;
} chttp_header;

// Allocating space for a chttp_header.
chttp_header *chttp_header_allocate();

// Representing the entire set of HTTP headers.
typedef struct
{
    int size;
    int len;
    chttp_header **headers;
} chttp_header_set;

// Allocating the space for a chttp_header_set.
chttp_header_set *chttp_header_set_allocate();

// Adding a header to the header set.
void chttp_add_header(chttp_header_set *set,  char *header,  char *value);

// Getting a header from the header set.
char *chttp_get_header(chttp_header_set *set,  char *header);

// The set of methods a client can request.
typedef enum
{
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    OTHER
} chttp_method;

// Representing a request from a client.
typedef struct
{
    chttp_method method;
    char *uri;
    char *http_version;

    chttp_header_set *headers;

    char *body;
} chttp_request;

// Allocating the space for a chttp_request.
chttp_request *chttp_request_allocate();

// Representing a response to a client.
typedef struct
{
    char *http_version;
    int code;
    char *reason_phrase;

    chttp_header_set *headers;

    char *body;
} chttp_response;

// Allocating the space for a chttp_response.
chttp_response *chttp_response_allocate();

// Parsing a chttp_request from a given string. Returns the number of characters
// read on success. Returns -1 on failure. Inverse of chttp_sprint_request.
int chttp_parse_request(chttp_request *r, const char *string, int len);

// Parsing a chttp_response from a given string. Returns the number of
// characters read on success. Returns -1 on failure. Inverse of
// chttp_sprint_response.
int chttp_parse_response(chttp_response *r, const char *string, int len);

// Printing a chttp_request to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_parse_request.
int chttp_sprint_request(chttp_request *r, char *string, int len);

// Printing a chttp_response to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_arse_response.
int chttp_sprint_response(chttp_response *r, char *string, int len);

// Printing a chttp_request to FILE *f.
void chttp_fprint_request(FILE *f, chttp_request *r);

// Printing a chttp_response to FILE *f.
void chttp_fprint_response(FILE *f, chttp_response *r);

// Printing a chttp_request to stdout.
void chttp_print_request(chttp_request *r);

// Printing a chttp_response to stdout.
void chttp_print_response(chttp_response *r);

#endif
