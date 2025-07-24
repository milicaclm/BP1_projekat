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
    FILE *datoteka_log = otvori_datoteku("log.dat", "rb+");

    char ime_datoteke_automobila[31];
    char ime_datoteke_parkiranja[31];

    void meni_deo1(FILE *datoteka_automobila, FILE *datoteka_parkiranja, FILE *datoteka, FILE *datoteka_log);
    void meni_deo2(FILE *datoteka_automobila, FILE *datoteka_parkiranja, FILE *datoteka);
    void meni_deo3(FILE *datoteka, FILE *datoteka_log);

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
            meni_deo1(datoteka_automobila, datoteka_parkiranja, datoteka, datoteka_log);
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

void meni_deo1(FILE *datoteka_automobila, FILE *datoteka_parkiranja, FILE *datoteka, FILE *datoteka_log) {
        char opcija;
        char ime_datoteke_automobila[31];
        char ime_datoteke_parkiranja[31];
        while(1) {
            printf("Izaberite opciju:\n");
            printf("1. Kreiraj evidencije automobila i parkiranja\n");
            printf("2. Upis novog automobila\n");
            printf("3. Upis novog parkiranja\n");
            printf("4. Ispis automobila po registarskoj oznaci\n");
            printf("5. Prikaz svih automobila\n");
            printf("6. Modifikacija automobila\n");
            printf("0. Nazad\n");
            printf("Unesite opciju: ");
            scanf("%1hhd", &opcija);
            while(getchar() != '\n');
            
            
        
        switch (opcija)
        {
            case 1:
                printf("Unesite ime datoteke za evidenciju automobila: ");
                while (fgets(ime_datoteke_automobila, sizeof(ime_datoteke_automobila), stdin) == NULL || ime_datoteke_automobila[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_automobila[strcspn(ime_datoteke_automobila, "\n")] = 0;
                strcat(ime_datoteke_automobila, ".dat");
                datoteka_automobila = kreiraj_praznu_evidenciju_automobila(ime_datoteke_automobila);

                printf("Unesite ime datoteke za evidenciju parkiranja: ");
                while (fgets(ime_datoteke_parkiranja, sizeof(ime_datoteke_parkiranja), stdin) == NULL || ime_datoteke_parkiranja[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_parkiranja[strcspn(ime_datoteke_parkiranja, "\n")] = 0;
                strcat(ime_datoteke_parkiranja, ".dat");
                datoteka_parkiranja = kreiraj_praznu_evidenciju_parkinga(ime_datoteke_parkiranja);
                break;

            case 2: 
                Automobil *novi_automobil = upis_automobila(datoteka_automobila);
                if (novi_automobil != NULL) {
                    propagiraj_unos_automobila(datoteka, *novi_automobil);
                    free(novi_automobil);
                }
                break;

            case 3: 
                Parkiranje *novi_parkiranje = upis_parkiranja(datoteka_parkiranja, datoteka_automobila);
                if (novi_parkiranje != NULL) {
                    propagiraj_unos_parkiranja(datoteka, datoteka_log, *novi_parkiranje);
                    free(novi_parkiranje);
                }
                break;
            
            case 4: ispisi_automobil(datoteka_automobila);
                break;

            case 5: prikaz_automobila(datoteka_automobila, datoteka_parkiranja);
                break;

            case 6: 
                
                Automobil *modifikovani_automobil = modifikacija_automobila(datoteka_automobila);
                if (modifikovani_automobil != NULL) {
                    propagiraj_modifikaciju_automobila(datoteka, *modifikovani_automobil);
                    free(modifikovani_automobil);
                }
                break;

            case 0:
                return;
            default:
                break;
        }
    }
}

void meni_deo2(FILE *datoteka_automobila, FILE *datoteka_parkiranja, FILE *datoteka) {
    char opcija;
    while (1) {
        printf("Izaberite opciju:\n");
        printf("1. Upis novih automobila\n");
        printf("2. Upis novog sloga\n");
        printf("3. Prikaz duzine boravka u beloj zoni\n");
        printf("4. Prikaz automobila proizvedenih u 2024. godini\n");
        printf("5. Logicko brisanje sloga\n");
        printf("0. Nazad\n");
        printf("Unesite opciju: ");
        scanf("%1hhd", &opcija);
        while (getchar() != '\n');
        switch (opcija) {
            case 1:
                char ime_datoteke[31];
                printf("Unesite naziv datoteke: ");
                while (fgets(ime_datoteke, sizeof(ime_datoteke), stdin) == NULL || ime_datoteke[0] == '\n');
                ime_datoteke[strcspn(ime_datoteke, "\n")] = '\0'; // Ukloni novi red
                strcat(ime_datoteke, ".dat");
                datoteka = fopen(ime_datoteke, "wb+");
                upis_svih_automobila(datoteka, datoteka_automobila, datoteka_parkiranja);
                break;
            case 2:
                upis_novog_sloga(datoteka);
                break;
            case 3: {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                prikaz_duzine_bela_zona(datoteka, registarska_oznaka);
                break;
            }
            case 4:
                prikaz_automobila_2024(datoteka);
                break;
            case 5: {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                logicko_brisanje(datoteka, registarska_oznaka);
                break;
            }
            case 0:
                return;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }
}

void meni_deo3(FILE *datoteka, FILE *datoteka_log) {
    char opcija;
    while (1) {
        printf("Izaberite opciju:\n");
        printf("1. Ispis datoteke logova\n");
        printf("2. Ispis registarskih oznaka i duzina boravka\n");
        printf("0. Nazad\n");
        printf("Unesite opciju: ");
        scanf("%1hhd", &opcija);
        while (getchar() != '\n');
        
        switch (opcija) {
            case 1:
                ispisi_log_datoteku(datoteka_log);
                break;
            case 2:
                ispisi_log_datoteku_oznaka_duzina(datoteka_log);
                break;
            case 0:
                return;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }
}





