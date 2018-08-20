
#include "headers.h"

// PINFO
void PINFO(int t,char **argv,int io)
{
 	char a[100000],b[1000];
	char y[100];
	char *x[100];
	FILE *fd;
	if(t==1)
	{
   	 	char ss[1000];
   	 	sprintf(ss,"%d"/*,pid*/,io);
		strcpy(b,"/proc/");
		strcat(b,ss);
		strcat(b,"/stat");
		fd=fopen(b,"r");
  	 	fscanf(fd,"%[^\n]",y);
   	 	x[0]=strtok(y," \t");
    	int j=0;
    	char s1[1000];
    	sprintf(s1,"/proc/%d/exe",io);
    	char path[500];
    	readlink(s1,path,500);	
    	while(x[j]!=NULL)
    	{	
	    	j++;
	    	x[j]=strtok(NULL," \t");
   	 	}
    	printf("pid -- %s\nProcess Status -- %s\nmemory - %s\nExecutable Path --%s\n",x[0],x[2],x[8],path);
	}
    if(t>1)
	  {
		strcpy(b,"/proc/");
		strcat(b,argv[1]);
		strcat(b,"/stat");
		fd=fopen(b,"r");
		fscanf(fd,"%[^\n]",y);
		x[0]=strtok(y," \t");
		int j=0;
		char s1[1000];
    	sprintf(s1,"/proc/%d/exe",io);
    	char path[500];
    	readlink(s1,path,500);
		while(x[j]!=NULL)
		{
			j++;
			x[j]=strtok(NULL," \t");
		}
		printf("pid -- %s\nProcess Status -- %s\nmemory - %s\nExecutable Path --%s\n",x[0],x[2],x[8],path);
	}
}