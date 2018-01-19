#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define DEBUG 0


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

void ustawKolory();
void odswiezLewaPlansze();
void rysujPlansze();
void zmienStanDebug();
void zmienStan();
void odswiezPole();
void ruchKursora();
void wyswietlSciage();
void rysujPodgladOkretow();
//void odswiezPodgladOkretow();
void czytajKlawisz();
void inicjalizujPlansze();
void ustawOkretyGracza();
void ustawOkret();
int sprawdzMiejsce();
void gra();
void turaGracza();
void turaAI();
int strzalGracza();
int strzalAI();
int sprawdzMiejsceAI();
void ustawOkretyAI();
void ustawOkretAI();
void zmienStanAI();

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

/*struct Player {
	int okret_4;
	int okret_3;
	int okret_2;
	int okret_1;
};

struct Player gracz;
struct Player komputer;
*/

int kratkiOkretowGracza;
int kratkiOkretowAI;

int x_cur, y_cur;
int zwyciestwo;

int main()
{
	
	//inicjalizacja planszy gracza i AI

	inicjalizujPlansze();
	
	//inicjalizacja stanu okretow gracza i komputera

	kratkiOkretowGracza = 20;
	kratkiOkretowAI = 20;


	initscr();

	start_color();

	ustawKolory();

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
	odswiezLewaPlansze();

	int seed;
	time_t czas;
    seed = time(&czas);
    srand(seed);

	ustawOkretyGracza();
	ustawOkretyAI();

	//rysujPlansze();

	//rysujPodgladOkretow();

	gra();
	
	endwin();
	return 0;
}

void ustawKolory() {
	init_pair(WODA, COLOR_CYAN, COLOR_BLUE);
	init_pair(WODA_NIEDOZWOLONA, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(STATEK_USTAWIANY, COLOR_MAGENTA, COLOR_YELLOW);
	init_pair(STATEK, COLOR_MAGENTA, COLOR_GREEN);
	init_pair(TRAFIONY, COLOR_YELLOW, COLOR_RED);
	init_pair(ZATOPIONY, COLOR_RED, COLOR_BLACK);
	init_pair(PUDLO, COLOR_BLUE, COLOR_BLACK);
	init_pair(KURSOR, COLOR_BLACK, COLOR_GREEN);
}

void rysujPlansze() {
	int x, y, plansza, cyferka;
	char pole;
	y = x = 0;
	pole = '~';	//woda
	cyferka = 1;
	
	//rysuj wode na srodku

	//plansza gracza

	
	for (y = TARGET_Y_MIN; y <= TARGET_Y_MAX ; y++) {
		for (x = TARGET_X_MIN; x <= TARGET_X_MAX; x = x + 2 ) {
			odswiezPole(x, y);
		}
	}

	//plansza AI


	for (y = STATUS_Y_MIN; y <= STATUS_Y_MAX ; y++) {
		for (x = STATUS_X_MIN; x <= STATUS_X_MAX; x = x + 2 ) {
			odswiezPole(x, y);
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

void odswiezLewaPlansze() {
	int x, y;
	//int cyferka;
	char pole;
	y = x  = 0;
	//cyferka = 1;
	
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

void zmienStanDebug(int x_curs, int y_curs) {
	//planszaGracza[x][y].obiekt = statek;
	//planszaGracza[x][y].obiekt = ((planszaGracza[x][y].obiekt) % 6)+1;
	int x, y;
	x = x_curs/2 - 7;
	y = y_curs - 6;

	//mvprintw (0, WIDTH-21, "zmiana x = %c, y = %d ", x+64, y);
	//mvprintw (1, WIDTH-18, "tab x = %d, y = %d ", x-1, y-1);

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
//	mvprintw (0, WIDTH-40, "status: %d", planszaGracza[x-1][y-1].obiekt);

}

void zmienStan(int x_curs, int y_curs, int stan) {
	//planszaGracza[x][y].obiekt = statek;
	//planszaGracza[x][y].obiekt = ((planszaGracza[x][y].obiekt) % 6)+1;
	int x, y;
	x = x_curs/2 - 7;
	y = y_curs - 6;
	if (x > 0 && x <= 10 && y > 0 && y <= 10) {
	//	mvprintw (0, WIDTH-21, "zmiana x = %c, y = %d ", x+64, y);
	//	mvprintw (1, WIDTH-18, "tab x = %d, y = %d ", x-1, y-1);

		switch ( stan ) {
			case WODA:
				planszaGracza[x-1][y-1].obiekt = woda;
				break;
			case STATEK_USTAWIANY:
				planszaGracza[x-1][y-1].obiekt = statek_ustawiany;
				break;
			case STATEK:
				planszaGracza[x-1][y-1].obiekt = statek;
				planszaGracza[x-1][y-1].czyMoznaUstawicStatek = niedozwolony;
				break;
			case TRAFIONY:
				planszaGracza[x-1][y-1].obiekt = trafiony;
				break;
			case ZATOPIONY:
				planszaGracza[x-1][y-1].obiekt = zatopiony;
				break;
			case PUDLO:
				planszaGracza[x-1][y-1].obiekt = pudlo;
				break;
			case WODA_NIEDOZWOLONA:
				planszaGracza[x-1][y-1].obiekt = woda;
				planszaGracza[x-1][y-1].czyMoznaUstawicStatek = niedozwolony;
				break;
		}
	//	mvprintw (0, WIDTH-40, "status: %d", planszaGracza[x-1][y-1].obiekt);
	//	mvprintw (1, WIDTH-40, "dozwol: %d", planszaGracza[x-1][y-1].czyMoznaUstawicStatek);
	}
}

void odswiezPole(int x_curs, int y_curs) {
	int x, y;
	int stan;
	int dozw;
	
	y = y_curs - 6;
	if (x_curs < 36) {
		x = x_curs/2 - 7;
		stan = planszaGracza[x - 1][y - 1].obiekt;
		dozw = planszaGracza[x - 1][y - 1].czyMoznaUstawicStatek;
		attron(COLOR_PAIR(stan));
		switch (stan) {
			case WODA:
				if (dozw == dozwolony) {
					mvprintw(y+6,(x+7)*2+1,"~");						//zaznaczanie statek	
				} else {
					attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
					mvprintw(y+6,(x+7)*2+1,"-");
					attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));
				}
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

	else  if (x_curs > 42 ) {
		x = (x_curs-1)/2 - 20;
		stan = planszaAI[x -1][y - 1].obiekt;
		dozw = planszaAI[x -1][y - 1].czyMoznaUstawicStatek;
		attron(COLOR_PAIR(stan));
		switch (stan) {
			case WODA:
				if (dozw == dozwolony) {
					mvprintw(y_curs, x_curs,"~");						//zaznaczanie statek	
				} else {
					attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
					mvprintw(y+6,(x+7)*2+1,"-");
					attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));
				}
				break;
			case STATEK_USTAWIANY:
				mvprintw(y_curs, x_curs,"U");						//zaznaczanie statek
				break;
			case STATEK:
				mvprintw(y_curs, x_curs,"S");						//zaznaczanie statek
				break;
			case TRAFIONY:
				mvprintw(y_curs, x_curs,"*");						//zaznaczanie trafienie
				break;
			case ZATOPIONY:
				mvprintw(y_curs, x_curs,"#");
				break;
			case PUDLO:
				mvprintw(y_curs, x_curs,"O");						//zaznaczanie pudlo
				break;
		}
		attroff(COLOR_PAIR(stan));
	}


	
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

#if DEBUG == 1	

	mvprintw(0,0, "Polozenie kursora: x = %c, y = %d ", (x_cur/2 - 7)+64, y_cur-6);
	mvprintw(1,15, "tab x = %d, y = %d", (x_cur/2 - 7)-1, y_cur-7);
	mvprintw(2,15, "cur x = %d, y = %d", x_cur, y_cur);

	mvprintw (0, WIDTH-40, "status: %d", planszaGracza[x_cur/2 - 8][y_cur-7].obiekt);
	mvprintw (1, WIDTH-40, "dozwol: %d", planszaGracza[x_cur/2 - 8][y_cur-7].czyMoznaUstawicStatek);

	move(y_cur, x_cur);


#endif
}

void wyswietlSciage() {
	int szerokosc_pojedynczej_sciagi;
	szerokosc_pojedynczej_sciagi = 14;

	attron(COLOR_PAIR(WODA));
	mvprintw(HEIGHT-2, 0*szerokosc_pojedynczej_sciagi, "~");
	attroff(COLOR_PAIR(WODA));

	attron(COLOR_PAIR(WODA_NIEDOZWOLONA));
	mvprintw(HEIGHT-2, 1*szerokosc_pojedynczej_sciagi, "-");
	attroff(COLOR_PAIR(WODA_NIEDOZWOLONA));

	attron(COLOR_PAIR(STATEK_USTAWIANY));
	mvprintw(HEIGHT-2, 2*szerokosc_pojedynczej_sciagi, "U");
	attroff(COLOR_PAIR(STATEK_USTAWIANY));

	attron(COLOR_PAIR(STATEK));
	mvprintw(HEIGHT-2, 3*szerokosc_pojedynczej_sciagi, "S");
	attroff(COLOR_PAIR(STATEK));

	attron(COLOR_PAIR(TRAFIONY));
	mvprintw(HEIGHT-2, 4*szerokosc_pojedynczej_sciagi, "*");
	attroff(COLOR_PAIR(TRAFIONY));

//	attron(COLOR_PAIR(ZATOPIONY));
//	mvprintw(HEIGHT-3, 5*szerokosc_pojedynczej_sciagi, "#");
//	attroff(COLOR_PAIR(ZATOPIONY));

	attron(COLOR_PAIR(PUDLO));
	mvprintw(HEIGHT-2, 5*szerokosc_pojedynczej_sciagi, "O");
	attroff(COLOR_PAIR(PUDLO));

	attron(A_REVERSE);

	mvprintw(HEIGHT-2, 1 + 0*szerokosc_pojedynczej_sciagi, " woda        ");
	mvprintw(HEIGHT-2, 1 + 1*szerokosc_pojedynczej_sciagi, " zajete      ");
	mvprintw(HEIGHT-2, 1 + 2*szerokosc_pojedynczej_sciagi, " ustawiany   ");
	mvprintw(HEIGHT-2, 1 + 3*szerokosc_pojedynczej_sciagi, " statek      ");
	mvprintw(HEIGHT-2, 1 + 4*szerokosc_pojedynczej_sciagi, " trafiony    ");
//	mvprintw(HEIGHT-2, 1 + 5*szerokosc_pojedynczej_sciagi, " zatopiony ");
	mvprintw(HEIGHT-2, 1 + 5*szerokosc_pojedynczej_sciagi, " pudlo   ");

	mvprintw(HEIGHT-1, 0, "Uzyj strzalek do poruszania kursorem. Spacja zatwierdza. R obraca statek.       ");
	//mvprintw(HEIGHT, 0, "F5 resetuje plansze. 1, 2, 3, 4, 5 zaznaczaja testowo.                          ");
	
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
/*
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
*/
void czytajKlawisz() {
	int key;

	while ( (key = getch() ) !=' ') {

		if (zwyciestwo != 0) {
			break;
		}
	/*	else
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
				zmienStanDebug(x_cur, y_cur);			//zmiana stanu
				//printw("%d",planszaGracza[x_cur][y_cur].obiekt );
				odswiezPole(x_cur, y_cur);
				//odswiezLewaPlansze();
				break;			
	//		case KEY_F(5):								//reset planszy
	//			inicjalizujPlansze();
	//			rysujPlansze();
	//			break;                     
		}					*/
		key = 0;

		move(y_cur, x_cur);
		refresh();
	}

}

void inicjalizujPlansze() {
	int iter_x, iter_y;
	for ( iter_y = 0; iter_y < 10; iter_y++ ) {
		for ( iter_x = 0; iter_x < 10; iter_x++ ) {
			planszaGracza[iter_x][iter_y].obiekt = woda;
			planszaGracza[iter_x][iter_y].czyMoznaUstawicStatek = dozwolony;
			planszaAI[iter_x][iter_y].obiekt = woda;
			planszaAI[iter_x][iter_y].czyMoznaUstawicStatek = dozwolony;
		}
	}
	/*

	planszaAI[4][0].obiekt = statek;
	planszaAI[4][1].obiekt = statek;
	planszaAI[4][2].obiekt = statek;
	planszaAI[4][3].obiekt = statek;
	planszaAI[4][4].obiekt = statek;
	planszaAI[4][5].obiekt = statek;
	planszaAI[4][6].obiekt = statek;
	planszaAI[4][7].obiekt = statek;
	planszaAI[4][8].obiekt = statek;
	planszaAI[4][9].obiekt = statek;
	planszaAI[5][0].obiekt = statek;
	planszaAI[5][1].obiekt = statek;
	planszaAI[5][2].obiekt = statek;
	planszaAI[5][3].obiekt = statek;
	planszaAI[5][4].obiekt = statek;
	planszaAI[5][5].obiekt = statek;
	planszaAI[5][6].obiekt = statek;
	planszaAI[5][7].obiekt = statek;
	planszaAI[5][8].obiekt = statek;
	planszaAI[5][9].obiekt = statek;
	*/
}

void ustawOkretyGracza() {
	int i;
	y_cur = TARGET_Y_MIN;
	x_cur = TARGET_X_MIN;
	ustawOkret(4);
	for (i = 0; i < 2; ++i) {
		ustawOkret(3);
	}
	for (i = 0; i < 3; ++i) {
		ustawOkret(2);
	}
	for (i = 0; i < 4; ++i) {
		ustawOkret(1);
	}
}

void ustawOkret(int wielkoscOkretu) {
	
	move(y_cur, x_cur);
	int klawisz;
	int i;
	int obrot;
	int hasToBreak;
	hasToBreak = 0;
	switch (wielkoscOkretu) {
		case 1:
			while ( (klawisz = getch() ) != 'q') {
				odswiezLewaPlansze();
				if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
					ruchKursora(klawisz);
				}
				attron(COLOR_PAIR(STATEK_USTAWIANY));
				mvprintw(y_cur,x_cur,"U");
				attroff(COLOR_PAIR(STATEK_USTAWIANY));
				move(y_cur, x_cur);
				if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1) {
					int xdelta;
					int ydelta;
					for (xdelta = (-2); xdelta <= 2; xdelta = xdelta + 2) {
						for (ydelta = (-1); ydelta <=1; ++ydelta) {
							zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
						}
					}
					move(y_cur, x_cur);
					zmienStan(x_cur, y_cur, STATEK);
					move(y_cur, x_cur);
					odswiezLewaPlansze();
					break;
				}
			}
			break;
		case 2:
			obrot = 0;
			while ( (klawisz = getch() ) != 'q') {
				odswiezLewaPlansze();
				move(y_cur, x_cur);



				switch (klawisz) {
					case 'r':
						if (x_cur == 35) {
							x_cur = x_cur - 2;
						}
						if (y_cur == 16) {
							--y_cur;
						}
						obrot = (obrot + 1)%2;
						if (obrot == 0) {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						} else {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						}
						break;
					default:
						if (obrot == 0) {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ( (x_cur == 33 && klawisz == KEY_RIGHT ) || (x_cur == 17 && klawisz == KEY_LEFT) ) {
									x_cur = 35;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur+2, y_cur) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 4; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=1; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur + 2, y_cur, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						} else {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ( (y_cur == 15 && klawisz == KEY_DOWN ) || (y_cur == 7 && klawisz == KEY_UP) ) {
									y_cur = 16;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur, y_cur+1) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 2; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=2; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur, y_cur + 1, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						}
				}
				if (hasToBreak == 1) {
					break;
				}


			}
			break;

		case 3:
			obrot = 0;
			while ( (klawisz = getch() ) != 'q') {
				odswiezLewaPlansze();
				move(y_cur, x_cur);



				switch (klawisz) {
					case 'r':
						if (x_cur == 33 || x_cur == 35) {
							x_cur = 31;
						}
						if (y_cur == 15 || y_cur == 16) {
							y_cur = 14;
						}
						obrot = (obrot + 1)%2;
						if (obrot == 0) {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							mvprintw(y_cur,x_cur+4,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						} else {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							mvprintw(y_cur+2,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						}
						break;
					default:
						if (obrot == 0) {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ((x_cur == 31 || x_cur == 33) && klawisz == KEY_RIGHT ) {
									x_cur = 35;
								}
								if  (x_cur == 17 && klawisz == KEY_LEFT) {
									x_cur = 33;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							mvprintw(y_cur,x_cur+4,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur+2, y_cur) == 1 && sprawdzMiejsce(x_cur+4, y_cur) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 6; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=1; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur + 2, y_cur, STATEK);
								zmienStan(x_cur + 4, y_cur, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						} else {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ( ((y_cur == 14 || y_cur == 15) && klawisz == KEY_DOWN )) {
								 	y_cur = 16;
								}
								if (y_cur == 7 && klawisz == KEY_UP) {
									y_cur = 15;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							mvprintw(y_cur+2,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur, y_cur+1) == 1 && sprawdzMiejsce(x_cur, y_cur+2) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 2; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=3; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur, y_cur + 1, STATEK);
								zmienStan(x_cur, y_cur + 2, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						}
				}
				if (hasToBreak == 1) {
					break;
				}


			}
			break;
		case 4:
			obrot = 0;
			while ( (klawisz = getch() ) != 'q') {
				odswiezLewaPlansze();
				move(y_cur, x_cur);



				switch (klawisz) {
					case 'r':
						if (x_cur == 31 || x_cur == 33 || x_cur == 35) {
							x_cur = 29;
						}
						if (y_cur == 14 || y_cur == 15 || y_cur == 16) {
							y_cur = 13;
						}
						obrot = (obrot + 1)%2;
						if (obrot == 0) {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							mvprintw(y_cur,x_cur+4,"U");
							mvprintw(y_cur,x_cur+6,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						} else {
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							mvprintw(y_cur+2,x_cur,"U");
							mvprintw(y_cur+3,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
						}
						break;
					default:
						if (obrot == 0) {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ((x_cur == 29 || x_cur == 31 || x_cur == 33) && klawisz == KEY_RIGHT ) {
									x_cur = 35;
								}
								if  (x_cur == 17 && klawisz == KEY_LEFT) {
									x_cur = 31;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur,x_cur+2,"U");
							mvprintw(y_cur,x_cur+4,"U");
							mvprintw(y_cur,x_cur+6,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur+2, y_cur) == 1 && sprawdzMiejsce(x_cur+4, y_cur) == 1 && sprawdzMiejsce(x_cur+6, y_cur) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 8; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=1; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur + 2, y_cur, STATEK);
								zmienStan(x_cur + 4, y_cur, STATEK);
								zmienStan(x_cur + 6, y_cur, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						} else {
							if ( klawisz == KEY_UP || klawisz == KEY_DOWN || klawisz == KEY_LEFT || klawisz == KEY_RIGHT ) {
								if ( ((y_cur == 13 || y_cur == 14 || y_cur == 15) && klawisz == KEY_DOWN )) {
								 	y_cur = 16;
								}
								if (y_cur == 7 && klawisz == KEY_UP) {
									y_cur = 14;
								}
								ruchKursora(klawisz);
							}
							attron(COLOR_PAIR(STATEK_USTAWIANY));
							mvprintw(y_cur,x_cur,"U");
							mvprintw(y_cur+1,x_cur,"U");
							mvprintw(y_cur+2,x_cur,"U");
							mvprintw(y_cur+3,x_cur,"U");
							attroff(COLOR_PAIR(STATEK_USTAWIANY));
							move(y_cur, x_cur);
							if ( klawisz == ' ' && sprawdzMiejsce(x_cur, y_cur) == 1 && sprawdzMiejsce(x_cur, y_cur+1) == 1 && sprawdzMiejsce(x_cur, y_cur+2) == 1 && sprawdzMiejsce(x_cur, y_cur+3) == 1) {
								int xdelta;
								int ydelta;
								for (xdelta = (-2); xdelta <= 2; xdelta = xdelta + 2) {
									for (ydelta = (-1); ydelta <=4; ++ydelta) {
										zmienStan((x_cur + xdelta), (y_cur + ydelta), WODA_NIEDOZWOLONA);
									}
								}
								move(y_cur, x_cur);
								zmienStan(x_cur, y_cur, STATEK);
								zmienStan(x_cur, y_cur + 1, STATEK);
								zmienStan(x_cur, y_cur + 2, STATEK);
								zmienStan(x_cur, y_cur + 3, STATEK);
								odswiezLewaPlansze();
								move(y_cur, x_cur);
								hasToBreak = 1;
								break;
							}
						}
				}
				if (hasToBreak == 1) {
					break;
				}


			}
			break;
	}
	move(y_cur, x_cur);
}

int sprawdzMiejsce(int x_curs, int y_curs) {
	int x, y;

	
	y = y_curs - 6;
	if (x_curs < 36) {
		x = x_curs/2 - 7;
	}
	else  if (x_curs > 42 ) {
		x = (x_curs-1)/2 - 20;
	}
	mvprintw(WIDTH-40, 3, "Sprawdzam pole x= %d, y= %d", x, y);

	if ( planszaGracza[x-1][y-1].czyMoznaUstawicStatek == dozwolony ) {
		return 1;
	}
	else {
		return -1;
	}
}

void gra() {

	
	zwyciestwo = 0;											// ZMIEN NA 0

	x_cur = STATUS_X_MIN;
	y_cur = STATUS_Y_MIN;

	while (zwyciestwo == 0) {
	
		turaGracza();
		if (zwyciestwo == 1) {
			break;
		}
		turaAI();
	
	}

	if (zwyciestwo == 1) {
		mvprintw(7,17,  "                                             ");
		mvprintw(8,17,  "                                             ");
		mvprintw(9,17,  "                                             ");
		mvprintw(10,17, "                                             ");
		mvprintw(11,17, "                   WYGRANA                   ");
		mvprintw(12,17, "                                             ");
		mvprintw(13,17, "                                             ");
		mvprintw(14,17, "                                             ");
		mvprintw(15,17, "                                             ");
		mvprintw(16,17, "                                             ");
		curs_set(0);
	} else {
		mvprintw(7,17,  "                                             ");
		mvprintw(8,17,  "                                             ");
		mvprintw(9,17,  "                                             ");
		mvprintw(10,17, "                                             ");
		mvprintw(11,17, "                  PRZEGRANA                  ");
		mvprintw(12,17, "                                             ");
		mvprintw(13,17, "                                             ");
		mvprintw(14,17, "                                             ");
		mvprintw(15,17, "                                             ");
		mvprintw(16,17, "                                             ");
		curs_set(0);
	}
	czytajKlawisz();
}

void turaGracza() {
	int key;
	int strzal;
	int koniecTury;
	int czyTrafione;
	koniecTury = 0;
	while ( (key = getch())!= 'q' && koniecTury == 0 && zwyciestwo == 0) {

		
		switch( key ) {
			case KEY_UP:
				if ( y_cur > STATUS_Y_MIN ) {
					y_cur -= 1;
				} else {
					y_cur = STATUS_Y_MAX;
				}
				break;
			case KEY_DOWN:
				if ( y_cur < STATUS_Y_MAX ) {
					y_cur += 1;
				} else {
					y_cur = STATUS_Y_MIN;
				}
				break;
			case KEY_LEFT:
				if ( x_cur > STATUS_X_MIN ) {
					x_cur -= 2;
				} else {
					x_cur = STATUS_X_MAX;
				}
				break;
			case KEY_RIGHT:
				if ( x_cur < STATUS_X_MAX ) {
					x_cur += 2;
				} else {
					x_cur = STATUS_X_MIN;
				}
				break;
			case ' ':
				czyTrafione = strzalGracza(x_cur, y_cur);
				if (czyTrafione == 0 ) {
					koniecTury = 1;
					break;			
				}
				break;
			case 'x':
				attron(COLOR_PAIR(PUDLO));
				mvprintw(y_cur, x_cur, "X");						//zaznaczanie woda niedozwolona
				attroff(COLOR_PAIR(PUDLO));
				break;
			case 'p':
				attron(COLOR_PAIR(WODA));
				mvprintw(y_cur, x_cur, "~");						//zaznaczanie woda niedozwolona
				attroff(COLOR_PAIR(WODA));
				break;
			case 'm':
				zwyciestwo = 1;
				break;
			case 'n':
				zwyciestwo = -1;
				break;
		}
		
		key = 0;

		move(y_cur, x_cur);
		refresh();
	}
}

void turaAI() {
	int koniecTuryAI;
	do {
		koniecTuryAI = strzalAI();
	} while (koniecTuryAI == 0);
}

int strzalGracza(int x_curs, int y_curs) {
	int x_tab, y_tab;
	x_tab = (x_curs-43)/2;
	y_tab = y_curs - 7;

	if (planszaAI[x_tab][y_tab].obiekt == woda ) {
		planszaAI[x_tab][y_tab].obiekt = pudlo;
		odswiezPole(x_curs, y_curs);
		return 0;
	}
	else if (planszaAI[x_tab][y_tab].obiekt == statek ) {
		planszaAI[x_tab][y_tab].obiekt = trafiony;
		odswiezPole(x_curs, y_curs);
		kratkiOkretowAI--;
		mvprintw(2,42,"Pozostalo %d pol okretow.  ", kratkiOkretowAI);
		if (kratkiOkretowAI < 1) {
			zwyciestwo = 1;
		}
		return 1;
		

	}
	else if (planszaAI[x_tab][y_tab].obiekt == trafiony) {
		odswiezPole(x_curs, y_curs);
		return 1;
	}
	else if (planszaAI[x_tab][y_tab].obiekt == pudlo) {
		odswiezPole(x_curs, y_curs);
		return 1;
	}
	else {
		return 0;
	}

}

int strzalAI() {
	int x_traf, y_traf;
    
    x_traf = rand()%10;
    y_traf = rand()%10;

	if (planszaGracza[x_traf][y_traf].obiekt == woda ) {
		planszaGracza[x_traf][y_traf].obiekt = pudlo;
		odswiezPole((x_traf*2)+17, y_traf+7);
		move(y_cur, x_cur);
		refresh();
		return 1;
	}
	else if (planszaGracza[x_traf][y_traf].obiekt == statek ) {
		planszaGracza[x_traf][y_traf].obiekt = trafiony;
		--kratkiOkretowGracza;
		odswiezPole((x_traf*2)+17, y_traf+7);
		mvprintw(2,13,"Pozostalo %d pol okretow. ", kratkiOkretowGracza);
		if (kratkiOkretowGracza < 1) {
			zwyciestwo = -1;
		}
		move(y_cur, x_cur);
		refresh();
		return 0;
	}
	else if (planszaGracza[x_traf][y_traf].obiekt == trafiony ) {
		return 0;
	}
	else if (planszaGracza[x_traf][y_traf].obiekt == pudlo ) {
		return 0;
	} else {
		return 1;
	}
}

int sprawdzMiejsceAI(int x, int y) {

	if ( planszaAI[x][y].czyMoznaUstawicStatek == dozwolony ) {
		return 1;
	}
	else {
		return -1;
	}
}

void ustawOkretyAI() {
	int i;
	ustawOkretAI(4);
	for (i = 0; i < 2; ++i) {
		ustawOkretAI(3);
	}
	for (i = 0; i < 3; ++i) {
		ustawOkretAI(2);
	}
	for (i = 0; i < 4; ++i) {
		ustawOkretAI(1);
	}
}

void ustawOkretAI(int wielkoscOkretu) {
	
	int i;
	int x, y;
	int obrot;
	int czyStatekOK;
	czyStatekOK = 0;
	switch (wielkoscOkretu) {
		case 1:
			do {
				x = rand()%10;
				y = rand()%10;
				if ( sprawdzMiejsceAI(x, y) == 1) {
					czyStatekOK = 1;
					int xdelta;
					int ydelta;
					for (xdelta = (-1); xdelta <= 1; xdelta = xdelta + 1) {
						for (ydelta = (-1); ydelta <=1; ++ydelta) {
							zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
						}
					}
					zmienStanAI(x, y, STATEK);
					break;
				}
			} while (czyStatekOK != 1);
			break;
		case 2:
			do {
				obrot = rand()%2;
				if (obrot == 0) {
					x = rand()%9;
					y = rand()%10;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x+1, y) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 2; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <=1; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x + 1, y, STATEK);
						break;
					}
				} else {
					x = rand()%10;
					y = rand()%9;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x, y+1) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 1; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <=2; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x, y + 1, STATEK);
						break;
					}
				}
			} while (czyStatekOK != 1);
			break;
		case 3:
			do {
				obrot = rand()%2;
				if (obrot == 0) {
					x = rand()%8;
					y = rand()%10;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x+1, y) == 1 && sprawdzMiejsceAI(x+2, y) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 3; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <=1; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x + 1, y, STATEK);
						zmienStanAI(x + 2, y, STATEK);
						break;
					}
				} else {
					x = rand()%10;
					y = rand()%8;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x, y+1) == 1 && sprawdzMiejsceAI(x, y+2) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 1; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <=3; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x, y + 1, STATEK);
						zmienStanAI(x, y + 2, STATEK);
						break;
					}
				}
			} while (czyStatekOK != 1);
			break;
		case 4:
			do {
				obrot = rand()%2;
				if (obrot == 0) {
					x = rand()%7;
					y = rand()%10;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x+1, y) == 1 && sprawdzMiejsceAI(x+2, y) == 1 && sprawdzMiejsceAI(x+3, y) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 4; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <= 1; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x + 1, y, STATEK);
						zmienStanAI(x + 2, y, STATEK);
						zmienStanAI(x + 3, y, STATEK);
						break;
					}
				} else {
					x = rand()%10;
					y = rand()%7;
					if (sprawdzMiejsceAI(x, y) == 1 && sprawdzMiejsceAI(x, y+1) == 1 && sprawdzMiejsceAI(x, y+2) == 1 && sprawdzMiejsceAI(x, y+3) == 1) {
						czyStatekOK = 1;
						int xdelta;
						int ydelta;
						for (xdelta = (-1); xdelta <= 1; xdelta = xdelta + 1) {
							for (ydelta = (-1); ydelta <= 4; ++ydelta) {
								zmienStanAI((x + xdelta), (y + ydelta), WODA_NIEDOZWOLONA);
							}
						}
						zmienStanAI(x, y, STATEK);
						zmienStanAI(x, y + 1, STATEK);
						zmienStanAI(x, y + 2, STATEK);
						zmienStanAI(x, y + 3, STATEK);
						break;
					}
				}
			} while (czyStatekOK != 1);
			break;
	}
}

void zmienStanAI(int x, int y, int stan) {
	if (x >= 0 && x < 10 && y >= 0 && y < 10) {
		switch ( stan ) {
			case WODA:
				planszaAI[x][y].obiekt = woda;
				break;
			case STATEK_USTAWIANY:
				planszaAI[x][y].obiekt = statek_ustawiany;
				break;
			case STATEK:
				planszaAI[x][y].obiekt = statek;
				planszaAI[x][y].czyMoznaUstawicStatek = niedozwolony;
				break;
			case TRAFIONY:
				planszaAI[x][y].obiekt = trafiony;
				break;
			case ZATOPIONY:
				planszaAI[x][y].obiekt = zatopiony;
				break;
			case PUDLO:
				planszaAI[x][y].obiekt = pudlo;
				break;
			case WODA_NIEDOZWOLONA:
				planszaAI[x][y].obiekt = woda;
				planszaAI[x][y].czyMoznaUstawicStatek = niedozwolony;
				break;
		}

	}
}