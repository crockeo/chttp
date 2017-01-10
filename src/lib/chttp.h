#ifndef _CHTTP_HTTP_H_
#define _CHTTP_HTTP_H_

#include <stdio.h>
#include "chttp_defines.h"

// Representing an HTTP header.
typedef struct
{
    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
} chttp_header;

// Representing the entire set of HTTP headers.
typedef struct
{
    int size;
    int len;
    chttp_header *headers;
} chttp_header_set;

// Allocating the space for a chttp_header_set.
chttp_header_set *chttp_header_set_allocate();

// Freeing a chttp_header_set.
void chttp_header_set_free(chttp_header_set *s);

// Adding a header to the header set.
void chttp_add_header(chttp_header_set *set, const char *header, const char *value);

// Getting a header from the header set.
char *chttp_get_header(chttp_header_set *set, const char *header);

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

// Printing a method to a string.
size_t chttp_sprint_method(chttp_method method, char *str, int len);

// Representing a request from a client.
typedef struct
{
    chttp_method method;
    char uri[CHTTP_URI_LENGTH];
    char http_version[CHTTP_HTTP_VERSION_LENGTH];

    chttp_header_set *headers;

    char body[CHTTP_BODY_LENGTH];
} chttp_request;

// Allocating the space for a chttp_request.
chttp_request *chttp_request_allocate();

// Freeing a chttp_request.
void chttp_request_free(chttp_request *r);

// Representing a response to a client.
typedef struct
{
    char http_version[CHTTP_HTTP_VERSION_LENGTH];
    int code;
    char reason_phrase[CHTTP_REASON_PHRASE_LENGTH];

    chttp_header_set *headers;

    char body[CHTTP_BODY_LENGTH];
} chttp_response;

// Allocating the space for a chttp_response.
chttp_response *chttp_response_allocate();

// Freeing a chttp_response.
void chttp_response_free(chttp_response *r);

// Parsing a chttp_request from a given string. Returns the number of characters
// read on success. Returns -1 on failure. Inverse of chttp_sprint_request.
size_t chttp_parse_request(chttp_request *r, FILE *f);

// Parsing a chttp_response from a given string. Returns the number of
// characters read on success. Returns -1 on failure. Inverse of
// chttp_sprint_response.
size_t chttp_parse_response(chttp_response *r, FILE *f);

// Pipes string to a FILE * and calls chttp_parse_request.
size_t chttp_sparse_request(chttp_request *r, const char *string, int len);

// Pipes string to a FILE * and falls chttp_parse_response.
size_t chttp_sparse_response(chttp_response *r, const char *string, int len);

// Printing a chttp_request to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_parse_request.
size_t chttp_sprint_request(chttp_request *r, char *string, int len);

// Printing a chttp_response to a given string. Returns the number of characters
// printed if there is enough room. If not, it returns -1. Inverse of
// chttp_parse_response.
size_t chttp_sprint_response(chttp_response *r, char *string, int len);

// Printing a chttp_request to FILE *f.
void chttp_fprint_request(FILE *f, chttp_request *r);

// Printing a chttp_response to FILE *f.
void chttp_fprint_response(FILE *f, chttp_response *r);

// Printing a chttp_request to stdout.
void chttp_print_request(chttp_request *r);

// Printing a chttp_response to stdout.
void chttp_print_response(chttp_response *r);

#endif
