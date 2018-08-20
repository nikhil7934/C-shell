
RUN "make" to compile all the programs

|
|---> This creates a executable file called "shell", after running the make 
|
|---> Then run "./shell"  to start the shell
|
|---> Use "quit" to come out of the shell

builtin.c :

	This provides a function to check builtin commands like cd,echo,pwd and run them with functions

com_echo.c:
	This provides all varieties of echo
com_pipe.c:
	This provides  functions for piping
com_redir.c:
	This provides functions of redirections

curr_dir.c :

	This provides a function for returning the current directory all the time and each time

ls.c :
	
	This provides a function for running the ls command with options -l,-a ONLY
	and with directory name also

pinfo.c:

	This provides the function to print the result of pinfo commmand

com_exc.c

	This provides the functions for tokenizing the command and executing the command also
	routing to the  corresponding functions of commands

main.c:

	In this we control the all the functions we written 
var_env.c:
	This is provides setenv and unsetenv
