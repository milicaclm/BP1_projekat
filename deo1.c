/*Program treba da ponudi sledeće funkcionalnosti kroz formu tekstualnog korisničkog menija: 
1.  glavni  meni  sa  mogućnošću  odabira  opcija  rada  nad  datotekom  sa  podacima 
studenata  ili  podacima  o  polaganju  ispita,  dok  svaka  opcija  ima  podmeni  koji  se 
sastoji od stavki navedenih u nastavku; 
2.  formiranje  prazne  datoteke  za  evidenciju  automobila  (sa  faktorom  blokiranja  f1  )  i 
datoteke  za  evidenciju  parkiranja  (sa  faktorom  blokiranja  f2  )  pri  čemu  korisnik  zadaje 
naziv nove datoteke; 
3.  upis  novog  sloga  u  aktivnu  datoteku  (obe  datoteke)  uz  proveru  postojanja  vrednosti 
identifikatora u datoteci, direktnim unosom podataka u realnom vremenu; 
4.  prikaz  boje  i  marke  automobilaza  zadatu  registarsku  oznaku  zajedno  sa  adresom 
bloka  i  rednim  brojem  sloga  u  bloku,  ukoliko  slog  nije  pronađen  potrebno  je  prikazati 
povratnu informaciju na standardnom izlazu; 
5.  prikaz  svih  automobila  zajedno  sa  informacijama  o  svakom  parkiranju  koje  je  trajalo 
duže  od  tri  sata  zajedno  sa  adresom  bloka  i  rednim  brojem  sloga  u  bloku,  ukoliko  je 
skup  rezultata  prazan  potrebno  je  prikazati  povratnu  informaciju  na  standardnom 
izlazu; 
6.  modifikacija  marke,  modela,  godine  proizvodnje  i  boje  u  evidenciji  automobila  na 
osnovu identifikatora;*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define f1 4
#define f2 5

typedef struct {
        int registarska_oznaka;
        char marka[31];
        char model[31];
        int godina_proizvodnje;
        char boja[21];
    } Automobil;

typedef struct {
        int identifikator;
        int registarska_oznaka;
        char datum_vreme[17];
        int duzina_boravka;
        char zona[7];
    } Parkiranje;

typedef struct {
        Automobil automobili[f1];
    } Blok_automobila;

typedef struct {
        Parkiranje parkinga[f2];
    } Blok_parkiranja;

Automobil default_automobil = {0, "", "", 0, ""};
Parkiranje default_parkiranje = {0, 0, "", 0, ""};

FILE *otvori_datoteku(const char *ime_datoteke, const char *mod)
{
    FILE *datoteka = fopen(ime_datoteke, mod);
    if (datoteka == NULL) {
        perror("Otvaranje datoteke nije uspelo!");
        fcloseall();
        exit(EXIT_FAILURE);
    }
    return datoteka;
}

size_t fwrite_safe(const void *buffer, size_t size, size_t count, FILE *datoteka) {
    if (fwrite(buffer, size, count, datoteka) != count) {
        perror("Upis u datoteku nije uspeo!");
        fcloseall();
        exit(EXIT_FAILURE);
    }
    return count;
}


void dodaj_prazan_blok_automobila(FILE *datoteka_automobila, Automobil slog_za_dodavanje) {
    Blok_automobila blok_automobila;
    blok_automobila.automobili[0] = slog_za_dodavanje;
    for (int i = 1; i < f1; i++) {
        blok_automobila.automobili[i] = default_automobil;
    }

    fseek(datoteka_automobila,0,SEEK_END);
    fwrite_safe(&blok_automobila, sizeof(blok_automobila), 1, datoteka_automobila);

}

void dodaj_prazan_blok_parkiranja(FILE *datoteka_parkiranja, Parkiranje *slog_za_dodavanje) {
    Blok_parkiranja blok_parkiranja;
    blok_parkiranja.parkinga[0] = *slog_za_dodavanje;
    for (int i = 1; i < f2; i++) {
         blok_parkiranja.parkinga[i] = default_parkiranje;
    }

    fseek(datoteka_parkiranja,0,SEEK_END);
    fwrite_safe(&blok_parkiranja, sizeof(blok_parkiranja), 1, datoteka_parkiranja);
}

FILE *kreiraj_praznu_evidenciju_automobila(const char *ime_datoteke_automobila){
    
    FILE *datoteka_automobila = otvori_datoteku(ime_datoteke_automobila, "wb+");

    dodaj_prazan_blok_automobila(datoteka_automobila, default_automobil);
    

    return datoteka_automobila;

}

FILE *kreiraj_praznu_evidenciju_parkinga(const char *ime_datoteke_parkinga){
    FILE *datoteka_parkinga = otvori_datoteku(ime_datoteke_parkinga, "wb+");

    dodaj_prazan_blok_parkiranja(datoteka_parkinga, &default_parkiranje);

    return datoteka_parkinga;
}

bool registarska_oznaka_postoji_sekvencijalna(FILE *datoteka_automobila, int registarska_oznaka) {
    Blok_automobila blok_automobila;
    rewind(datoteka_automobila);
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].registarska_oznaka) {
                return false; 
            }
            if (blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
                return true; 
            }
        }
    }
    return false; 
}

bool identifikator_postoji(FILE *datoteka_parkiranja, int identifikator) {
    Blok_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    while (fread(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja) == 1) {
        for (int i = 0; i < f2; i++) {
            if (!blok_parkiranja.parkinga[i].identifikator) {
                return false; 
            }
            if (blok_parkiranja.parkinga[i].identifikator == identifikator) {
                return true; 
            }
        }
    }
    return false; 
}

Automobil* upis_automobila(FILE *datoteka_automobila) {
    Automobil* novi_automobil = malloc(sizeof(Automobil));
    if (!novi_automobil) {
        perror("Greska pri alokaciji memorije");
        fcloseall();
        exit(EXIT_FAILURE);
    }
    *novi_automobil = default_automobil;
    bool postoji = false;
    do
    {
        if (postoji) {
            printf("Automobil sa zadatom registarskom oznakom vec postoji!\n");
            printf("Unesite validnu registarsku oznaku.\n");
        }
        printf("Unesite registarsku oznaku: ");
        scanf("%d", &novi_automobil->registarska_oznaka);
        while(getchar() != '\n');
    } while (novi_automobil->registarska_oznaka <= 0 || (postoji = registarska_oznaka_postoji_sekvencijalna(datoteka_automobila, novi_automobil->registarska_oznaka)));

    printf("Unesite marku: ");
    scanf("%30s", novi_automobil->marka);
    while(getchar() != '\n');
    printf("Unesite model: ");
    scanf("%30s", novi_automobil->model);
    while(getchar() != '\n');
    printf("Unesite godinu proizvodnje (npr. 2023): ");
    bool valid = false;
    int god = novi_automobil->godina_proizvodnje;
    while (!valid) {
        scanf("%d", &god);
        while(getchar() != '\n');
        valid = true;
        
        if (god < 1000 || god > 9999) {
            valid = false;
            printf("Greska: godina proizvodnje mora imati tacno 4 cifre i biti numericka.\n");
        }
    }

    novi_automobil->godina_proizvodnje = god;
    printf("Unesite boju: ");
    scanf("%20s", novi_automobil->boja);
    while(getchar() != '\n');


    Blok_automobila blok_automobila;
    rewind(datoteka_automobila);


    while(fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) 
    {
        
        for(int i=0;i<f1;++i) {
            if(!blok_automobila.automobili[i].registarska_oznaka) {
                blok_automobila.automobili[i] = *novi_automobil;
                fseek(datoteka_automobila, -(long)sizeof(Blok_automobila), SEEK_CUR);
                fwrite_safe(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila);
                fflush(datoteka_automobila);
                return novi_automobil;
            }

            if (novi_automobil->registarska_oznaka == blok_automobila.automobili[i].registarska_oznaka) {
                printf("Automobil sa zadatom registarskom oznakom vec postoji!");
                return NULL;
            }

            if(novi_automobil->registarska_oznaka < blok_automobila.automobili[i].registarska_oznaka) {
                int j=i;
                Automobil temp, sledeci_slog = *novi_automobil;

                do{
                    fseek(datoteka_automobila, -(long)sizeof(Blok_automobila), SEEK_CUR);
                    for(;j<f1;j++) {
                        if(!blok_automobila.automobili[j].registarska_oznaka) {
                            blok_automobila.automobili[j] = sledeci_slog;
                            fwrite_safe(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila);
                            fflush(datoteka_automobila);
                            return novi_automobil;
                        }
                        temp = blok_automobila.automobili[j];
                        blok_automobila.automobili[j] = sledeci_slog;
                        sledeci_slog = temp;
                    }
                    fwrite_safe(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila);
                    j=0;
                } while(fread(&blok_automobila,sizeof(Blok_automobila), 1, datoteka_automobila)==1);
                dodaj_prazan_blok_automobila(datoteka_automobila, sledeci_slog);
                fflush(datoteka_automobila);
                return novi_automobil;
            }
        } 
    } 
    dodaj_prazan_blok_automobila(datoteka_automobila, *novi_automobil);
    fflush(datoteka_automobila);
    return novi_automobil;
}

bool format_datuma_vremena(int godina, int mesec, int dan, int sat, int minut) {
    if (godina < 1800 || godina > 2100 || mesec < 1 || mesec > 12 || dan < 1 || dan > 31 || sat < 0 || sat > 23 || minut < 0 || minut > 59) {
        return false;
    }
    if ((mesec == 4 || mesec == 6 || mesec == 9 || mesec == 11) && dan > 30) {
        return false;
    }
    if (mesec == 2) {
        if ((godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0)) {
            return (dan <= 29);
        } else {
            return (dan <= 28);
        }
    }

    return true;
}

Parkiranje *upis_parkiranja(FILE *datoteka_parkiranja, FILE *datoteka_automobila) {
    Parkiranje *novo_parkiranje = malloc(sizeof(Parkiranje));
    if (!novo_parkiranje) {
        printf("Greska pri alokaciji memorije.\n");
        free(novo_parkiranje);
        fcloseall();
        exit(EXIT_FAILURE);
    }
    do
    {
        printf("Unesite identifikator: ");
        scanf("%d", &novo_parkiranje->identifikator);
        while(getchar() != '\n');
    } while (novo_parkiranje->identifikator <= 0 || identifikator_postoji(datoteka_parkiranja, novo_parkiranje->identifikator));
    printf("Unesite registarsku oznaku: ");
    scanf("%d", &novo_parkiranje->registarska_oznaka);
    while(getchar() != '\n');
    if (!registarska_oznaka_postoji_sekvencijalna(datoteka_automobila, novo_parkiranje->registarska_oznaka)) {
            printf("Registarska oznaka nije pronađena na spisku automobila.\n");
            return NULL;
    }
    printf("Unesite datum i vreme (npr. 15-5-2025 10:15): ");
    int godina, mesec, dan, sat, minut;
    while (scanf("%2d-%2d-%4d %2d:%2d", &dan, &mesec, &godina, &sat, &minut) != 5 || !format_datuma_vremena(godina, mesec, dan, sat, minut)) {

        printf("Neispravan format ili nepostojeci datum. Pokusajte ponovo: ");
        while (getchar() != '\n');
    }
    while(getchar() != '\n');
    sprintf(novo_parkiranje->datum_vreme, "%2d-%2d-%4d %02d:%02d", dan, mesec, godina, sat, minut);
    while(getchar() != '\n');
    printf("Unesite duzinu boravka (u satima): ");
    scanf("%d", &novo_parkiranje->duzina_boravka);
    while(getchar() != '\n');
    unsigned char izbor;
    do {
        printf("Unesite zonu (P=PLAVA, B=BELA, C=CRVENA): ");
        scanf("%1c", &izbor);
        while(getchar() != '\n');
        switch (izbor) {
        case 'P': strcpy(novo_parkiranje->zona, "PLAVA"); break;
        case 'B': strcpy(novo_parkiranje->zona, "BELA"); break;
        case 'C': strcpy(novo_parkiranje->zona, "CRVENA"); break;
        default:
            printf("Neispravan unos. Molimo unesite P, B ili C.\n");
            izbor = ' ';
            continue; 
        }
        break;
    } while (1);

    Blok_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    while(fread(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja) == 1) 
    {
        for(int i=0;i<f2;++i) {
            if (novo_parkiranje->identifikator == blok_parkiranja.parkinga[i].identifikator) {
                printf("Parkiranje sa zadatim identifikatorom vec postoji!");
                return NULL;
            }

            if(!blok_parkiranja.parkinga[i].registarska_oznaka) {
                blok_parkiranja.parkinga[i] = *novo_parkiranje;
                fseek(datoteka_parkiranja, -(long)sizeof(Blok_parkiranja), SEEK_CUR);
                fwrite_safe(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja);
                fflush(datoteka_parkiranja);
                return novo_parkiranje;
            }
        } 
    } 
    dodaj_prazan_blok_parkiranja(datoteka_parkiranja, novo_parkiranje);
    fflush(datoteka_parkiranja);
}

void ispisi_automobil(FILE *datoteka_automobila) {
    Blok_automobila blok_automobila;
    rewind(datoteka_automobila);  
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while(getchar() != '\n');
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
                printf("Boja: %s\n", blok_automobila.automobili[i].boja);
                printf("Marka: %s\n", blok_automobila.automobili[i].marka);
                printf("Adresa bloka: %p\n", &blok_automobila);
                printf("Redni broj sloga u bloku: %d\n", i);
                return;
            }
        }
    }
    
    printf("Automobil sa registarskom oznakom %d nije pronadjen.\n", registarska_oznaka);
    
}

void prikaz_parkiranja(FILE *datoteka_parkiranja, int registarska_oznaka) {
    Blok_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    bool pronadjen = false;
    while (fread(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja) == 1) {
        for (int i = 0; i < f2; i++) {
            if(!blok_parkiranja.parkinga[i].registarska_oznaka) {
                if(!pronadjen) printf("Nema parkiranja za automobil sa registarskom oznakom %d koje traje duze od 3 sata.\n", registarska_oznaka);
                return;
            }
            if (blok_parkiranja.parkinga[i].registarska_oznaka == registarska_oznaka && 
                blok_parkiranja.parkinga[i].duzina_boravka > 3) {
                printf("Parkiranje:\n");
                printf("Identifikator: %d\n", blok_parkiranja.parkinga[i].identifikator);
                printf("Datum i vreme: %s\n", blok_parkiranja.parkinga[i].datum_vreme);
                printf("Duzina boravka: %d sati\n", blok_parkiranja.parkinga[i].duzina_boravka);
                printf("Zona: %s\n", blok_parkiranja.parkinga[i].zona);
                printf("Adresa bloka: %p\n", &blok_parkiranja);
                printf("Redni broj sloga u bloku: %d\n", i);
                pronadjen = true;
            }
        }
    }
    printf("_________________________\n");
    
}

void prikaz_automobila(FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    Blok_automobila blok_automobila;
    rewind(datoteka_automobila);
    bool pronadjen = false;
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].registarska_oznaka) {
                if (!pronadjen) printf("Nema automobila u evidenciji.\n");
                return;
            }
            printf("Registarska oznaka: %d\n", blok_automobila.automobili[i].registarska_oznaka);
            printf("Marka: %s\n", blok_automobila.automobili[i].marka);
            printf("Model: %s\n", blok_automobila.automobili[i].model);
            printf("Godina proizvodnje: %d\n", blok_automobila.automobili[i].godina_proizvodnje);
            printf("Boja: %s\n", blok_automobila.automobili[i].boja);
            printf("Adresa bloka: %p\n", &blok_automobila);
            printf("Redni broj sloga u bloku: %d\n", i);
            printf("_________________________\n");
            prikaz_parkiranja(datoteka_parkiranja, blok_automobila.automobili[i].registarska_oznaka);
            rewind(datoteka_parkiranja);
            printf("-------------------------\n");
            pronadjen = true;
        }
        
    }
}

Automobil *modifikacija_automobila(FILE *datoteka_automobila) {
    Blok_automobila *blok_automobila;
    rewind(datoteka_automobila);
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while(getchar() != '\n');
    bool promenjen = false;
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila->automobili[i].registarska_oznaka) {
                printf("Nema automobila u evidenciji.\n");
                return NULL;
            }
            if (blok_automobila->automobili[i].registarska_oznaka == registarska_oznaka) {

                printf("Ako ne zelite da menjate vrednost, pritisnite Enter.\n");
                bool postoji = false;
                char unos[31];
                printf("Unesite marku ['%s']: ", blok_automobila->automobili[i].marka);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    unos[strcspn(unos, "\n")] = '\0';
                    strcpy(blok_automobila->automobili[i].marka, unos);
                    promenjen = true;
                }
                printf("Unesite model ['%s']: ", blok_automobila->automobili[i].model);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    strcpy(blok_automobila->automobili[i].model, unos);
                    unos[0] = '\0';
                    promenjen = true;
                }
                printf("Unesite godinu proizvodnje ['%d']: ", blok_automobila->automobili[i].godina_proizvodnje);
                int god = blok_automobila->automobili[i].godina_proizvodnje;
                while (1) {
                    fgets(unos, sizeof(unos), stdin);
                    if (unos[0] == '\n') {

                        break; 
                    }
                    if (sscanf(unos, "%d", &god) != 1) {
                        printf("Greska: godina proizvodnje mora biti numericka.\n");
                        continue;
                    }
                    
                    if (god < 1000 || god > 9999) {
                        printf("Greska: godina proizvodnje mora imati tacno 4 cifre i biti numericka.\n");
                        continue;
                    }
                    promenjen = true;
                    break;
                }

                blok_automobila->automobili[i].godina_proizvodnje = god;
                printf("Unesite boju ['%s']: ", blok_automobila->automobili[i].boja);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    unos[strcspn(unos, "\n")] = '\0';
                    strcpy(blok_automobila->automobili[i].boja, unos);
                    promenjen = true;
                }


                fseek(datoteka_automobila, -(long)sizeof(Blok_automobila), SEEK_CUR);
                fwrite_safe(blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila);
                if (promenjen) return &blok_automobila->automobili[i];
                else return NULL;
                fflush(datoteka_automobila);
                printf("Automobil sa registarskom oznakom %d je uspešno modifikovan.\n", registarska_oznaka);
                return &blok_automobila->automobili[i];
            }
        }
    }
    printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
}

void ispisi_datoteku_automobila(FILE *datoteka_automobila) {
    Blok_automobila blok_automobila;
    rewind(datoteka_automobila);
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila) == 1) {
        if (blok_automobila.automobili[0].registarska_oznaka == 0) {
            printf("Datoteka automobila je prazna.\n");
            return;
        }
        printf("Datoteka automobila:\n");
        printf("-------------------------\n");
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].registarska_oznaka) {
                printf("\t~END~\n");
                return;
            }
            printf("Registarska oznaka: %d\n", blok_automobila.automobili[i].registarska_oznaka);
            printf("Marka: %s\n", blok_automobila.automobili[i].marka);
            printf("Model: %s\n", blok_automobila.automobili[i].model);
            printf("Godina proizvodnje: %d\n", blok_automobila.automobili[i].godina_proizvodnje);
            printf("Boja: %s\n", blok_automobila.automobili[i].boja);
            printf("-------------------------\n");
        }
    }
}

void ispisi_datoteku_parkiranja(FILE *datoteka_parkiranja) {
    Blok_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    while (fread(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja) == 1) {
        if (blok_parkiranja.parkinga[0].identifikator == 0) {
            printf("Datoteka parkiranja je prazna.\n");
            return;
        }
        printf("Datoteka parkiranja:\n");
        printf("-------------------------\n");
        for (int i = 0; i < f2; i++) {
            if (!blok_parkiranja.parkinga[i].registarska_oznaka) {
                return; 
            }
            printf("Identifikator: %d\n", blok_parkiranja.parkinga[i].identifikator);
            printf("Registarska oznaka: %d\n", blok_parkiranja.parkinga[i].registarska_oznaka);
            printf("Datum i vreme: %s\n", blok_parkiranja.parkinga[i].datum_vreme);
            printf("Duzina boravka: %d sati\n", blok_parkiranja.parkinga[i].duzina_boravka);
            printf("Zona: %s\n", blok_parkiranja.parkinga[i].zona);
            printf("-------------------------\n");
        }
    }
}

