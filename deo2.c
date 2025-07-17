/*Program treba da ponudi sledeće funkcionalnosti kroz formu tekstualnog korisničkog menija: 
1.  glavnom  meniju  pridodati  opciju  rada  nad  datotekom  sa  statičkom  rasutom 
organizacijom,  ova  opcija  treba  da  uključi  podmeni  koji  se  sastoji  od  stavki  navedenih 
u nastavku; 
2.  formiranje  datoteke  u  dva  prolaza  (sa  B  baketa  i  b  faktorom  baketiranja)  pri  čemu 
korisnik  zadaje  naziv  nove  datoteke,  a  datoteka  se  formira  u  odnosu  dve  ulazne 
datoteke:  datoteke  sa  evidentiranim  podacima  o  automobilima  i  datoteke  sa 
evidentiranim parkiranjima; 
3.  upis  novog  sloga  u  aktivnu  datoteku,  uz  proveru  postojanja  vrednosti  identifikatora  u 
datoteci, direktnim unosom podataka u realnom vremenu; 
4.  prikaz  dužine  boravka  automobila  u  beloj  zoni  u  aktivnoj  datoteci  za  zadati  registarski 
broj  zajedno  sa  adresom  baketa  i  rednim  brojem  sloga  u  baketu,  ukoliko  je  slog 
prekoračilac  prikazati  informaciju,  a  ukoliko  slog  nije  pronađen  potrebno  je  prikazati 
povratnu informaciju na standardnom izlazu; 
5.  prikaz  svih  automobila  proizvedenih  u  2024.  godini  sa  prihodom  od  njihovog 
parkiranja  u  svakoj  zoni  ako  je  sat  parkiranja  u  zonama  sledeći:  bela  150  dinara, 
crvena  120  dinara,  plava  80  dinara,  zajedno  sa  adresom  baketa  i  rednim  brojem 
sloga  u  baketu,  ukoliko  je  slog  prekoračilac  prikazati  informaciju,  a  ukoliko  je  skup 
rezultata prazan potrebno je prikazati povratnu informaciju na standardnom izlazu; 
6.  logičko brisanje aktuelnog sloga iz aktivne datoteke;*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "deo1.c"
#define B 9 
#define k 1
#define b 5

typedef struct {
    int registarska_oznaka;
    char marka[31];
    char model[31];
    int godina_proizvodnje;
    char boja[21];
    int duz_bela;
    int duz_crvena;
    int duz_plava;
} Parkirani_automobil;

typedef struct {
    Parkirani_automobil automobili[b];
    int index;
} Baket;

int index_baketa = 0;

Parkirani_automobil drugi_prolaz[50];

Parkirani_automobil default_parkirani_automobil = {0, "", "", 0, "", 0, 0, 0};

FILE *otvori_datoteku(const char *ime_datoteke, const char *mod) {
    FILE *datoteka = fopen(ime_datoteke, mod);
    if (datoteka == NULL) {
        perror("Otvaranje datoteke nije uspelo!");
        exit(EXIT_FAILURE);
    }
    return datoteka;
}

int hash_funkcija(int registarska_oznaka) {
    return (registarska_oznaka % B);
}

void kreiraj_praznu_evidenciju(FILE *datoteka) {
    Baket prazni_baket;
    for (int i = 0; i < b; i++) {
        prazni_baket.automobili[i] = default_parkirani_automobil;
    }
    for (int i = 0; i < B; i++) {
        prazni_baket.index = ++index_baketa;
        fwrite(&prazni_baket, sizeof(Baket), 1, datoteka);
    }
    fflush(datoteka);
}

Parkirani_automobil popuni_podatke_automobila(FILE *datoteka_parkiranja, Automobil automobil) {
    Parkirani_automobil parkirani_automobil;
    parkirani_automobil.registarska_oznaka = automobil.registarska_oznaka;
    strcpy(parkirani_automobil.marka, automobil.marka);
    strcpy(parkirani_automobil.model, automobil.model);
    parkirani_automobil.godina_proizvodnje = automobil.godina_proizvodnje;
    strcpy(parkirani_automobil.boja, automobil.boja);
    rewind(datoteka_parkiranja);
    Parkiranje parkiranje;
    while (fread(&parkiranje, sizeof(parkiranje), 1, datoteka_parkiranja)) {
        if (parkiranje.registarska_oznaka != automobil.registarska_oznaka) continue;

        if (strcmp(parkiranje.zona, "BELA") == 0) {
            parkirani_automobil.duz_bela += parkiranje.duzina_boravka;
        } else if (strcmp(parkiranje.zona, "CRVENA") == 0) {
            parkirani_automobil.duz_crvena += parkiranje.duzina_boravka;
        } else if (strcmp(parkiranje.zona, "PLAVA") == 0) {
            parkirani_automobil.duz_plava += parkiranje.duzina_boravka;
        } else {
            printf("Nepoznata zona: %s\n", parkiranje.zona);
        }
        
    }

    return parkirani_automobil;
}

void upis_svih_automobila(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    Baket trenutni_baket;
    Automobil trenutni_automobil;
    BLOK_automobila blok_automobila;
    rewind(datoteka_automobila);
    rewind(datoteka);

    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila) == 1) {
        for (int i = 0; i < f1; i++) {
            if (blok_automobila.automobili[i].registarska_oznaka == 0) continue;

            trenutni_automobil = blok_automobila.automobili[i];
            Parkirani_automobil parkirani_automobil = popuni_podatke_automobila(datoteka_parkiranja, trenutni_automobil);
            int indeks = hash_funkcija(parkirani_automobil.registarska_oznaka);

            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka);

            for (int j = 0; j < b; j++) {
                if (trenutni_baket.automobili[j].registarska_oznaka == 0) {
                    trenutni_baket.automobili[j] = parkirani_automobil;
                    trenutni_baket.index = ++index_baketa;
                    fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
                    fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                    break;
                }
            }
        }
    }
    
    
}
