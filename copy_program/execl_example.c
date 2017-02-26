#include<stdio.h>
//#include<sys/types.h>

int main(int argc, char *argv[])
{
	
	if(fork()==0)
	{ 
		printf("before execl\n\n");		
		execl("/bin/date", "date", 0);
		printf("after execl\n\n");		
	}
	wait((int *)0);
	printf("copy done\n\n");	
	
		//pid = getpid();
		//execl("copy", "copy", argv[1], argv[2], 0);

	
	
	        //pid = 
		//wait((int*)0);

		//printf("PPID = %d\n", ppid);
	
	return 0;
}
