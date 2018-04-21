#define _GNU_SOURCE

#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <memory.h>
#include "lab4_util.h"


#define SYS_WRITE 1
#define STDOUT 1
#define SYS_OPEN 2
#define SYS_READ 0
#define SYS_LSEAK 8
#define SYS_CLOSE 3
#define SYS_GETDENTS 78

#define BUF_SIZE 1024
#define handle_error(msg) \exit(EXIT_FAILURE);


extern int system_call();

char *connect_strings(char *a, char *b);

int str_size(char *str);

void file_name_search(char *path, char *filename);

char *connect_strings(char *a, char *b) {
    int sa = str_size(a);
    int sb = str_size(b);
    char *output = malloc((size_t) (sa + sb + 2));
    int i = 0, j = 0;
    for (; i < sa; i++)
        output[i] = a[i];
    output[i] = '/';
    i++;
    for (; j < sb; j++)
        output[i + j] = b[j];
    output[i+j]='\0';
    return output;

}


int str_size(char *str) {

    int i;
    for (i = 0; str[i] != '\0'; ++i);
    return i;
}

struct linux_dirent {
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[];
};

int main(int argc, char *argv[]) {
    char *filename;
    int i, flag = 0;
    for (i = 0; i < argc; i++) {
        //todo- change strcmp
        if (strcmp(argv[i], "-n") == 0){
            i++;
            filename = argv[i];
            flag = 1;
        }
    }
    if (flag == 1)
        file_name_search(".", filename);
    else
        dir_search(".");
    return 0;
}

void dir_search(char *path) {
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    char *name;
    printf("%s\n", path);
    fd = open(path, 0);
    //fd=system_call(SYS_OPEN,argc > 1 ? argv[1] : ".", 2,0777);
    if (fd == -1)
        return;
    for (;;) {

        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        // nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
        if (nread == -1)
            return;

        if (nread == 0)
            break;

        /*system_call(SYS_WRITE,STDOUT, nread, bpos);*/

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if (d->d_ino != 0) {
                name = d->d_name;

                if (d_type == 8 || d_type == 4) {
                    if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
                        printf("%s\n", connect_strings(path, name));
                        if (d_type == 4)
                            dir_search(connect_strings(path, name));
                    }
                }
                //system_call(SYS_WRITE,STDOUT, d->d_name,simple_strlen(d->d_name));

                //system_call(SYS_WRITE,STDOUT, (char *) "\n",1)
                bpos += d->d_reclen;
            }
        }
    }
}

void file_name_search(char *path, char *filename) {
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    char *name;
    if(strcmp(filename,".")==0)
        printf("%s\n", path);
    fd = open(path, 0);
    //fd=system_call(SYS_OPEN,argc > 1 ? argv[1] : ".", 2,0777);
    if (fd == -1)
        return;
    for (;;) {

        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        // nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
        if (nread == -1)
            return;

        if (nread == 0)
            break;

        /*system_call(SYS_WRITE,STDOUT, nread, bpos);*/

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if (d->d_ino != 0) {
                name = d->d_name;

                if (d_type == 8 || d_type == 4) {
                    if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
                        if (strcmp(name, filename) == 0)
                            printf("%s \n", connect_strings(path, name));
                        if (d_type == 4)
                            file_name_search(connect_strings(path, name), filename);
                    }
                }

                //system_call(SYS_WRITE,STDOUT, d->d_name,simple_strlen(d->d_name));

                //system_call(SYS_WRITE,STDOUT, (char *) "\n",1)
                bpos += d->d_reclen;
            }
        }
    }
}

