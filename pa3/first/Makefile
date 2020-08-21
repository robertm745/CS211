all: first 

first: first.c
	gcc -g -o first first.c -Wall -Werror -fsanitize=address

clean:
	rm -rf first
