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

    datoteka_automobila = otvori_datoteku("automobili.dat", "rb+"); 
    datoteka_parkiranja = otvori_datoteku("parkiranja.dat", "rb+");
    //datoteka_parkiranja_automobila = otvori_datoteku("parkiranja_automobila.dat", "rb+"); 
    
    
    if((datoteka_log = fopen("log.dat", "rb+")) == NULL) {
        kreiraj_datoteku_log("log.dat");
    }


    void meni_deo1();
    void meni_deo2();
    void meni_deo3();

    while(1) {
        printf("Izaberite opciju:\n");
        printf("1. Upravljaj evidencijama automobila i parkiranja\n");
        printf("2. Upravljaj evidencijom parkiranja automobila\n");
        printf("3. Upravljaj log datotekom\n");
        printf("0. Izlaz\n");
        printf("Unesite opciju: ");
        scanf("%c", &opcija);
        while(getchar() != '\n');
        
        switch (opcija)
        {
        case '1':
            meni_deo1();
            break;
        case '2':
            meni_deo2();
            break;
        case '3':
            meni_deo3();
            break;
        case '0':
            zatvori_datoteke();
            printf("Izlaz iz programa.\n");
            exit(0);
        default:
            break;
        }
    }



    return 0;
}

void meni_deo1() {
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
            printf("7. Ispis datoteke automobila\n");
            printf("8. Ispis datoteke parkiranja\n");
            printf("0. Nazad\n");
            printf("Unesite opciju: ");
            scanf("%c", &opcija);
            while(getchar() != '\n');
            
            
        
        switch (opcija)
        {
            case '1':
                printf("Unesite ime datoteke za evidenciju automobila: ");
                while (fgets(ime_datoteke_automobila, sizeof(ime_datoteke_automobila), stdin) == NULL || ime_datoteke_automobila[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_automobila[strcspn(ime_datoteke_automobila, "\n")] = 0;
                strcat(ime_datoteke_automobila, ".dat");
                kreiraj_praznu_evidenciju_automobila(ime_datoteke_automobila);

                printf("Unesite ime datoteke za evidenciju parkiranja: ");
                while (fgets(ime_datoteke_parkiranja, sizeof(ime_datoteke_parkiranja), stdin) == NULL || ime_datoteke_parkiranja[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_parkiranja[strcspn(ime_datoteke_parkiranja, "\n")] = 0;
                strcat(ime_datoteke_parkiranja, ".dat");
                kreiraj_praznu_evidenciju_parkinga(ime_datoteke_parkiranja);
                break;

            case '2': 
                Automobil *novi_automobil = upis_automobila();
                if (novi_automobil != NULL) {
                    propagiraj_unos_automobila(*novi_automobil);
                    free(novi_automobil);
                }
                break;

            case '3': 
                bool samo_belo;
                Parkiranje *novi_parkiranje = upis_parkiranja(&samo_belo);
                if (novi_parkiranje != NULL) {
                    if (samo_belo) {
                        loguj_parkiranje(novi_parkiranje->registarska_oznaka, novi_parkiranje->duzina_boravka);
                    }
                    propagiraj_unos_parkiranja(*novi_parkiranje);
                    free(novi_parkiranje);
                }
                break;

            case '4': ispisi_automobil();
                break;

            case '5': prikaz_automobila();
                break;

            case '6': 

                Automobil *modifikovani_automobil = modifikacija_automobila();
                if (modifikovani_automobil != NULL) {
                    propagiraj_modifikaciju_automobila(*modifikovani_automobil);
                    free(modifikovani_automobil);
                }
                break;
            case '7':
                ispisi_datoteku_automobila();
                break;
            case '8':
                ispisi_datoteku_parkiranja();
                break;
            case '0':
                return;
            default:
                break;
        }
    }
}

void meni_deo2() {
    char opcija;
    while (1) {
        printf("Izaberite opciju:\n");
        printf("1. Upis svih automobila\n");
        printf("2. Upis novog sloga\n");
        printf("3. Prikaz duzine boravka u beloj zoni\n");
        printf("4. Prikaz automobila proizvedenih u 2024. godini\n");
        printf("5. Logicko brisanje sloga\n");
        printf("6. Ispis datoteke parkiranja automobila\n");
        printf("0. Nazad\n");
        printf("Unesite opciju: ");
        scanf("%c", &opcija);
        while (getchar() != '\n');
        switch (opcija) {
            case '1':
                char ime_datoteke[31];
                printf("Unesite naziv datoteke: ");
                while (fgets(ime_datoteke, sizeof(ime_datoteke), stdin) == NULL || ime_datoteke[0] == '\n');
                ime_datoteke[strcspn(ime_datoteke, "\n")] = '\0'; // Ukloni novi red
                strcat(ime_datoteke, ".dat");
                datoteka_parkiranja_automobila = otvori_datoteku(ime_datoteke, "wb+");
                kreiraj_praznu_evidenciju();
                upis_svih_automobila();
                break;
            case '2':
                upis_novog_sloga();
                break;
            case '3': {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                prikaz_duzine_bela_zona(registarska_oznaka);
                break;
            }
            case '4':
                prikaz_automobila_2024();
                break;
            case '5': {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                logicko_brisanje(registarska_oznaka);
                break;
            }
            case '6':
                ispisi_datoteku_parkiranja_automobila();
                break;
            case '0':
                return;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }
}

void meni_deo3() {
    char opcija;
    while (1) {
        printf("Izaberite opciju:\n");
        printf("1. Ispis datoteke logova\n");
        printf("2. Ispis registarskih oznaka i duzina boravka\n");
        printf("3. Ocisti log datoteku\n");
        printf("0. Nazad\n");
        printf("Unesite opciju: ");
        scanf("%c", &opcija);
        while (getchar() != '\n');
        
        switch (opcija) {
            case '1':
                ispisi_log_datoteku();
                break;
            case '2':
                ispisi_log_datoteku_oznaka_duzina();
                break;
            case '3':
                ocisti_log_datoteku();
                break;
            case '0':
                return;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }
}


