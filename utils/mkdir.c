/* SPDX-License-Identifier: BSD-2-Clause */
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static void usage(void);
static int  build_path(char *, mode_t, mode_t);

int mkdir_run(int argc, char **argv)
{
    int c, missing = 0;
    mode_t mode, dir_mode;
    const char *str_mode = NULL;

    mode = 0777 & ~umask(0);
    dir_mode = mode | S_IWUSR | S_IXUSR;

    while ((c = getopt(argc, argv, "pm:h")) != -1) {
        switch (c) {
        case 'p':
            missing = 1;
            break;
        case 'm':
            str_mode = optarg;
            break;
        case 'h':
        default:
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (argc == 0)
        usage();

    if (str_mode != NULL) {
        fprintf(stderr, "-m is not supported yet!\n");
        return 1;
    }

    for (int rv = 0; argc > 0; argc--, argv++) {
        char *slash;

        /* Removing trailing slash */
        slash = strrchr(*argv, '\0');
        while (--slash > *argv && *slash == '/')
            *slash = '\0';

        if (missing) {
            rv = build_path(*argv, mode, dir_mode);
        } else {
            rv = mkdir(*argv, mode);

            if (rv == 0 && mode > 0777)
                rv = chmod(*argv, mode);
        }

        if (rv == -1) {
            fprintf(stderr, "mkdir: %s: %s\n", *argv, strerror(errno));
            return 1;
        }
    }

    return 0;
}

static int build_path(char *path, mode_t mode, mode_t dir_mode)
{
    struct stat st;
    char *slash;
    int done;

    slash = path;

    for (;;) {
        slash += strspn(slash, "/");
        slash += strcspn(slash, "/");

        done = *slash == '\0';
        *slash = '\0';

        if (mkdir(path, done ? mode : dir_mode) == 0) {
            if (mode > 0777 && chmod(path, mode) == -1)
                return -1;
        } else {
            int mkdir_errno = errno;

            if (stat(path, &st) == -1) {
                errno = mkdir_errno;
                return -1;
            }

            if (!S_ISDIR(st.st_mode)) {
                errno = ENOTDIR;
                return -1;
            }
        }

        if (done)
            break;

        *slash = '/';
    }

    return 0;
}

static void usage(void)
{
    fprintf(stderr, "usage: mkdir [-p] [-m mode] dir...\n");
    exit(1);
}
