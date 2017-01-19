#include "chttp.h"

#include <string.h>
#include <ctype.h>

#include "chttp_fmemopen.h"

// Filling a token (as defined by whitespace) for parsing.
static size_t fill_token(FILE *f, char *buf, size_t len)
{
    char c;
    while (isspace(c = fgetc(f)) && !feof(f)) { }
    ungetc(c, f);

    size_t i;
    for (i = 0; i < len; i++)
    {
        c = fgetc(f);
        if (c == '\0' || isspace(c))
        {
            buf[i] = '\0';
            if (isspace(c))
                ungetc(c, f);
            break;
        }

        buf[i] = c;
    }

    return i;
}

#define chttp_pmp(buf, method_var, method) \
    do { if (strcmp(buf, #method) == 0) method_var = method; } while (0);

// Parsing out a chttp_method from a FILE.
static size_t parse_method(FILE *f, chttp_method *method)
{
    const int l = CHTTP_METHOD_LENGTH + 1;
    char buf[CHTTP_METHOD_LENGTH];
    size_t len = fill_token(f, buf, l);

    *method = OTHER;
    chttp_pmp(buf, *method, OPTIONS);
    chttp_pmp(buf, *method, GET);
    chttp_pmp(buf, *method, HEAD);
    chttp_pmp(buf, *method, POST);
    chttp_pmp(buf, *method, PUT);
    chttp_pmp(buf, *method, DELETE);
    chttp_pmp(buf, *method, TRACE);
    chttp_pmp(buf, *method, CONNECT);

    return len;
}

// Parsing a chttp_request from a given string. Returns the number of characters
// read on success. Returns -1 on failure. Inverse of chttp_sprint_request.
size_t chttp_parse_request(chttp_request *r, FILE *f)
{
    size_t start = ftell(f);
    parse_method(f, &r->method);

    fill_token(f, r->uri, CHTTP_URI_LENGTH);
    fill_token(f, r->http_version, CHTTP_HTTP_VERSION_LENGTH);

    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
    size_t e = 0;
    while (!feof(f))
    {
        e = fill_token(f, header, CHTTP_HEADER_KEY_LENGTH);
        if (header[e - 1] != ':')
            break;
        header[e - 1] = '\0';
        fill_token(f, value, CHTTP_HEADER_VALUE_LENGTH);

        chttp_add_header(r->headers, header, value);
    }

    memcpy(r->body, header, e);
    size_t n = fread(r->body + e, sizeof(char), CHTTP_BODY_LENGTH - 1, f);
    (r->body + e)[n] = '\0';

    return ftell(f) - start;
}

// Parsing a chttp_response from a given string. Returns the number of
// characters read on success. Returns -1 on failure. Inverse of
// chttp_sprint_response.
size_t chttp_parse_response(chttp_response *r, FILE *f)
{
    size_t start = ftell(f);
    fill_token(f, r->http_version, CHTTP_HTTP_VERSION_LENGTH);
    fscanf(f, "%d", &r->code);
    fill_token(f, r->reason_phrase, CHTTP_REASON_PHRASE_LENGTH);

    char header[CHTTP_HEADER_KEY_LENGTH];
    char value[CHTTP_HEADER_VALUE_LENGTH];
    size_t e = 0;
    while (!feof(f))
    {
        e = fill_token(f, header, CHTTP_HEADER_KEY_LENGTH);
        if (header[e - 1] != ':')
            break;
        header[e - 1] = '\0';
        fill_token(f, value, CHTTP_HEADER_VALUE_LENGTH);

        chttp_add_header(r->headers, header, value);
    }

    memcpy(r->body, header, e);
    size_t n = fread(r->body + e, sizeof(char), CHTTP_BODY_LENGTH - 1, f);
    (r->body + e)[n] = '\0';

    return ftell(f) - start;
}

// Pipes string to a FILE * and calls chttp_parse_request.
size_t chttp_sparse_request(chttp_request *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_request(r, f);
    fclose(f);
    return n;
}

// Pipes string to a FILE * and falls chttp_parse_response.
size_t chttp_sparse_response(chttp_response *r, const char *string, int len)
{
    FILE *f = fmemopen((char *)string, len, "r");
    if (f == NULL)
        return -1;

    int n = chttp_parse_response(r, f);
    fclose(f);
    return n;
}
