#include "headers.h"

char * curr_dir(char *ind)
{
	char * pp;
	pp = (char *)malloc(1000*sizeof(char));

	char cud[1020];
	getcwd(cud,sizeof(cud));
	/* If both are same we return '~'
		else we return remaining directory */
	if(!(strcmp(cud,ind)))
	{
		pp[0] = '~';
		for (int i = 1; i < 1000; ++i)
		{
			pp[i] = '\0';
 		}
		return pp;
	}	
	else
	{
		int l = strlen(ind);
		int f = strlen(cud);
		if (f < l)
		{
			/* code */
			for(int i=0;i<f;i++)
			{
				pp[i] = cud[i];
			}
			for (int i = f; i <=99 ; i++)
			{
				pp[i] = '\0';
			}	
			return pp;
		}
		else
		{
			pp[0] = '~';
			for (int i = 1 ; i <=f-l ; i++)
			{
				pp[i] = cud[i+l-1];
			}
			return pp;
		}
	}
}
