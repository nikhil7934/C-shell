#include "headers.h"

int flag2;
int m,i,bytes,len,flag,pos,poi,status;
int ef;
char *s;
Jobs *tmp,*tmp2;
char startdir[1024],lastdir[1024];
char currdir[1025];
size_t tot=10000;
char h[2]="~";
char hostname[1024];
char *token;
struct passwd *pw;
uid_t uid;

char ind[100];

char * prompting(char *ind)
{
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	char sysname[100];
	// sys call  for getting user  who logged
	// sys call for obtaining system name
	struct utsname sysnam;
	int t = uname(&sysnam);
	// printing the prompt
	char * pp;
	pp = curr_dir(ind);
	printf("\x1b[33m%s\x1b[0m@\x1b[35m%s\x1b[0m:\x1b[32m%s\x1b[0m\x1b[31m$ \x1b[0m",pw->pw_name,sysnam.sysname,pp);
	return pp;
}

void sighand(int sig)
{
    flag2=1;
    switch(sig)
    {
        case SIGINT:
            printf("\n");
            char * pp1;
           	pp1 = prompting(ind);
            fflush(stdout);
            break;
    }
    return;
}


void sig_br(int sig)
{
    signal(sig,SIG_IGN);
    printf("\n");
    char * pp1;
    pp1 = prompting(ind);
    fflush(stdout);
  //  return;
}
void sig_d(int sig)
{
	signal(sig,SIG_IGN);
	printf("\n");
    char * pp3;
    pp3 = prompting(ind);
    fflush(stdout);
}
// Main Program to run SHELL
int main()
{// To get the user name and system_name for prompting
	int bgproc_size = 0;
	int bgprocs[2017];
	
	printf("\n");
	printf("********##The C SHELL BEGINS FROM HERE##***********\n");	
	printf("\n");
	// obtaining the directory where the shell invoked
	const char amp = '&';
//	char ind[100];
	getcwd(ind,sizeof(ind));
	// From this shell start working 
	while(1)
	{
		char *pp;
		pp = prompting(ind);
		signal(SIGTSTP,sig_br);
        signal(SIGINT,sighand);
        signal(SIGQUIT,sig_d);
		// Taking the command as input
		char c[1024];
		fgets(c, 1024, stdin);
		char *argu[70];
		c[strlen(c)-1] = '\0';
		bg_jobs();		
		char *part[1000];
		int u = 0;
		part[u] = strtok(c,";\t");				
		while(part[u] != NULL)
		{
			u = u + 1;
			part[u] = strtok(NULL,";\t");
		}		
		/* Now excueting each command */
		int cr = 0;
		while(cr<u)
		{
			com_exc(part[cr],ind,pp);
			cr = cr + 1;
		}
	}
	return 0;
}

