Kompajliranje programa:
gcc -std=c11 -Wall -Wextra -O2 main.c deo1.c deo2.c deo3.c -o a.out

Pokretanje:

1. Bez umetanja postojecih datoteka
Linux/macOS:
./a.out
Windows:
./a.exe

2. Sa umetanjem postojecih fajlova za automobile i parkiranja

Linux/macOS:
./a.out [naziv_datoteke_automobila] [naziv_datoteke_parkiranja]
Windows:
./a.exe [naziv_datoteke_automobila] [naziv_datoteke_parkiranja]

3. Sa umetanjem postojecih fajlova za automobile, parkiranja i parkiranja automobila

Linux/macOS:
./a.out [naziv_datoteke_automobila] [naziv_datoteke_parkiranja] [naziv_datoteke_parkiranja_automobila]
Windows:
./a.exe [naziv_datoteke_automobila] [naziv_datoteke_parkiranja] [naziv_datoteke_parkiranja_automobila]


