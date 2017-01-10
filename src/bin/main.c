#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#else
#error "Can only run chttp_server on a POSIX operating system."
#endif

// chttp_server_args
//   Description:
//     Structured container for server arguments.
struct chttp_server_args
{
    char address[16];
    uint16_t port;
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
    strncpy(args->address, "127.0.0.1", 16);
    args->port = 3000;

    int c;
    while ((c = getopt(argc, argv, "a:p:")) >= 0)
    {
        switch (c)
        {
        case 'a':
            strncpy(args->address, "127.0.0.1", 16);
            break;
        case 'p':
            args->port = atoi(optarg);
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
        printf("Failed to parse args.\n");
        return 1;
    }

    if (chttp_server_args_validate(&args))
    {
        printf("Failed to validate args.\n");
        return 1;
    }

    printf("%s %d\n", args.address, args.port);
    // TODO

    return 0;
}
