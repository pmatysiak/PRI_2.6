
/*! 
 *  \brief     Podstawy programowania - projekt 2.6 - Program realizujący grę „Okręty” w wersji dla jednego gracza.
Oponentem ma być „komputer-program”.
 *  \details   Zasady gry: Gracz rozstawia na planszy o rozmiarach 10x10 następujące okręty: czteryjednomasztowce (zajmujące jedną pozycję na planszy), trzy dwumasztowce (zajmujące dwa miejsca na planszy), dwa trzymasztowce (zajmujące trzy miejsca na planszy), jeden czteromasztowiec (zajmujący cztery miejsca na planszy). Okręty nie mogą się za sobą stykać w żaden sposób – również narożnikami pól, na których się znajdują. Oponent rozstawia swoje okręty losowo, zgodnie z powyższymi zasadami. Gracze naprzemiennie „strzelają” w planszę przeciwnika aż do zatopienia wszystkich jego okrętów. Po oddaniu strzału celnego gracz ma prawo strzelać dalej aż do chybienia.
 *  \author    Piotr Matysiak - grupa 1AR
 *  \date      2017-2018
 */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

/// zmienna do testowania
#define DEBUG 0

/// szerokosc terminala
#define WIDTH 80
/// wysokosc terminala
#define HEIGHT 24

/// pozycja lewej krawedzi lewej planszy
#define TARGET_X_MIN 17
/// pozycja prawe krawedzi lewej planszy
#define TARGET_X_MAX 17 + 18
/// pozycja gornej krawedzi lewej planszy
#define TARGET_Y_MIN 7
/// pozycja dolnej krawedzi lewej planszy
#define TARGET_Y_MAX 7 + 9

/// pozycja lewej krawedzi prawej planszy
#define STATUS_X_MIN 43
/// pozycja prawej krawedzi prawej planszy
#define STATUS_X_MAX 43 + 18
/// pozycja gornej krawedzi prawej planszy
#define STATUS_Y_MIN 7
/// pozycja dolnej krawedzi prawej planszy
#define STATUS_Y_MAX 7 + 9

/// obiekt woda
#define WODA 1
/// obiekt statek ustawiany w poczatkowej fazie gry
#define STATEK_USTAWIANY 2
/// obiekt statek
#define STATEK 3
/// obiekt statek trafiony
#define TRAFIONY 4
/// obiekt statek zatopiony - nieuzywany w tej wersji
#define ZATOPIONY 5
/// obiekt puste pole trafione
#define PUDLO 6
/// wyglad wody na ktorej nie mozna postawic okretu
#define WODA_NIEDOZWOLONA 7
/// wyglad kursora
#define KURSOR 8

/// Funkcja ustawKolory ustawia pary kolorow w ncurses dla roznych pol.

void ustawKolory() {}

/// Funkcja rysujPlansze rysuje na ekranie plansze z ramkami, literami i cyframi i wypelnia je woda.

void rysujPlansze() {}

/// Funkcja odswiezLewaPlansze wywoluje funkcje odswiezPole dla kazdego pola lewj planszy.

void odswiezLewaPlansze() {}

/// Funkcja zmienStan przyporzadkowuje polu odpowiedni obiekt: statek, statek ustawiany, statek trafiony, wode lub pudlo i ustawia legalnosc postawienia na polu statku.
/// \param[in] x_curs wspolrzedna x polozenia kursora
/// \param[in] y_curs wspolrzedna y polozenia kursora
/// \param[in] stan docelowy stan pola

void zmienStan(int x_curs, int y_curs, int stan) {}

/// Funkcja odswiezPole rysuje pojedyncze pole na planszy zgodnie z zawartoscia tabeli danych.
/// \param[in] x_curs wspolrzedna x polozenia kursora
/// \param[in] y_curs wspolrzedna y polozenia kursora

void odswiezPole(int x_curs, int y_curs) {}

/// Funkcja ruchKursora obsluguje ruch kursora po naciskaniu strzalek na klawiaturze.
/// \param[in] key wartosc liczbowa nacisnietego klawisza.

void ruchKursora(int key) {}

/// Funkcja wyswietlSciage wypisuje na dole ekranu przydatne informacje.

void wyswietlSciage() {}

/// Funkcja czytajKlawisz czeka na nacisniecie spacji aby wyjsc z gry.

void czytajKlawisz() {}

/// Funkcja inicjalizujPlansze wypelnia tablice danych gracza i komputera woda.

void inicjalizujPlansze() {}

/// Funkcja ustawOkretyGracza wywoluje funkcje ustawOkret dla czteromasztowca, dwoch trojmasztowcow, trzech dwumasztowcow i czterech jednomasztowcow gracza.

void ustawOkretyGracza() {}

/// Funkcja ustawOkret probuje ustawic na planszy okret danej wielkosci w miejscu okreslonym przez gracza. Aby sprawdzic legalnosc ruchu wywoluje funkcje sprawdzMiejsce, a nastepnie wywoluje funkcje zmienStan dla okreslonych pol. Na koniec wywoluje funkcje odswiezLewaPlansze.
/// \param[in] wielkoscOkretu dlugosc okretu rowna 1, 2, 3 lub 4 pola.

void ustawOkret(int wielkoscOkretu) {}

/// Funkcja sprawdzMiejsce sprawdza czy dane pole tablicy ma wartosc czyMoznaUstawicStatek == dozwolony.
/// \param[in] x_curs wspolrzedna x polozenia kursora
/// \param[in] y_curs wspolrzedna y polozenia kursora
/// \return wartosc 1 jezeli ruch jest legalny, i -1 jezeli nie jest.

int sprawdzMiejsce(int x_curs, int y_curs) {}

/// Funkcja gra wywoluje po kolei funkcje turaGracza i turaAI az do trafienia wszystkich statkow przez gracza lub komputer. Na koniec wyswietla stosowny komunikat i wywoluje funkcje cztajKlawisz.

void gra() {}

/// Funcja turaGracza obsluguje ruch kursora przy ataku, wywoluje funkcje strzalGracza po nacisnieciu spacji tak dlugo jak trafiany jest statek lub uprzednio trafione pole oraz pozwala na zaznaczanie pol, ktore gracz uwaza za pewne pudla X-em i rysowanie wody w wypadku pomylki.

void turaGracza() {}

/// Funkcja turaAI wywoluje funkcje strzalAI tak dlugo jak trafiony zostaje statek.

void turaAI() {}

/// Funckja strzalGracza sprawdza jaki jest stan trafionego pola, zmienia ten stan w odpowiedni sposob i wywoluje funkcje odswiezPole. Wypisuje na gorze ekranu liczbe pozostalych pol do trafienia. Funkcja zmienia zmienna globalna zwyciestwo na 1 jezeli zostaly trafione wszystkie okrety komputera.
/// \param[in] x_curs wspolrzedna x polozenia kursora
/// \param[in] y_curs wspolrzedna y polozenia kursora
/// \return wartosc 1 jezeli zostal trafiony statek lub uprzednio trafione pole, i 0 jezeli gracz spudlowal.

int strzalGracza(int x_curs, int y_curs) {}

/// Funckja strzalAI wybiera za pomoca generatora liczb pseudolosowych pole do trafienia przez komputer, sprawdza jaki jest stan trafionego pola, zmienia ten stan w odpowiedni sposob i wywoluje funkcje odswiezPole. Wypisuje na gorze ekranu liczbe pozostalych pol do trafienia. Funkcja zmienia zmienna globalna zwyciestwo na -1 jezeli zostaly trafione wszystkie okrety gracza.
/// \return wartosc 0 jezeli zostal trafiony statek lub uprzednio trafione pole, i 1 jezeli komputer spudlowal.

int strzalAI() {}

/// Funkcja sprawdzMiejsceAI sprawdza czy dane pole tablicy ma wartosc czyMoznaUstawicStatek == dozwolony.
/// \param[in] x wspolrzedna x trafienia komputera
/// \param[in] y wspolrzedna y trafienia komputera
/// \return wartosc 1 jezeli ruch jest legalny, i -1 jezeli nie jest.

int sprawdzMiejsceAI(int x, int y) {}

/// Funkcja ustawOkretyAI wywoluje funkcje ustawOkretAI dla czteromasztowca, dwoch trojmasztowcow, trzech dwumasztowcow i czterech jednomasztowcow komputera.

void ustawOkretyAI() {}

/// Funkcja ustawOkretAI probuje ustawic na planszy okret danej wielkosci w miejscu wylosowanym przez generator liczb pseudolosowych. Aby sprawdzic legalnosc ruchu wywoluje funkcje sprawdzMiejsceAI, a nastepnie wywoluje funkcje zmienStanAI dla okreslonych pol.
/// \param[in] wielkoscOkretu dlugosc okretu rowna 1, 2, 3 lub 4 pola.

void ustawOkretAI(int wielkoscOkretu) {}

/// Funkcja zmienStanAI przyporzadkowuje polu odpowiedni obiekt: statek, statek ustawiany, statek trafiony, wode lub pudlo i ustawia legalnosc postawienia na polu statku.
/// \param[in] x wspolrzedna x trafienia komputera
/// \param[in] y wspolrzedna y trafienia komputera
/// \param[in] stan docelowy stan pola

void zmienStanAI(int x, int y, int stan) {}




/// Pojedyncze pole planszy
struct PolePlanszy {
	/// obiekt umieszony w polu planszy
	enum stan {
		woda = WODA,
		statek_ustawiany = STATEK_USTAWIANY,
		statek = STATEK,
		trafiony = TRAFIONY,
		zatopiony = ZATOPIONY,
		pudlo = PUDLO
	} obiekt;
	/// okreslenie, czy mozna na tym polu ustawic statek w poczatkowej fazie gry
	enum dozwolenie {
		niedozwolony = 0,
		dozwolony = 1
	} czyMoznaUstawicStatek;
};

/// tablica dwuwymiarowa pol planczy gracza
struct PolePlanszy planszaGracza[10][10];

/// tablica dwuwymiarowa pol planczy komputera
struct PolePlanszy planszaAI[10][10];

/// liczba pol na ktorych sa okrety gracza.
int kratkiOkretowGracza;

/// liczba pol na ktorych sa okrety komputera.
int kratkiOkretowAI;

/// wspolrzedna x kursora.
int x_cur;

/// wspolrzedna y kursora.
int y_cur;

/// wartosc 0 dla trwajacej gry, 1 po zwyciestwie gracza i -1 po zwyciestwie komputera.
int zwyciestwo;


/// Funkcja glowna gry
int main()
{
	
	//inicjalizacja planszy gracza i AI

	inicjalizujPlansze();
	
	//inicjalizacja stanu okretow gracza i komputera

	kratkiOkretowGracza = 20;
	kratkiOkretowAI = 20;

	//inicjalizacja ekranu ncurses

	initscr();

		//ustawienia kolorow

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
	
	//rysowanie planczy i ustawienie kursora w lewym gornym rogu lewej planszy

	rysujPlansze();

	x_cur = TARGET_X_MIN;
	y_cur = TARGET_Y_MIN;

	//ruch kursorem

	move(y_cur,x_cur);
	refresh();
	odswiezLewaPlansze();

	//inicjalizacja generatora liczb pseudolosowych

	int seed;
	time_t czas;
    seed = time(&czas);
    srand(seed);

    //ustawianie okretow przez gracza i komputer

	ustawOkretyGracza();
	ustawOkretyAI();

	gra();
	
	//wylaczenie ncurses

	endwin();
	return 0;
}