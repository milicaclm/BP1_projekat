#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "deo3.c"
#define f1 4
#define f2 5

int main()
{

    char opcija;
    /*
    FILE *datoteka_automobila = NULL;
    FILE *datoteka_parkiranja = NULL;
    FILE *datoteka = NULL;
    */
    FILE *datoteka_automobila = otvori_datoteku("automobili.dat", "rb+");
    FILE *datoteka_parkiranja = otvori_datoteku("parkiranja.dat", "rb+");
    FILE *datoteka = otvori_datoteku("parkiranja_automobila.dat", "rb+");

    char ime_datoteke_automobila[31];
    char ime_datoteke_parkiranja[31];
    while(1) {
        printf("Izaberite opciju:\n");
        printf("1. Upravljaj evidencijama automobila i parkiranja\n");
        printf("2. Upravljaj evidencijom parkiranja automobila\n");
        printf("0. Izlaz\n");
        printf("Unesite opciju: ");
        scanf("%1hhd", &opcija);
        while(getchar() != '\n');
        
        switch (opcija)
        {
        case 1:
            meni_deo1(datoteka_automobila, datoteka_parkiranja);
            break;
        case 2:
            meni_deo2(datoteka_automobila, datoteka_parkiranja, datoteka);
            break;
        case 0:
            fcloseall();
            printf("Izlaz iz programa.\n");
            exit(0);
        default:
            break;
        }
    }



    return 0;
}




