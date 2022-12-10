app: main.c mymalloc.c mymalloc.h
	gcc -o app -g -Wall -Werror -fsanitize=address main.c mymalloc.c