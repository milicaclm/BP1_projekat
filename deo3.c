/*1.  prilikom  unosa  i  modifikacije  podataka  o  evidentiranim  automobilima  u  serijskoj 
datoteci potrebno je propagirati unos u rasutu datoteku; 
2.  prilikom  unosa  podataka  u  datoteku  o  parkiranjima  potrebno  je  za  dati  automobila 
izvršiti modifikaciju sloga u rasutoj datoteci tako da on oslikava aktuelno stanje 
a.  ukoliko  se  pojavi  situacija  da  je  jedan  automobil  pakirao  samo  u  beloj  zoni 
takve  događaje  evidentirati  u  posebnu  izlaznu  log  datoteke  da  bi  se  mogao 
pratiti boravak datih klijenata u beloj zoni; 
3.  omogućiti prikaz izveštaja o izlaznoj datoteci (log datoteka) na standardnom izlazu 
a.  omogućiti prikaz sadržaja čitave datoteke; 
b.  omogućiti prikaz registarske oznake i dužine boravka u beloj zoni; */

#include "deo2.c"
#define f 5


typedef struct {
    int identifikator;
    int registarska_oznaka;
    int duzina_boravka;
} Log;

typedef struct {
    Log logovi[f];
} Baket_log;

void propagiraj_unos_automobila(FILE *datoteka, Automobil novi_automobil)
{
    Baket trenutni_baket;
    Parkirani_automobil novi_parkirani_automobil = {
        .registarska_oznaka = novi_automobil.registarska_oznaka,
        .marka = novi_automobil.marka,
        .model = novi_automobil.model,
        .godina_proizvodnje = novi_automobil.godina_proizvodnje,
        .boja = novi_automobil.boja,
        .duz_bela = 0,
        .duz_crvena = 0,
        .duz_plava = 0,
        .status = POPUNJEN
    };
    int indeks = hash_funkcija(novi_parkirani_automobil.registarska_oznaka);
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
                trenutni_baket.automobili[j] = novi_parkirani_automobil;
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

void propagiraj_modifikaciju_automobila(FILE *datoteka, Automobil automobil)
{
    Baket trenutni_baket;
    int indeks = hash_funkcija(automobil.registarska_oznaka);
    for (int i = 0; i < B; i++)
    {
        indeks = (hash_funkcija(automobil.registarska_oznaka) + i * k) % B;
        if(i > 0 && indeks == hash_funkcija(automobil.registarska_oznaka)){
            break;
        }
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int j = 0; j < b; j++)
        {
            if (trenutni_baket.automobili[j].status != POPUNJEN)
            {
                trenutni_baket.automobili[j] = (Parkirani_automobil){
                    .registarska_oznaka = automobil.registarska_oznaka,
                    .marka = automobil.marka,
                    .model = automobil.model,
                    .godina_proizvodnje = automobil.godina_proizvodnje,
                    .boja = automobil.boja,
                    .duz_bela = trenutni_baket.automobili[j].duz_bela,
                    .duz_crvena = trenutni_baket.automobili[j].duz_crvena,
                    .duz_plava = trenutni_baket.automobili[j].duz_plava,
                    .status = POPUNJEN
                };
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                
                printf("Novi slog uspešno upisan.\n");
                fflush(datoteka);
                return;
            }
        }
    }
    printf("Automobil nije pronađen.\n");
}

void propagiraj_unos_parkiranja(FILE *datoteka, Parkiranje parkiranje)
{
    Baket trenutni_baket;
    int indeks = hash_funkcija(parkiranje.registarska_oznaka);
    for (int i = 0; i < B; i++)
    {
        indeks = (hash_funkcija(parkiranje.registarska_oznaka) + i * k) % B;
        if(i > 0 && indeks == hash_funkcija(parkiranje.registarska_oznaka)){
            break;
        }
        fseek(datoteka, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka);
        for (int j = 0; j < b; j++)
        {
            if (trenutni_baket.automobili[j].registarska_oznaka == parkiranje.registarska_oznaka)
            {
                switch (parkiranje.zona[0]) {
                    case 'B':
                        trenutni_baket.automobili[j].duz_bela += parkiranje.duzina_boravka;
                        break;
                    case 'C':
                        trenutni_baket.automobili[j].duz_crvena += parkiranje.duzina_boravka;
                        break;
                    case 'P':
                        trenutni_baket.automobili[j].duz_plava += parkiranje.duzina_boravka;
                        break;
                    default:
                        printf("Nepoznata zona: %s\n", parkiranje.zona);
                        exit(EXIT_FAILURE);
                }
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite(&trenutni_baket, sizeof(Baket), 1, datoteka);
                
                printf("Novi slog uspesno upisan.\n");
                fflush(datoteka);
                return;
            }
        }
    }
    printf("Datoteka je puna, ne moze se uneti novi slog.\n");
}