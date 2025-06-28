#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define f1 4
#define f2 5

typedef struct {
        int registarska_oznaka;
        char marka[31];
        char model[31];
        int godina_proizvodnje[4];
        char boja[21];
        bool prazan_slog;
    } automobil;

typedef struct {
        int identifikator;
        int registarska_oznaka;
        char datum_vremena[17];
        int duzina_boravka;
        char zona[7];
        bool prazan_slog;

    } parkiranje;

typedef struct {
        automobil automobili[f1];
        int index;
    } BLOK_automobila;

typedef struct {
        parkiranje parkinga[f2];
        int index;
    } BLOK_parkiranja;

const automobil default_automobil = {0, "", "", {0}, "", true};
const parkiranje default_parkiranje = {0, 0, "", 0, "", true};

FILE *otvori_datoteku(const char *ime_datoteke, const char *mod)
{
    FILE *datoteka = fopen(ime_datoteke, mod);
    if (datoteka == NULL) {
        perror("Otvaranje datoteke nije uspelo!");
        exit(EXIT_FAILURE);
    }
    return datoteka;
}

void *dodaj_prazan_blok_automobila(FILE *datoteka_automobila) {
    BLOK_automobila blok_automobila;
    for (int i = 0; i < f1; i++) {
        blok_automobila.automobili[i] = default_automobil;
    }

    fseek(datoteka_automobila,0,SEEK_END);
    blok_automobila.index = (ftell(datoteka_automobila) / sizeof(BLOK_automobila));
    fwrite(&blok_automobila, sizeof(blok_automobila), 1, datoteka_automobila);

}

void *dodaj_prazan_blok_parkiranja(FILE *datoteka_parkiranja) {
    BLOK_parkiranja blok_parkiranja;
    for (int i = 0; i < f2; i++) {
        blok_parkiranja.parkinga[i] = default_parkiranje;
    }

    fseek(datoteka_parkiranja,0,SEEK_END);
    blok_parkiranja.index = (ftell(datoteka_parkiranja) / sizeof(BLOK_parkiranja));
    fwrite(&blok_parkiranja, sizeof(blok_parkiranja), 1, datoteka_parkiranja);
}



FILE *kreiraj_praznu_evidenciju_automobila(const char *ime_datoteke_automobila){
    FILE *datoteka_automobila = otvori_datoteku(ime_datoteke_automobila, "wb");

    dodaj_prazan_blok_automobila(datoteka_automobila);
    

    return datoteka_automobila;

}

FILE *kreiraj_praznu_evidenciju_parkinga(const char *ime_datoteke_parkinga){
    FILE *datoteka_parkinga = otvori_datoteku(ime_datoteke_parkinga, "wb");

    dodaj_prazan_blok_parkiranja(datoteka_parkinga);

    return datoteka_parkinga;
}

int registrarska_oznaka_postoji(FILE *datoteka_automobila, int registarska_oznaka) {
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].prazan_slog && blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
                return 1; 
            }
        }
    }
    return 0; 
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
    } while (novi_automobil.registarska_oznaka <= 0 || registrarska_oznaka_postoji(datoteka_automobila, novi_automobil.registarska_oznaka));

    printf("Unesite registarsku oznaku: ");
    scanf("%d", &novi_automobil.registarska_oznaka);
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
    novi_automobil.prazan_slog = false;
    BLOK_automobila *blok_automobila;
    rewind(datoteka_automobila);
    while(fread(blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) 
    {
        for(int i=0;i<f1;++i) {
            automobil *trenutni_slog = &blok_automobila->automobili[i];
            if(novi_automobil.registarska_oznaka < trenutni_slog->registarska_oznaka || trenutni_slog->prazan_slog) {
                fseek(datoteka_automobila, -(long)sizeof(BLOK_parkiranja), SEEK_CUR);
                long trenutna_pozicija = ftell(datoteka_automobila);
                int j;
                for(j=f1-1;j>=0;--j) {
                    if (blok_automobila->automobili[j].prazan_slog) break;
                }
                if(!blok_automobila->automobili[f1-1].prazan_slog) {
                    dodaj_prazan_blok_automobila(datoteka_automobila);
                    fseek(datoteka_automobila, -(long)sizeof(BLOK_parkiranja), SEEK_END);
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
        } 
    } 
    dodaj_prazan_blok_automobila(datoteka_automobila);
    fseek(datoteka_automobila, -(long)sizeof(BLOK_parkiranja), SEEK_END);
    fwrite(&novi_automobil, sizeof(automobil), 1, datoteka_automobila);
}

void upis_parkiranja(FILE *datoteka_parkiranja, parkiranje novo_parkiranje) {
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
            if(novo_parkiranje.identifikator < trenutni_slog->identifikator || trenutni_slog->prazan_slog) {
                fseek(datoteka_parkiranja, -(long)sizeof(BLOK_parkiranja), SEEK_CUR);
                long trenutna_pozicija = ftell(datoteka_parkiranja);
                int j;
                for(j=f2-1;j>=0;--j) {
                    if (blok_parkiranja->parkinga[j].prazan_slog) break;
                }
                if(!blok_parkiranja->parkinga[f2-1].prazan_slog) {
                    dodaj_prazan_blok_parkiranja(datoteka_parkiranja);
                    fseek(datoteka_parkiranja, -(long)sizeof(BLOK_parkiranja), SEEK_END);
                } else fseek(datoteka_parkiranja,-(f2-1-j)*sizeof(parkiranje),SEEK_END);
                long kraj_fajla = ftell(datoteka_parkiranja);
                long velicina_buffera = kraj_fajla - trenutna_pozicija;
                parkiranje *buffer = malloc(velicina_buffera);
                fseek(datoteka_parkiranja, -velicina_buffera, SEEK_CUR);
                fread(buffer, 1, velicina_buffera, datoteka_parkiranja);
                fseek(datoteka_parkiranja, -velicina_buffera, SEEK_CUR);
                fwrite(&novo_parkiranje, sizeof(parkiranje), 1, datoteka_parkiranja);
                fwrite(buffer, 1, velicina_buffera, datoteka_parkiranja);
                azuriranje_indeksa_blokova(datoteka_parkiranja);
                free(buffer);
                return;
            }
        } 
    } 
    dodaj_prazan_blok_parkiranja(datoteka_parkiranja);
    fseek(datoteka_parkiranja, -(long)sizeof(BLOK_parkiranja), SEEK_END);
    fwrite(&novo_parkiranje, sizeof(parkiranje), 1, datoteka_parkiranja);
}

void ispisi_automobil(FILE *datoteka_automobila, int registarska_oznaka) {
    //pronađi automobil po registarskoj oznaci
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);    
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].prazan_slog && blok_automobila.automobili[i].registarska_oznaka == registarska_oznaka) {
                blok_automobila.automobili[i].prazan_slog = false;
                
                return;
            }
        }
    }
}



