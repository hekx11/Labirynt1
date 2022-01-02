#include <SFML/Graphics.hpp> //import bibliotek
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <queue>
#include "Plansza.h"
#include <set>

void Plansza::Ustaw(int x, int y) //ustawienie ilosci kwadracikow
{
	m_xkolumny = x;
	m_ykolumny = y;
}
void Plansza::Pobieranie() //pobieranie z pliku
{
	std::ifstream file;
	std::string wiersz;
	std::string wiersze[258];
	file.open("dane.txt");
	if (file.is_open())
	{
		int b = 0;

		while (!file.eof()) //eof - end of file
		{
			getline(file, wiersz);
			wiersze[b] = wiersz;
			b++;
		}
		file.close();
	}
	else
		std::cout << "Nie udalo sie otworzyc pliku";
	int l = 0;
	int wymiary_planszy[7] = {0}; //maksymalnie może być 7 znaków w pierwszym wierszu (xxxspacjaxxx)

	for (int i = 0; wiersze[0][i] != '\0'; i++)
	{
		if (wiersze[0][i] == ' ')
		{
			l++;
		}
		else
		{
			wymiary_planszy[l] = wymiary_planszy[l] * 10 + (wiersze[0][i] - 48); //zamiana string na int
		}
	}
	Ustaw(wymiary_planszy[0], wymiary_planszy[1]);
	for (int i = 0; i < wymiary_planszy[1]; ++i)
	{
		m_tablica[i] = wiersze[i + 1]; //usuniecie pierwszej linii array'a
		for (int j = 0; j < m_ykolumny; ++j)
		{
			if (m_tablica[i][j] == 'B')
				mat[i][j] = 0;
			else
				mat[i][j] = 1;
		}
	}
}
void Plansza::Window() //wyswietlanie okna
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Labirynt"); //okno 640x480
	int n = m_xkolumny;
	int m = m_ykolumny;
	sf::RectangleShape **shape;
	shape = new sf::RectangleShape *[n];
	for (int i = 0; i < n; i++)
		shape[i] = new sf::RectangleShape[m];
	int position_x = 0, position_y = 10; //pozycja pierwszego kwadracika
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close(); //zamyanie programu i "niszczenie" zawartości
		}
		window.clear(sf::Color(200, 200, 200, 255)); //kolor tła
		for (int i = 0; i < n; i++)					 //rysowanie kwadracikow po osi x
		{
			for (int j = 0; j < m; j++) //rysowanie kwadracikow po osi y
			{
				shape[i][j].setSize(sf::Vector2f(10, 10)); //rozmiar jednego kwadracika
				if (m_tablica[j][i] == 'B')
				{
					shape[i][j].setFillColor(sf::Color(0, 0, 0, 255)); //kolor czarny kwadracika
				}
				else
				{
					shape[i][j].setFillColor(sf::Color(255, 255, 255, 255)); //kolor bialy kwadracika
				}
				shape[i][j].setPosition(position_x, position_y); //ustawienie pozycji kwadracika

				window.draw(shape[i][j]); //rysowanie kwadracika na planszy
				position_y += 11;		  //przesuniecie kwadracika w dol
			}
			position_x += 11; //przesumiecie kwadracika w prawo
			position_y = 0;	  //wyzerowanie pozycji pionowej kwadracika
		}
		position_x = 0;	  // wyzerowanie pozycji poziomej kwadracika
		window.display(); //wyswietlanie okna
	}
	for (int i = 0; i < n; i++) //usuniecie z pamieci
		delete[] shape[i];
	delete[] shape;
}
int Plansza::ZnajdzOdleglosc(Punkt poczatek, Punkt koniec)
{
	struct punktKolejki
	{
		Punkt wsp;	   //wspolrzedne punktu
		int odleglosc; //odleglosc od poczatku
	};
	int nrWiersza[] = {-1, 0, 0, 1};
	int nrKolumn[] = {0, -1, 1, 0};

	if (!mat[poczatek.x][poczatek.y] || !mat[koniec.x][koniec.y])
		return -1;

	memset(odwiedzone, false, sizeof odwiedzone);
	odwiedzone[poczatek.x][poczatek.y] = true;
	std::queue<punktKolejki> kolejka;
	punktKolejki a = {poczatek, 0};
	kolejka.push(a);
	int table[200][200];
	while (!kolejka.empty())
	{
		punktKolejki aktualny = kolejka.front();
		Punkt wsp = aktualny.wsp;
		if (wsp.x == koniec.x && wsp.y == koniec.y)
			return aktualny.odleglosc;
		kolejka.pop();
		for (int i = 0; i < 4; i++)
		{
			int wiersz = wsp.x + nrWiersza[i];
			int kolumna = wsp.y + nrKolumn[i];
			if (Sprawdz(wiersz, kolumna) && mat[wiersz][kolumna] && !odwiedzone[wiersz][kolumna])
			{
				odwiedzone[wiersz][kolumna] = true;
				punktKolejki b = {{wiersz, kolumna, nullptr}, aktualny.odleglosc + 1};
				kolejka.push(b);
			}
		}
	}
	return -1;
}
std::vector<std::pair<int, int>> Plansza::ZnajdzDroge(Punkt poczatek, Punkt koniec)
{
	std::vector<std::pair<int, int>> droga;
	int c = poczatek.x;
	int d = poczatek.y;
	int nrWiersza[] = {-1, 0, 0, 1};
	int nrKolumn[] = {0, -1, 1, 0};
	if (!mat[poczatek.x][poczatek.y] || !mat[koniec.x][koniec.y])
		return droga;
	memset(odwiedzone, false, sizeof odwiedzone);
	odwiedzone[poczatek.x][poczatek.y] = true;
	std::queue<Punkt *> kolejka;
	Punkt *a = new Punkt(c, d, nullptr);
	kolejka.push(a);
	int e = 0;

	while (!kolejka.empty())
	{
		Punkt *aktualny = kolejka.front();
		kolejka.pop();
		int a = aktualny->x;
		int b = aktualny->y;
		if (a == koniec.x && b == koniec.y)
		{
			ZnajdzTrase(aktualny, droga);
			return droga;
		}
		for (int i = 0; i < 4; i++)
		{
			int wiersz = a + nrWiersza[i];
			int kolumna = b + nrKolumn[i];
			if (Sprawdz(wiersz, kolumna) && mat[wiersz][kolumna] && !odwiedzone[wiersz][kolumna])
			{
				odwiedzone[wiersz][kolumna] = true;
				Punkt *kolejny = new Punkt(wiersz, kolumna, aktualny);
				kolejka.push(kolejny);
			}
		}
	}
	return droga;
}
bool Plansza::Sprawdz(int wiersze, int kolumny)
{
	return (wiersze >= 0) && (wiersze < m_xkolumny) && (kolumny >= 0) && (kolumny < m_ykolumny);
}
void Plansza::ZnajdzTrase(Punkt *punkt, std::vector<std::pair<int, int>> &droga)
{
	if (punkt != nullptr)
	{
		ZnajdzTrase(punkt->parent, droga);
		droga.push_back(std::make_pair(punkt->x, punkt->y));
	}
}
template <typename T>
void printPairs(std::vector<std::pair<T, T>> const &input)
{
	int n = input.size();
	for (auto const &p : input)
	{
		std::cout << '(' << p.first << ", " << p.second << ')';
		if (--n)
		{
			std::cout << ", ";
		}
	}
}
void Plansza::Trasa()
{
	int temp = 0, a = 0, b = 0;
	int dist[260][260];
	for (int i = 0; i < m_ykolumny; i++)
	{
		for (int j = 0; j < m_ykolumny; j++)
		{
			Punkt poczatek = {0, i, nullptr};
			Punkt koniec = {m_ykolumny - 1, j, nullptr};
			dist[i][j] = ZnajdzOdleglosc(poczatek, koniec);
			if (dist[i][j] != -1)
			{
				temp = dist[i][j];
			}
		}
	}

	for (int i = 0; i < m_ykolumny; i++)
	{
		for (int j = 0; j < m_ykolumny; j++)
		{
			if (dist[i][j] != -1)
				if (dist[i][j] < temp)
				{
					temp = dist[i][j];
					a = i;
					b = j;
				}
		}
	}
	Punkt poczatekF = {0, a, nullptr};
	Punkt koniecF = {m_ykolumny - 1, b, nullptr};

	std::cout << "Najkrotsza droga wynosi: " << temp << std::endl;
	std::cout << "Poczatek: " << a << ", 0" << std::endl;
	std::cout << "Koniec: " << b << ", " << m_ykolumny - 1 << std::endl;
	std::vector<std::pair<int, int>> path = ZnajdzDroge(poczatekF, koniecF);

	if (path.size() > 0)
	{
		std::cout << "The shortest path is ";
		printPairs(path);
	}
	else
	{
		std::cout << "Destination not possible";
	}
}