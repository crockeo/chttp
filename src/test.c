#include "chttp.h"

#include <stdlib.h>
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
char *test_allocates()
{
    return NULL;
}

char *test_get()
{
    return NULL;
}

char *test_set()
{
    return NULL;
}

char *test_headers()
{
    chttp_run_test(allocates);
    chttp_run_test(get);
    chttp_run_test(set);

    return NULL;
}

////
// Parse
char *test_parse_request()
{
    return NULL;
}

char *test_parse_response()
{
    return NULL;
}

char *test_parse()
{
    chttp_run_test(parse_request);
    chttp_run_test(parse_response);

    return NULL;
}

///
// Print
char *test_sprint_request()
{
    return NULL;
}

char *test_fprint_request()
{
    return NULL;
}

char *test_sprint_response()
{
    return NULL;
}

char *test_fprint_response()
{
    return NULL;
}

char *test_print()
{
    chttp_run_test(sprint_request);
    chttp_run_test(fprint_request);
    chttp_run_test(sprint_response);
    chttp_run_test(fprint_response);

    return NULL;
}

////
// All
char *test_all()
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
