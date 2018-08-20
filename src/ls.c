#include "headers.h"

// priting the details of ls -l

void putdetails(struct stat *fileStat,struct dirent *mydir)
{
	printf( (S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf( (fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%lu ",fileStat->st_nlink);
    struct passwd *p;
    struct group *g;
    p = getpwuid(fileStat->st_uid);
    g = getgrgid(fileStat->st_gid);
    printf("%s %s",p->pw_name,g->gr_name);
    printf("\t%ld",fileStat->st_size);
    char *rt;
    printf("\t");
    rt = ctime(&fileStat->st_mtime);
    for(int i =4;i<16;i++)
		printf("%c",rt[i]);
	printf("\t");
  
	return;
}

void exc_ls(int t,char **argv,int flag, char *ind )
{
	if(t==1)
		RUNLS(1,0,".");
	else if(t==2)
	{
		if(!strcmp(argv[1],"-a"))
			RUNLS(0,0,".");
		else if(!strcmp(argv[1],"-l"))
			RUNLS(1,1,".");
		else if(!strcmp(argv[1],"-al"))
			RUNLS(0,1,".");
		else if(!strcmp(argv[1],"-la"))
			RUNLS(0,1,".");
		else
			RUNLS(1,0,argv[1]);
	}
	else if(t==3)
	{
		if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-l")))
			RUNLS(0,1,".");
		else if(!strcmp(argv[1],"-l") && !(strcmp(argv[2],"-a")))
			RUNLS(0,1,".");
		else if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-a")))
			RUNLS(0,0,".");
		else if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-l")))
			RUNLS(1,1,".");			
		else
			if(!strcmp(argv[1],"-a"))
				RUNLS(0,0,argv[2]);
			else if(!strcmp(argv[1],"-l"))
				RUNLS(1,1,argv[2]);
			else if(!strcmp(argv[1],"-al"))
				RUNLS(0,1,argv[2]);
			else if(!strcmp(argv[1],"-la"))
				RUNLS(0,1,argv[2]);
	}
	else if(t==4)
	{
		if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-l")))
			RUNLS(0,1,argv[3]);
		else if(!strcmp(argv[1],"-l") && !(strcmp(argv[2],"-a")))
			RUNLS(0,1,argv[3]);
		else if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-a")))
			RUNLS(0,0,argv[3]);
		else if(!strcmp(argv[1],"-a") && !(strcmp(argv[2],"-l")))
			RUNLS(1,1,argv[3]);

	}
	return;
}


long unsigned int TSIZE(long unsigned int e)
{
	double k =  e/1024.00;
	long unsigned int t = e/1024;
	switch(t%4)
	{
		case 0:
			if(t==k)
				return t;
			else if(t<k)
				return t+4;
		case 1:
			return t+3;
		case 2:
			return t+2;
		case 3:
				return t+1;
		default:
			return t;

	}
}

// Function to run ls command without the options

void RUNLS(int y,int z,char *e)
{
	char cwd[512];
	getcwd(cwd,sizeof(cwd));

	struct stat drt;
	stat(cwd,&drt);
	
	long unsigned int total=0;
	double r;
	DIR *di;
	DIR *de;
	struct dirent *mydir;
	struct dirent *myd;
	struct stat filstat;

	char buf[512];

	di = opendir(e);

	if(z==1)
	{
		mydir=readdir(di);
		while(mydir)
		{
			strcpy(cwd,"");
			getcwd(cwd,sizeof(cwd));
			strcat(cwd,"/");
			strcat(cwd,e);
			strcat(cwd,"/");
			strcat(cwd,mydir->d_name);
			stat(cwd,&filstat);
			total = total + TSIZE(filstat.st_size);
			mydir = readdir(di);
		}
		closedir(di);
		if(y==1)
			printf("total %lu\n",total-8);
		else
			printf("total %lu\n",total);
	}
	de = opendir(e);
	myd = readdir(de);
	while(myd)
	{
		strcpy(cwd,"");
		getcwd(cwd,sizeof(cwd));
		if(y==1)
		{
			if(strcmp(myd->d_name,".") && strcmp(myd->d_name,".."))
			{
				strcat(cwd,"/");
				strcat(cwd,e);
				strcat(cwd,"/");
				strcat(cwd,myd->d_name);
				stat(cwd,&filstat);
				if(z==1)
				{
					putdetails(&filstat,myd);
				}
				if(S_ISDIR(filstat.st_mode))
					printf("\x1b[34m%s\x1b[0m\n",myd->d_name);
				else if(filstat.st_mode & S_IXUSR)
					printf("\x1b[32m%s\x1b[0m\n",myd->d_name);
				else
					printf("%s\n",myd->d_name);
			}
		}
		else if(y==0)
		{
			strcat(cwd,"/");
			strcat(cwd,e);
			strcat(cwd,"/");
			strcat(cwd,myd->d_name);
			stat(cwd,&filstat);
			if(z==1)
			{
				putdetails(&filstat,myd);
			}
			if(S_ISDIR(filstat.st_mode))
				printf("\x1b[34m%s\x1b[0m\n",myd->d_name);
			else if(filstat.st_mode & S_IXUSR)
				printf("\x1b[32m%s\x1b[0m\n",myd->d_name);
			else
				printf("%s\n",myd->d_name);
		}	
		myd = readdir(de);
	}
	closedir(de);
	return;
}