#include "funkcije.h"



Automobil default_automobil = {0, "", "", 0, ""};
Parkiranje default_parkiranje = {0, 0, "", 0, ""};
FILE *datoteka_automobila = NULL;
FILE *datoteka_parkiranja = NULL;
FILE *datoteka_parkiranja_automobila = NULL;
FILE *datoteka_log = NULL;
Parkirani_automobil *prekoracioci = NULL;
Parkirani_automobil *visak = NULL;
int index_prekoracioca = 0;
int index_viska = 0;
Parkirani_automobil default_parkirani_automobil = {0, "", "", 0, "", 0, 0, 0, PRAZAN};
Log default_log = {0, 0, 0};
void automatsko_testiranje() {
    printf("Pokrecem automatsko testiranje...\n");

    //0. kreiraj datoteke
    printf("Kreiram prazne evidencije...\n");
    // Zatvori sve eventualno otvorene datoteke i obrisi stare fajlove da test krece od nule
    zatvori_datoteke();
    remove("automobili.dat");
    remove("parkiranja.dat");
    remove("parkiranja_automobila.dat");
    // log moze ostati, ali za cist test ga brisemo
    remove("log.dat");
    // Reset globalnih pokazivaca
    datoteka_automobila = NULL;
    datoteka_parkiranja = NULL;
    datoteka_parkiranja_automobila = NULL;
    datoteka_log = NULL;
    kreiraj_praznu_evidenciju_automobila("automobili.dat");
    kreiraj_praznu_evidenciju_parkinga("parkiranja.dat");

    // 1. Dodaj devet automobila
    printf("Dodajem automobile...\n");
    Automobil automobili[] = {
        {1234, "Toyota", "Corolla", 2020, "Crvena"},
        {5678, "Honda", "Civic", 2024, "Plava"},
        {9101, "Ford", "Focus", 2021, "Bela"},
        {1121, "BMW", "X5", 2022, "Crna"},
        {3141, "Audi", "A4", 2024, "Siva"},
        {5161, "Mercedes", "C200", 2018, "Zelena"},
        {7181, "Volkswagen", "Golf", 2017, "Zuta"},
        {9202, "Hyundai", "Elantra", 2023, "Narandzasta"},
        {1023, "Kia", "Sportage", 2024, "Ljubicasta"}
    };
    printf("Upisujem automobile...\n");
    // Obezbedi da tokom upisa automobila nema propagacije u rasutu datoteku
    datoteka_parkiranja_automobila = NULL;
    for (int i = 0; i < 9; i++) {
        printf("Automobil: %d\n", automobili[i].registarska_oznaka);
        upis_automobila(&automobili[i]);
    }

    // 2. Kreiraj datoteku parkiranja automobila pre dodavanja parkiranja
    
    printf("Dodajem parkiranja...\n");
    Parkiranje parkiranja[10] = {
        {1, 1234, "15-5-2025 10:15", 3, "BELA"},
        {2, 1234, "16-5-2025 12:30", 5, "CRVENA"},
        {3, 5678, "17-5-2025 14:00", 2, "BELA"},
        {4, 5678, "18-5-2025 16:45", 4, "PLAVA"},
        {5, 9101, "19-5-2025 09:00", 6, "BELA"},
        {6, 9101, "20-5-2025 11:30", 3, "CRVENA"},
        {7, 1121, "21-5-2025 13:15", 7, "BELA"},
        {8, 1121, "22-5-2025 15:00", 2, "PLAVA"},
        {9, 3141, "23-5-2025 10:00", 5, "BELA"},
        {10, 3141, "24-5-2025 12:45", 3, "CRVENA"}
    };
    printf("Upisujem parkiranja...\n");
    for (int i = 0; i < 10; i++) {
        upis_parkiranja(&parkiranja[i]);
    }

    // 3. Kreiraj datoteku parkiranja automobila
    printf("Kreiram datoteku parkiranja automobila...\n");
    datoteka_parkiranja_automobila = otvori_datoteku("parkiranja_automobila.dat", "wb+");
    kreiraj_praznu_evidenciju();
    upis_svih_automobila();

    // 4. Dodaj 10 parkiranja automobila (5 na belu zonu)
    printf("Dodajem parkiranja automobila...\n");
    Parkirani_automobil parkirani_automobili[] = {
        {1234, "Toyota", "Corolla", 2020, "Crvena", 10, 5, 3, POPUNJEN},
        {5678, "Honda", "Civic", 2019, "Plava", 7, 3, 2, POPUNJEN},
        {9101, "Ford", "Focus", 2021, "Bela", 8, 4, 1, POPUNJEN},
        {1121, "BMW", "X5", 2024, "Crna", 6, 2, 3, POPUNJEN},
        {3141, "Audi", "A4", 2020, "Siva", 9, 3, 2, POPUNJEN},
        {5161, "Mercedes", "C200", 2014, "Zelena", 5, 2, 1, POPUNJEN},
        {7181, "Volkswagen", "Golf", 2017, "Zuta", 4, 1, 2, POPUNJEN},
        {9202, "Hyundai", "Elantra", 2024, "Narandzasta", 3, 2, 1, POPUNJEN},
        {1023, "Kia", "Sportage", 2024, "Ljubicasta", 7, 3, 2, POPUNJEN},
        {1121, "BMW", "X5", 2022, "Crna", 6, 2, 3, POPUNJEN}
    };
    for (int i = 0; i < 10; i++) {
        upis_novog_sloga(parkirani_automobili[i]);
    }

    // 5. Dodaj tri nova automobila
    printf("Dodajem nove automobile...\n");
    Automobil novi_automobili[] = {
        {2024, "Mazda", "CX-5", 2023, "Crvena"},
        {3035, "Subaru", "Outback", 2022, "Plava"},
        {4046, "Tesla", "Model 3", 2021, "Bela"}
    };
    for (int i = 0; i < 3; i++) {
        upis_automobila(&novi_automobili[i]);
    }

    // 6. Dodaj još pet parkiranja (2 na belu zonu)
    printf("Dodajem dodatna parkiranja...\n");
    Parkiranje dodatna_parkiranja[] = {
        {11, 2024, "25-5-2025 10:15", 3, "BELA"},
        {12, 3035, "26-5-2025 12:30", 5, "CRVENA"},
        {13, 4046, "27-5-2025 14:00", 2, "BELA"},
        {14, 1234, "28-5-2025 16:45", 4, "PLAVA"},
        {15, 5678, "29-5-2025 09:00", 6, "CRVENA"}
    };
    for (int i = 0; i < 5; i++) {
        upis_parkiranja(&dodatna_parkiranja[i]);
    }

    // 7. Dodaj još dva parkiranja automobila
    printf("Dodajem dodatna parkiranja automobila...\n");
    Parkirani_automobil dodatni_parkirani[] = {
        {2024, "Mazda", "CX-5", 2023, "Crvena", 5, 2, 1, POPUNJEN},
        {3035, "Subaru", "Outback", 2024, "Plava", 4, 1, 2, POPUNJEN}
    };
    for (int i = 0; i < 2; i++) {
        upis_novog_sloga(dodatni_parkirani[i]);
    }

    printf("Automatsko testiranje zavrseno.\n");
}

int main(int argc, char *argv[])
{

    char opcija;

    if(argc == 1) {
    } else if (argc == 3)
    {
        
        datoteka_automobila = otvori_datoteku(strcat(argv[1], ".dat"), "rb+");
        datoteka_parkiranja = otvori_datoteku(strcat(argv[2], ".dat"), "rb+");
    } else if (argc == 4)
    {
        datoteka_automobila = otvori_datoteku(strcat(argv[1], ".dat"), "rb+");
        datoteka_parkiranja = otvori_datoteku(strcat(argv[2], ".dat"), "rb+");
        datoteka_parkiranja_automobila = otvori_datoteku(strcat(argv[3], ".dat"), "rb+");
    }
    else
    {
        printf("Pogresan broj argumenata komandne linije.\n");
        exit(EXIT_FAILURE);
    }
    

    
    if((datoteka_log = fopen("log.dat", "rb+")) == NULL) {
        kreiraj_datoteku_log("log.dat");
    }


    void meni_deo1();
    void meni_deo2();
    void meni_deo3();

    while(1) {
        printf("Izaberite opciju:\n");
        printf("t. Unos podataka za testiranje\n");
        printf("1. Upravljaj evidencijama automobila i parkiranja\n");
        printf("2. Upravljaj evidencijom parkiranja automobila\n");
        printf("3. Upravljaj log datotekom\n");
        printf("0. Izlaz\n");
        printf("Unesite opciju: ");
        scanf("%c", &opcija);
        while(getchar() != '\n');
        
        switch (opcija)
        {

        case 't':
            automatsko_testiranje();
            break;
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
                upis_automobila(unos_automobila_sa_tastature());
                break;

            case '3': 
                upis_parkiranja(unos_parkiranja_sa_tastature());
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
                upis_novog_sloga(unos_parkiranog_automobila_sa_tastature());
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


