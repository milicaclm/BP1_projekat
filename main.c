#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deo1.c"
#define f1 4
#define f2 5

int main()
{

    int opcija;
    FILE *datoteka_automobila = NULL;
    FILE *datoteka_parkiranja = NULL;
    char ime_datoteke_automobila[31];
    char ime_datoteke_parkiranja[31];
    while(1) {
        printf("Izaberite opciju:\n");
        printf("1. Kreiraj evidenciju automobila\n");
        printf("2. Kreiraj evidenciju parkiranja\n");
        printf("3. Upis novog automobila\n");
        printf("4. Upis novog parkiranja\n");
        printf("0. Izlaz\n");
        printf("Unesite opciju: ");
        char opcija_str[16];
        fgets(opcija_str, sizeof(opcija_str), stdin);
        opcija = atoi(opcija_str);
        
        switch (opcija)
        {
            case 1:
                printf("Unesite ime datoteke za evidenciju automobila: ");
                while (fgets(ime_datoteke_automobila, sizeof(ime_datoteke_automobila), stdin) == NULL || ime_datoteke_automobila[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_automobila[strcspn(ime_datoteke_automobila, "\n")] = 0;
                datoteka_automobila = kreiraj_praznu_evidenciju_automobila(ime_datoteke_automobila);
                break;

            case 2:
                printf("Unesite ime datoteke za evidenciju parkiranja: ");
                while (fgets(ime_datoteke_parkiranja, sizeof(ime_datoteke_parkiranja), stdin) == NULL || ime_datoteke_parkiranja[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_parkiranja[strcspn(ime_datoteke_parkiranja, "\n")] = 0;
                datoteka_parkiranja = kreiraj_praznu_evidenciju_parkinga(ime_datoteke_parkiranja);
                break;

            case 3: upis_automobila(datoteka_automobila);
                break;

            case 4: upis_parkiranja(datoteka_parkiranja);
                break;
            
            case 5: ispisi_automobil(datoteka_automobila);
                break;

            case 6: prikaz_automobila(datoteka_automobila, datoteka_parkiranja);
                break;

            case 7: modifikacija_automobila(datoteka_automobila);
                break;

            case 0:
                fcloseall();
                exit(0);
            default:
                break;
        }
    }



    return 0;
}




