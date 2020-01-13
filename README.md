# Pacman
Klon Pacman'a napisany w C++ i z wykorzystaniem Qt

![Screenshot](https://github.com/lnarolski/Pacman/blob/master/Screenshots/screenshot1.png)

# Działanie
Do sterowania wykorzystywane są klawisze strzałek. Najlepsze sterowanie uzyskuje się poprzez wciskanie i przytrzymywanie klawiszy. 

# Tworzenie plansz
Do tworzenia plansz wykorzystuje się dokumenty Word'a (przykłady w folderze *mapsCreate*). W pierwszej linii dokumentu podaje się rozmiary planszy, a w kolejnej tworzy tabelę z mapą (numery w komórkach oznaczają określony rodzaj pól). Utworzony plik Word'a należy wyeksportować do pliku *.txt i umieścić go w folderze *maps*.

## Znaczenie numerów pól
Znaczenie | Numer
--- | ---
Ściana | 0
Nie istnieje | -1
Punkt | 1
Kulka pozwalająca 'zjadać' duchy | 2
Respawn duchów | 5
Wyjście duchów | 7
Puste pole | 8
Skrzyżowanie (informacja dla duchów o możliwości zmiany kierunku) | 10
Skrzyżowanie z punktem (informacja dla duchów o możliwości zmiany kierunku) | 11
Skrzyżowanie w respawnie duchów (informacja dla duchów o możliwości zmiany kierunku) | 12
Tunel | 18

# TODO
- Poprawki błędów

# [Zrzuty ekranu](https://github.com/lnarolski/Pacman/tree/master/Screenshots)
