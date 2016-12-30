#include "chttp.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define chttp_assert(message, test) do { if (!(test)) return message; } while (0)
#define chttp_run_test(test_fn) do { \
    printf("- Running test: " #test_fn ".\n"); \
    tests_run++; \
    char *message = test_##test_fn(); \
    if (message) return message; \
} while (0)

static int tests_run = 0;

////
// Header
static char *test_allocates()
{
    chttp_header_set *set = chttp_header_set_allocate();
    chttp_assert("Header set is improperly sized.", set->size == 1);
    chttp_assert("Header set has an improper length.", set->len == 0);
    free(set);

    return NULL;
}

static char *test_add()
{
    chttp_header_set *set = chttp_header_set_allocate();

    const char *test_key = "test_key";
    const char *test_value = "test_value";

    for (int i = 0; i < 5; i++)
        chttp_add_header(set, test_key, test_value);

    chttp_assert("Header set is improperly sized.", set->size == 8);
    chttp_assert("Header set has an improper length.", set->len == 5);
    chttp_assert("Header does not contain the proper key.", strcmp(set->headers[0].header, test_key) == 0);
    chttp_assert("Header does not contain the proper value.", strcmp(set->headers[0].value, test_value) == 0);

    free(set);

    return NULL;
}

static char *test_get()
{
    chttp_header_set *set = chttp_header_set_allocate();

    char a[1];
    for (int i = 0; i < 26; i++)
    {
        a[0] = 'a' + i;
        chttp_add_header(set, a, a);
    }

    char *r;
    for (int i = 0; i < 26; i++)
    {
        a[0] = 'a' + i;
        r = chttp_get_header(set, a);
        chttp_assert("Returned the wrong value.", strcmp(r, a) == 0);
    }

    free(set);

    return NULL;
}

static char *test_headers()
{
    chttp_run_test(allocates);
    chttp_run_test(add);
    chttp_run_test(get);

    return NULL;
}

////
// Parse
static char *test_parse_request()
{
    return NULL;
}

static char *test_parse_response()
{
    return NULL;
}

static char *test_parse()
{
    chttp_run_test(parse_request);
    chttp_run_test(parse_response);

    return NULL;
}

///
// Print
static char *test_sprint_request()
{
    return NULL;
}

static char *test_fprint_request()
{
    return NULL;
}

static char *test_sprint_response()
{
    return NULL;
}

static char *test_fprint_response()
{
    return NULL;
}

static char *test_print()
{
    chttp_run_test(sprint_request);
    chttp_run_test(fprint_request);
    chttp_run_test(sprint_response);
    chttp_run_test(fprint_response);

    return NULL;
}

////
// All
static char *test_all()
{
    chttp_run_test(headers);
    chttp_run_test(parse);
    chttp_run_test(print);

    return NULL;
}

////
// Main
int main(int argc, char **argv)
{
    printf("Starting test suite...\n");
    char *message = test_all();
    printf("Tests ran: %d\n", tests_run);

    if (message)
    {
        printf("Failed with message: %s\n", message);
        return 1;
    }

    return 0;
}
