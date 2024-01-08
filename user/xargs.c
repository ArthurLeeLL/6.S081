//
// Created by 64949 on 2023-12-26.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

//char
//*strcat(char *str1, char *str2) {
//    char *pre = str1;
//    while (*pre != '\0') pre++;
//
//    char *rear = str2;
//
//    while(*rear != '\0') {
//        *pre = *rear;
//        pre++;
//        rear++;
//    }
//
//    *pre = '\0';
//
//    return str1;
//}

//int
//main(int argc, char *argv[]) {
////    for (int i = 0; i < argc; ++i) {
////        printf("%s\n", argv[i]);
////    }
////    char buf[512];
////    printf("--------\n");
////    while(read(0, &buf, sizeof(buf))) {
////        printf("%s\n", buf);
////        printf("--------\n");
////    }
////    printf("finished\n");
//
//    char *argvc[MAXARG];
//    for (int i = 1; i < argc; ++i) {
//        argvc[i-1] = argv[i];
//    }
//
//    char buf[512];
//    read(0, &buf, sizeof(buf));
//    char *prefix = "./";
//    char *pre = buf;
//    char *p = buf;
////    p = strtok(buf, "\n");
//    while(*p != '\n' && *p != '\0') {
//        printf("%c\n", *p);
//        p++;
//    }
//
//    if(*p == '\0') {
//        printf("single");
//        if(fork() == 0) {
//            exec(strcat(prefix, argvc[0]), argvc);
//        }
//        wait(0);
//        printf(" %s", pre);
//        exit(0);
//        return 0;
//    }
//
//    while(*p == '\n') {
//        *p = '\0';
//        if(fork() == 0) {
//            exec(strcat(prefix, argvc[0]), argvc);
//        }
//        wait(0);
//        printf(" %s", pre);
//        p++;
//        pre = p;
//        while(*p != '\n' && *p != '\0') p++;
//        if(*p == '\0') {
//            if(fork() == 0) {
//                exec(strcat(prefix, argvc[0]), argvc);
//            }
//            wait(0);
//            printf(" %s", pre);
//            exit(0);
//            return 0;
//        }
//    }
//    exit(0);
//    return 0;
//}

int
readline(char *buf) {
    char c;
    int i = 0;

    while(read(0, &c, 1)) {
        i++;
        if(c != '\n') {
            *buf++ = c;
        } else {
            *buf = '\0';
            break;
        }
    }
    return i;
}

int
main(int argc, char *argv[]) {
    char buf[512];
    char *argvc[MAXARG];

    while(readline(buf)) {
        if(fork() == 0) {
            for (int i = 1; i < argc; ++i) {
                argvc[i-1] = argv[i];
            }
            argvc[argc - 1] = buf;
            exec(argvc[0], argvc);
        } else {
            memset(argvc, 0, sizeof(char*) * MAXARG);
            wait(0);
        }
    }
    exit(0);
}
