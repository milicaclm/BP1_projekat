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
} Blok_log;

Log default_log = {0, 0, 0};
int log_identifikator = 0;

void dodaj_blok_log(FILE *datoteka_log, Log log)
{
    Blok_log blok_log;
    blok_log.logovi[0] = log;
    for (int i = 1; i < f; i++) {
        blok_log.logovi[i].identifikator = 0;
        blok_log.logovi[i].registarska_oznaka = 0;
        blok_log.logovi[i].duzina_boravka = 0;
    }

    fseek(datoteka_log, 0, SEEK_END);
    fwrite_safe(&blok_log, sizeof(blok_log), 1, datoteka_log);
}

void propagiraj_unos_automobila(FILE *datoteka, Automobil novi_automobil)
{
    Baket trenutni_baket;
    Parkirani_automobil novi_parkirani_automobil;
    novi_parkirani_automobil.registarska_oznaka = novi_automobil.registarska_oznaka;
    strcpy(novi_parkirani_automobil.marka, novi_automobil.marka);
    strcpy(novi_parkirani_automobil.model, novi_automobil.model);
    novi_parkirani_automobil.godina_proizvodnje = novi_automobil.godina_proizvodnje;
    strcpy(novi_parkirani_automobil.boja, novi_automobil.boja);
    novi_parkirani_automobil.duz_bela = 0;
    novi_parkirani_automobil.duz_crvena = 0;
    novi_parkirani_automobil.duz_plava = 0;
    novi_parkirani_automobil.status = POPUNJEN;
    rewind(datoteka);
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
                fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka);
                
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
                trenutni_baket.automobili[j].registarska_oznaka = automobil.registarska_oznaka;
                strcpy(trenutni_baket.automobili[j].marka, automobil.marka);
                strcpy(trenutni_baket.automobili[j].model, automobil.model);
                trenutni_baket.automobili[j].godina_proizvodnje = automobil.godina_proizvodnje;
                strcpy(trenutni_baket.automobili[j].boja, automobil.boja);
                trenutni_baket.automobili[j].status = POPUNJEN;
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka);
                printf("Novi slog uspešno upisan.\n");
                fflush(datoteka);
                return;
            }
        }
    }
    printf("Automobil nije pronađen.\n");
}
void loguj_parkiranje(FILE *datoteka_log, int registarska_oznaka, int duzina_boravka) {

    Log novi_log = {
        .identifikator = log_identifikator++,
        .registarska_oznaka = registarska_oznaka,
        .duzina_boravka = duzina_boravka
    };
    Blok_log blok_log;
    rewind(datoteka_log);
    while (fread(&blok_log, sizeof(Blok_log), 1, datoteka_log)) {
        for (int i = 0; i < f; i++) {
            if (blok_log.logovi[i].identifikator == 0) {
                blok_log.logovi[i] = novi_log;
                fseek(datoteka_log, -(long)sizeof(Blok_log), SEEK_CUR);
                fwrite_safe(&blok_log, sizeof(Blok_log), 1, datoteka_log);
                log_identifikator++;
                return;
            }
        }
    }
    dodaj_blok_log(datoteka_log, novi_log);
    printf("Log uspešno upisan.\n");
    fflush(datoteka_log);
}

void propagiraj_unos_parkiranja(FILE *datoteka, FILE *datoteka_log, Parkiranje parkiranje)
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
                        fcloseall();
                        exit(EXIT_FAILURE);
                }
                fseek(datoteka, -(long)sizeof(Baket), SEEK_CUR);
                fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka);
                if(trenutni_baket.automobili[j].duz_bela > 0 && trenutni_baket.automobili[j].duz_crvena == 0 && trenutni_baket.automobili[j].duz_plava == 0) {
                    loguj_parkiranje(datoteka_log, trenutni_baket.automobili[j].registarska_oznaka, parkiranje.duzina_boravka);
                }
                printf("Novi slog uspesno upisan.\n");
                fflush(datoteka);
                return;
            }
        }
    }
    printf("Datoteka je puna, ne moze se uneti novi slog.\n");
}


void kreiraj_datoteku_log(const char *ime_datoteke_log) {
    FILE *datoteka_log = fopen(ime_datoteke_log, "wb+");
    if (datoteka_log == NULL) {
        perror("Otvaranje datoteke log nije uspelo!");
        fcloseall();
         (EXIT_FAILURE);
    }
    dodaj_blok_log(datoteka_log, default_log);
    fflush(datoteka_log);
}



void ispisi_log_datoteku(FILE *datoteka_log) {
    Blok_log blok_log;
    rewind(datoteka_log);
    printf("Identifikator |Registarska oznaka |Duzina boravka\n");
    printf("-------------------------------------------------\n");
    while (fread(&blok_log, sizeof(Blok_log), 1, datoteka_log)) {
        for (int i = 0; i < f; i++) {
            if (blok_log.logovi[i].identifikator != 0) {
                printf("%-15d | %-18d | %-15d\n",
                       blok_log.logovi[i].identifikator,
                       blok_log.logovi[i].registarska_oznaka,
                       blok_log.logovi[i].duzina_boravka);

                printf("-------------------------------------------------\n");
            }
        }
    }
}

void ispisi_log_datoteku_oznaka_duzina(FILE *datoteka_log) {
    Blok_log blok_log;
    rewind(datoteka_log);
    printf("Registarska oznaka | Duzina boravka\n");
    printf("---------------------------------------\n");
    while (fread(&blok_log, sizeof(Blok_log), 1, datoteka_log)) {
        for (int i = 0; i < f; i++) {
            if (blok_log.logovi[i].identifikator != 0) {
                printf("%-18d | %-15d\n",
                       blok_log.logovi[i].registarska_oznaka,
                       blok_log.logovi[i].duzina_boravka);

                printf("---------------------------------------\n");
            }
        }
    }
}