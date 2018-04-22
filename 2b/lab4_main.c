
#include <sys/types.h>
#include "lab4_util.h"


#define SYS_WRITE 1
#define STDOUT 1
#define SYS_OPEN 2
#define SYS_READ 0
#define SYS_LSEAK 8
#define SYS_CLOSE 3
#define SYS_GETDENTS 78
#define SYS_EXIT 60
#define O_RDONLY 0

#define BUF_SIZE 1024


extern int system_call();
void connect_strings(char *a, char *b, char* output);
int str_size(char *str);
void file_name_search(char *path, char *filename);
void dir_search(char *path);


void connect_strings(char *a, char *b, char* output) {
    int sa = str_size(a);
    int sb = str_size(b);
    /*char *output = malloc((size_t) (sa + sb + 2));*/
    int i = 0, j = 0;
    for (; i < sa; i++)
        output[i] = a[i];
    output[i] = '/';
    i++;
    for (; j < sb; j++)
        output[i + j] = b[j];
    output[i+j]='\0';
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
    int i, flagN = 0;
    for (i = 0; i < argc; i++) {

        if (simple_strcmp(argv[i], "-n") == 0){
            i++;
            filename = argv[i];
            flagN = 1;
        }
    }
    if (flagN == 1)
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
    /*printf("%s\n", path);
    fd = open(path, 0);*/
    fd=system_call(SYS_OPEN,path,O_RDONLY,0777);
    system_call(SYS_WRITE, STDOUT, path, str_size(path));
    system_call(SYS_WRITE, STDOUT, "\n", 1);
    if (fd == -1)
        system_call(SYS_EXIT,0x55);
    for (;;) {

        /*nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);*/
        nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
        if (nread == -1)
            system_call(SYS_EXIT,0x55);

        if (nread == 0)
            break;

        /*system_call(SYS_WRITE,STDOUT, nread, bpos);*/

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if (d->d_ino != 0) {
                name = d->d_name;
                if (d_type == 8 || d_type == 4) {
                    if (simple_strcmp(name, ".") != 0 && simple_strcmp(name, "..") != 0) {
			char newPath[str_size(path)+str_size(name)+2];
                        /*printf("%s\n", connect_strings(path, name,newPath));*/
			connect_strings(path, name,newPath);
			system_call(SYS_WRITE, STDOUT,newPath, str_size(newPath));
   			system_call(SYS_WRITE, STDOUT, "\n", 1);
                        if (d_type == 4)
                            dir_search(newPath);
                    }
                }
                /*system_call(SYS_WRITE,STDOUT, d->d_name,simple_strlen(d->d_name));
                system_call(SYS_WRITE,STDOUT, (char *) "\n",1)*/
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
    fd=system_call(SYS_OPEN,path,O_RDONLY,0777);
    /*system_call(SYS_WRITE, STDOUT, path, str_size(path));
    system_call(SYS_WRITE, STDOUT, "\n", 1);*/
 
    if(simple_strcmp(filename,".")==0){
        /*printf("%s\n", path);*/
	system_call(SYS_WRITE, STDOUT, path, str_size(path));
	system_call(SYS_WRITE, STDOUT, "\n", 1);
	}

    for (;;) {
        /*nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);*/
        nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
    	if (fd == -1)
        system_call(SYS_EXIT,0x55);
        if (nread == 0)
            break;

        /*system_call(SYS_WRITE,STDOUT, nread, bpos);*/

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if (d->d_ino != 0) {
                name = d->d_name;

                if (d_type == 8 || d_type == 4) {
                    if (simple_strcmp(name, ".") != 0 && simple_strcmp(name, "..") != 0) {
			char newPath[str_size(path)+str_size(name)+2];
                        if (simple_strcmp(name, filename) == 0){
                            /*printf("%s \n", connect_strings(path, name));*/
			connect_strings(path, name,newPath);
			system_call(SYS_WRITE, STDOUT, newPath, str_size(newPath));
   			system_call(SYS_WRITE, STDOUT, "\n", 1);
			}
                        if (d_type == 4){
			    connect_strings(path, name,newPath);
                            file_name_search(newPath, filename);}
                    }
                }

                /*system_call(SYS_WRITE,STDOUT, d->d_name,simple_strlen(d->d_name));

                system_call(SYS_WRITE,STDOUT, (char *) "\n",1)*/
                bpos += d->d_reclen;
            }
        }
    }
}

