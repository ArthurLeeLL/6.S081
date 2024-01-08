//
// Created by 64949 on 2023-12-18.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    char buf[512];

    pipe(p);
    if(fork() == 0) {
        int pidChild = getpid();
        read(p[0], buf, sizeof buf);
        fprintf(2, "%d: ", pidChild);
        fprintf(2, "received ");
        for (int i = 0; i < 4; ++i) {
            fprintf(2, "%c", buf[i]);
        }
        fprintf(2, "\n");
        write(p[1], "pong", 4);
    } else {
        int pidParent = getpid();
        write(p[1], "ping", 4);
        wait(0);
        read(p[0], buf, sizeof buf);
        fprintf(2, "%d: ", pidParent);
        fprintf(2, "received ");
        for (int i = 0; i < 4; ++i) {
            fprintf(2, "%c", buf[i]);
        }
        fprintf(2, "\n");
    }
    exit(0);
}
