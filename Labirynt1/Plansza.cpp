#include <SFML/Graphics.hpp> //import bibliotek
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <queue>
#include "Plansza.h"

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
	std::cout << m_tablica[0];
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
	int d = 0;
	int e = 0;
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
				punktKolejki b = {{wiersz, kolumna}, aktualny.odleglosc + 1};
				kolejka.push(b);
				table[d][e] = nrKolumn[i], nrWiersza[i];

				e++;
			}
		}
		d++;
	}
	return -1;
}
bool Plansza::Sprawdz(int wiersze, int kolumny)
{
	return (wiersze >= 0) && (wiersze < m_xkolumny) &&
		   (kolumny >= 0) && (kolumny < m_ykolumny);
}
void Plansza::Trasa()
{
	int temp;
	std::vector<std::vector<int>> dist;
	for (int i = 0; i < m_ykolumny; i++)
	{
		for (int j = 0; j < m_ykolumny; j++)
		{
			Punkt poczatek = {0, i};
			Punkt koniec = {m_ykolumny - 1, j};
			dist[i][j] = ZnajdzOdleglosc(poczatek, koniec);
			if (dist[i][j] != -1)
			{
				std::cout << "Shortest Path is " << dist << std::endl;
				&temp = dist[i][j];
			}
			//temp2 = i,j;
		}
	}

	for (int i = 0; i < m_ykolumny; i++)
	{
		for (int j = 0; j < m_ykolumny; j++)
		{
			if (dist[i][j] < temp)
			{
				temp = dist[i][j];
			}
		}
	}
	std::cout << temp << std::endl;
	fghj
			std::cout
		<< "najkrotsza droga wynosi: " << temp;
}
