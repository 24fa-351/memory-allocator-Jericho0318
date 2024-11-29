my_malloc: my_malloc.c
	gcc -o my_malloc my_malloc.c

system_malloc: system_malloc.c
	gcc -o system_malloc system_malloc.c

clean:
	rm my_malloc
	rm system_malloc