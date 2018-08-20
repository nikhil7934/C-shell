/*** HEADER FILES ***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <error.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

/*************** VARIABLES ***********/

typedef struct jobs{
		char name[1000];
		long int pid;
		struct jobs *nxt;
		int type;
}Jobs;

//Jobs *head=NULL;

/************ FUNCTIONS DECELARATION ***********************/

void var_env(int argc, char *argv[]);

int checkBuiltin(char *com);

void exec_cd(char *array[1000],int count,char startdir[1024],char lastdir[1024],int flag);

int redir(char *s2,char startdir[1024],char lastdir[1024],int type,int fd_input,int cond,Jobs *head);

int execute2(char *s2,char startdir[1024],char lastdir[1024],int fin,int fout,int fd_input,int flag2,Jobs *head);

void exec_echo(char *s2,int count,char *array[1000],char startdir[1024],int flag);

char * curr_dir(char *ind);

void putdetails(struct stat *fileStat,struct dirent *mydir);

int com_exc(char *s2,char startdir[1024],char lastdir[1024]);

void pipecmd(char *s2,char startdir[1024],char lastdir[1024],Jobs *head);

long unsigned int TSIZE(long unsigned int e);

void exc_ls(int t,char **argv,int flag, char *ind);

void RUNLS(int y,int z,char *e);

void PINFO(int t,char **argv,int io);

void bg_jobs();