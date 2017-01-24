#ifndef _CHTTP_HTTP_H_
#define _CHTTP_HTTP_H_

#include <stdio.h>

#include "chttp_defines.h"

// chttp_header
//   Struct containing header/value key-pair for a given header.
typedef struct
{
    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
} chttp_header;

// chttp_header_set
//   Struct used as a front-end for a "header_set" whose "methods" are
//   implemented below. Internal values should NOT be used, as they are subject
//   to change and optimization.
typedef struct
{
    int size;
    int len;
    chttp_header *headers;
} chttp_header_set;

// chttp_header_set_fill
//   Parameters:
//     * s - The header set to fill.
//
//   Description:
//     Filling a chttp_header_set with data *in-place*, without allocating the
//     structure itself. Allows programmers to fill data into the header set
//     without dynamically allocating the struct itself.
void chttp_header_set_fill(chttp_header_set *s);

// chttp_header_set_allocate
//   Description:
//     Allocates space for a chttp_header_set and calls chttp_header_set_fill
//     on it.
//
//   Returns:
//     The allocated chttp_header_set.
chttp_header_set *chttp_header_set_allocate();

// chttp_header_set_free
//   Parameters:
//     * s - The header set to free.
//
//   Description:
//     Freeing a header set pointer. Assumes it must also free the data within
//     the header set.
void chttp_header_set_free(chttp_header_set *s);

// chttp_add_header
//   Parameters:
//     * set    - Pointer to the header set.
//     * header - Header key.
//     * value  - Header value.
//
//   Description:
//     Adds a header to the header set.
void chttp_add_header(chttp_header_set *set, const char *header, const char *value);

// chttp_get_header
//   Parameters:
//     * set    - The header set.
//     * header - Header key.
//
//   Description:
//     Retrieves a header specified from the header set.
//
//   Returns:
//     NULL if the header is not found. Otherwise, a string with the header's
//     value.
char *chttp_get_header(chttp_header_set *set, const char *header);

// chttp_method
//   Enumeration of all possible HTTP methods. Used in place of a string (e.g.
//   "OPTIONS" and "GET") for type-safety.
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

// chttp_sprint_method
//   Parameters:
//     * method - The HTTP method to print.
//     * str    - The string to print.
//     * len    - The length of the buffer described by str.
//
//   Description:
//     Prints the passed chttp_method into the str.
//
//   Returns:
//     The number of characters printed into the string.
size_t chttp_sprint_method(chttp_method method, char *str, int len);

// chttp_request
//   Modeling the data passed from a browser requesting a given page. Used in
//   tandem with the parse functions below to deal with HTTP requests.
typedef struct
{
    chttp_method method;
    char uri[CHTTP_URI_LENGTH];
    char http_version[CHTTP_HTTP_VERSION_LENGTH];

    chttp_header_set *headers;

    char body[CHTTP_BODY_LENGTH];
} chttp_request;

// chttp_request_fill
//   Parameters:
//     * r - The request to fill.
//
//   Description:
//     Fills a chttp_request with informaion in-place (that is, sans
//     allocation outside of allocating a header_set).
void chttp_request_fill(chttp_request *r);

// chttp_request_allocate
//   Description:
//     Allocates a chttp_request and calls chttp_request_fill on that newly
//     created pointer.
//
//   Returns:
//     Returns the allocating chttp_request.
chttp_request *chttp_request_allocate();

// chttp_request_free
//   Parameters:
//     * r - The request to free.
//
//   Description:
//     Freeing a chttp_request.
void chttp_request_free(chttp_request *r);

// chttp_response
//   Modeling the data passed from a server would pass back to the client. Used
//   in tandem with the print functions below to deal with HTTP responses.
typedef struct
{
    char http_version[CHTTP_HTTP_VERSION_LENGTH];
    int code;
    char reason_phrase[CHTTP_REASON_PHRASE_LENGTH];

    chttp_header_set *headers;

    char body[CHTTP_BODY_LENGTH];
} chttp_response;

// chttp_response_fill
//   Parameters:
//     * r - The response to fill.
//
//   Description:
//     Fills a chttp_response with informaion in-place (that is, sans
//     allocation outside of allocating a header_set).
void chttp_response_fill(chttp_response *r);

// chttp_response_allocate
//   Description:
//     Allocates a chttp_response and calls chttp_response_fill on that newly
//     created pointer.
//
//   Returns:
//     Returns the allocating chttp_response.
chttp_response *chttp_response_allocate();

// chttp_response_free
//   Parameters:
//     * r - The response to free.
//
//   Description:
//     Freeing a chttp_response.
void chttp_response_free(chttp_response *r);

// chttp_parse_request
//   Parameters:
//     * r - The request to fill.
//     * f - The file pointer to read.
//
//   Description:
//     Parsing a chttp_request from a FILE pointer. Inverse of
//     chttp_sprint_request.
//
//   Returns:
//     The number of characters read on success. Returns -1 on failure.
size_t chttp_parse_request(chttp_request *r, FILE *f);

// chttp_parse_response
//   Parameters:
//     * r - The response to fill.
//     * f - The file pointer to read.
//
//   Description:
//     Parsing a chttp_response from a FILE pointer. Inverse of
//     chttp_sprint_response.
//
//   Returns:
//     The number of characters read on success. Returns -1 on failure.
size_t chttp_parse_response(chttp_response *r, FILE *f);

// chttp_sparse_request
//   Parameters:
//     * r      - The request to fill.
//     * string - The string to parse.
//     * int    - The maximum length of the string.
//
//   Description:
//     Parsing a chttp_request from a given string. Inverse of
//     chttp_sprint_response.
//
//   Returns:
//     The number of characters read from the string. Returns -1 on failure.
size_t chttp_sparse_request(chttp_request *r, const char *string, int len);

// chttp_sparse_response
//   Parameters:
//     * r      - The response to fill.
//     * string - The string to parse.
//     * len    - The maximum length of the string.
//
//   Description:
//     Parsing a chttp_response from a given string. Inverse of
//     chttp_sprint_response.
//
//   Returns:
//     The number of characters read from the string. Returns -1 on failure.
size_t chttp_sparse_response(chttp_response *r, const char *string, int len);

// chttp_sprint_request
//   Parameters:
//     * r      - Request to print.
//     * string - Buffer printed to.
//     * len    - Maximum length of the string.
//
//   Description:
//     Printing the value of a request to a string. Inverse of
//     chttp_sparse_request.
//
//   Returns:
//     The number of characters printed into the strong. Returns -1 on failure.
size_t chttp_sprint_request(chttp_request *r, char *string, int len);

// chttp_sprint_response
//   Parameters:
//     * r      - Request to print.
//     * string - Buffer printed to.
//     * len    - Maximum length of the string.
//
//   Description:
//     Printing the value of a response to a string. Inverse of
//     chttp_sparse_response.
//
//   Returns:
//     The number of characters printed into the strong. Returns -1 on failure.
size_t chttp_sprint_response(chttp_response *r, char *string, int len);

// chttp_fprint_request
//   Parameters:
//     * f - The file to print to.
//     * r - The request to print.
//
//   Description:
//     Prints a request out to a file.
size_t chttp_fprint_request(FILE *f, chttp_request *r);

// chttp_fprint_response
//   Parameters:
//     * f - The file to print to.
//     * r - The response to print.
//
//   Description:
//     Prints a response out to a file.
size_t chttp_fprint_response(FILE *f, chttp_response *r);

// chttp_print_request
//   Parameters:
//     * r - The request to print.
//
//   Description:
//     Prints a request out to stdout.
size_t chttp_print_request(chttp_request *r);

// chttp_print_response
//   Parameters:
//     * r - The response to print.
//
//   Description:
//     Prints a response out to stdout.
size_t chttp_print_response(chttp_response *r);

// chttp_uri_suffix
//   Parameters:
//     * uri -
//     * len -
//
//   Description:
//     Takes a URI and finds its suffix. Used in choosing MIME types.
//
//   Returns:
//     A pointer to the URI where the suffix begins. If the function cannot find
//     a suffix, it instead returns NULL.
const char *chttp_uri_suffix(const char *uri, size_t len);

// chttp_uri_mime
//   Parameters:
//     * suffix -
//     * len    -
//
//   Description:
//     Takes a valid suffix (as generated by chttp_get_suffix), and returns its
//     appropriate MIME through writing it to buf.
//
//   Returns:
//     -1 upon error, either through size contraints or suffix being invalid.
//     0 upon success.
int chttp_uri_mime(const char *suffix, size_t suffix_len, char *buf, size_t buf_len);

#endif
