//
// Created by 64949 on 2023-12-20.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void childProcessFunc(int p[]);

int
main(int argc, char *argv[])
{
    int pall[2];
    pipe(pall);

    if(fork() == 0) {
        childProcessFunc(pall);
    } else {
        close(pall[0]);
        for (int i = 2; i < 36; ++i) {
            write(pall[1], &i, sizeof(i));
        }
        close(pall[1]);
        wait(0);
    }
    exit(0);
}

void
childProcessFunc(int pl[]) {
    int p;
    close(pl[1]);

    if(read(pl[0], &p, sizeof(p)) == 0) {
        close(pl[0]);
        exit(0);
    }

    int pr[2];
    pipe(pr);

    if(fork() == 0) {
        childProcessFunc(pr);
    } else {
        int num;
        close(pr[0]);
        printf("prime %d\n", p);
        while(read(pl[0], &num, sizeof(num)) != 0) {
            if(num % p != 0) {
                write(pr[1], &num, sizeof(num));
            }
        }
        close(pr[1]);
        close(pl[0]);
        wait(0);
    }
    exit(0);
}