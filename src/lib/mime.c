#include "chttp.h"

// TODO

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
const char *chttp_uri_suffix(const char *uri, size_t len)
{
    return NULL;
}

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
int chttp_uri_mime(const char *suffix, size_t suffix_len, char *buf, size_t buf_len)
{
    return -1;
}