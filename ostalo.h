#ifndef OSTALO_H
#define OSTALO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define f1 4
#define f2 5
#define B 9
#define k 1
#define b 5
#define f 5

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

extern Automobil default_automobil;
extern Parkiranje default_parkiranje;
extern FILE *datoteka_automobila;
extern FILE *datoteka_parkiranja;
extern FILE *datoteka_parkiranja_automobila;
extern FILE *datoteka_log;

typedef enum
{
    PRAZAN,
    POPUNJEN,
    OBRISAN
} Status;

typedef struct
{
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

typedef struct
{
    Parkirani_automobil automobili[b];
    int index;
} Baket;


extern Parkirani_automobil *prekoracioci;
extern Parkirani_automobil *visak;
extern int index_prekoracioca;
extern int index_viska;

extern Parkirani_automobil default_parkirani_automobil;

typedef struct {
    int identifikator;
    int registarska_oznaka;
    int duzina_boravka;
} Log;

typedef struct {
    Log logovi[f];
} Blok_log;

extern Log default_log;


#endif // OSTALO_H