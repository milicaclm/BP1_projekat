#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include "ostalo.h"

// Uopstene funkcije za rad sa datotekama
void zatvori_datoteke();
FILE *otvori_datoteku(const char *ime_datoteke, const char *mod);
size_t fwrite_safe(const void *buffer, size_t size, size_t count, FILE *datoteka);

// Funkcije vezane za evidenciju automobila i parkiranja
void dodaj_prazan_blok_automobila(Automobil slog_za_dodavanje);
void dodaj_prazan_blok_parkiranja(Parkiranje *slog_za_dodavanje);
void kreiraj_praznu_evidenciju_automobila(const char *ime_datoteke_automobila);
void kreiraj_praznu_evidenciju_parkinga(const char *ime_datoteke_parkinga);
bool registarska_oznaka_postoji_sekvencijalna(int registarska_oznaka);
bool identifikator_postoji(int identifikator);
Automobil* unos_automobila_sa_tastature();
Automobil* dodavanje_automobila(Automobil *novi_automobil);
void upis_automobila(Automobil* novi_automobil);
Parkiranje* unos_parkiranja_sa_tastature();
Parkiranje *dodavanje_parkiranja(Parkiranje* novo_parkiranje, bool *samo_belo);
void upis_parkiranja(Parkiranje* novo_parkiranje);
void ispisi_automobil();
void prikaz_parkiranja(int registarska_oznaka);
void prikaz_automobila();
Automobil *modifikacija_automobila();
void ispisi_datoteku_automobila();
void ispisi_datoteku_parkiranja();

// Funkcije vezane za rasutu datoteku parkiranja
int hash_funkcija(int registarska_oznaka);
void kreiraj_praznu_evidenciju();
Parkirani_automobil vrati_podatke_parkiranog_automobila(FILE *datoteka_parkiranja, Automobil automobil);
Parkirani_automobil *prvi_prolaz();
void drugi_prolaz();
void upis_svih_automobila();
bool registarska_oznaka_postoji_rasuta(int registarska_oznaka);
Parkirani_automobil unos_parkiranog_automobila_sa_tastature();
void upis_novog_sloga(Parkirani_automobil novi_automobil);
void prikaz_duzine_bela_zona(int registarska_oznaka);
void prikaz_automobila_2024();
void logicko_brisanje(int registarska_oznaka);
Parkirani_automobil *pronadji_automobil(int registarska_oznaka);
void ispisi_datoteku_parkiranja_automobila();

// Funkcije vezane za logovanje
void dodaj_blok_log(Log log);
void kreiraj_datoteku_log(const char *ime_datoteke_log);
void propagiraj_unos_automobila(Automobil novi_automobil);
void propagiraj_modifikaciju_automobila(Automobil automobil);
void loguj_parkiranje(int registarska_oznaka, int duzina_boravka);
void propagiraj_unos_parkiranja(Parkiranje parkiranje);
void ispisi_log_datoteku();
void ispisi_log_datoteku_oznaka_duzina();
void ocisti_log_datoteku();

#endif // FUNKCIJE_H