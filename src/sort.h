/*
 * sort.h: Lines sorting module
 *
 * This module collects lines and prints them in sorted order.
 * Because of the typical usage, a single line can be wrapped in
 * two or more buffers (until the newline '\n' character is seen)
 * as well as the other way round, a single buffer can contain
 * more than one line:
 *
 * CSortPtr sorter;
 *
 * if (!(sorter = csort_new())) goto error;
 *
 * while (1) {
 *     char buf[100];
 *     ssize_t n = read(fd, buf, sizeof(buf));
 *
 *     if (n < 0) goto error;
 *     if (n == 0) break;
 *
 *     csort_add_buffer(&sorter, buf, n);
 * }
 * if (csort_output(&sorter, stdout) < 0) goto error;
 *
 * csort_free(sorter);
 *
 */

#include <stdio.h>

/* These are exposed just so that others can use this module. However, the
 * struct _CSort is to be implemented privately. */
typedef struct _CSort CSort;
typedef CSort *CSortPtr;

/**
 * csort_new:
 *
 * Create and initialize new sorting object.
 *
 * Returns: address to the object on success, NULL otherwise.
 */
CSortPtr csort_new(void);

/**
 * csort_add_buffer:
 * @sort: CSort object
 * @buf: buffer to add
 * @len: length of @buf
 *
 * Copies over @buf which is @len bytes long somewhere into @sort internals.
 */
void csort_add_buffer(CSortPtr sort, const char *buf, size_t len);

/**
 * csort_output:
 * @sort: CSort object
 * @fp: file to be written to
 *
 * For all the lines stored by previous calls of csort_add_buffer(), print them
 * sorted out alphabetically into the file @fp. If an error has occurred during
 * either csort_add_buffer() or csort_output() this function returns -1.
 *
 * Returns: 0 on success,
 *         -1 on failure (either csort_output() or csort_add_buffer())
 */
int csort_output(CSortPtr sort, FILE *fp);

/**
 * csort_free:
 * @sort: CSort object
 *
 * Free passed object. This function accepts NULL.
 */
void csort_free(CSortPtr sort);
