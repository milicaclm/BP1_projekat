#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "deo1.c"
#define f1 4
#define f2 5

int main()
{

    char opcija;
    FILE *datoteka_automobila = NULL;
    FILE *datoteka_parkiranja = NULL;
    char ime_datoteke_automobila[31];
    char ime_datoteke_parkiranja[31];
    while(1) {
        printf("Izaberite opciju:\n");
        printf("1. Kreiraj evidenciju automobila\n");
        printf("2. Kreiraj evidenciju parkiranja\n");
        printf("3. Upis novog automobila\n");
        scanf("%1hhd", &opcija);
        while(getchar() != '\n');
        
        switch (opcija)
        {
        case 1:
            meni_deo1();
            break;
        case 2:
            meni_deo2();
            break;
        case 3:
            meni_deo3();
            break;
        default:
            break;
        }
    }



    return 0;
}




