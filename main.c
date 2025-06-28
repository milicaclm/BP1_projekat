#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deo1.c"
#define f1 4
#define f2 5

int main()
{

    int opcija;
    printf("Izaberite opciju:\n");
    printf("1. Kreiraj evidenciju automobila\n");
    printf("2. Kreiraj evidenciju parkiranja\n");
    printf("3. Upis novog automobila\n");
    printf("4. Upis novog parkiranja\n");
    printf("0. Izlaz\n");
    printf("Unesite opciju: ");
    scanf("%d", &opcija);

    switch (opcija)
    {
        //TODO: Implement the options for the menu
    case 1:
        kreiraj_praznu_evidenciju_automobila("automobili.dat");
        break;

    case 2:
        kreiraj_praznu_evidenciju_parkinga("parking.dat");
        break;
    
    case 3: upis_automobila(otvori_datoteku("automobili.dat", "rb+"));
        break;
    

    case 0:
        printf("Izlaz iz programa.\n");
        exit(0);
    default:
        break;
    }



    return 0;
}




