#include "headers.h"

int execute2(char *s2,char startdir[1024],char lastdir[1024],int fin,int fout,int fd_input,int flag2,Jobs *head)
{
    char *seperate,*array2[1000];
    int i,count,status;
    int p[2],id;
    i=0;
    seperate=strtok(s2," ");
    while(seperate!=NULL)
    {
        array2[i]=seperate;
        seperate=strtok(NULL," ");
        i++;
    }
    array2[i]=NULL;
    count=i;
    if(count==0)
        return 0;
    else
    {
        if(pipe(p)!=0)
            perror("pipe failed");
        if((id=fork())==-1)
            perror("fork failed");
        else if(id==0)
        {
            if(fin==0)
                dup2(fd_input,0);
            if(flag2==0 && fout==0)
                dup2(p[1],1);
            close(p[0]);
            if(strcmp(array2[0],"pinfo")==0)
            {
                PINFO(i,array2,getpid());
            }
            else if(strcmp(array2[0],"jobs")==0)
            {
                int eid=1;
                Jobs *tmp2;
                tmp2= head;
                while(tmp2!=NULL)
                {
                    printf("[%d] %s [%ld]\n",eid,tmp2->name,tmp2->pid);
                    eid++;
                    tmp2=tmp2->nxt;
                }
            }
            else if((execvp(array2[0],array2))<0)
            {
                perror("error in execvp function");
                exit(0);
            }
        }
        else
        {
            waitpid(id,&status,WUNTRACED);            
            close(p[1]);
            fd_input=p[0];
        }
    }
    return fd_input;
}

void pipecmd(char *s2,char startdir[1024],char lastdir[1024],Jobs *head)
{
    char *seperate;
    char *array[1000];
    int i,count,len,flag,pos,status,j,flag2;
    int id,fd_input;
    i=0;
    seperate=strtok(s2,"|");
    while(seperate!=NULL)
    {
        array[i]=seperate;
        i++;
        seperate=strtok(NULL,"|");

    }
    flag2=0;
    array[i]=NULL;
    count=i;
    if(count==0)
        return;
    fd_input=0;
    for(i=0;i<count;i++)
    {
        flag=0;
        if(i==count-1)
            flag2=1;
         len=strlen(array[i]);
         for(j=0;j<len;j++)
         {
             if(array[i][j]=='<' || array[i][j]=='>')
             {
                 fd_input=redir(array[i],startdir,lastdir,1,fd_input,flag2,head);
                 flag=1;
                 break;
             }
         }
         if(flag!=1)
             fd_input=execute2(array[i],startdir,lastdir,0,0,fd_input,flag2,head);
    }
    return;
}