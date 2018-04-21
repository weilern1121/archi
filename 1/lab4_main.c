#include "lab4_util.h"


#define SYS_WRITE 1
#define STDOUT 1
#define SYS_OPEN 2
#define SYS_READ 0
#define SYS_LSEAK 8
#define SYS_CLOSE 3

extern int system_call();

int str_size(char* str){
	/*int output=1;
	while(*str!= '\0'){
	output++;
	++str;	
	}
	return output;*/
	int i;
	for(i = 0; str[i] != '\0'; ++i);
	return i;
}

int main (int argc , char* argv[], char* envp[])
{
	int size,file_descriptor;
	char * shira_name=argv[1];
	char * x_name=argv[2];

	file_descriptor=system_call(SYS_OPEN,shira_name, 2,0777);
	system_call(SYS_LSEAK,file_descriptor, 0x1015,0);
	size=str_size(x_name);
	if(size>5)
	 return 0x55;
	system_call(SYS_WRITE,file_descriptor, x_name, size);
	int space=5-str_size(x_name);
	while(space> 0){
		system_call(SYS_WRITE,file_descriptor, "", 1);
		space--;
	}
	system_call(SYS_CLOSE,file_descriptor);
	return 0;
}

