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
        bool popunjen;
    } automobil;

typedef struct {
        int identifikator;
        int registarska_oznaka;
        char datum_vreme[17];
        int duzina_boravka;
        char zona[7];
        bool popunjen;

    } parkiranje;

typedef struct {
        automobil automobili[f1];
        int index;
    } BLOK_automobila;

typedef struct {
        parkiranje parkinga[f2];
        int index;
    } BLOK_parkiranja;

automobil default_automobil = {0, "", "", 0, "", false};
parkiranje default_parkiranje = {0, 0, "", 0, "", false};

FILE *otvori_datoteku(const char *ime_datoteke, const char *mod)
{
    FILE *datoteka = fopen(ime_datoteke, mod);
    if (datoteka == NULL) {
        perror("Otvaranje datoteke nije uspelo!");
        exit(EXIT_FAILURE);
    }
    return datoteka;
}

void dodaj_prazan_blok_automobila(FILE *datoteka_automobila, automobil slog_za_dodavanje) {
    BLOK_automobila blok_automobila;
    blok_automobila.automobili[0] = slog_za_dodavanje;
    for (int i = 1; i < f1; i++) {
        blok_automobila.automobili[i] = default_automobil;
    }

    fseek(datoteka_automobila,0,SEEK_END);
    blok_automobila.index = (ftell(datoteka_automobila) / sizeof(BLOK_automobila));
    fwrite(&blok_automobila, sizeof(blok_automobila), 1, datoteka_automobila);

}

void dodaj_prazan_blok_parkiranja(FILE *datoteka_parkiranja, parkiranje *slog_za_dodavanje) {
    BLOK_parkiranja blok_parkiranja;
    blok_parkiranja.parkinga[0] = *slog_za_dodavanje;
    for (int i = 1; i < f2; i++) {
        blok_parkiranja.parkinga[i] = default_parkiranje;
    }

    fseek(datoteka_parkiranja,0,SEEK_END);
    blok_parkiranja.index = (ftell(datoteka_parkiranja) / sizeof(BLOK_parkiranja));
    fwrite(&blok_parkiranja, sizeof(blok_parkiranja), 1, datoteka_parkiranja);
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

bool registarska_oznaka_postoji(FILE *datoteka_automobila, int registarska_oznaka) {
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila.automobili[i].popunjen && blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
                return true; 
            }
        }
    }
    return false; 
}

bool identifikator_postoji(FILE *datoteka_parkiranja, int identifikator) {
    BLOK_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    while (fread(&blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja) == 1) {
        for (int i = 0; i < f2; i++) {
            if (blok_parkiranja.parkinga[i].popunjen && blok_parkiranja.parkinga[i].identifikator == identifikator) {
                return true; 
            }
        }
    }
    return false; 
}

void azuriranje_indeksa_blokova_automobila(FILE *datoteka) {
    fseek(datoteka, 0, SEEK_END);
    long velicina = ftell(datoteka);
    int broj_blokova = velicina / sizeof(BLOK_automobila);
    fseek(datoteka, 0, SEEK_SET);
    for (int i = 0; i < broj_blokova; i++) {
        BLOK_automobila blok;
        fread(&blok, sizeof(BLOK_automobila), 1, datoteka);
        blok.index = i;
        fseek(datoteka, -(long)sizeof(BLOK_automobila), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_automobila), 1, datoteka);
    }
}

void azuriranje_indeksa_blokova_parkiranja(FILE *datoteka) {
    fseek(datoteka, 0, SEEK_END);
    long velicina = ftell(datoteka);
    int broj_blokova = velicina / sizeof(BLOK_parkiranja);
    fseek(datoteka, 0, SEEK_SET);
    for (int i = 0; i < broj_blokova; i++) {
        BLOK_parkiranja blok;
        fread(&blok, sizeof(BLOK_parkiranja), 1, datoteka);
        blok.index = i;
        fseek(datoteka, -(long)sizeof(BLOK_parkiranja), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK_parkiranja), 1, datoteka);
    }
}

void upis_automobila(FILE *datoteka_automobila) {
    automobil novi_automobil = default_automobil;
    novi_automobil.popunjen = true;
    bool postoji = false;
    do
    {
        if (postoji) {
            printf("Automobil sa zadatom registarskom oznakom vec postoji!\n");
            printf("Unesite validnu registarsku oznaku.\n");
        }
        printf("Unesite registarsku oznaku: ");
        scanf("%d", &novi_automobil.registarska_oznaka);
        while(getchar() != '\n');
    } while (novi_automobil.registarska_oznaka <= 0 || (postoji = registarska_oznaka_postoji(datoteka_automobila, novi_automobil.registarska_oznaka)));

    printf("Unesite marku: ");
    scanf("%30s", novi_automobil.marka);
    while(getchar() != '\n');
    printf("Unesite model: ");
    scanf("%30s", novi_automobil.model);
    while(getchar() != '\n');
    printf("Unesite godinu proizvodnje (npr. 2023): ");
    bool valid = false;
    int god = novi_automobil.godina_proizvodnje;
    while (!valid) {    
        scanf("%d", &god);
        while(getchar() != '\n');
        valid = true;
        
        if (god < 1000 || god > 9999) {
            valid = false;
            printf("Greska: godina proizvodnje mora imati tacno 4 cifre i biti numericka.\n");
        }
    }

    novi_automobil.godina_proizvodnje = god;
    printf("Unesite boju: ");
    scanf("%20s", novi_automobil.boja);
    while(getchar() != '\n');


    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);


    while(fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) 
    {
        
        for(int i=0;i<f1;++i) {
            if(!blok_automobila.automobili[i].popunjen) {
                blok_automobila.automobili[i] = novi_automobil;
                blok_automobila.automobili[i].popunjen = true;
                fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                fwrite(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                azuriranje_indeksa_blokova_automobila(datoteka_automobila);
                fflush(datoteka_automobila);
                return;
            }

            if (novi_automobil.registarska_oznaka == blok_automobila.automobili[i].registarska_oznaka && blok_automobila.automobili[i].popunjen) {
                printf("Automobil sa zadatom registarskom oznakom vec postoji!");
                return;
            }

            if(novi_automobil.registarska_oznaka < blok_automobila.automobili[i].registarska_oznaka) {
                int j=i;
                automobil temp, sledeci_slog = novi_automobil;

                do{
                    fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                    for(;j<f1;j++) {
                        if(!blok_automobila.automobili[j].popunjen) {
                            blok_automobila.automobili[j] = sledeci_slog;
                            blok_automobila.automobili[j].popunjen = true;
                            fwrite(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                            azuriranje_indeksa_blokova_automobila(datoteka_automobila);
                            fflush(datoteka_automobila);
                            return;
                        }
                        temp = blok_automobila.automobili[j];
                        blok_automobila.automobili[j] = sledeci_slog;
                        sledeci_slog = temp;
                    }
                    fwrite(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                    j=0;
                } while(fread(&blok_automobila,sizeof(BLOK_automobila), 1, datoteka_automobila)==1);
                dodaj_prazan_blok_automobila(datoteka_automobila, sledeci_slog);
                fflush(datoteka_automobila);
                return;
            }
        } 
    } 
    dodaj_prazan_blok_automobila(datoteka_automobila, novi_automobil);
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

void upis_parkiranja(FILE *datoteka_parkiranja) {
    parkiranje novo_parkiranje;
    do
    {
        printf("Unesite identifikator: ");
        scanf("%d", &novo_parkiranje.identifikator);
        while(getchar() != '\n');
    } while (novo_parkiranje.identifikator <= 0 || identifikator_postoji(datoteka_parkiranja, novo_parkiranje.identifikator));
    printf("Unesite registarsku oznaku: ");
    scanf("%d", &novo_parkiranje.registarska_oznaka);
    while(getchar() != '\n');
    printf("Unesite datum i vreme (npr. 2023-10-01 12:00): ");
    int godina, mesec, dan, sat, minut;
    while (scanf("%4d-%2d-%2d %2d:%2d", &godina, &mesec, &dan, &sat, &minut) != 5 || !format_datuma_vremena(godina, mesec, dan, sat, minut)) {
        printf("Neispravan format ili nepostojeci datum. Pokusajte ponovo: ");
        while (getchar() != '\n');
    }
    while(getchar() != '\n');
    sprintf(novo_parkiranje.datum_vreme, "%04d-%02d-%02d %02d:%02d", godina, mesec, dan, sat, minut);
    while(getchar() != '\n');
    printf("Unesite duzinu boravka (u satima): ");
    scanf("%d", &novo_parkiranje.duzina_boravka);
    while(getchar() != '\n');
    unsigned char izbor;
    do {
        printf("Unesite zonu (P=PLAVA, B=BELA, C=CRVENA): ");
        scanf("%1c", &izbor);
        while(getchar() != '\n');
        switch (izbor) {
        case 'P': strcpy(novo_parkiranje.zona, "PLAVA"); break;
        case 'B': strcpy(novo_parkiranje.zona, "BELA"); break;
        case 'C': strcpy(novo_parkiranje.zona, "CRVENA"); break;
        default:
            printf("Neispravan unos. Molimo unesite P, B ili C.\n");
            izbor = ' ';
            continue; 
        }
        break;
    } while (1);

    
    novo_parkiranje.popunjen = true;

    BLOK_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    while(fread(&blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja) == 1) 
    {
        for(int i=0;i<f2;++i) {
            if (novo_parkiranje.identifikator == blok_parkiranja.parkinga[i].identifikator) {
                printf("Parkiranje sa zadatim identifikatorom vec postoji!");
                return;
            }

            if(!blok_parkiranja.parkinga[i].popunjen) {
                blok_parkiranja.parkinga[i] = novo_parkiranje;
                blok_parkiranja.parkinga[i].popunjen = true;
                fseek(datoteka_parkiranja, -(long)sizeof(BLOK_parkiranja), SEEK_CUR);
                fwrite(&blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja);
                azuriranje_indeksa_blokova_parkiranja(datoteka_parkiranja);
                fflush(datoteka_parkiranja);
                return;
            }
        } 
    } 
    dodaj_prazan_blok_parkiranja(datoteka_parkiranja, &novo_parkiranje);
    fflush(datoteka_parkiranja);
}

void ispisi_automobil(FILE *datoteka_automobila) {
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);  
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while(getchar() != '\n');
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila.automobili[i].popunjen && blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
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
    BLOK_parkiranja blok_parkiranja;
    rewind(datoteka_parkiranja);
    bool pronadjen = false;
    while (fread(&blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja) == 1) {
        for (int i = 0; i < f2; i++) {
            if(!blok_parkiranja.parkinga[i].popunjen) {
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
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);
    bool pronadjen = false;
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].popunjen) {
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

void modifikacija_automobila(FILE *datoteka_automobila) {
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while(getchar() != '\n');
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].popunjen) {
                printf("Nema automobila u evidenciji.\n");
                return;
            }
            if (blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {

                printf("Ako ne zelite da menjate vrednost, pritisnite Enter.\n");
                bool postoji = false;
                char unos[31];
                printf("Unesite marku ['%s']: ", blok_automobila.automobili[i].marka);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    unos[strcspn(unos, "\n")] = '\0';
                    strcpy(blok_automobila.automobili[i].marka, unos);
                }
                printf("Unesite model ['%s']: ", blok_automobila.automobili[i].model);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    strcpy(blok_automobila.automobili[i].model, unos);
                    unos[0] = '\0';
                }
                printf("Unesite godinu proizvodnje ['%d']: ", blok_automobila.automobili[i].godina_proizvodnje);
                int god = blok_automobila.automobili[i].godina_proizvodnje;
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
                    break;
                }

                blok_automobila.automobili[i].godina_proizvodnje = god;
                printf("Unesite boju ['%s']: ", blok_automobila.automobili[i].boja);
                fgets(unos, sizeof(unos), stdin);
                if (unos[0] != '\n') {
                    unos[strcspn(unos, "\n")] = '\0';
                    strcpy(blok_automobila.automobili[i].boja, unos);
                }


                fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                fwrite(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                fflush(datoteka_automobila);
                printf("Automobil sa registarskom oznakom %d je uspešno modifikovan.\n", registarska_oznaka);
                return;
            }
        }
    }
    printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
}


void meni_deo1() {
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
            printf("4. Upis novog parkiranja\n");
            printf("5. Ispis automobila po registarskoj oznaci\n");
            printf("6. Prikaz svih automobila\n");
            printf("7. Modifikacija automobila\n");
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
                break;

            case 2:
                printf("Unesite ime datoteke za evidenciju parkiranja: ");
                while (fgets(ime_datoteke_parkiranja, sizeof(ime_datoteke_parkiranja), stdin) == NULL || ime_datoteke_parkiranja[0] == '\n')
                    printf("Ime datoteke ne sme biti prazno. Unesite ponovo: ");
                ime_datoteke_parkiranja[strcspn(ime_datoteke_parkiranja, "\n")] = 0;
                strcat(ime_datoteke_parkiranja, ".dat");
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
}
