#include "headers.h"

void var_env(int argc, char *argv[])
{
//	printf("%d\n",argc);
//	printf("%s\n",argv[0]);
	if(argc<=1 && argc>3)
	{
		printf("Usage of command is wrong\n");
		return;
	}
	if(argc == 2 && !(strcmp(argv[0], "unsetenv")))
	{

		int x = unsetenv(argv[1]);
		char *p;
		p = getenv(argv[1]);
		printf("Destroyed env variable %s = %s\n",argv[1],p);

	}
	else if(argc == 3 && !(strcmp(argv[0], "setenv")))
	{
		int x = setenv(argv[1], argv[2],1);
		char *p;
		p = getenv(argv[1]);
		printf("Created env variable %s = %s\n",argv[1],p);
	}
	else
	{
		printf("Usage of command is wrong\n");
	}
	
	return;
}