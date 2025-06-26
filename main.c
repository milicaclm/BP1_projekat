#include <stdio.h>
#include <stdlib.h>


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


FILE *otvori_datoteku(const char *ime_datoteke, const char *mod);
FILE *kreiraj_prazne_evidencije(const char *ime_datoteke_automobila, const char *ime_datoteke_parkinga, const int f1, const int f2);



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

//f1 i f2 su faktori blokiranja, koji se koriste za određivanje veličine blokova u datotekama
//pored kreiranja trbe ada inicijalizuje i prvi prazan blok
FILE *kreiraj_prazne_evidencije(const char *ime_datoteke_automobila, const char *ime_datoteke_parkinga, const int f1, const int f2){
    FILE *datoteka_automobila = otvori_datoteku(ime_datoteke_automobila, "wb");
    FILE *datoteka_parkinga = otvori_datoteku(ime_datoteke_parkinga, "wb");

    // Dodavanje inicijalnog bloka
    automobil prazni_automobili[4] = {0};
    parkiranje prazni_parking[4] = {0};

    fwrite(prazni_automobili, sizeof(automobil), 4, datoteka_automobila);
    fwrite(prazni_parking, sizeof(parkiranje), 4, datoteka_parkinga);

    
    return datoteka_automobila;
    
}
