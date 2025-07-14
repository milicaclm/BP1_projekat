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
        prazni_baket.automobili[i] = default_automobil;
    }
    for (int i = 0; i < B; i++) {
        prazni_baket.index = ++index_baketa;
        fwrite(&prazni_baket, sizeof(Baket), 1, datoteka);
    }
    fflush(datoteka);
}

Parkirani_automobil popuni_podatke_automobila(FILE *datoteka_parkiranja, automobil automobil) {
    Parkirani_automobil parkirani_automobil;
    /*na osnovu registarske oznake pronadji i popuni podatke u Parkirani_automobil koristeći datoteka_automobila*/
    FILE *datoteka_automobila = otvori_datoteku("datoteka_automobila.txt", "r");
    return parkirani_automobil;
}

void upis_automobila(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    Baket trenutni_baket;
    //procitaj automobil iz datoteka_automobila i za svaki automobil pronadji ostale podatke tako da popunis Parkirani_automobil
}
