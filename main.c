#include <stdio.h>
#include <stdlib.h>
#define f1 4
#define f2 5

typedef struct {
        int registarska_oznaka;
        char marka[31];
        char model[31];
        int godina_proizvodnje[5];
        char boja[21];
    } automobil;

typedef struct {
        int identifikator;
        int registarska_oznaka;
        char datum_vremena[17];
        int duzina_boravka;
        char zona[7]

    } parkiranje;

typedef struct {
        automobil automobili[f1];
        int indeks;
    } BLOK_automobila;

typedef struct {
        parkiranje parkinga[f2];
        int indeks;
    } BLOK_parkinga;



FILE *otvori_datoteku(const char *ime_datoteke, const char *mod);
FILE *kreiraj_prazne_evidencije(const char *ime_datoteke_automobila);

int main()
{
    
    int opcija;
    printf("Unesite opciju: ");




    switch (opcija)
    {
        //TODO: Implement the options for the menu
    case 0:
        break;
    
    default:
        break;
    }



    return 0;
}

FILE *otvori_datoteku(const char *ime_datoteke, const char *mod)
{
    FILE *datoteka = fopen(ime_datoteke, mod);
    if (datoteka == NULL) {
        perror("Ne mogu otvoriti datoteku");
        exit(EXIT_FAILURE);
    }
    return datoteka;
}

FILE *kreiraj_praznu_evidenciju(const char *ime_datoteke_automobila){
    FILE *datoteka_automobila = otvori_datoteku(ime_datoteke_automobila, "wb");

    automobil automobil = {0, "", "", {0}, ""};
    BLOK_automobila blok_automobila;
    for (int i = 0; i < f1; i++) {
        blok_automobila.automobili[i] = automobil;
    }
    

    fwrite(&blok_automobila, sizeof(blok_automobila), 1, datoteka_automobila);

    return datoteka_automobila;
    
}
