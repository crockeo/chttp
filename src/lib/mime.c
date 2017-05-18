#include "chttp.h"

#include <stdlib.h>
#include <string.h>

// chttp_validate_suffix
//   Parameters:
//     * suffix     - The suffix to check.
//     * suffix_len - The length of the suffix.
//
//   Description:
//     Checking a suffix for its validity.
//
//   Returns:
//     -1 if the suffix is invalid. 0 if the suffix is valid.
static int chttp_validate_suffix(const char *suffix, size_t suffix_len)
{
    for (int i = 0; i < suffix_len; i++)
        if (suffix[i] < 'a' || suffix[i] > 'z')
            return -1;
    return 0;
}

// chttp_uri_suffix
//   Parameters:
//     * uri - The URI to parse.
//     * len - The length of the URI.
//
//   Description:
//     Takes a URI and finds its suffix. Used in choosing MIME types.
//
//   Returns:
//     A pointer to the URI where the suffix begins. If the function cannot find
//     a suffix, it instead returns NULL.
const char *chttp_uri_suffix(const char *uri, size_t len)
{
    for (int i = len - 1; i >= 0; i--)
    {
        if (uri[i] == '.')
            return uri + i + 1;
    }

    return NULL;
}

// chttp_mime_map_allocate
//   Description:
//     Allocating and initializing a chttp_mime_map.
//
//   Returns:
//     The allocated chttp_mime_map.
chttp_mime_map *chttp_mime_map_allocate()
{
    chttp_mime_map *map = (chttp_mime_map *)malloc(sizeof(chttp_mime_map));

    memset(map->children, 0, 26 * sizeof(chttp_mime_map *));
    map->end = 0;
    map->type = NULL;
    map->type_len = 0;

    return map;
}

// chttp_mime_map_free
//   Parameters:
//     * map - The chttp_mime_map to free.
//
//   Description:
//     Frees a map and all of its children (recursively).
void chttp_mime_map_free(chttp_mime_map *map)
{
    for (int i = 0; i < 26; i++)
        if (map->children[i] != NULL)
            chttp_mime_map_free(map->children[i]);
    free(map);
}

// chttp_mime_map_add
//   Parameters:
//     * map        - chttp_mime_map.
//     * suffix     - Suffix to add.
//     * suffix_len - Length of suffix.
//     * type       - Type to add.
//     * type_len   - Length of type.
//
//   Description:
//     Adds a new entry to the chttp_mime_map.
void chttp_mime_map_add(chttp_mime_map *map, const char *suffix, size_t suffix_len, const char *type, size_t type_len)
{
    if (chttp_validate_suffix(suffix, suffix_len))
        return;

    chttp_mime_map *curr = map;
    int idx;
    for (int i = 0; i < suffix_len; i++)
    {
        idx = suffix[i] - 'a';
        if (curr->children[idx] == NULL)
            curr->children[idx] = chttp_mime_map_allocate();
        curr = curr->children[idx];
    }

    curr->end = 1;

    curr->type = (char *)malloc(sizeof(char) * type_len);
    strncpy(curr->type, type, type_len);
    curr->type_len = type_len;
}

// chttp_mime_map_get
//   Parameters:
//     * map        - chttp_mime_map
//     * suffix     - Suffix.
//     * suffix_len - Length of suffix.
//     * type_len   - Length of type to fill.
//
//   Description:
//     Returns the type and type length of a given MIME suffix.
//
//   Returns:
//     The type directly through the return signature, and the length of the
//     type through a pointer (*type_len) passed into the function.
const char *chttp_mime_map_get(chttp_mime_map *map, const char *suffix, size_t suffix_len, size_t *type_len)
{
    if (chttp_validate_suffix(suffix, suffix_len))
        return NULL;

    chttp_mime_map *curr = map;
    for (int i = 0; i < suffix_len && curr != NULL; i++)
        curr = curr->children[suffix[i] - 'a'];
    if (curr != NULL && curr->end)
        return curr->type;
    return NULL;
}

// chttp_mime_map_contains
//   Parameters:
//     * map        - chttp_mime_map
//     * suffix     - Suffix.
//     * suffix_len - Length of suffix.
//
//   Description:
//     Checking if the chttp_mime_map contains a given suffix.
//
//   Returns:
//     1 if the suffix exists, 0 otherwise.
int chttp_mime_map_contains(chttp_mime_map *map, const char *suffix, size_t suffix_len)
{
    size_t n;
    return chttp_mime_map_get(map, suffix, suffix_len, &n) != NULL;
}

// chttp_mime_init
//   Parameters:
//     * path - The path to the MIME types file.
//
//   Description:
//     Loads MIME definitions from the global MIME types file specified in the
//     path.
//
//   Returns:
//     -1 upon a failure, 0 on success. MIME types are cached globally.
int chttp_mime_map_init(chttp_mime_map *map, const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
        return -1;

    char *pos;
    int l;
    char line[256];
    while (fgets(line, 256, f))
    {
        if (feof(f))
            break;
        l = strlen(line);

        pos = strchr(line, ' ');
        chttp_mime_map_add(map, line, (size_t)(pos - line), pos + 1, (size_t)(l - (int)(pos - line) - 2));
    }

    return 0;
}

// chttp_uri_mime
//   Parameters:
//     * suffix     - The file suffix to check.
//     * suffix_len - The length of the file suffix.
//     * buf        - The buffer to output content type.
//     * buf_len    - The length of the buffer.
//
//   Description:
//     Takes a valid suffix (as generated by chttp_get_suffix), and returns its
//     appropriate MIME through writing it to buf.
//
//   Returns:
//     -1 upon error, either through size contraints or suffix being invalid.
//     0 upon success.
int chttp_uri_mime(chttp_mime_map *map, const char *suffix, size_t suffix_len, char *buf, size_t buf_len)
{
    if (map == NULL)
        return -1;
    size_t type_len;
    const char *type = chttp_mime_map_get(map, suffix, suffix_len, &type_len);
    if (type == NULL || type_len > buf_len)
        return -1;

    strncpy(buf, type, buf_len);
    return 0;
}
