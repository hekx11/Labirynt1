#include <SFML/Graphics.hpp>
#include <fstream> //import biblioteki fstream
#include <stdio.h>
#include <iostream>
#include "Plansza.h"

int Plansza::Xkolumny()
{
	return m_xkolumny;
}
int Plansza::Ykolumny()
{
	return m_ykolumny;
}
void Plansza::Window(int x, int y)
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
	std::fstream file;
	char wiersz[260];
	file.open("dane.txt");
	if (file.is_open())
	{
		file.getline(wiersz, 260);
		file.close();
	}
	else
		std::cout << "Nie udało się otworzyć pliku";

	std::cout << wiersz[0] << std::endl;
	sf::RectangleShape shape[x][y];
	int position_x = 0, position_y = 10;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(200, 200, 200, 255));
		for (int i = 0; i < sizeof(shape) / sizeof(shape[0]); i++)
		{
			for (int j = 0; j < sizeof(shape[i]) / sizeof(shape[0][0]); j++)
			{
				shape[i][j].setSize(sf::Vector2f(10, 10));
				if (j % 2 == 0)
				{
					shape[i][j].setFillColor(sf::Color(0, 0, 0, 255));
				}
				else
				{
					shape[i][j].setFillColor(sf::Color(255, 255, 255, 255));
				}
				shape[i][j].setPosition(x, y);

				window.draw(shape[i][j]);
				position_y += 11;
			}
			position_x += 11;
			position_y = 0;
		}
		position_x = 0;
		window.display();
	}
}
