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
void ruchKursora();
void wyswietlSciage();
void rysujPodgladOkretow();
void odswiezPodgladOkretow();
void czytajKlawisz();

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

struct Player {
	int okret_4;
	int okret_3;
	int okret_2;
	int okret_1;
};

struct Player gracz;
struct Player komputer;


int x_cur, y_cur;

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

	//inicjalizacja stanu okretow gracza i komputera

	gracz.okret_4 = 4;
	gracz.okret_3 = 6;
	gracz.okret_2 = 6;
	gracz.okret_1 = 4;

	komputer.okret_4 = 4;
	komputer.okret_3 = 6;
	komputer.okret_2 = 6;
	komputer.okret_1 = 4;


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

	wyswietlSciage();
	
	rysujPlansze();

	x_cur = TARGET_X_MIN;
	y_cur = TARGET_Y_MIN;

	//ruch kursorem


	move(y_cur,x_cur);
	refresh();
	odswiezPlansze();

	rysujPodgladOkretow();



/*
			TESTOWE ZAZNACZANIE STATUSU OKRETOW

	komputer.okret_1 = 0;
	odswiezPodgladOkretow();


*/

	czytajKlawisz();



	
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


	mvprintw(TARGET_Y_MIN-2, TARGET_X_MIN-1, " A B C D E F G H I J       A B C D E F G H I J ");
	mvaddch(TARGET_Y_MIN-2, TARGET_X_MIN-2, ACS_VLINE);
	mvaddch(TARGET_Y_MIN-2, TARGET_X_MAX+2, ACS_VLINE);
	mvaddch(STATUS_Y_MIN-2, STATUS_X_MIN-2, ACS_VLINE);
	mvaddch(STATUS_Y_MIN-2, STATUS_X_MAX+2, ACS_VLINE);

	//rysuje poziome ramki na gorze i na dole

	int fori;
	mvaddch(TARGET_Y_MIN-1, TARGET_X_MIN-5, ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_PLUS);

	for (fori = 0; fori < TARGET_X_MAX - TARGET_X_MIN + 3; ++fori)
		{
			addch(ACS_HLINE);
		}
	addch(ACS_RTEE);

	mvaddch(STATUS_Y_MIN-1, STATUS_X_MIN-2, ACS_LTEE);
	for (fori = 0; fori < STATUS_X_MAX - STATUS_X_MIN + 3; ++fori)
		{
			addch(ACS_HLINE);
		}
	addch(ACS_PLUS);
	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);

	mvaddch(TARGET_Y_MAX+1, TARGET_X_MIN-5, ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_BTEE);

	for (fori = 0; fori < TARGET_X_MAX - TARGET_X_MIN + 3; ++fori)
		{
			addch(ACS_HLINE);
		}
	addch(ACS_LRCORNER);

	mvaddch(STATUS_Y_MAX+1, STATUS_X_MIN-2, ACS_LLCORNER);

	for (fori = 0; fori < STATUS_X_MAX - STATUS_X_MIN + 3; ++fori)
		{
			addch(ACS_HLINE);
		}
	addch(ACS_BTEE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);
	addch(ACS_HLINE);
	

	//rysuje cyferki po boku i ramki pionowe

	for ( y = TARGET_Y_MIN; y <= TARGET_Y_MAX; y++) {
																								//cyferki po lewej
		if (y < TARGET_Y_MAX ) mvprintw(y, TARGET_X_MIN-5, " %d", cyferka + y - TARGET_Y_MIN);	//jesli 0-9 to zero wiodace
		else mvprintw(y, TARGET_X_MIN-5, "%d", cyferka + y - TARGET_Y_MIN);						//jesli 10 to nie

		mvaddch(y, TARGET_X_MIN-2, ACS_VLINE);														//linie pionowe
		mvaddch(y, TARGET_X_MAX+2, ACS_VLINE);
		mvaddch(y, STATUS_X_MIN-2, ACS_VLINE);
		mvaddch(y, STATUS_X_MAX+2, ACS_VLINE);

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
				odswiezPole((x - TARGET_X_MIN) + 16, y - TARGET_Y_MIN + 7);/* {
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

void zmienStan(int x_cur, int y_cur) {
	//planszaGracza[x][y].obiekt = statek;
	//planszaGracza[x][y].obiekt = ((planszaGracza[x][y].obiekt) % 6)+1;
	int x, y;
	x = x_cur/2 - 7;
	y = y_cur - 6;

	mvprintw (0, WIDTH-21, "zmiana x = %c, y = %d ", x+64, y);
	mvprintw (1, WIDTH-18, "tab x = %d, y = %d ", x-1, y-1);

	switch ( planszaGracza[x-1][y-1].obiekt ) {
		case woda:
			planszaGracza[x-1][y-1].obiekt = statek_ustawiany;
			break;
		case statek_ustawiany:
			planszaGracza[x-1][y-1].obiekt = statek;
			break;
		case statek:
			planszaGracza[x-1][y-1].obiekt = trafiony;
			break;
		case trafiony:
			planszaGracza[x-1][y-1].obiekt = zatopiony;
			break;
		case zatopiony:
			planszaGracza[x-1][y-1].obiekt = pudlo;
			break;
		case pudlo:
			planszaGracza[x-1][y-1].obiekt = woda;
			break;
	}
	mvprintw (0, WIDTH-40, "status: %d", planszaGracza[x-1][y-1].obiekt);

}

void odswiezPole(int x_cur, int y_cur) {
	int x, y;
	x = x_cur/2 - 7;
	y = y_cur - 6;

	int stan = planszaGracza[x -1][y - 1].obiekt;
	attron(COLOR_PAIR(stan));
	switch (stan) {
		case WODA:
			mvprintw(y+6,(x+7)*2+1,"~");						//zaznaczanie statek	
			break;
		case WODA_NIEDOZWOLONA:
			mvprintw(y+6,(x+7)*2+1,"-");						//zaznaczanie statek	
			break;
		case STATEK_USTAWIANY:
			mvprintw(y+6,(x+7)*2+1,"U");						//zaznaczanie statek
			break;
		case STATEK:
			mvprintw(y+6,(x+7)*2+1,"S");						//zaznaczanie statek
			break;
		case TRAFIONY:
			mvprintw(y+6,(x+7)*2+1,"*");						//zaznaczanie trafienie
			break;
		case ZATOPIONY:
			mvprintw(y+6,(x+7)*2+1,"#");
			break;
		case PUDLO:
			mvprintw(y+6,(x+7)*2+1,"O");						//zaznaczanie pudlo
			break;
	}
	attroff(COLOR_PAIR(stan));
}
void ruchKursora(int key) {
	switch( key ) {
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
	}
	mvprintw(0,0, "Polozenie kursora: x = %c, y = %d ", (x_cur/2 - 7)+64, y_cur-6);
	mvprintw(1,15, "tab x = %d, y = %d", (x_cur/2 - 7)-1, y_cur-7);


	mvprintw (0, WIDTH-40, "status: %d", planszaGracza[x_cur/2 - 8][y_cur-7].obiekt);
}

void wyswietlSciage() {
	int szerokosc_pojedynczej_sciagi;
	szerokosc_pojedynczej_sciagi = 12;

	attron(COLOR_PAIR(WODA));
	mvprintw(HEIGHT-3, 0*szerokosc_pojedynczej_sciagi, "~");
	attroff(COLOR_PAIR(WODA));

	attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
	mvprintw(HEIGHT-3, 1*szerokosc_pojedynczej_sciagi, "-");
	attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));

	attron(COLOR_PAIR(STATEK_USTAWIANY));
	mvprintw(HEIGHT-3, 2*szerokosc_pojedynczej_sciagi, "U");
	attroff(COLOR_PAIR(STATEK_USTAWIANY));

	attron(COLOR_PAIR(STATEK));
	mvprintw(HEIGHT-3, 3*szerokosc_pojedynczej_sciagi, "S");
	attroff(COLOR_PAIR(STATEK));

	attron(COLOR_PAIR(TRAFIONY));
	mvprintw(HEIGHT-3, 4*szerokosc_pojedynczej_sciagi, "*");
	attroff(COLOR_PAIR(TRAFIONY));

	attron(COLOR_PAIR(ZATOPIONY));
	mvprintw(HEIGHT-3, 5*szerokosc_pojedynczej_sciagi, "#");
	attroff(COLOR_PAIR(ZATOPIONY));

	attron(COLOR_PAIR(PUDLO));
	mvprintw(HEIGHT-3, 6*szerokosc_pojedynczej_sciagi, "O");
	attroff(COLOR_PAIR(PUDLO));

	attron(A_REVERSE);

	mvprintw(HEIGHT-3, 1 + 0*szerokosc_pojedynczej_sciagi, " woda      ");
	mvprintw(HEIGHT-3, 1 + 1*szerokosc_pojedynczej_sciagi, " zajete    ");
	mvprintw(HEIGHT-3, 1 + 2*szerokosc_pojedynczej_sciagi, " ustawiany ");
	mvprintw(HEIGHT-3, 1 + 3*szerokosc_pojedynczej_sciagi, " statek    ");
	mvprintw(HEIGHT-3, 1 + 4*szerokosc_pojedynczej_sciagi, " trafiony  ");
	mvprintw(HEIGHT-3, 1 + 5*szerokosc_pojedynczej_sciagi, " zatopiony ");
	mvprintw(HEIGHT-3, 1 + 6*szerokosc_pojedynczej_sciagi, " pudlo     ");

	mvprintw(HEIGHT-2, 0, "Uzyj strzalek do poruszania kursorem. Spacja zaznacza kratke. Q konczy program. ");
	mvprintw(HEIGHT-1, 0, "R resetuje plansze. 1, 2, 3, 4, 5 zaznaczaja testowo.                           ");
	
	attroff(A_REVERSE);
}

void rysujPodgladOkretow() {

	mvprintw(4, 5, "Gracz");
	mvprintw(4, WIDTH-9, "AI");


	attron(COLOR_PAIR(STATEK));
	
		mvprintw (9, 7, "SSSS");

		mvprintw(11, 4,   "SSS");
		mvprintw(11, 8,   "SSS");

		mvprintw(13, 3,   "SS");
		mvprintw(13, 6,   "SS");
		mvprintw(13, 9,   "SS");
		
		mvprintw(15, 4,  "S");
		mvprintw(15, 6,  "S");
		mvprintw(15, 8,  "S");
		mvprintw(15, 10,  "S");



		mvprintw (9, 68, "SSSS");

		mvprintw(11, 68,   "SSS");
		mvprintw(11, 72,   "SSS");

		mvprintw(13, 68,   "SS");
		mvprintw(13, 71,   "SS");
		mvprintw(13, 74,   "SS");
		
		mvprintw(15, 68,  "S");
		mvprintw(15, 70,  "S");
		mvprintw(15, 72,  "S");
		mvprintw(15, 74,  "S");

	attroff(COLOR_PAIR(STATEK));
}

void odswiezPodgladOkretow() {
		
		if (gracz.okret_4 == 0) {
			attron(COLOR_PAIR(ZATOPIONY));
			mvprintw (9, 7, "####");
			attroff(COLOR_PAIR(ZATOPIONY));
		}

		switch (gracz.okret_3) {
			case 3:
			case 2:
			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw (11, 4, "###");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw (11, 4, "###");
				mvprintw (11, 8, "###");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}

		switch (gracz.okret_2) {
			case 4:
			case 3:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 3,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 2:
			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 3,   "##");
				mvprintw(13, 6,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 3,   "##");
				mvprintw(13, 6,   "##");
				mvprintw(13, 9,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}

		switch (gracz.okret_1) {
			case 3:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 4,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 2:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 4,  "#");
				mvprintw(15, 6,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 4,  "#");
				mvprintw(15, 6,  "#");
				mvprintw(15, 8,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 4,  "#");
				mvprintw(15, 6,  "#");
				mvprintw(15, 8,  "#");
				mvprintw(15, 10,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}




		if (komputer.okret_4 == 0) {
			attron(COLOR_PAIR(ZATOPIONY));
			mvprintw (9, 68, "####");
			attroff(COLOR_PAIR(ZATOPIONY));
		}

		switch (komputer.okret_3) {
			case 3:
			case 2:
			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw (11, 68, "###");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw (11, 68, "###");
				mvprintw (11, 72, "###");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}

		switch (komputer.okret_2) {
			case 4:
			case 3:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 68,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 2:
			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 68,   "##");
				mvprintw(13, 71,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(13, 68,   "##");
				mvprintw(13, 71,   "##");
				mvprintw(13, 74,   "##");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}

		switch (komputer.okret_1) {
			case 3:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 68,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 2:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 68,  "#");
				mvprintw(15, 70,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 1:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 68,  "#");
				mvprintw(15, 70,  "#");
				mvprintw(15, 72,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;

			case 0:
				attron(COLOR_PAIR(ZATOPIONY));
				mvprintw(15, 68,  "#");
				mvprintw(15, 70,  "#");
				mvprintw(15, 72,  "#");
				mvprintw(15, 74,  "#");
				attroff(COLOR_PAIR(ZATOPIONY));
				break;
		}

}

void czytajKlawisz() {
	int key;

	while ( (key = getch() ) !='q') {

		if ( key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT ) {
			ruchKursora(key);
		}
		else
		switch ( key ) {
			
	#if DEBUG == 1
			
			case '1':
				attron(COLOR_PAIR(WODA));
				printw("~");						//zaznaczanie statek
				attroff(COLOR_PAIR(WODA));
				break;
			case '2':
				attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
				printw("-");						//zaznaczanie woda niedozwolona
				attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));
				break;					
			case '3':
				attron(COLOR_PAIR(STATEK));
				printw("S");						//zaznaczanie statek
				attroff(COLOR_PAIR(STATEK));
				break;
			case '4':
				attron(COLOR_PAIR(TRAFIONY));
				printw("*");						//zaznaczanie trafienie
				attroff(COLOR_PAIR(TRAFIONY));
				break;
			case '5':
				attron(COLOR_PAIR(PUDLO));
				printw("O");						//zaznaczanie pudlo
				attroff(COLOR_PAIR(PUDLO));
				break;	


	#endif
					
			case ' ':
				//planszaGracza[x_cur][y_cur].obiekt = statek;
				zmienStan(x_cur, y_cur);			//zmiana stanu
				//printw("%d",planszaGracza[x_cur][y_cur].obiekt );
				odswiezPole(x_cur, y_cur);
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

};