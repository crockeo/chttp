#ifndef _CHTTP_HTTP_H_
#define _CHTTP_HTTP_H_

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

#endif
