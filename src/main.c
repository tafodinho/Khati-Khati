/*
 * Sort
 *
 * This is the main program that should allow you to test your solution.
 * You should not need to modify anything here.
 */

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sort.h"

static void
print_usage(const char *progname)
{
    const char *unified_progname;

    if (!(unified_progname = strrchr(progname, '/')))
        unified_progname = progname;
    else
        unified_progname++;

    printf("\n%s [options] [FILE]\n\n"
           "  options:\n"
           "    -h | --help         print this help\n"
           "    -f | --file         read lines from FILE\n"
           "\n"
           "Sort lines of text files. Use '-' to denote STDIN\n",
           unified_progname);
}

int main(int argc, char *argv[])
{
    int ret = EXIT_FAILURE;
    const char *fileIn = NULL;
    int fd = -1;
    CSortPtr sorter;
    int arg;
    struct option opt[] = {
        {"help", no_argument, NULL, 'h'},
        {"file", required_argument, NULL, 'f'},
        {NULL, 0, NULL, 0}
    };

    /* Parse commandline arguments */
    while ((arg = getopt_long(argc, argv, "+:hf:", opt, NULL)) != -1) {
        switch (arg) {
        case 'h':
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 'f':
            fileIn = optarg;
            break;
        case ':':
            fprintf(stderr, "Option '-%c' requires an argument\n", optopt);
            exit(EXIT_FAILURE);
        case '?':
            if (optopt)
                fprintf(stderr, "Unsupported option '-%c'. See --help.\n", optopt);
            else
                fprintf(stderr, "Unsupported option '%s'. See --help.\n", argv[optind - 1]);
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "Unknown option\n");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > optind)
        fileIn = argv[optind];

    if (fileIn && strcmp(fileIn, "-") != 0) {
        if ((fd = open(fileIn, O_RDONLY)) < 0) {
            fprintf(stderr, "Unable to open %s: %s\n", fileIn, strerror(errno));
            goto cleanup;
        }
    } else {
        fd = STDIN_FILENO;
    }

    if (!(sorter = csort_new())) {
        fprintf(stderr, "Unable to create sorter object\n");
        goto cleanup;
    }

    while (1) {
        char buf[10];
        ssize_t n;

        n = read(fd, buf, sizeof(buf));

        if (n < 0) {
            fprintf(stderr, "Unable to read from input file: %s\n", strerror(errno));
            goto cleanup;
        }

        if (n == 0)
            break;

        csort_add_buffer(sorter, buf, n);
    }

    if (csort_output(sorter, stdout) < 0) {
        fprintf(stderr, "There has been an error while sorting input file");
        goto cleanup;
    }

    ret = EXIT_SUCCESS;
 cleanup:
    if (fd != -1)
        close(fd);
    csort_free(sorter);
    return ret;
}
