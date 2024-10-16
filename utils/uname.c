/* SPDX-License-Identifier: BSD-2-Clause */
#include <sys/utsname.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void usage(void);

#define PRINT_SYSNAME       1
#define PRINT_NODENAME      1 << 1
#define PRINT_RELEASE       1 << 2
#define PRINT_VERSION       1 << 3
#define PRINT_MACHINE       1 << 4
#define PRINT_OS            1 << 5

int uname_run(int argc, char **argv)
{
    struct utsname u;
    int c, space = 0, print_mask = 0;

    while ((c = getopt(argc, argv, "amnrsvh")) != -1) {
        switch (c) {
            case 'a':
                print_mask |= PRINT_SYSNAME | PRINT_NODENAME | PRINT_RELEASE |
                    PRINT_RELEASE | PRINT_VERSION | PRINT_MACHINE | PRINT_OS;
                break;
            case 'm':
                print_mask |= PRINT_MACHINE;
                break;
            case 'n':
                print_mask |= PRINT_NODENAME;
                break;
            case 'o':
                print_mask |= PRINT_OS;
                break;
            case 'r':
                print_mask |= PRINT_RELEASE;
                break;
            case 's':
                print_mask |= PRINT_SYSNAME;
                break;
            case 'v':
                print_mask |= PRINT_VERSION;
                break;
            case 'h':
            default:
                usage();
        }
    }

    if (optind != argc)
        usage();

    if (!print_mask)
        print_mask = PRINT_SYSNAME;

    if (uname(&u) == -1)
        perror("uname");

    if (print_mask & PRINT_SYSNAME) {
        space++;
        fputs(u.sysname, stdout);
    }

    if (print_mask & PRINT_NODENAME) {
        if (space++)
            putchar(' ');

        fputs(u.nodename, stdout);
    }

    if (print_mask & PRINT_RELEASE) {
        if (space++)
            putchar(' ');

        fputs(u.release, stdout);
    }

    if (print_mask & PRINT_VERSION) {
        if (space++)
            putchar(' ');

        fputs(u.version, stdout);
    }

    if (print_mask & PRINT_MACHINE) {
        if (space++)
            putchar(' ');

        fputs(u.machine, stdout);
    }

    putchar('\n');

    return 0;
}

static void usage(void)
{
    fprintf(stderr, "usage: uname [-amnrsv]\n");
    exit(1);
}
