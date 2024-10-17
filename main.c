/* SPDX-License-Identifier: BSD-2-Clause */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macros shall not be used! */
#define fn(name) int name ## _run(int argc, char **argv)
#define check_cmd(name, argc, argv) if (*argv != NULL &&\
    run_command(#name, *argv)) { return name ## _run(argc, argv); }

static void usage(void);
int run_command(const char *name, const char *arg);

fn(uname);
fn(mkdir);

int main(int argc, char **argv)
{
    int adjusted = 0;

    char *maybe_path = strrchr(*argv, '/');
    if (maybe_path == NULL) {
        if (strcmp(*argv, "prootbox") == 0)
            goto adjustment;
    } else {
        if (strcmp(maybe_path + 1, "prootbox") == 0) {
adjustment:
            argv++;
            argc--;
            adjusted = 1;
        }
    }

    if (argc == 0 && adjusted != 0)
        usage();

    check_cmd(uname, argc, argv);
    check_cmd(mkdir, argc, argv);
}

int run_command(const char *name, const char *arg)
{
    char *maybe_path = strrchr(arg, '/');   

    if (maybe_path == NULL)
        return strcmp(name, arg) == 0;

    return strcmp(name, maybe_path + 1) == 0;
}

static void usage(void)
{
    fprintf(stderr, "bin: uname mkdir\n");
    exit(1);
}
