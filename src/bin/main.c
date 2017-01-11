#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <getopt.h>
#else
#error "Can only run chttp_server on a POSIX operating system."
#endif

// chttp_print_error
//   Parameters:
//     * f - File pointer to print to.
//
//   Description:
//     Printing the program's error page.
void chttp_print_error(FILE *f)
{
    fprintf(f, "chttp_server: use '--help' to view help page.\n");
}

// chttp_print_help
//   Parameters:
//     * f - File pointer to print to.
//
//   Description:
//     Printing the program's help page.
void chttp_print_help(FILE *f)
{
    fprintf(f, "chttp_server use:\n");
    fprintf(f, "  --help          Display this page.\n");
    fprintf(f, "  --address (-a)  Set the IP address (\"all\"=listen on all addresses.)\n");
    fprintf(f, "  --port (-p)     Set the port.\n");
}

// chttp_server_args
//   Description:
//     Structured container for server arguments.
struct chttp_server_args
{
    char address[16];
    uint16_t port;
    bool help;
};
typedef struct chttp_server_args chttp_server_args;

// chttp_server_args_parse
//   Parameters:
//     * argc - Argument count.
//     * argv - Argument list pointer.
//     * args - Argument struct to populate.
//
//   Description:
//     Uses POSIX getopt to parse information from a program's arguments into a
//     formatted args structure.
//
//   Returns:
//     -1 if error. 0 if success.
int chttp_server_args_parse(int argc, char **argv, chttp_server_args *args)
{
    memset(args, 0, sizeof(chttp_server_args));
    strncpy(args->address, "all", 16);
    args->port = 3000;

    struct option options[] =
    {
        { "help", no_argument, (int *)&args->help, 1 },

        { "address", required_argument, 0, 'a' },
        { "port"   , required_argument, 0, 'p' },

        { 0, 0, 0, 0 }
    };

    int idx = 0;
    int c;
    while ((c = getopt_long(argc, argv, "a:p:", options, &idx)) >= 0)
    {
        switch (c)
        {
        case 0:
            break;
        case 'a':
            strncpy(args->address, optarg, 16);
            break;
        case 'p':
            args->port = atoi(optarg);
        default:
            chttp_print_error(stdout);
            break;
        }
    }

    return 0;
}

// chttp_server_args_validate
//   Parameters:
//     * args - pointer to a chttp_server_args structure to validate.
//
//   Description:
//     Checks a chttp_server_args for invalid data.
//
//   Returns:
//     -1 if invalid. 0 if valid.
int chttp_server_args_validate(chttp_server_args *args)
{
    // TODO
    return 0;
}

// main
//   Parameters:
//     * argc - Program-passed argument count.
//     * argv - Program-passed argument list.
int main(int argc, char **argv)
{
    chttp_server_args args;
    if (chttp_server_args_parse(argc, argv, &args))
    {
        chttp_print_error(stderr);
        return 1;
    }

    if (chttp_server_args_validate(&args))
    {
        chttp_print_error(stderr);
        return 1;
    }

    if (args.help)
    {
        chttp_print_help(stdout);
        return 0;
    }

    printf("%s %d\n", args.address, args.port);
    // TODO

    return 0;
}
