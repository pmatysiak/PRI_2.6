all: znaki.c
	gcc znaki.c -o statki.out -lncurses
clean:
	$(RM) statki.out