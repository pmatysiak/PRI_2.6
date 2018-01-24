#include <stdio.h>
#include <stdlib.h>  /* For exit() function */

#define WODA 1
#define STATEK_USTAWIANY 2
#define STATEK 3
#define TRAFIONY 4
#define ZATOPIONY 5
#define PUDLO 6
#define WODA_NIEDOZWOLONA 7
#define KURSOR 8


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

struct PolePlanszy planszaGracza[101][10][10];
struct PolePlanszy planszaAI[101][10][10];

int tura;                     //tymczasowo


void inicjalizujPlansze() {
   int iter_tura, iter_x, iter_y;
   for ( iter_tura = 0; iter_tura <= 100; ++iter_tura) {
      for ( iter_y = 0; iter_y < 10; iter_y++ ) {
         for ( iter_x = 0; iter_x < 10; iter_x++ ) {
            planszaGracza[iter_tura][iter_x][iter_y].obiekt = woda;
            planszaGracza[iter_tura][iter_x][iter_y].czyMoznaUstawicStatek = dozwolony;
            planszaAI[iter_tura][iter_x][iter_y].obiekt = woda;
            planszaAI[iter_tura][iter_x][iter_y].czyMoznaUstawicStatek = dozwolony;
         }
      }
   }
   

   // planszaAI[0][4][0].obiekt = statek;
   // planszaAI[0][4][1].obiekt = statek;
   // planszaAI[0][4][2].obiekt = statek;
   // planszaAI[0][4][3].obiekt = statek;
   // planszaAI[0][4][4].obiekt = statek;
   // planszaAI[0][4][5].obiekt = statek;
   // planszaAI[0][4][6].obiekt = statek;
   // planszaAI[0][4][7].obiekt = statek;
   // planszaAI[0][4][8].obiekt = statek;
   // planszaAI[0][4][9].obiekt = statek;
   // planszaAI[0][5][0].obiekt = statek;
   // planszaAI[0][5][1].obiekt = statek;
   // planszaAI[0][5][2].obiekt = statek;
   // planszaAI[0][5][3].obiekt = statek;
   // planszaAI[0][5][4].obiekt = statek;
   // planszaAI[0][5][5].obiekt = statek;
   // planszaAI[0][5][6].obiekt = statek;
    planszaAI[0][5][7].obiekt = trafiony;
    planszaAI[0][5][8].obiekt = pudlo;
    planszaAI[0][5][9].obiekt = statek;
   
}

char*  stanPola(int stan) {
   switch ( stan ) {
      case WODA:
         return "woda";
      case STATEK:
         return "statek";
      case TRAFIONY:
         return "trafiony";
      case PUDLO:
         return "pudlo";
   }
}




int main()
{
   int tura_iter;
   tura = 6;                  //tymczasowo
   char* s;                   //napis do tagów i tooltipów
   int x, y;
   inicjalizujPlansze();

   FILE *plik;

   plik = fopen("program.html", "w");
   if(plik == NULL)
   {
      printf("Error!");
      exit(1);
   }

//zapis nagłówka

   fprintf(plik,"<!DOCTYPE html>\n<html>\n<head>\n\t<meta charset=\"utf-8\">\n\t<meta http-equiv=\"refresh\" content=\"100\" >\n\t<title>Okręty - projekt 2.6 - Piotr Matysiak</title>\n\t<style>\n\t\tbody {\n\t\t\t/*background-color: #363841;*/\n\t\t\tbackground: repeating-linear-gradient(\n\t\t\t\t\t\t45deg,\n\t\t\t\t\t\t#202532,\n\t\t\t\t\t\t#232A32 4px,\n\t\t\t\t\t\t#1F2023 2px,\n\t\t\t\t\t\t#292F37 6px\n\t\t\t\t\t\t);\n\t\t\tcolor: #FFFBDD;\n\t\t\tfont-family: Verdana,/*, Century Gothic, Ubuntu,*/ sans-serif;\n\t\t\tmargin: 0px;\n\t\t}\n\t\tp, h1, h3, h4, h5, h6 {\n\t\t\ttext-align: center;\n\t\t\tcolor: #FFFBDD;\n\t\t\ttext-shadow: 1px 1px 1px #2D2D2D;\n\t\t}\n\t\th1 {\n\t\t\tmargin-top: 10px;\n\t\t}\n\t\ttable, td, th {\n\t\t\tborder: 1px solid white;\n\t\t\ttext-align: center;\n\t\t\tborder-collapse: collapse;\n\t\t}\n\t\ttable {\n\t\t\tborder-left: 0px;\n\t\t\tborder-top: 0px;\n\t\t}\n\t\ttd, th {\n\t\t\twidth: 24px;\n\t\t\theight: 24px;\n\t\t\tbackground-clip: padding-box;\n\t\t}\n\t\t.liczby {\n\t\t\tborder-left: 0px;\n\t\t}\n\t\t.litery {\n\t\t\tborder-top: 0px;\n\t\t}\n\t\t.tura {\n\t\t\tbackground-color: #383F4E;\n\t\t\tbackground-color: rgba(60, 60, 60, 0.7);\n\t\t\tmargin: 0px;\n\t\t\tpadding: 30px;\n\t\t\tpadding-top: 1px;\n\t\t\ttext-align: center;\n\t\t\tposition: relative;\n\t\t}\n\t\t.tura:nth-of-type(even) {\n\t\t\tbackground-color: #242433;\n\t\t\tbackground-color: rgba(36, 36, 36, 0.7);\n\t\t}\n\t\t.gracz, .ai {\n\t\t\tdisplay: inline-block;\n\t\t\ttransition-property: transform;\n    \t\ttransition-duration: 0.3s;\n    \t\tmargin: 0px;\n    \t\tposition: relative;\n\t\t}\n\t\t.gracz {\n\t\t\t/*border: red 1px solid;*/\n\t\t\ttransform: rotate3d(1, 1, -0.15, 20deg);\n\t\t\tanimation-name: gracz_ani;\n\t\t\tanimation-duration: 2s;\n\t\t\tanimation-iteration-count: 1;\n\t\t\tz-index: 2;\n\t\t}\n\t\t.ai {\n\t\t\tmargin-left: 70px;\n\t\t\tpadding: 0px;\n\t\t\t/*border: red 1px solid;*/\n\t\t\ttransform: rotate3d(-1, 1, 0.15, 20deg);\n\t\t\tz-index: 1;\n\t\t}\n\t\t.ai table {\n\t\t\tposition: relative;\n\t\t\tz-index: 0;\n\t\t}\n\t\t.ai table, .gracz table, .gracz_podglad table, .ai_podglad table {\n\t\t\tbox-shadow: 3px 3px 5px #808080;\n\t\t}\n\n\t\t.ai:hover, .gracz:hover {\n\t\t\ttransform: scale(1.1); \n\t\t}\n\n\t\t@keyframes gracz_ani {\n\t\t\tfrom {margin-right: 200px;}\n\t\t\tto {margin-right: 0px;}\n\t\t}\n\t\t.tooltip {\n\t\t    position: relative;\n\t\t}\n\t\t.tooltip .tooltiptext {\n\t\t\tfont-size: 9pt;\n\t\t    visibility: hidden;\n\t\t    width: 80px;\n\t\t    background-color: rgba(199,199,199,1);\n\t\t    color: rgba(52,52,52,1);\n\t\t    text-align: center;\n\t\t    padding: 5px 0;\n\t\t    border: 2px white solid;\n\t\t    box-shadow: 2px 2px 7px black;\n\t\t    border-radius: 20px;\n\t\t    border-top-left-radius: 0px;\n\t\t    position: absolute;\n\t\t    top: 28px;\n\t\t    left: 28px;\n\t\t    z-index: 3;\n\t\t}\n\t\t.tooltip:hover .tooltiptext {\n\t\t    visibility: visible;\n\t\t}\n\n\t\t.podglad {\n\t\t\tposition: fixed;\n\t\t\tleft: -440px;\n\t\t\theight: 190px;\n\t\t\twidth: 400px;\n\t\t\ttop: 10px;\n\t\t\t/*transform: scale(0.6);*/\n\t\t\tbackground-color: #383F4E;\n\t\t\tmargin: 0px;\n\t\t\tpadding: 40px;\n\t\t\tpadding-top: 1px;\n\t\t\tfont-size: 7pt;\n\t\t\ttext-align: center;\n\t\t\ttransition-property: left;\n    \t\ttransition-duration: 0.3s;\n    \t\tborder-radius: 0px 40px 40px 0px;\n    \t\tborder: 3px black solid;\n    \t\tborder-left: 0px;\n    \t\tz-index: 4;\n\t\t}\n\t\t.podglad:hover {\n\t\t\tleft: -40px;\n\t\t}\n\t\t.gracz_podglad {\n\t\t\tdisplay: inline-block;\n    \t\tmargin: 0px;\n\t\t}\n\t\t.ai_podglad {\n\t\t\tdisplay: inline-block;\n    \t\tmargin: 0px;\n    \t\tmargin-left: 40px;\n\t\t\tpadding: 0px;\n\t\t}\n\t\t.podglad p {\n\t\t\tfont-size: 10pt;\n\t\t\tdisplay: inline-block;\n\t\t\tposition: relative;\n\t\t\tleft: 220px;\n\t\t\ttop: -106px;\n\t\t\ttransform: rotate(90deg);\n\t\t}\n\t\t.podglad td, .podglad th {\n\t\t\twidth: 12px;\n\t\t\theight: 12px;\n\t\t\tfont-size: 6pt;\n\t\t}\n\t\t.podglad .tooltip .tooltiptext {\n\t\t\tfont-size: 7pt;\n\t\t    visibility: hidden;\n\t\t    width: 70px;\n\t\t    background-color: #C7C7C7;\n\t\t    color: #343434;\n\t\t    text-align: center;\n\t\t    padding: 3px 0;\n\t\t    border: 1px white solid;\n\t\t    box-shadow: 1px 1px 4px black;\n\t\t    border-radius: 10px;\n\t\t    border-top-left-radius: 0px;\n\t\t    position: absolute;\n\t\t    top: 20px;\n\t\t    left: 20px;\n\t\t    z-index: 2;\n\t\t}\n\t\t.podglad .tooltip:hover .tooltiptext {\n\t\t    visibility: visible;\n\t\t}\n\n\t\t/*POLA*/\n\n\t/*WODA*/\n\t\t.woda {\n\t\t\tcolor: #6acbf2;\n\t\t\tbackground-color: #2E4EF2;\n\t\t\ttransition-property: background-color;\n\t\t\ttransition-duration: 1s;\n\t\t}\n\t\t.woda:before {\n\t\t\tcontent: \"~\";\n\t\t}\n\t\t.woda:hover {\n\t\t\tbackground-color: #1855A9;\n\t\t\tfont-weight: bolder;\n\t\t}\n\t/*STATEK*/\n\t\t.statek {\n\t\t\tcolor: #951ca8;\n\t\t\tbackground-color: #569340;\n\t\t\tcolor: yellow;\n\t\t\ttransition-property: background-color;\n\t\t\ttransition-duration: 1s;\n\t\t}\n\t\t.statek:before {\n\t\t\tcontent: \"S\";\n\t\t}\n\t\t.statek:hover {\n\t\t\tbackground-color: #487837;\n\t\t\tfont-weight: bolder;\n\t\t}\n\t/*TRAFIONY*/\n\t\t.trafiony {\n\t\t\tcolor: #e2ca0d;\n\t\t\tbackground-color: #b50000;\n\t\t\ttransition-property: background-color;\n\t\t\ttransition-duration: 1s;\n\t\t}\n\t\t.trafiony:before {\n\t\t\tcontent: \"*\";\n\t\t}\n\t\t.trafiony:hover {\n\t\t\tbackground-color: #d60000;\n\t\t\tfont-weight: bolder;\n\t\t}\n\t/*PUDLO*/\n\t\t.pudlo {\n\t\t\tcolor: #2d45bc;\n\t\t\tbackground-color: black;\n\t\t\ttransition-property: color;\n\t\t\ttransition-duration: 1s;\n\t\t}\n\t\t.pudlo:before {\n\t\t\tcontent: \"O\";\n\t\t}\n\t\t.pudlo:hover {\n\t\t\tcolor: #586bc9;\n\t\t\tfont-weight: bolder;\n\t\t}\n\n\n\t</style>\n</head>\n");

//zapis topu

   fprintf(plik, "<body>\n\t<h1>Okręty</h1>\n\t<h3>Przebieg rozgrywki</h3>\n\n");

//zapis podgladu

   //poczatek

   fprintf(plik,"\t<div class=\"podglad\">\n");

   //podglad statków AI dla gracza

   fprintf(plik,"\t\t<div class=\"gracz_podglad\">\n\t\t\t<h4>Okręty AI</h4>\n\t\t\t<table>\n\t\t\t\t<!-- rząd literek -->\n\t\t\t\t<tr> \n\t\t\t\t\t<th class=\"litery liczby\"></th>\n\t\t\t\t\t<th class=\"litery\">A</th>\n\t\t\t\t\t<th class=\"litery\">B</th>\n\t\t\t\t\t<th class=\"litery\">C</th>\n\t\t\t\t\t<th class=\"litery\">D</th>\n\t\t\t\t\t<th class=\"litery\">E</th>\n\t\t\t\t\t<th class=\"litery\">F</th>\n\t\t\t\t\t<th class=\"litery\">G</th>\n\t\t\t\t\t<th class=\"litery\">H</th>\n\t\t\t\t\t<th class=\"litery\">I</th>\n\t\t\t\t\t<th class=\"litery\">J</th>\n\t\t\t\t</tr>\n\n\t\t\t\t<!-- 1 -->\n");
   for( y = 0; y < 10; ++y) {
      fprintf(plik,"\t\t\t\t<tr>\n\t\t\t\t\t<th class=\"liczby\">%d</th>\n", y+1);
      for (x = 0; x < 10; ++x) {
         s = stanPola(planszaAI[0][x][y].obiekt);
         fprintf(plik, "\t\t\t\t\t<td class=\"tooltip %s\"><span class=\"tooltiptext\">%s</span></td>\n", s, s);
      }
      fprintf(plik,"\t\t\t\t</tr>\n");
   }
   fprintf(plik,"\t\t\t</table>\n\t\t</div>\n");

   //podglad "dla AI"

   fprintf(plik,"\t\t<div class=\"ai_podglad\">\n\t\t\t<h4>Okręty gracza</h4>\n\t\t\t<table>\n\t\t\t\t<!-- rząd literek -->\n\t\t\t\t<tr> \n\t\t\t\t\t<th class=\"litery liczby\"></th>\n\t\t\t\t\t<th class=\"litery\">A</th>\n\t\t\t\t\t<th class=\"litery\">B</th>\n\t\t\t\t\t<th class=\"litery\">C</th>\n\t\t\t\t\t<th class=\"litery\">D</th>\n\t\t\t\t\t<th class=\"litery\">E</th>\n\t\t\t\t\t<th class=\"litery\">F</th>\n\t\t\t\t\t<th class=\"litery\">G</th>\n\t\t\t\t\t<th class=\"litery\">H</th>\n\t\t\t\t\t<th class=\"litery\">I</th>\n\t\t\t\t\t<th class=\"litery\">J</th>\n\t\t\t\t</tr>\n\n\t\t\t\t<!-- 1 -->\n");
   for( y = 0; y < 10; ++y) {
      fprintf(plik,"\t\t\t\t<tr>\n\t\t\t\t\t<th class=\"liczby\">%d</th>\n", y+1);
      for (x = 0; x < 10; ++x) {
         s = stanPola(planszaGracza[0][x][y].obiekt);
         fprintf(plik, "\t\t\t\t\t<td class=\"tooltip %s\"><span class=\"tooltiptext\">%s</span></td>\n", s, s);
      }
      fprintf(plik,"\t\t\t\t</tr>\n");
   }
   fprintf(plik,"\t\t\t</table>\n\t\t</div>\n");


   fprintf(plik,"\t\t<div>\n\t\t\t<p>Podgląd ułożenia okrętów</p>\n\t\t</div>\n\t</div>\n");

//koniec podgladu

//tury

   for( tura_iter = 1; tura_iter < tura; ++tura_iter) {
      fprintf(plik, "\t<div class=\"tura\">\n\t\t<h5>Tura %d</h5>\n", tura_iter);

      //gracza
      fprintf(plik, "\t\t<div class=\"gracz\">\n\t\t\t<h6>Ruch gracza</h6>\n\t\t\t<table>\n\t\t\t\t<!-- rząd literek -->\n\t\t\t\t<tr> \n\t\t\t\t\t<th class=\"litery liczby\"></th>\n\t\t\t\t\t<th class=\"litery\">A</th>\n\t\t\t\t\t<th class=\"litery\">B</th>\n\t\t\t\t\t<th class=\"litery\">C</th>\n\t\t\t\t\t<th class=\"litery\">D</th>\n\t\t\t\t\t<th class=\"litery\">E</th>\n\t\t\t\t\t<th class=\"litery\">F</th>\n\t\t\t\t\t<th class=\"litery\">G</th>\n\t\t\t\t\t<th class=\"litery\">H</th>\n\t\t\t\t\t<th class=\"litery\">I</th>\n\t\t\t\t\t<th class=\"litery\">J</th>\n\t\t\t\t</tr>\n\n\t\t\t\t<!-- 1 -->\n");
      
      for( y = 0; y < 10; ++y) {
         fprintf(plik,"\t\t\t\t<tr>\n\t\t\t\t\t<th class=\"liczby\">%d</th>\n", y+1);
         for (x = 0; x < 10; ++x) {
            s = stanPola(planszaAI[tura_iter][x][y].obiekt);
            fprintf(plik, "\t\t\t\t\t<td class=\"tooltip %s\"><span class=\"tooltiptext\">%s</span></td>\n", s, s);
         }
         fprintf(plik,"\t\t\t\t</tr>\n");
      }

      fprintf(plik, "\t\t\t</table>\n\t\t</div>\n");
      
      //ai
      fprintf(plik, "\t\t<div class=\"ai\">\n\t\t\t<h6>Ruch AI</h6>\n\t\t\t<table>\n\t\t\t\t<!-- rząd literek -->\n\t\t\t\t<tr> \n\t\t\t\t\t<th class=\"litery liczby\"></th>\n\t\t\t\t\t<th class=\"litery\">A</th>\n\t\t\t\t\t<th class=\"litery\">B</th>\n\t\t\t\t\t<th class=\"litery\">C</th>\n\t\t\t\t\t<th class=\"litery\">D</th>\n\t\t\t\t\t<th class=\"litery\">E</th>\n\t\t\t\t\t<th class=\"litery\">F</th>\n\t\t\t\t\t<th class=\"litery\">G</th>\n\t\t\t\t\t<th class=\"litery\">H</th>\n\t\t\t\t\t<th class=\"litery\">I</th>\n\t\t\t\t\t<th class=\"litery\">J</th>\n\t\t\t\t</tr>\n\n\t\t\t\t<!-- 1 -->\n");
      
      for( y = 0; y < 10; ++y) {
         fprintf(plik,"\t\t\t\t<tr>\n\t\t\t\t\t<th class=\"liczby\">%d</th>\n", y+1);
         for (x = 0; x < 10; ++x) {
            s = stanPola(planszaAI[tura_iter][x][y].obiekt);
            fprintf(plik, "\t\t\t\t\t<td class=\"tooltip %s\"><span class=\"tooltiptext\">%s</span></td>\n", s, s);
         }
         fprintf(plik,"\t\t\t\t</tr>\n");
      }

      fprintf(plik, "\t\t\t</table>\n\t\t</div>\n");

      fprintf(plik, "\t</div>\n\n");
   }

//koniec pliku
   char* kto;
   int zwyciestwo = -1;           //tymczasowo
   if (zwyciestwo == 1) kto = "gracz";
   else kto = "komputer";

   fprintf(plik,"\t<div>\n\t\t<p>Koniec zapisanej rozgrywki.</p>\n\t\t<p>Zwyciężył %s.</p>\n\t</div>\n\n</body>\n</html>", kto);

   fclose(plik);

   return 0;
}