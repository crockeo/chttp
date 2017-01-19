#ifndef _CHTTP_FMEMOPEN_H_
#define _CHTTP_FMEMOPEN_H_

#if defined(__APPLE__) || defined(__WIN32)
#include <ctype.h>
#include <stdio.h>

// fmemopen
//   Parameters:
//     * buffer - The buffer to open.
//     * len    - The length of the buffer.
//     * mode   - The mode, equivalent to fopen mode.
//
//   Description:
//     Opening a memory buffer as a FILE*.
//
//   Returns:
//     A digital FILE* pointer to the location in memory.
FILE *fmemopen(void *buffer, size_t len, const char *mode);
#endif

#endif
