all:
	gcc *.c -o bonsai.o -Werror

main:
	gcc main.c dataset.c vector.c dectree.c -o main.o -Werror
