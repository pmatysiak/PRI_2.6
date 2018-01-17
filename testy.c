#include <ncurses.h>
#include <unistd.h>

#define DELAY 300000

void splashscreen();

int main(int argc, char *argv[]) {
	
	int max_y = 0, max_x = 0;


	initscr();
	noecho();
	curs_set(TRUE);


	getmaxyx(stdscr, max_y, max_x);
	splashscreen(max_y, max_x);

	endwin();
}

void splashscreen(int max_y, int max_x) {

	int i;
	int yTitle = 15;
	if (((max_y/2)-4) > yTitle) yTitle = (max_y/2)-4;

	for(i=0; i<(max_x-80);i++) {
		mvprintw(0, i, "                                      |__                                            ");
		mvprintw(1, i, "                                      |\\/                                           ");
		mvprintw(2, i, "                                      ---                                            ");
		mvprintw(3, i, "                                      / | [                                          ");
		mvprintw(4, i, "                               !      | |||                                          ");
		mvprintw(5, i, "                             _/|     _/|-++'                                         ");
		mvprintw(6, i, "                         +  +--|    |--|--|_ |-                                      ");
		mvprintw(7, i, "                      { /|__|  |/\\__|  |--- |||__/                                  ");
		mvprintw(8, i, "                     +---------------___[}-_===_.'____                 /\\           ");
		mvprintw(9, i, "                 ____`-' ||___-{]_| _[}-  |     |_[___\\==--            \\/   _      ");
		mvprintw(10, i, "  __..._____--==/___]_|__|_____________________________[___\\==--____,------' .7    ");
		mvprintw(11, i, " |                                                              ORP ELKA     /      ");
		mvprintw(12, i, "  \\_________________________________________________________________________|      ");

		mvprintw(yTitle, (max_x-73)/2, " .d88888b.  888    d8P  8888888b.  8888888888 88888888888 Y88b   d88P    ");
		mvprintw(yTitle+1, (max_x-73)/2, "d88P\" \"Y88b 888   d8P   888   Y88b 888            888      Y88b d88P     ");
		mvprintw(yTitle+2, (max_x-73)/2, "888     888 888  d8P    888    888 888            888       Y88o88P      ");
		mvprintw(yTitle+3, (max_x-73)/2, "888     888 888d88K     888   d88P 8888888        888        Y888P       ");
		mvprintw(yTitle+4, (max_x-73)/2, "888     888 8888888b    8888888P\"  888            888         888        ");
		mvprintw(yTitle+5, (max_x-73)/2, "888     888 888  Y88b   888 T88b   888            888         888        ");
		mvprintw(yTitle+6, (max_x-73)/2, "Y88b. .d88P 888   Y88b  888  T88b  888            888         888        ");
		mvprintw(yTitle+7, (max_x-73)/2, " \"Y88888P\"  888    Y88b 888   T88b 8888888888     888         888        ");
		mvprintw(yTitle+8, (max_x-73)/2, "                                           bo                            ");

		if ((i/2)%4 != 0) {
			mvprintw(yTitle+12, (max_x-18)/2, "+================+");
			mvprintw(yTitle+13, (max_x-18)/2, "| NACISNIJ ENTER |");
			mvprintw(yTitle+14, (max_x-18)/2, "+================+");
		}
		else {
			mvprintw(yTitle+12, (max_x-18)/2, "                  ");
			mvprintw(yTitle+13, (max_x-18)/2, "                  ");
			mvprintw(yTitle+14, (max_x-18)/2, "                  ");
		}
		refresh();

		usleep(DELAY);
	}
}