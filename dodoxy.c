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

	//wyswietlanie tytulu
	attron(A_REVERSE);
	mvprintw(0, 0, "                                     OKRETY                                     ");
	attroff(A_REVERSE);
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

void ustawKolory() {}

void rysujPlansze() {}

void odswiezLewaPlansze() {}

void zmienStanDebug(int x_curs, int y_curs) {}

void zmienStan(int x_curs, int y_curs, int stan) {}

void odswiezPole(int x_curs, int y_curs) {}

void ruchKursora(int key) {}

void wyswietlSciage() {}

void rysujPodgladOkretow() {}

void czytajKlawisz() {}

void inicjalizujPlansze() {}

void ustawOkretyGracza() {}

void ustawOkret(int wielkoscOkretu) {}

int sprawdzMiejsce(int x_curs, int y_curs) {}

void gra() {}

void turaGracza() {}

void turaAI() {}

int strzalGracza(int x_curs, int y_curs) {}

int strzalAI() {}

int sprawdzMiejsceAI(int x, int y) {}

void ustawOkretyAI() {}

void ustawOkretAI(int wielkoscOkretu) {}

void zmienStanAI(int x, int y, int stan) {}