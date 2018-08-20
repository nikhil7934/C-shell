#include "headers.h"



Jobs *head=NULL;

void exec_exit(int pid)
{
    //    kill(pid,SIGKILL);
    printf("\n");
    exit(0);
    return;
}

void bg_jobs()
{

    Jobs *tmp,*tmp2;
    int status,flag;
    tmp2=head;
    while(tmp2!=NULL)
    {
        flag=0;


        if(waitpid(tmp2->pid,&status,WNOHANG))
        {
            //       if(WIFEXITED(status))
            //      {
            printf("*****%s with pid %ld exited normally******\n",tmp2->name,tmp2->pid);
            tmp=head;
            while(((tmp->nxt)!=tmp2) && ((tmp->nxt)!=NULL) && (tmp!=tmp2)) 
            {
                tmp=tmp->nxt;
            }
            if(tmp==tmp2 && tmp2->nxt==NULL)
            {
                head=tmp2->nxt;
                flag=2;
                tmp2=tmp2->nxt;
                free(tmp);

            }
            else if(tmp==tmp2)
            {
                flag=2;

                head=tmp2->nxt;
                tmp2=tmp2->nxt;
                free(tmp);
            }
            else if((tmp->nxt)==tmp2)
            {
                tmp->nxt=tmp2->nxt;
                flag=1;
            }
            if(flag==1)
            {
                free(tmp2);
                tmp2=tmp->nxt;
            }
            //         }
        }
        if(flag==0)
            tmp2=tmp2->nxt;
    }
    return;
}


int id;
char temp[1024];
void sig_bg(int sig)
{
    Jobs *tmp,*tmp2;
    tmp2=head;
    if(tmp2!=NULL)
    {
        while(tmp2->nxt!=NULL)
        {
            tmp2=tmp2->nxt;
        }
    }
    tmp=malloc(sizeof(Jobs));
    strcpy(tmp->name,temp);
    tmp->pid=id;
    tmp->type=1;
    if(head==NULL)
    {
        tmp->nxt=NULL;
        head=tmp;

    }
    else
    {
        tmp2->nxt=tmp;
        tmp->nxt=NULL;
    }

    printf("\n");
    signal(SIGTSTP,SIG_IGN);
    return;
}
int com_exc(char *s2,char startdir[1024],char lastdir[1024])
{
    int i;
    i=0;
    int count,len,flag,pos,status;
    int eid=0;
    int dfg,ef;
    dfg=0;
    flag=0;
    strcpy(temp,s2);
    char *array[1000];
    char *seperate;
    Jobs *tmp,*tmp2;

    flag=0;
    id=strlen(s2);
    for(i=0;i<id;i++)
    {
        if(s2[i]=='|')
        {
            pipecmd(s2,startdir,lastdir,head);
            dfg=2;
            break;
        }
    }
    if(dfg==2)
    {
        return 0;
    }

    for(i=0;i<id;i++)
    {
        if(s2[i]=='<'||s2[i]=='>')
        {
            redir(s2,startdir,lastdir,0,0,0,head);
            dfg=1;
            break;
        }
    }
    if(dfg==1)
        return 0;
    seperate=strtok(s2," ");
    i=0;
    signal(SIGTSTP,sig_bg);
    while(seperate!=NULL)
    {
        array[i]=seperate;
        if((array[i][strlen(array[i])-1]=='&') || (strcmp(array[i],"&")==0))
        {
            if(array[i][strlen(array[i])-1]=='&' && (strcmp(array[i],"&")!=0))
            {
                array[i][strlen(array[i])-1]='\0';
                i++;
            }
            flag=1;
            break;
        }
        i++;
        seperate=strtok(NULL," ");
    }
    // maintaining the tail of head list
    tmp2=head;
    if(tmp2!=NULL)
    {
        while((tmp2->nxt)!=NULL)
            tmp2=tmp2->nxt;
    }

    count=i;
    if(count>0)
        strcpy(temp,array[0]);
    for(i=1;i<count;i++)
    {
        strcat(temp," ");
        strcat(temp,array[i]);
    }

    if(flag==1)
    {
        tmp=malloc(sizeof(Jobs));
        if(count>0)
        {
            strcpy(tmp->name,array[0]);
            for(i=1;i<count;i++)
            {
                strcat(tmp->name," ");
                strcat(tmp->name,array[i]);
            }
        }
        tmp->type=0;

    }
    array[i]=NULL;
    if(count==0)
        return 1;
    if(strcmp(array[0],"pwd")==0)
    {
        char cwd[1025];
        getcwd(cwd,sizeof(cwd));
        printf("%s\n",cwd);
    }
    else if(strcmp(array[0],"cd")==0)
    {
        exec_cd(array,count,startdir,lastdir,flag);
    }
    else if(strcmp(array[0],"echo")==0)
    {

        exec_echo(temp,count,array,startdir,flag);
    }
    else if(strcmp(array[0],"pinfo")==0)
    {
        PINFO(i,array,getpid());
    }
    else if(strcmp(array[0],"quit")==0 && count==1)
    {
        int pid=getpid();
        exec_exit(pid);
    }
    else if(strcmp(array[0],"jobs")==0)
    {
        eid=1;
        tmp2=head;
        while(tmp2!=NULL)
        {
            char a[100000],b[1000];
            char y[100];
            char *x[100];
            FILE *fd;
            char ss[1000];
            sprintf(ss,"%ld"/*,pid*/,tmp2->pid);
            strcpy(b,"/proc/");
            strcat(b,ss);
            strcat(b,"/stat");
            fd=fopen(b,"r");
            fscanf(fd,"%[^\n]",y);
            x[0]=strtok(y," \t");
            int j=0;
            char s1[1000];
            sprintf(s1,"/proc/%ld/exe",tmp2->pid);
            char path[500];
            readlink(s1,path,500);  
            while(x[j]!=NULL)
            {   
                j++;
                x[j]=strtok(NULL," \t");
            }
            if (!(strcmp(x[2],"R")))
            {
                printf("[%d] Running %s [%ld]\n",eid,tmp2->name,tmp2->pid);
            }
            else if(!(strcmp(x[2],"S")))
            {
                printf("[%d] In-Sleep %s [%ld]\n",eid,tmp2->name,tmp2->pid);
            }
            else if(!(strcmp(x[2],"Z")))
            {
                printf("[%d] Zombie %s [%ld]\n",eid,tmp2->name,tmp2->pid);
            }
            else if(!(strcmp(x[2],"T")))
            {
                printf("[%d] Stopped %s [%ld]\n",eid,tmp2->name,tmp2->pid);
            }
            eid++;
            tmp2=tmp2->nxt;
        }
    }
    else if (!(strcmp(array[0],"setenv")) || !(strcmp(array[0],"unsetenv")))
    {
        var_env(i,array);
    }
    else if(strcmp(array[0],"overkill")==0)
    {
        eid=0;
        tmp2=head;
        while(tmp2!=NULL)
        {
            tmp=tmp2;
            head=tmp2->nxt;
            tmp2=tmp2->nxt;
            kill(tmp->pid,SIGKILL);
            free(tmp);

        }
    }
    else if(strcmp(array[0],"fg")==0)
    {
        if(count<2)
        {
            fprintf(stderr,"invalid input\n");
            return 0;
        }
        eid=0;
        ef=0;
        id=strlen(array[1]);
        for(ef=0;ef<id;ef++)
        {
            eid=eid*10+(array[1][ef]-'0');
        }
        tmp2=head;
        ef=1;
        while(tmp2!=NULL)
        {
            if(eid==ef)
                break;
            tmp2=tmp2->nxt;
            ef++;
        }
        if(tmp2!=NULL)
        {
            if(tmp2->type==0)
            {
                printf("%s\n",tmp2->name);
                waitpid(tmp2->pid,&status,0);
            }
            else
            {
                kill(tmp2->pid,SIGCONT);
                waitpid(tmp2->pid,&status,0);
            }
        }

        else
            perror("invalid input");

    }
    else if(strcmp(array[0],"bg")==0)
    {
        if(count<2)
        {
            fprintf(stderr,"invalid input\n");
            return 0;
        }
        eid=0;
        ef=0;
        id=strlen(array[1]);
        for(ef=0;ef<id;ef++)
        {
            eid=eid*10+(array[1][ef]-'0');
        }
        tmp2=head;
        ef=1;
        while(tmp2!=NULL)
        {
            if(eid==ef)
                break;
            tmp2=tmp2->nxt;
            ef++;
        }
        if(tmp2!=NULL)
        {
            if(tmp2->type==0)
            {
                printf("The process is already running\n");
            }
            else
            {
                kill(tmp2->pid,SIGCONT);
             //   waitpid(tmp2->pid,&status,0);
            }
        }
        else
            perror("invalid input");   
    }
    else if(strcmp(array[0],"kjob")==0)
    {
        if(count==3)
        {

            eid=0;
            ef=0;
            id=strlen(array[1]);
            for(ef=0;ef<id;ef++)
            {
                eid=eid*10+(array[1][ef]-'0');
            }
            tmp2=head;
            ef=1;
            while(tmp2!=NULL)
            {
                if(eid==ef)
                    break;
                tmp2=tmp2->nxt;
                ef++;
            }
            if(tmp2!=NULL)
            {
                eid=0;
                id=strlen(array[2]);
                for(ef=0;ef<id;ef++)
                {
                    eid=eid*10+(array[2][ef]-'0');
                }

                if(kill(tmp2->pid,eid)==-1)
                    fprintf(stderr,"invalid input\n");
            }

            else
                fprintf(stderr,"invalid input\n");

        }
        else
            fprintf(stderr,"invalid input\n");
    }

    else if(count>0)
    {

        if((id=fork())<0)
            perror("error while forking");
        else if(id==0)
        {
            if(execvp(*array,array)<0)
            {
                perror("ERROR in exec function");
                exit(0);
            }

        }
        else if(flag==0)
            waitpid(id,&status,WUNTRACED);
        //           while(wait(&status)!=id);
        else if(flag==1)
        {
            tmp->pid=id;
            printf("@@@@@@@>> Process with ID %ld is running in Background <<@@@@@@@\n",tmp->pid);
            if(head==NULL)
            {
                tmp->nxt=NULL;
                head=tmp;

            }
            else
            {
                tmp2->nxt=tmp;
                tmp->nxt=NULL;
            }

        }

    }
    return 1;
}
