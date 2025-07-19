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

typedef enum {
    PRAZAN,
    POPUNJEN,
    OBRISAN
} Status;

typedef struct {
    int registarska_oznaka;
    char marka[31];
    char model[31];
    int godina_proizvodnje;
    char boja[21];
    int duz_bela;
    int duz_crvena;
    int duz_plava;
    Status status;
} Parkirani_automobil;

typedef struct {
    Parkirani_automobil automobili[b];
    int index;
} Baket;

int index_baketa = 0;

Parkirani_automobil *prekoracioci = NULL;
Parkirani_automobil *visak = NULL;
int index_prekoracioca = 0;
int index_viska = 0;

Parkirani_automobil default_parkirani_automobil = {0, "", "", 0, "", 0, 0, 0, PRAZAN};

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
        prazni_baket.index = index_baketa++;
        fwrite(&prazni_baket, sizeof(Baket), 1, datoteka);
    }
    fflush(datoteka);
}

Parkirani_automobil vrati_podatke_parkiranog_automobila(FILE *datoteka_parkiranja, Automobil automobil) {
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

Parkirani_automobil *prvi_prolaz(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    Automobil trenutni_automobil;
    Baket trenutni_baket;
    BLOK_automobila blok_automobila;
    prekoracioci = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    if (prekoracioci == NULL) {
        perror("Alokacija memorije nije uspela");
        exit(EXIT_FAILURE);
    }
    rewind(datoteka);
    rewind(datoteka_automobila);
    rewind(datoteka_parkiranja);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila)) {
        for (int i = 0; i < f1; i++) {
            if (!blok_automobila.automobili[i].registarska_oznaka) continue;

            trenutni_automobil = blok_automobila.automobili[i];
            Parkirani_automobil parkirani_automobil = vrati_podatke_parkiranog_automobila(datoteka_parkiranja, trenutni_automobil);
            int indeks = hash_funkcija(parkirani_automobil.registarska_oznaka);

            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
            bool prekoracilac = false;

            for (int j = 0; j < b; j++) {
                if (trenutni_baket.automobili[j].status != POPUNJEN) {
                    trenutni_baket.automobili[j] = parkirani_automobil;
                    fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
                    fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                    prekoracilac = true;
                    break;
                }
            }

            if (index_prekoracioca >= 20) {
                prekoracioci = (Parkirani_automobil *)realloc(prekoracioci, (index_prekoracioca + 20) * sizeof(Parkirani_automobil));
                if (prekoracioci == NULL) {
                    perror("Reallocacija memorije nije uspela");
                    exit(EXIT_FAILURE);
                }
            }
            if (!prekoracilac) {
                prekoracioci[index_prekoracioca++] = parkirani_automobil;
            }
        }
    }
}

void drugi_prolaz(FILE *datoteka) {
    if (index_prekoracioca == 0) {
        printf("Nema prekoračilaca.\n");
        return;
    }
    rewind(datoteka);
    Baket trenutni_baket;
    visak = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    if (visak == NULL) {
        perror("Alokacija memorije nije uspela");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < index_prekoracioca; i++) {
        Parkirani_automobil parkirani_automobil = prekoracioci[i];
        for(int j = 1; j <= B; j++) {
            int indeks = (hash_funkcija(parkirani_automobil.registarska_oznaka)+k*j)%B;
            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
            int z=0;
            for (; z < b; z++) {
                if (trenutni_baket.automobili[z].registarska_oznaka != POPUNJEN) {
                    trenutni_baket.automobili[z] = parkirani_automobil;
                    fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                    fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                    break;
                }
            }
            if (trenutni_baket.automobili[z].status != POPUNJEN) break;
            if (index_viska >= 20) {
                visak = (Parkirani_automobil *)realloc(visak, (index_viska + 20) * sizeof(Parkirani_automobil));
                if (visak == NULL) {
                    perror("Reallocacija memorije nije uspela");
                    exit(EXIT_FAILURE);
                }
            }
            if(indeks == hash_funkcija(parkirani_automobil.registarska_oznaka)) {
                visak[index_viska++] = parkirani_automobil;
            }

           
        }
    }

    if (index_viska > 0) {
        printf("Prekoračili ste kapacitet baketa za sledeće automobile:\n");
        for (int i = 0; i < index_viska; i++) {
            Parkirani_automobil pa = visak[i];
            printf("Registarska oznaka: %d, Marka: %s, Model: %s, Godina proizvodnje: %d, Boja: %s\n",
                   pa.registarska_oznaka, pa.marka, pa.model, pa.godina_proizvodnje, pa.boja);
        }
    }
}

void upis_svih_automobila(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja) {
    prvi_prolaz(datoteka, datoteka_automobila, datoteka_parkiranja);
    drugi_prolaz(datoteka);
    fflush(datoteka);
    free(prekoracioci);
    free(visak);
}

bool registarska_oznaka_postoji(FILE *datoteka, int registarska_oznaka) {
    Baket trenutni_baket;
    int indeks = hash_funkcija(registarska_oznaka);
    fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
    fread(&trenutni_baket, sizeof(Baket), 1, datoteka);

    for (int i = 0; i < b; i++) {
        if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka) {
            return true;
        }
    }

    for (int j = 1; j <= k; j++) {
        indeks = (indeks + j*k) % B;
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka) {
                return true;
            }
        }
    }

    return false;
}

void upis_novog_sloga(FILE *datoteka) {
    Parkirani_automobil novi_automobil;
    do {
        printf("Unesite registarsku oznaku: ");
        scanf("%d", &novi_automobil.registarska_oznaka);
        while(getchar() != '\n'); // Čišćenje ulaznog bafera
        if (novi_automobil.registarska_oznaka <= 0) {
            printf("Registarska oznaka mora biti veća od 0. Pokušajte ponovo.\n");
            continue;
        }
    }while (novi_automobil.registarska_oznaka <= 0 || registarska_oznaka_postoji(datoteka, novi_automobil.registarska_oznaka));

    printf("Unesite marku automobila: ");
    fgets(novi_automobil.marka, sizeof(novi_automobil.marka), stdin);
    novi_automobil.marka[strcspn(novi_automobil.marka, "\n")] = 0;
    printf("Unesite model automobila: ");
    fgets(novi_automobil.model, sizeof(novi_automobil.model), stdin);
    novi_automobil.model[strcspn(novi_automobil.model, "\n")] = 0;
    printf("Unesite godinu proizvodnje: ");
    scanf("%d", &novi_automobil.godina_proizvodnje);
    while(getchar() != '\n'); // Čišćenje ulaznog bafera
    printf("Unesite boju automobila: ");
    fgets(novi_automobil.boja, sizeof(novi_automobil.boja), stdin);
    novi_automobil.boja[strcspn(novi_automobil.boja, "\n")] = 0;

    // TODO: zavrsi zadatak
    novi_automobil.status = POPUNJEN;
    novi_automobil.duz_bela = 0;
    novi_automobil.duz_crvena = 0;
    novi_automobil.duz_plava = 0;
    
    int indeks = hash_funkcija(novi_automobil.registarska_oznaka);
    Baket trenutni_baket;
    fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
    fread(&trenutni_baket, sizeof(Baket), 1, datoteka);

    for (int i = 0; i < b; i++) {
        if (trenutni_baket.automobili[i].status != POPUNJEN) {
            trenutni_baket.automobili[i] = novi_automobil;
            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
            printf("Novi slog uspešno upisan.\n");
            return;
        }
    }
    printf("Baket je pun, ne može se uneti novi slog.\n");
}


