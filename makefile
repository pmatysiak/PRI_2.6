all: znaki.c
	gcc znaki.c -o statki.out -lncurses -Wall
clean:
	$(RM) statki.out