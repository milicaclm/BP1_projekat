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

#include "funkcije.h"


int hash_funkcija(int registarska_oznaka)
{
    return (registarska_oznaka % B);
}

void kreiraj_praznu_evidenciju()
{
    Baket prazni_baket;
    for (int i = 0; i < b; i++)
    {
        prazni_baket.automobili[i] = default_parkirani_automobil;
    }
    for (int i = 0; i < B; i++)
    {
        fwrite_safe(&prazni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
    }
    fflush(datoteka_parkiranja_automobila);
}

Parkirani_automobil vrati_podatke_parkiranog_automobila(FILE *datoteka_parkiranja, Automobil automobil)
{
    static int call_count = 0;
    call_count++;
    printf("vrati_podatke_parkiranog_automobila pozvan %d put za automobil %d\n", call_count, automobil.registarska_oznaka);
    
    Parkirani_automobil parkirani_automobil;
    parkirani_automobil.registarska_oznaka = automobil.registarska_oznaka;
    strcpy(parkirani_automobil.marka, automobil.marka);
    strcpy(parkirani_automobil.model, automobil.model);
    parkirani_automobil.godina_proizvodnje = automobil.godina_proizvodnje;
    strcpy(parkirani_automobil.boja, automobil.boja);
    rewind(datoteka_parkiranja);
    
    Blok_parkiranja blok_parkiranja;
    parkirani_automobil.duz_bela = 0;
    parkirani_automobil.duz_crvena = 0;
    parkirani_automobil.duz_plava = 0;
    parkirani_automobil.status = POPUNJEN;
    
    while (fread(&blok_parkiranja, sizeof(Blok_parkiranja), 1, datoteka_parkiranja) == 1)
    {
        for (int i = 0; i < f2; i++) {
            if (!blok_parkiranja.parkinga[i].registarska_oznaka) {
                break; // No more parking records in this block
            }
            
            if (blok_parkiranja.parkinga[i].registarska_oznaka != automobil.registarska_oznaka)
                continue;

            if (strcmp(blok_parkiranja.parkinga[i].zona, "BELA") == 0)
            {
                parkirani_automobil.duz_bela += blok_parkiranja.parkinga[i].duzina_boravka;
            }
            else if (strcmp(blok_parkiranja.parkinga[i].zona, "CRVENA") == 0)
            {
                parkirani_automobil.duz_crvena += blok_parkiranja.parkinga[i].duzina_boravka;
            }
            else if (strcmp(blok_parkiranja.parkinga[i].zona, "PLAVA") == 0)
            {
                parkirani_automobil.duz_plava += blok_parkiranja.parkinga[i].duzina_boravka;
            }
            else
            {
                printf("Nepoznata zona: %s\n", blok_parkiranja.parkinga[i].zona);
            }
        }
    }

    return parkirani_automobil;
}

Parkirani_automobil *prvi_prolaz()
{
    Automobil trenutni_automobil;
    Baket trenutni_baket;
    Blok_automobila blok_automobila;
    prekoracioci = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    index_prekoracioca = 0;
    if (prekoracioci == NULL)
    {
        perror("Alokacija memorije nije uspela");
        zatvori_datoteke();
        exit(EXIT_FAILURE);
    }
    rewind(datoteka_parkiranja_automobila);
    rewind(datoteka_automobila);
    rewind(datoteka_parkiranja);
    
    printf("Pocinje prvi prolaz...\n");
    int blok_counter = 0;
    
    while (fread(&blok_automobila, sizeof(Blok_automobila), 1, datoteka_automobila))
    {
        blok_counter++;
        printf("Obradjuje se blok %d\n", blok_counter);
        
        if (blok_counter > 10) { // Prevent infinite loop - max 10 blocks
            printf("UPOZORENJE: Prekoracen broj blokova, prekidam!\n");
            break;
        }
        
        for (int i = 0; i < f1; i++)
        {
            if (!blok_automobila.automobili[i].registarska_oznaka)
                continue;

            trenutni_automobil = blok_automobila.automobili[i];
            printf("Obradjuje se automobil: %d\n", trenutni_automobil.registarska_oznaka);
            
            Parkirani_automobil parkirani_automobil = vrati_podatke_parkiranog_automobila(datoteka_parkiranja, trenutni_automobil);
            int indeks = hash_funkcija(parkirani_automobil.registarska_oznaka);

            fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
            bool prekoracilac = true;

            for (int j = 0; j < b; j++)
            {
                if (trenutni_baket.automobili[j].status != POPUNJEN)
                {
                    trenutni_baket.automobili[j] = parkirani_automobil;
                    fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
                    fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
                    prekoracilac = false;
                    printf("Automobil %d upisan u baket %d, pozicija %d\n", parkirani_automobil.registarska_oznaka, indeks, j);
                    break;
                }
            }

            if (index_prekoracioca > 0 && index_prekoracioca % 20 == 0)
            {
                prekoracioci = (Parkirani_automobil *)realloc(prekoracioci, (index_prekoracioca + 20) * sizeof(Parkirani_automobil));
                if (prekoracioci == NULL)
                {
                    perror("Realokacija memorije nije uspela");
                    zatvori_datoteke();
                    exit(EXIT_FAILURE);
                }
            }
            if (prekoracilac)
            {
                printf("Automobil %d je prekoracilac\n", parkirani_automobil.registarska_oznaka);
                prekoracioci[index_prekoracioca++] = parkirani_automobil;
            }
        }
    }
    printf("Prvi prolaz zavrsen. Ukupno blokova: %d, prekoracioca: %d\n", blok_counter, index_prekoracioca);
    return prekoracioci;
}

void drugi_prolaz()
{
    index_viska = 0;
    if (index_prekoracioca == 0)
    {
        printf("Nema prekoracilaca.\n");
        return;
    }
    rewind(datoteka_parkiranja_automobila);
    Baket trenutni_baket;
    visak = (Parkirani_automobil *)calloc(20, sizeof(Parkirani_automobil));
    if (visak == NULL)
    {
        perror("Alokacija memorije nije uspela");
        zatvori_datoteke();
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
            fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
            fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
            int z = 0;
            for (; z < b; z++)
            {
                if (trenutni_baket.automobili[z].status != POPUNJEN)
                {
                    trenutni_baket.automobili[z] = parkirani_automobil;
                    fseek(datoteka_parkiranja_automobila, -(long)sizeof(Baket), SEEK_CUR);
                    fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
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
                    zatvori_datoteke();
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

    }
}

void upis_svih_automobila()
{
    prvi_prolaz();
    drugi_prolaz();
    fflush(datoteka_parkiranja_automobila);
    
    // Free allocated memory and set to NULL to prevent double free
    if (prekoracioci != NULL) {
        free(prekoracioci);
        prekoracioci = NULL;
    }
    
    // visak is already freed in drugi_prolaz()
    if (visak != NULL) {
        free(visak);
        visak = NULL;
    }
}

bool registarska_oznaka_postoji_rasuta(int registarska_oznaka)
{
    Baket trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++)
    {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
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

Parkirani_automobil unos_parkiranog_automobila_sa_tastature(){
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
    } while (novi_automobil.registarska_oznaka <= 0 || registarska_oznaka_postoji_rasuta(novi_automobil.registarska_oznaka));

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
    return novi_automobil;
}

void upis_novog_sloga(Parkirani_automobil novi_automobil)
{
    

    int indeks;
    Baket trenutni_baket;

    for (int i = 0; i < B; i++)
    {
        indeks = (hash_funkcija(novi_automobil.registarska_oznaka) + i * k) % B;
        fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
        for (int j = 0; j < b; j++)
        {
            if (trenutni_baket.automobili[j].status != POPUNJEN)
            {
                trenutni_baket.automobili[j] = novi_automobil;
                fseek(datoteka_parkiranja_automobila, -(long)sizeof(Baket), SEEK_CUR);
                fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
                
                printf("Novi slog uspešno upisan.\n");
                fflush(datoteka_parkiranja_automobila);
                return;
            }
        }
    }
    printf("Datoteka je puna, ne moze se uneti novi slog.\n");
}

void prikaz_duzine_bela_zona(int registarska_oznaka) {
    Baket trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++) {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka && trenutni_baket.automobili[i].status == POPUNJEN) {
                printf("Duzina boravka u beloj zoni: %d\n", trenutni_baket.automobili[i].duz_bela);
                printf("Adresa baketa: %p\n", (void*)&trenutni_baket);
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

void prikaz_automobila_2024() {
    Baket trenutni_baket;
    bool pronadjen = false;

    printf("Automobili proizvedeni u 2024. godini:\n");
    printf("%-20s | %-10s | %-10s | %-6s | %-10s | %-8s | %-15s | %-20s | %-12s\n",
           "Registarska oznaka", "Marka", "Model", "Godina", "Boja", "Prihod", "Adresa baketa", "Redni broj sloga", "Prekoracilac");
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

    int prihod;
    for (int j = 0; j < B; j++) {
        fseek(datoteka_parkiranja_automobila, j * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].godina_proizvodnje == 2024 && trenutni_baket.automobili[i].status == POPUNJEN) {
                prihod = trenutni_baket.automobili[i].duz_bela * 150 + trenutni_baket.automobili[i].duz_crvena * 120 + trenutni_baket.automobili[i].duz_plava * 80;
                pronadjen = true;
                printf("%-20d | %-10s | %-10s | %-6d | %-10s | %-8d | %-15p | %-20d | %-12s\n",
                       trenutni_baket.automobili[i].registarska_oznaka,
                       trenutni_baket.automobili[i].marka,
                       trenutni_baket.automobili[i].model,
                       trenutni_baket.automobili[i].godina_proizvodnje,
                       trenutni_baket.automobili[i].boja,
                       prihod,
                       (void*)&trenutni_baket,
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

void logicko_brisanje(int registarska_oznaka) {
    Baket trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++) {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
        fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].registarska_oznaka == registarska_oznaka) {
                trenutni_baket.automobili[i].status = OBRISAN;
                fseek(datoteka_parkiranja_automobila, -(long)sizeof(Baket), SEEK_CUR);
                fwrite_safe(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
                printf("Slog sa registarskom oznakom %d je uspesno obrisan.\n", registarska_oznaka);
                return;
            }
        }
    }

    printf("Automobil sa registarskom oznakom %d nije pronađen.\n", registarska_oznaka);
}


Parkirani_automobil *pronadji_automobil(int registarska_oznaka) {
    Baket *trenutni_baket;
    int indeks;

    for (int j = 0; j < B; j++) {
        indeks = (hash_funkcija(registarska_oznaka) + j * k) % B;
        fseek(datoteka_parkiranja_automobila, indeks * sizeof(Baket), SEEK_SET);
        trenutni_baket = (Baket *)malloc(sizeof(Baket));
        if (trenutni_baket == NULL) {
            perror("Alokacija memorije nije uspela");
            zatvori_datoteke();
            exit(EXIT_FAILURE);
        }
        fread(trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila);
        for (int i = 0; i < b; i++) {
            if (trenutni_baket->automobili[i].registarska_oznaka == registarska_oznaka) {
                return &trenutni_baket->automobili[i];
            }
        }
    }

    return NULL;
}

void ispisi_datoteku_parkiranja_automobila() {
    if (datoteka_parkiranja_automobila == NULL) {
        printf("Datoteka parkiranja automobila nije otvorena.\n");
        return;
    }

    Baket trenutni_baket;
    rewind(datoteka_parkiranja_automobila);
    bool pronadjen = false;

    printf("Svi parkirani automobili:\n");
    printf("-------------------------\n");

    while (fread(&trenutni_baket, sizeof(Baket), 1, datoteka_parkiranja_automobila) == 1) {
        for (int i = 0; i < b; i++) {
            if (trenutni_baket.automobili[i].status == PRAZAN) {
                break;
            }
            if (trenutni_baket.automobili[i].status == OBRISAN) {
                continue;
            }

            pronadjen = true;
            printf("Registarska oznaka: %d\n", trenutni_baket.automobili[i].registarska_oznaka);
            printf("Marka: %s\n", trenutni_baket.automobili[i].marka);
            printf("Model: %s\n", trenutni_baket.automobili[i].model);
            printf("Godina proizvodnje: %d\n", trenutni_baket.automobili[i].godina_proizvodnje);
            printf("Boja: %s\n", trenutni_baket.automobili[i].boja);
            printf("Duzina boravka u beloj zoni: %d\n", trenutni_baket.automobili[i].duz_bela);
            printf("Duzina boravka u crvenoj zoni: %d\n", trenutni_baket.automobili[i].duz_crvena);
            printf("Duzina boravka u plavoj zoni: %d\n", trenutni_baket.automobili[i].duz_plava);
            printf("-------------------------\n");
        }
    }

    if (!pronadjen) {
        printf("Nema parkiranih automobila u evidenciji.\n");
    }
}
