#include <stdio.h>
#include <ncurses.h>

#define DEBUG 1


#define WIDTH 80
#define HEIGHT 24

#define TARGET_X_MIN 17
#define TARGET_X_MAX 17 + 18
#define TARGET_Y_MIN 7
#define TARGET_Y_MAX 7 + 9

#define STATUS_X_MIN 43
#define STATUS_X_MAX 43 + 18
#define STATUS_Y_MIN 7
#define STATUS_Y_MAX 7 + 9

#define WODA 1
#define STATEK_USTAWIANY 2
#define STATEK 3
#define TRAFIONY 4
#define ZATOPIONY 5
#define PUDLO 6
#define WODA_NIEDOZWOLONA 7
#define KURSOR 8

void odswiezPlansze();
void rysujPlansze();
void zmienStan();
void odswiezPole();

struct PolePlanszy {
		enum stan {
			woda = WODA,
			statek_ustawiany = STATEK_USTAWIANY,
			statek = STATEK,
			trafiony = TRAFIONY,
			zatopiony = ZATOPIONY,
			pudlo = PUDLO
		} obiekt;
		enum dozwolenie {
			niedozwolony = 0,
			dozwolony = 1
		} czyMoznaUstawicStatek;
	};

struct PolePlanszy planszaGracza[10][10];
struct PolePlanszy planszaAI[10][10];

int main()
{
	
	//inicjalizacja planszy gracza i AI

	
	

	int iter_x, iter_y;
	for ( iter_y = 0; iter_y < 10; iter_y++ ) {
		for ( iter_x = 0; iter_x < 10; iter_x++ ) {
			planszaGracza[iter_x][iter_y].obiekt = woda;
			planszaAI[iter_x][iter_y].obiekt = woda;
		}
	}





	int key;

	initscr();
	start_color();

	init_pair(WODA, COLOR_CYAN, COLOR_BLUE);
	init_pair(WODA_NIEDOZWOLONA, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(STATEK_USTAWIANY, COLOR_MAGENTA, COLOR_YELLOW);
	init_pair(STATEK, COLOR_MAGENTA, COLOR_GREEN);
	init_pair(TRAFIONY, COLOR_YELLOW, COLOR_RED);
	init_pair(ZATOPIONY, COLOR_RED, COLOR_BLACK);
	init_pair(PUDLO, COLOR_BLUE, COLOR_BLACK);
	init_pair(KURSOR, COLOR_BLACK, COLOR_GREEN);

	raw();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(1);

//sprawdzanie tablic


/*	for ( iter_y = 0; iter_y < 10; iter_y++ ) {
		for ( iter_x = 0; iter_x < 10; iter_x++ ) {
			mvprintw(iter_y,iter_x,"%d", planszaGracza[iter_x][iter_y].obiekt);
			mvprintw(iter_y,iter_x+12,"%d", planszaAI[iter_x][iter_y].obiekt);
		}
	}
*/


	//wyswietlanie sciagi na dole

	int szerokosc;
	szerokosc = 16;

	attron(COLOR_PAIR(WODA));
	mvprintw(HEIGHT-4, 0*szerokosc, "~");
	attroff(COLOR_PAIR(WODA));

	attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
	mvprintw(HEIGHT-4, 1*szerokosc, "-");
	attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));

	attron(COLOR_PAIR(STATEK));
	mvprintw(HEIGHT-4, 2*szerokosc, "S");
	attroff(COLOR_PAIR(STATEK));

	attron(COLOR_PAIR(TRAFIONY));
	mvprintw(HEIGHT-4, 3*szerokosc, "*");
	attroff(COLOR_PAIR(TRAFIONY));

	attron(COLOR_PAIR(PUDLO));
	mvprintw(HEIGHT-4, 4*szerokosc, "O");
	attroff(COLOR_PAIR(PUDLO));

	attron(A_REVERSE);

	mvprintw(HEIGHT-4, 1 + 0*szerokosc, " - woda        ");
	mvprintw(HEIGHT-4, 1 + 1*szerokosc, " - niedozwolony");
	mvprintw(HEIGHT-4, 1 + 2*szerokosc, " - statek      ");
	mvprintw(HEIGHT-4, 1 + 3*szerokosc, " - trafiony    ");
	mvprintw(HEIGHT-4, 1 + 4*szerokosc, " - pudlo       ");

	mvprintw(HEIGHT-3, 0, "                                                                                ");
	
	mvprintw(HEIGHT-2, 0, "Uzyj strzalek do poruszania kursorem. Spacja zaznacza kratke. Q konczy program. ");
	mvprintw(HEIGHT-1, 0, "R resetuje plansze. 1, 2, 3 zaznaczaja testowo.                                 ");
	
	attroff(A_REVERSE);



	rysujPlansze();



	int x_cur, y_cur;

	x_cur = TARGET_X_MIN;
	y_cur = TARGET_Y_MIN;

	//ruch kursorem


	move(y_cur,x_cur);
	refresh();
	odswiezPlansze();

	while ( (key = getch() ) !='q') {
		switch ( key ) {
			case KEY_UP:
				if ( y_cur > TARGET_Y_MIN ) {
					y_cur -= 1;
				} else {
					y_cur = TARGET_Y_MAX;
				}
				break;
			case KEY_DOWN:
				if ( y_cur < TARGET_Y_MAX ) {
					y_cur += 1;
				} else {
					y_cur = TARGET_Y_MIN;
				}
				break;
			case KEY_LEFT:
				if ( x_cur > TARGET_X_MIN ) {
					x_cur -= 2;
				} else {
					x_cur = TARGET_X_MAX;
				}
				break;
			case KEY_RIGHT:
				if ( x_cur < TARGET_X_MAX ) {
					x_cur += 2;
				} else {
					x_cur = TARGET_X_MIN;
				}
				break;

	#if DEBUG == 1
				
			case '1':
				attron(COLOR_PAIR(STATEK));
				printw("S");						//zaznaczanie statek
				attroff(COLOR_PAIR(STATEK));
				break;
			case '2':
				attron(COLOR_PAIR(TRAFIONY));
				printw("*");						//zaznaczanie trafienie
				attroff(COLOR_PAIR(TRAFIONY));
				break;
			case '3':
				attron(COLOR_PAIR(PUDLO));
				printw("O");						//zaznaczanie pudlo
				attroff(COLOR_PAIR(PUDLO));
				break;	
			case '4':
				attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
				printw("-");						//zaznaczanie woda niedozwolona
				attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));
				break;

	#endif
				
			case ' ':
				//planszaGracza[x_cur][y_cur].obiekt = statek;
				zmienStan(x_cur, y_cur);			//zmiana stanu
				//printw("%d",planszaGracza[x_cur][y_cur].obiekt );
				odswiezPole(planszaGracza[x_cur][y_cur].obiekt);
				//odswiezPlansze();
				break;			
			case 'r':								//reset planszy
				rysujPlansze();
				break;
		}
		key = 0;

		move(y_cur, x_cur);
		refresh();
	}

	endwin();
	return 0;
}

void rysujPlansze() {
	int x, y, plansza, cyferka;
	char pole;
	y = x = plansza = 0;
	pole = '~';	//woda
	cyferka = 1;
	
	//rysuj wode na srodku

	for (plansza = 1; plansza <= 2; plansza++) {
		for (y = TARGET_Y_MIN; y <= TARGET_Y_MAX ; y++) {
			for (x = (plansza == 1 ? TARGET_X_MIN : STATUS_X_MIN); x <= (plansza == 1 ? TARGET_X_MAX : STATUS_X_MAX); x = x + 2 ) {
				attron(COLOR_PAIR(WODA));
				mvprintw(y, x, "%c", pole);
				attroff(COLOR_PAIR(WODA));
			}
		}
	}


	//rysuje literki na gorze

	mvprintw(TARGET_Y_MIN-2, TARGET_X_MIN-2, "  A B C D E F G H I J       A B C D E F G H I J  ");

	//rysuje poziome ramki na gorze i na dole

	mvprintw(TARGET_Y_MIN-1, TARGET_X_MIN-2, "+---------------------+   +---------------------+");
	mvprintw(TARGET_Y_MAX+1, TARGET_X_MIN-2, "+---------------------+   +---------------------+");

	//rysuje cyferki po boku i ramki pionowe

	for ( y = TARGET_Y_MIN; y <= TARGET_Y_MAX; y++) {
																								//cyferki po lewej
		if (y < TARGET_Y_MAX ) mvprintw(y, TARGET_X_MIN-5, " %d", cyferka + y - TARGET_Y_MIN);	//jesli 0-9 to zero wiodace
		else mvprintw(y, TARGET_X_MIN-5, "%d", cyferka + y - TARGET_Y_MIN);						//jesli 10 to nie

		mvprintw(y, TARGET_X_MIN-2, "|");														//linie pionowe
		mvprintw(y, TARGET_X_MAX+2, "|");
		mvprintw(y, STATUS_X_MIN-2, "|");
		mvprintw(y, STATUS_X_MAX+2, "|");

		mvprintw(y, STATUS_X_MAX+4, "%d", cyferka + y - TARGET_Y_MIN);							//cyferki po prawej
	}
}

void odswiezPlansze() {
	int x, y, plansza, cyferka;
	char pole;
	y = x = plansza = 0;
	cyferka = 1;
	
	//rysuj wode na srodku

	
		for (y = TARGET_Y_MIN; y <= TARGET_Y_MAX ; y++) {
			for (x = TARGET_X_MIN; x <= TARGET_X_MAX ; x = x + 2 ) {
				move(y,x);
				odswiezPole(planszaGracza[(x - TARGET_X_MIN)/2][y - TARGET_Y_MIN].obiekt);/* {
					case woda:
						pole = '~';
						break;
					case statek:
						pole = 'S';
						break;
					case trafiony:
						pole = '*';
						break;
					case zatopiony:
						pole = '#';
						break;
					case pudlo:
						pole = 'O';
						break;
				}


				attron(COLOR_PAIR(WODA));
				mvprintw(y, x, "%c", pole);
				attroff(COLOR_PAIR(WODA));*/
			}
		}
}

void zmienStan(int x, int y) {
	//planszaGracza[x][y].obiekt = statek;
	//planszaGracza[x][y].obiekt = ((planszaGracza[x][y].obiekt) % 6)+1;
	x = (x - TARGET_X_MIN)/2;
	y = y - TARGET_Y_MIN;

	switch ( planszaGracza[x][y].obiekt ) {
		case woda:
			planszaGracza[x][y].obiekt = statek_ustawiany;
			break;
		case statek_ustawiany:
			planszaGracza[x][y].obiekt = statek;
			break;
		case statek:
			planszaGracza[x][y].obiekt = trafiony;
			break;
		case trafiony:
			planszaGracza[x][y].obiekt = zatopiony;
			break;
		case zatopiony:
			planszaGracza[x][y].obiekt = pudlo;
			break;
		case pudlo:
			planszaGracza[x][y].obiekt = woda;
			break;
	}

}

void odswiezPole(int stan) {
	attron(COLOR_PAIR(stan));
	switch (stan) {
		case WODA:
			printw("~");						//zaznaczanie statek	
			break;
		case WODA_NIEDOZWOLONA:
			printw("-");						//zaznaczanie statek	
			break;
		case STATEK_USTAWIANY:
			printw("U");						//zaznaczanie statek
			break;
		case STATEK:
			printw("S");						//zaznaczanie statek
			break;
		case TRAFIONY:
			printw("*");						//zaznaczanie trafienie
			break;
		case ZATOPIONY:
			printw("#");
			break;
		case PUDLO:
			printw("O");						//zaznaczanie pudlo
			break;
	}
	attroff(COLOR_PAIR(stan));
}