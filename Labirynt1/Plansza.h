#pragma once
class Plansza
{
private:
	int m_xkolumny;
	int m_ykolumny;

public:
	int Xkolumny() const;
	int Ykolumny() const;
	void Window(int x, int y);
};
