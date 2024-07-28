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

int main(int argc, char **argv)
{
    if (strcmp(strrchr(*argv, '/') + 1, "prootbox") == 0) {
        argv++;
        argc--;
    }

    check_cmd(uname, argc, argv);

    usage();
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
    fprintf(stderr, "bin: uname\n");
    exit(1);
}
