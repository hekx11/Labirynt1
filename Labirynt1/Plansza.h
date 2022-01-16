/*!
Program znajdujący najkrótszą drogę w labiryncie na podstawie danych wejściowych z pliku.
Obrazuje drogę w postaci kolorowanych kafelków.
Wypisuje drogę do pliku wynik.txt w formacie np.: tak D , D , D , L , L , D , D , P , D , D , D , D , D , lub 'nie' gdy nie ma drogi.
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
///Klasa planszy
class Plansza
{
private:
	///zmienna przechowujaca ilosc kolumn
	int m_xkolumny;
	///zmienna przechowujaca ilosc wierszy
	int m_ykolumny;
	///tablica znakow przechowująca dane z pliku
	std::string *m_tablica = new std::string[260];
	///macierz przechowująca dane z pliku w postaci liczbowej (B=1, nie B=0)
	int mat[260][260];
	///tablica przechowujaca odwiedzone pola
	bool odwiedzone[260][260];
	///wektor przechowujący końcową drogę
	std::vector<std::pair<int, int>> koncowaDroga;
	///struktura punktu
	struct Punkt
	{
		int x;
		int y;
		Punkt *parent;
		Punkt(int x, int y, Punkt *parent)
		{
			this->x = x;
			this->y = y;
			this->parent = parent;
		}
	};
	///strunktura punktu w kolejce
	struct punktKolejki
	{
		Punkt wsp;
		int odleglosc;
	};

public:
	///<summary>Ogólna funkcja odpowiedzialna za wyświetlanie planszy na podstawie zmiennych z pliku<summary>
	void Window();

	/// <summary>Funkcja ustawia wartości zmiennych prywatnych m_ykolumny i m_xkolumny</summary>
	void Ustaw(int x, int y);

	/// <summary>Funkcja pobiera dane z pliku tekstowego i ustawia zadaną ilość wierszy i kolumn</summary>
	void Pobieranie();

	/// <summary> Funkcja znajduje najkrótszą odległość między dwoma punktami w tablicy</summary>
	int ZnajdzOdleglosc(Punkt poczatek, Punkt koniec);

	/// <summary>Funkcja znajduje najkrótszą drogę między dwoma punktami w macierzy</summary>
	std::vector<std::pair<int, int>> ZnajdzDroge(Punkt poczatek, Punkt koniec);

	/// <summary>Funkcja sprawdza czy zadane pozycje są prawidłowe, zwraca TRUE albo FALSE</summary>
	bool Sprawdz(int x, int y);

	/// <summary>Funkcja do wydrukowania drogi z jednego punktu do drugiego</summary>
	void ZnajdzTrase(Punkt *punkt, std::vector<std::pair<int, int>> &droga);

	/// <summary>Funkcja do drukowania znalezionej drogi do pliku</summary>
	template <typename T>
	void Paruj(std::vector<std::pair<T, T>> const &in);

	/// <summary>Funkcja znajdująca i drukująca trasę za pomocą pozostałych funkcji</summary>
	void Trasa();
};
