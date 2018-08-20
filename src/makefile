CC=gcc
DEPS = headers.h
OBJ = com_pipe.o com_exc.o com_redir.o curr_dir.o ls.o pinfo.o builtin.o main.o var_env.o com_echo.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)
