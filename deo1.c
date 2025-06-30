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
        int godina_proizvodnje[4];
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

automobil default_automobil = {0, "", "", {0}, "", false};
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

void *dodaj_prazan_blok_automobila(FILE *datoteka_automobila, automobil *slog_za_dodavanje) {
    BLOK_automobila blok_automobila;
    blok_automobila.automobili[0] = *slog_za_dodavanje;
    for (int i = 1; i < f1; i++) {
        blok_automobila.automobili[i] = default_automobil;
    }

    fseek(datoteka_automobila,0,SEEK_END);
    blok_automobila.index = (ftell(datoteka_automobila) / sizeof(BLOK_automobila));
    fwrite(&blok_automobila, sizeof(blok_automobila), 1, datoteka_automobila);

}

void *dodaj_prazan_blok_parkiranja(FILE *datoteka_parkiranja, parkiranje *slog_za_dodavanje) {
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

    dodaj_prazan_blok_automobila(datoteka_automobila, &default_automobil);
    

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

void azuriranje_indeksa_blokova(FILE *datoteka) {
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

void upis_automobila(FILE *datoteka_automobila) {
    automobil novi_automobil;
    do
    {
        printf("Unesite registarsku oznaku: ");
        scanf("%d", &novi_automobil.registarska_oznaka);
    } while (novi_automobil.registarska_oznaka <= 0 || registarska_oznaka_postoji(datoteka_automobila, novi_automobil.registarska_oznaka));

    printf("Unesite marku: ");
    scanf("%30s", novi_automobil.marka);
    printf("Unesite model: ");
    scanf("%30s", novi_automobil.model);
    printf("Unesite godinu proizvodnje (npr. 2023): ");
    for (int i = 0; i < 4; i++) {
        scanf("%1d", &novi_automobil.godina_proizvodnje[i]);
    }
    printf("Unesite boju: ");
    scanf("%20s", novi_automobil.boja);
    novi_automobil.popunjen = false;
    BLOK_automobila *blok_automobila;
    rewind(datoteka_automobila);
    while(fread(blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) 
    {
        int prethodni_indeks;
        for(int i=0;i<f1;++i) {
            automobil *trenutni_slog = &blok_automobila->automobili[i];

            if (novi_automobil.registarska_oznaka == trenutni_slog->registarska_oznaka && trenutni_slog->popunjen) {
                printf("Automobil sa zadatom registarskom oznakom vec postoji!");
                return;
            }
            if(novi_automobil.registarska_oznaka >= trenutni_slog->registarska_oznaka && !trenutni_slog->popunjen) {
                *trenutni_slog = novi_automobil;
                trenutni_slog->popunjen = true;
                fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                fwrite(blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                azuriranje_indeksa_blokova(datoteka_automobila);
                return;
            }

            if(novi_automobil.registarska_oznaka > trenutni_slog->registarska_oznaka) {
                // Ispravljeno: koristi sizeof(BLOK_automobila) umesto sizeof(BLOK_parkiranja)
                fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                long trenutna_pozicija = ftell(datoteka_automobila);
                int j;
                for(j=f1-1;j>=0;--j) {
                    if (!blok_automobila->automobili[j].popunjen) break;
                }
                if(blok_automobila->automobili[f1-1].popunjen) {
                    dodaj_prazan_blok_automobila(datoteka_automobila, &novi_automobil);
                    fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_END);
                } else fseek(datoteka_automobila,-(f1-1-j)*sizeof(automobil),SEEK_END);
                long kraj_fajla = ftell(datoteka_automobila);
                long velicina_buffera = kraj_fajla - trenutna_pozicija;
                automobil *buffer = malloc(velicina_buffera);
                fseek(datoteka_automobila, -velicina_buffera, SEEK_CUR);
                fread(buffer, 1, velicina_buffera, datoteka_automobila);
                fseek(datoteka_automobila, -velicina_buffera, SEEK_CUR);
                fwrite(&novi_automobil, sizeof(automobil), 1, datoteka_automobila);
                fwrite(buffer, 1, velicina_buffera, datoteka_automobila);
                azuriranje_indeksa_blokova(datoteka_automobila);
                free(buffer);
                return;
            }
            prethodni_indeks = blok_automobila->index;
        } 
    } 
    dodaj_prazan_blok_automobila(datoteka_automobila, &novi_automobil);
}

void upis_parkiranja(FILE *datoteka_parkiranja) {
    parkiranje novo_parkiranje;
    do
    {
        printf("Unesite identifikator: ");
        scanf("%d", &novo_parkiranje.identifikator);
    } while (novo_parkiranje.identifikator <= 0 || identifikator_postoji(datoteka_parkiranja, novo_parkiranje.identifikator));
    printf("Unesite registarsku oznaku: ");
    scanf("%d", &novo_parkiranje.registarska_oznaka);
    printf("Unesite datum i vreme (npr. 2023-10-01 12:00): ");
    scanf("%16s", novo_parkiranje.datum_vreme);
    printf("Unesite duzinu boravka (u satima): ");
    scanf("%d", &novo_parkiranje.duzina_boravka);
    printf("Unesite zonu: ");
    scanf("%d", &novo_parkiranje.zona);
    novo_parkiranje.popunjen = false;

    BLOK_parkiranja *blok_parkiranja;
    rewind(datoteka_parkiranja);
    while(fread(blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja) == 1) 
    {
        for(int i=0;i<f2;++i) {
            parkiranje *trenutni_slog = &blok_parkiranja->parkinga[i];
            if (novo_parkiranje.identifikator == trenutni_slog->identifikator) {
                printf("Parkiranje sa zadatim identifikatorom vec postoji!");
                return;
            }

            if(!trenutni_slog->popunjen) {
                *trenutni_slog = novo_parkiranje;
                trenutni_slog->popunjen = true;
                fseek(datoteka_parkiranja, -(long)sizeof(BLOK_parkiranja), SEEK_CUR);
                fwrite(blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja);
                azuriranje_indeksa_blokova(datoteka_parkiranja);
                return;
            }
        } 
    } 
    dodaj_prazan_blok_parkiranja(datoteka_parkiranja, &novo_parkiranje);
}

void ispisi_automobil(FILE *datoteka_automobila) {
    BLOK_automobila *blok_automobila;
    rewind(datoteka_automobila);  
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila->automobili[i].popunjen && blok_automobila->automobili[i].registarska_oznaka == registarska_oznaka) {
                printf("Boja: %s\n", blok_automobila->automobili[i].boja);
                printf("Marka: %s\n", blok_automobila->automobili[i].marka);
                printf("Adresa bloka: %p\n", blok_automobila);
                printf("Redni broj sloga u bloku: %d\n", i);
                
                return;
            }
        }
        printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
    }
}

void prikaz_parkiranja(FILE *datoteka_parkiranja, int registarska_oznaka) {
    BLOK_parkiranja *blok_parkiranja;
    rewind(datoteka_parkiranja);
    bool pronadjen = false;
    while (fread(&blok_parkiranja, sizeof(BLOK_parkiranja), 1, datoteka_parkiranja) == 1) {
        for (int i = 0; i < f2; i++) {
            if (blok_parkiranja->parkinga[i].popunjen && 
                blok_parkiranja->parkinga[i].registarska_oznaka == registarska_oznaka && 
                blok_parkiranja->parkinga[i].duzina_boravka > 3) {
                printf("Parkiranje:\n");
                printf("Identifikator: %d\n", blok_parkiranja->parkinga[i].identifikator);
                printf("Datum i vreme: %s\n", blok_parkiranja->parkinga[i].datum_vreme);
                printf("Duzina boravka: %d sati\n", blok_parkiranja->parkinga[i].duzina_boravka);
                printf("Zona: %s\n", blok_parkiranja->parkinga[i].zona);
                printf("Adresa bloka: %p\n", &blok_parkiranja);
                printf("Redni broj sloga u bloku: %d\n", i);
                pronadjen = true;
            }
        }
    }
    if (!pronadjen) {
        printf("Nema parkiranja za automobil sa registarskom oznakom %d koje traje duze od 3 sata.\n", registarska_oznaka);
    }
    printf("_________________________\n");
    
}

void prikaz_automobila(FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    BLOK_automobila *blok_automobila;
    rewind(datoteka_automobila);
    bool prazan = true;
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila->automobili[i].popunjen) {
                printf("Registarska oznaka: %d\n", blok_automobila->automobili[i].registarska_oznaka);
                printf("Marka: %s\n", blok_automobila->automobili[i].marka);
                printf("Model: %s\n", blok_automobila->automobili[i].model);
                printf("Godina proizvodnje: %d%d%d%d\n", 
                       blok_automobila->automobili[i].godina_proizvodnje[0],
                       blok_automobila->automobili[i].godina_proizvodnje[1],
                       blok_automobila->automobili[i].godina_proizvodnje[2],
                       blok_automobila->automobili[i].godina_proizvodnje[3]);
                printf("Boja: %s\n", blok_automobila->automobili[i].boja);
                printf("Adresa bloka: %p\n", blok_automobila);
                printf("Redni broj sloga u bloku: %d\n", i);
                printf("_________________________\n");
                prikaz_parkiranja(datoteka_parkiranja, blok_automobila->automobili[i].registarska_oznaka);
                rewind(datoteka_parkiranja);
                printf("-------------------------\n");
                prazan = false;
            }
        }
    }
    if (prazan) {
        printf("Nema automobila u evidenciji.\n");
    }
}

void modifikacija_automobila(FILE *datoteka_automobila) {
    BLOK_automobila *blok_automobila;
    rewind(datoteka_automobila);
    int registarska_oznaka;
    printf("Unesite registarsku oznaku automobila: ");
    scanf("%d", &registarska_oznaka);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila->automobili[i].popunjen && blok_automobila->automobili[i].registarska_oznaka == registarska_oznaka) {
                
                printf("Ako ne zelite da menjate vrednost, pritisnite Enter.\n");
                char string[31];
                printf("Unesite novu marku [\"%s\"]: ", blok_automobila->automobili[i].marka);
                fgets(string, sizeof(blok_automobila->automobili[i].marka), stdin);
                if(string[0] != '\0') {
                    strcpy(blok_automobila->automobili[i].marka, string);
                }
                printf("Unesite novi model [\"%s\"]: ", blok_automobila->automobili[i].model);
                scanf("%30s", blok_automobila->automobili[i].model);
                printf("Unesite novu godinu proizvodnje (npr. 2023) [\"%d%d%d%d\"]: ", 
                blok_automobila->automobili[i].godina_proizvodnje[0],
                blok_automobila->automobili[i].godina_proizvodnje[1],
                blok_automobila->automobili[i].godina_proizvodnje[2],
                blok_automobila->automobili[i].godina_proizvodnje[3]);
                scanf("%4s", string);
                if(string[0] != '\0') {
                    for (int j = 0; j < 4; j++) {
                        blok_automobila->automobili[i].godina_proizvodnje[j] = string[j] - '0';
                    }
                }

                printf("Unesite novu boju [\"%s\"]: ", blok_automobila->automobili[i].boja);
                scanf("%20s", blok_automobila->automobili[i].boja);
                
                fseek(datoteka_automobila, -(long)sizeof(BLOK_automobila), SEEK_CUR);
                fwrite(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila);
                printf("Automobil sa registarskom oznakom %d je uspešno modifikovan.\n", registarska_oznaka);
                return;
            }
        }
    }
    printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
}
