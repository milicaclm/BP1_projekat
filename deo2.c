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


Parkirani_automobil *prekoracioci = NULL;
Parkirani_automobil *visak = NULL;
int index_prekoracioca = 0;
int index_viska = 0;

Parkirani_automobil default_parkirani_automobil = {0, "", "", 0, "", 0, 0, 0, PRAZAN};

int hash_funkcija(int registarska_oznaka)
{
    return (registarska_oznaka % B);
}

void kreiraj_praznu_evidenciju(FILE *datoteka)
{
    Baket prazni_baket;
    for (int i = 0; i < b; i++)
    {
        prazni_baket.automobili[i] = default_parkirani_automobil;
    }
    for (int i = 0; i < B; i++)
    {
        fwrite(&prazni_baket, sizeof(Baket), 1, datoteka);
    }
    fflush(datoteka);
}

Parkirani_automobil vrati_podatke_parkiranog_automobila(FILE *datoteka_parkiranja, Automobil automobil)
{
    Parkirani_automobil parkirani_automobil;
    parkirani_automobil.registarska_oznaka = automobil.registarska_oznaka;
    strcpy(parkirani_automobil.marka, automobil.marka);
    strcpy(parkirani_automobil.model, automobil.model);
    parkirani_automobil.godina_proizvodnje = automobil.godina_proizvodnje;
    strcpy(parkirani_automobil.boja, automobil.boja);
    rewind(datoteka_parkiranja);
    Parkiranje parkiranje;
    parkirani_automobil.duz_bela = 0;
    parkirani_automobil.duz_crvena = 0;
    parkirani_automobil.duz_plava = 0;
    while (fread(&parkiranje, sizeof(parkiranje), 1, datoteka_parkiranja))
    {
        if (parkiranje.registarska_oznaka != automobil.registarska_oznaka)
            continue;

        if (strcmp(parkiranje.zona, "BELA") == 0)
        {
            parkirani_automobil.duz_bela += parkiranje.duzina_boravka;
        }
        else if (strcmp(parkiranje.zona, "CRVENA") == 0)
        {
            parkirani_automobil.duz_crvena += parkiranje.duzina_boravka;
        }
        else if (strcmp(parkiranje.zona, "PLAVA") == 0)
        {
            parkirani_automobil.duz_plava += parkiranje.duzina_boravka;
        }
        else
        {
            printf("Nepoznata zona: %s\n", parkiranje.zona);
        }
    }

    return parkirani_automobil;
}

Parkirani_automobil *prvi_prolaz(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja)
{
    Automobil trenutni_automobil;
    Baket trenutni_baket;
    BLOK_automobila blok_automobila;
    prekoracioci = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    index_prekoracioca = 0;
    if (prekoracioci == NULL)
    {
        perror("Alokacija memorije nije uspela");
        exit(EXIT_FAILURE);
    }
    rewind(datoteka);
    rewind(datoteka_automobila);
    rewind(datoteka_parkiranja);
    while (fread(&blok_automobila, sizeof(BLOK_automobila), 1, datoteka_automobila))
    {
        for (int i = 0; i < f1; i++)
        {
            if (!blok_automobila.automobili[i].registarska_oznaka)
                continue;

            trenutni_automobil = blok_automobila.automobili[i];
            Parkirani_automobil parkirani_automobil = vrati_podatke_parkiranog_automobila(datoteka_parkiranja, trenutni_automobil);
            int indeks = hash_funkcija(parkirani_automobil.registarska_oznaka);

            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
            bool prekoracilac = true;

            for (int j = 0; j < b; j++)
            {
                if (trenutni_baket.automobili[j].status != POPUNJEN)
                {
                    trenutni_baket.automobili[j] = parkirani_automobil;
                    fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
                    fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                    prekoracilac = false;
                    break;
                }
            }

            if (index_prekoracioca > 0 && index_prekoracioca % 20 == 0)
            {
                prekoracioci = (Parkirani_automobil *)realloc(prekoracioci, (index_prekoracioca + 20) * sizeof(Parkirani_automobil));
                if (prekoracioci == NULL)
                {
                    perror("Realokacija memorije nije uspela");
                    exit(EXIT_FAILURE);
                }
            }
            if (prekoracilac)
            {
                prekoracioci[index_prekoracioca++] = parkirani_automobil;
            }
        }
    }
    return prekoracioci;
}

void drugi_prolaz(FILE *datoteka)
{
    index_viska = 0;
    if (index_prekoracioca == 0)
    {
        printf("Nema prekoracilaca.\n");
        return;
    }
    rewind(datoteka);
    Baket trenutni_baket;
    visak = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    if (visak == NULL)
    {
        perror("Alokacija memorije nije uspela");
        exit(EXIT_FAILURE);
    }
    int indeks;

    for (int i = 0; i < index_prekoracioca; i++)
    {
        Parkirani_automobil parkirani_automobil = prekoracioci[i];
        bool je_visak = true;
        for (int j = 1; j < B; j++)
        {
            indeks = (hash_funkcija(parkirani_automobil.registarska_oznaka) + k * j) % B;
            if(indeks == hash_funkcija(parkirani_automobil.registarska_oznaka)) {
                break;
            }
            fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
            int z = 0;
            for (; z < b; z++)
            {
                if (trenutni_baket.automobili[z].status != POPUNJEN)
                {
                    trenutni_baket.automobili[z] = parkirani_automobil;
                    fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                    fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                    je_visak = false;
                    break;
                }
            }
            if (!je_visak) break;
        }
        if (je_visak) {
            if (index_viska > 0 && index_viska % 20 == 0)
            {
                visak = (Parkirani_automobil *)realloc(visak, (index_viska + 20) * sizeof(Parkirani_automobil));
                if (visak == NULL)
                {
                    perror("Realokacija memorije nije uspela");
                    exit(EXIT_FAILURE);
                }
            }
            visak[index_viska++] = parkirani_automobil;
        }
    }
    
    if (index_viska > 0)
    {
        printf("Prekoracili ste kapacitet baketa za sledece automobile:\n");
        for (int i = 0; i < index_viska; i++)
        {
            Parkirani_automobil pa = visak[i];
            printf("Registarska oznaka: %d, Marka: %s, Model: %s, Godina proizvodnje: %d, Boja: %s\n",
                   pa.registarska_oznaka, pa.marka, pa.model, pa.godina_proizvodnje, pa.boja);
        }

        fflush(datoteka);
        exit(EXIT_FAILURE);
    }
}

void upis_svih_automobila(FILE *datoteka, FILE *datoteka_automobila, FILE *datoteka_parkiranja)
{
    prvi_prolaz(datoteka, datoteka_automobila, datoteka_parkiranja);
    drugi_prolaz(datoteka);
    fflush(datoteka);
    free(prekoracioci);
    free(visak);
}

bool registarska_oznaka_postoji_rasuta(FILE *datoteka, int registarska_oznaka)
{
    Baket trenutni_baket;
    int indeks = hash_funkcija(registarska_oznaka);

    for (int j = 0; j < B; j++)
    {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int i = 0; i < b; i++)
        {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka)
            {
                return true;
            }
        }
    }

    return false;
}

void upis_novog_sloga(FILE *datoteka)
{
    Parkirani_automobil novi_automobil;
    do
    {
        printf("Unesite registarsku oznaku: ");
        scanf("%d", &novi_automobil.registarska_oznaka);
        while (getchar() != '\n');
        if (novi_automobil.registarska_oznaka <= 0)
        {
            printf("Registarska oznaka mora biti veca od 0. Pokusajte ponovo.\n");
            continue;
        }
    } while (novi_automobil.registarska_oznaka <= 0 || registarska_oznaka_postoji_rasuta(datoteka, novi_automobil.registarska_oznaka));

    printf("Unesite marku automobila: ");
    scanf("%30s", novi_automobil.marka);
    while (getchar() != '\n'); 
    printf("Unesite model automobila: ");
    scanf("%30s", novi_automobil.model);
    while (getchar() != '\n'); 
    do
    {
        printf("Unesite godinu proizvodnje: ");
        scanf("%d", &novi_automobil.godina_proizvodnje);
        while (getchar() != '\n'); 
    } while (novi_automobil.godina_proizvodnje <= 1000 || novi_automobil.godina_proizvodnje > 2100);
    printf("Unesite boju automobila: ");
    scanf("%20s", novi_automobil.boja);
    while (getchar() != '\n'); 


    do {
        printf("Unesite duzinu boravka u beloj zoni (u satima): ");
        scanf("%d", &novi_automobil.duz_bela);
        while (getchar() != '\n');
        if (novi_automobil.duz_bela < 0) {
            printf("Duzina boravka ne moze biti negativna. Pokusajte ponovo.\n");
        }
    } while (novi_automobil.duz_bela < 0);

    do {
        printf("Unesite duzinu boravka u crvenoj zoni (u satima): ");
        scanf("%d", &novi_automobil.duz_crvena);
        while (getchar() != '\n');
        if (novi_automobil.duz_crvena < 0) {
            printf("Duzina boravka ne moze biti negativna. Pokusajte ponovo.\n");
        }
    } while (novi_automobil.duz_crvena < 0);

    do {
        printf("Unesite duzinu boravka u plavoj zoni (u satima): ");
        scanf("%d", &novi_automobil.duz_plava);
        while (getchar() != '\n');
        if (novi_automobil.duz_plava < 0) {
            printf("Duzina boravka ne moze biti negativna. Pokusajte ponovo.\n");
        }
    } while (novi_automobil.duz_plava < 0);
    novi_automobil.status = POPUNJEN;

    int indeks = hash_funkcija(novi_automobil.registarska_oznaka);
    Baket trenutni_baket;

    for (int i = 0; i < B; i++)
    {
        indeks = (hash_funkcija(novi_automobil.registarska_oznaka) + i * k) % B;
        if(i > 0 && indeks == hash_funkcija(novi_automobil.registarska_oznaka)){
            break;
        }
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int j = 0; j < b; j++)
        {
            if (trenutni_baket.automobili[j].status != POPUNJEN)
            {
                trenutni_baket.automobili[j] = novi_automobil;
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                
                printf("Novi slog uspešno upisan.\n");
                fflush(datoteka);
                return;
            }
        }
    }
    printf("Datoteka je puna, ne može se uneti novi slog.\n");
}

void prikaz_duzine_bela_zona(FILE *datoteka, int registarska_oznaka) {
    Baket trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++) {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka) {
                printf("Duzina boravka u beloj zoni: %d\n", trenutni_baket.automobili[i].duz_bela);
                printf("Adresa baketa: %p\n", &trenutni_baket);
                printf("Redni broj sloga u baketu: %d\n", i);
                if (j > 0) {
                    printf("Slog prekoracilac!\n");
                }
                return;
            }
        }
    }

    printf("Automobil sa registarskom oznakom %d nije pronadjen.\n", registarska_oznaka);
}

void prikaz_automobila_2024(FILE *datoteka) {
    Baket trenutni_baket;
    bool pronadjen = false;

    printf("Automobili proizvedeni u 2024. godini:\n");
    printf("%-20s | %-10s | %-10s | %-6s | %-10s | %-8s | %-15s | %-20s | %-12s\n",
           "Registarska oznaka", "Marka", "Model", "Godina", "Boja", "Prihod", "Adresa baketa", "Redni broj sloga", "Prekoracilac");
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

    int prihod;
    for (int j = 0; j < B; j++) {
        fseek(datoteka, j * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].godina_proizvodnje == 2024) {
                prihod = trenutni_baket.automobili[i].duz_bela * 150 + trenutni_baket.automobili[i].duz_crvena * 120 + trenutni_baket.automobili[i].duz_plava * 80;
                pronadjen = true;
                printf("%-20d | %-10s | %-10s | %-6d | %-10s | %-8d | %-15p | %-20d | %-12s\n",
                       trenutni_baket.automobili[i].registarska_oznaka,
                       trenutni_baket.automobili[i].marka,
                       trenutni_baket.automobili[i].model,
                       trenutni_baket.automobili[i].godina_proizvodnje,
                       trenutni_baket.automobili[i].boja,
                       prihod,
                       &trenutni_baket,
                       i,
                       (j > 0) ? "DA" : "NE");
                printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
            }
        }
    }

    if (!pronadjen) {
        printf("Nema automobila proizvedenih u 2024. godini.\n");
    }
}

void logicko_brisanje(FILE *datoteka, int registarska_oznaka) {
    Baket trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++) {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka) {
                trenutni_baket.automobili[i].status = OBRISAN;
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                printf("Slog sa registarskom oznakom %d je uspesno obrisan.\n", registarska_oznaka);
                return;
            }
        }
    }

    printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
}

void meni_deo2(FILE *datoteka_automobila, FILE *datoteka_parkiranja, FILE *datoteka) {
    char opcija;
    while (1) {
        printf("Izaberite opciju:\n");
        printf("1. Upis novih automobila\n");
        printf("2. Upis novog sloga\n");
        printf("3. Prikaz duzine boravka u beloj zoni\n");
        printf("4. Prikaz automobila proizvedenih u 2024. godini\n");
        printf("5. Logicko brisanje sloga\n");
        printf("0. Nazad\n");
        printf("Unesite opciju: ");
        scanf(" %c", &opcija);
        while (getchar() != '\n');
        switch (opcija) {
            case '1':
                char ime_datoteke[31];
                printf("Unesite naziv datoteke: ");
                while (fgets(ime_datoteke, sizeof(ime_datoteke), stdin) == NULL || ime_datoteke[0] == '\n');
                ime_datoteke[strcspn(ime_datoteke, "\n")] = '\0'; // Ukloni novi red
                strcat(ime_datoteke, ".dat");
                datoteka = fopen(ime_datoteke, "wb+");
                upis_svih_automobila(datoteka, datoteka_automobila, datoteka_parkiranja);
                break;
            case '2':
                upis_novog_sloga(datoteka);
                break;
            case '3': {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                prikaz_duzine_bela_zona(datoteka, registarska_oznaka);
                break;
            }
            case '4':
                prikaz_automobila_2024(datoteka);
                break;
            case '5': {
                int registarska_oznaka;
                printf("Unesite registarsku oznaku: ");
                scanf("%d", &registarska_oznaka);
                while (getchar() != '\n');
                logicko_brisanje(datoteka, registarska_oznaka);
                break;
            }
            case '0':
                return;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }
}


