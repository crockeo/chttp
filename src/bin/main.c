#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#else
#error "Can only run chttp_server on a POSIX operating system."
#endif

#include "../lib/chttp.h"

// chttp_print_error
//   Parameters:
//     * f - File pointer to print to.
//
//   Description:
//     Printing the program's error page.
void chttp_print_error(FILE *f, const char *message)
{
    if (errno != 0)
    {
        fprintf(f, "chttp_server: %s\n", strerror(errno));
        errno = 0;
    }

    fprintf(f, "chttp_server: %s\n", message);
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
    int backlog;
    bool help;
    bool verbose;
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
        { "verbose", no_argument, (int *)&args->verbose, 1 },

        { "address", required_argument, 0, 'a' },
        { "port"   , required_argument, 0, 'p' },

        { 0, 0, 0, 0 }
    };

    int idx = 0;
    int c;
    while ((c = getopt_long(argc, argv, "hva:p:", options, &idx)) >= 0)
    {
        switch (c)
        {
        case 0:
            break;
        case 'h':
            args->help = 1;
            break;
        case 'v':
            args->verbose = 1;
            break;
        case 'a':
            strncpy(args->address, optarg, 16);
            break;
        case 'p':
            args->port = atoi(optarg);
        default:
            return 1;
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
int chttp_server_args_validate(chttp_server_args args)
{
    // TODO
    return 0;
}

// chttp_kill_socket
//   Parameters:
//     * sock - The socket to close.
//
//   Description:
//     Kills and closes the socket described by 'sock'.
void chttp_kill_socket(int sock)
{
    // TODO: Anything else?
    close(sock);
}

// chttp_config_sockaddr
//   Parameters:
//     * args      - Application arguments.
//     * serv_addr - A pointer to the server address to configure.
//
//   Description:
//     Configures a socket address struct
//
//   Returns:
//     -1 on error. 0 on success.
int chttp_config_sockaddr(chttp_server_args args, struct sockaddr_in *serv_addr)
{
    memset(serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port   = htons(args.port);
    if (strcmp(args.address, "all") == 0)
        serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        serv_addr->sin_addr.s_addr = inet_addr(args.address);

    return 0;
}

// chttp_create_server
//   Parameters:
//     * args - Configuring the server through command line arguments.
//     * sock - Socket to output.
//
//   Description:
//     Configuring and opening the socket for use with the server.
//
//   Returns:
//     -1 on error. 0 on success.
int chttp_create_server(chttp_server_args args, int *sock)
{
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1)
        return -1;

    int opt = 1;
    if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        chttp_kill_socket(*sock);
        return -1;
    }

    // Configuring the socket address.
    struct sockaddr_in serv_addr;
    if (chttp_config_sockaddr(args, &serv_addr) < 0)
    {
        chttp_kill_socket(*sock);
        return -1;
    }

    if (bind(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        chttp_kill_socket(*sock);
        return -1;
    }

    if (listen(*sock, args.backlog) < 0)
    {
        chttp_kill_socket(*sock);
        return -1;
    }

    return 0;
}

// chttp_client
//   Description:
//     Structure to pass along client information to a response thread.
struct chttp_client
{
    socklen_t addr_size;
    int sock;
    struct sockaddr_in addr;
};
typedef struct chttp_client chttp_client;

// chttp_respond
//   Parameters:
//     * cli - Client pointer. Contains all information from accepting the
//             socket.
//
//   Description:
//     Handling a request from a given client. Under default behavior, sends a
//     given file back to a client with appropriate headers.
void *chttp_respond(void *arg)
{
    chttp_client *cli = (chttp_client *)arg;
    pthread_detach(pthread_self());

    FILE *sock = fdopen(cli->sock, "rw");
    if (!sock)
        return NULL;

    chttp_request req;
    chttp_request_fill(&req);
    chttp_parse_request(&req, sock);

    const int output_length = CHTTP_BODY_LENGTH + 1024;
    char output[output_length];

    FILE *f = fopen(req.uri, "r");
    if (!f)
    {
        chttp_response res =
        {
            .http_version = "HTTP/1.1",
            .code = 404,
            .reason_phrase = "File not found.",

            .headers = NULL
        };

        sprintf(res.body, "Error 404, file not found: %s", req.uri);
        chttp_sprint_response(&res, output, output_length);
    } else
    {
        chttp_response res;
        chttp_response_fill(&res);

        strcpy(res.http_version, "HTTP/1.1");
        res.code = 200;
        strcpy(res.reason_phrase, "OK");

        fread(res.body, 1, CHTTP_BODY_LENGTH, f);
        chttp_sprint_response(&res, output, output_length);
        fclose(f);
    }

    // TODO: Using strlen instead of output_length to make sure I don't send out
    //       extra 0s?
    fwrite(output, 1, strlen(output), sock);

    fclose(sock);
    chttp_kill_socket(cli->sock);
    free(cli);

    pthread_exit(NULL);
}

// chttp_handle_client
//   Parameters:
//     * cli - Client pointer. Contains all information from accepting the
//             socket.
//
//   Description:
//     Spawns a thread to handle the given client connection.
//
//   Returns:
//     -1 if error. 0 if success.
int chttp_handle_client(chttp_client *cli)
{
    pthread_t thread;
    if (pthread_create(&thread, NULL, &chttp_respond, (void *)cli))
    {
        return 1;
    }

    return 0;
}

// main
//   Parameters:
//     * argc - Program-passed argument count.
//     * argv - Program-passed argument list.
int main(int argc, char **argv)
{
    errno = 0;
    chttp_server_args args;
    if (chttp_server_args_parse(argc, argv, &args))
    {
        chttp_print_error(stderr, "Failed to parse args.");
        return 1;
    }

    if (chttp_server_args_validate(args))
    {
        chttp_print_error(stderr, "Failed to validate args.");
        return 1;
    }

    if (args.help)
    {
        chttp_print_help(stdout);
        return 0;
    }

    if (args.verbose)
    {
        printf("Starting server with:\n");
        printf("  Address: %s\n", args.address);
        printf("  Port: %u\n", args.port);
        printf("  Backlog: %d\n", args.backlog);
        printf("  Help: %d\n", args.help);
        printf("  Verbose: %d\n", args.verbose);
    }

    int sock;
    if (chttp_create_server(args, &sock))
    {
        chttp_print_error(stderr, "Failed to create server.");
        return 1;
    }

    chttp_client *cli;
    while (1)
    {
        cli = (chttp_client *)malloc(sizeof(chttp_client));
        memset(cli, 0, sizeof(chttp_client));

        cli->addr_size = sizeof(cli->addr);
        cli->sock = accept(sock, (struct sockaddr *)&cli->addr, &cli->addr_size);
        if (args.verbose)
            printf("Accepted connection!\n");
        chttp_handle_client(cli);
    }

    printf("%s %d\n", args.address, args.port);

    return 0;
}
