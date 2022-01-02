#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Plansza
{
private:
	int m_xkolumny;
	int m_ykolumny;
	std::string *m_tablica = new std::string[260];
	std::string table[260];
	int mat[260][260];
	bool odwiedzone[260][260];
	std::vector<std::pair<int, int>> koncowaDroga;

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
	struct punktKolejki
	{
		Punkt wsp;	   //wspolrzedne punktu
		int odleglosc; //odleglosc od poczatku
	};

public:
	void Window();
	void Ustaw(int x, int y);
	void Pobieranie();
	int ZnajdzOdleglosc(Punkt poczatek, Punkt koniec);
	std::vector<std::pair<int, int>> ZnajdzDroge(Punkt poczatek, Punkt koniec);
	bool Sprawdz(int x, int y);
	void ZnajdzTrase(Punkt *punkt, std::vector<std::pair<int, int>> &in);
	template <typename T>void Paruj(std::vector<std::pair<T, T>> const &in);
	void Trasa();
};
