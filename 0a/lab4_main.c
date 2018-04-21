#include "lab4_util.h"

#define SYS_WRITE 1
#define STDOUT 1

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int i;
  char * str;
  str = simple_itoa(argc);  
  system_call(SYS_WRITE,STDOUT, str,simple_strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", sizeof(" arguments \n"));
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", sizeof("argv["));
	  str = simple_itoa(i);
      system_call(SYS_WRITE,STDOUT,str,simple_strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",sizeof("] = "));
      system_call(SYS_WRITE,STDOUT,argv[i],simple_strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",sizeof("\n"));
    }
  return 0;
}
