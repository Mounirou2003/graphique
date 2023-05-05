#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <array>

int const hauteur = 720, longueur = 1080;

using Point = sf::Vector2f;
struct ligne
{
	sf::Vertex point1;
	sf::Vertex point2;
};

struct Intervalle2D
{
	Point point1;
	Point point2;
};

template<typename fonct>
std::vector<sf::Vertex> construit(fonct f, float valeur, float const precision, Intervalle2D I, float zoom);

Point moyenne(Point point1, Point point2);

float Ox(float x, float y) {
	return y;
}
float Oy(float x, float y) {
	return x;
}

float f1(float x, float y) {
	return 1 / tan(x * x) - y;
}

void arrange(std::vector<sf::Vertex>& tab, sf::Color couleur = sf::Color::White);

int main2()
{
	const float precision = 0.6f;
	float zoom = 250.f;
	float zoom1 = 1.f;
	float valeur = 0.f;
	std::vector<sf::Vector2i> mousePrecPos;

	Intervalle2D intervalle2D{ Point{-longueur / 2  , -hauteur / 2  }, Point{longueur / 2  , hauteur / 2 } };

	sf::RenderWindow window(sf::VideoMode(longueur, hauteur), "graphique");

	sf::Clock clock;

	auto f = [](float x, float y)->float {return f1(x, y); };

	std::vector<sf::Vertex> axeX = { sf::Vertex({-longueur / 2,0}),sf::Vertex({longueur / 2,0}) };
	std::vector<sf::Vertex> axeY = { sf::Vertex({0,-hauteur / 2 }), sf::Vertex({ 0,hauteur / 2  }) };

	/*Point centre1(25, 25);
	Point centre2(0, 2);
	Point centre3(0, -20);*/

	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(window.getSize()));
	while (window.isOpen())
	{
		std::vector<sf::Vertex> graphique = construit(f1, valeur, precision, intervalle2D, zoom);
		arrange(graphique, sf::Color::Green);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				sf::Vector2f posSouris = sf::Vector2f(sf::Mouse::getPosition(window)) - 0.5f * sf::Vector2f(window.getSize()); //position de la souris par rapport au centre de la fenêtre

				float k;
				if (event.mouseWheelScroll.delta > 0)
				{
					k = 9.f / 10; //diminue la constante de zoom si la molette va vers le bas
				}
				else
				{
					k = 10.f / 9;
				}
				view.zoom(k);
				view.move(posSouris * (1 - k) * zoom1);//permet de zoomer sans déplacer le point au niveau de la souris
				zoom1 *= k; //on garde trace du zoom de la vu
			}
		}

		auto temps1 = clock.getElapsedTime().asMilliseconds();
		if (temps1 > 5 && false) {
			clock.restart();
			zoom *= 0.95f;
		}
		/***************************** Gestion de position *****************************/
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!mousePrecPos.empty()) {
				view.move(zoom1 * (sf::Vector2f(*mousePrecPos.rbegin() - sf::Mouse::getPosition())));
				mousePrecPos.clear();
			}

			mousePrecPos.push_back(sf::Mouse::getPosition());
		}
		else if (!mousePrecPos.empty())
		{
			mousePrecPos.clear();
		}
		window.setView(view);

		window.clear();
		window.draw(&axeX[0], axeX.size(), sf::LinesStrip);
		window.draw(&axeY[0], axeY.size(), sf::LinesStrip);
		window.draw(&graphique[0], graphique.size(), sf::Lines);
		window.display();
	}

	return 0;
}

void arrange(std::vector<sf::Vertex>& tab, sf::Color couleur) {
	for (auto& point : tab) {
		point.position.y = -point.position.y;
		point.color = couleur;
	}
	return;
}
Point moyenne(Point point1, Point point2) {
	Point moy;
	moy.x = .5f * (point1.x + point2.x);
	moy.y = .5f * (point1.y + point2.y);
	return moy;
}

std::vector<sf::Vertex> construit(float (*f)(float, float), float valeur, float const precision, Intervalle2D I, float zoom1) {
	float const zoom = 1 / zoom1;

	auto fonction = [f, zoom](float x, float y)->float {return f(x * zoom, y * zoom); };

	std::vector<sf::Vertex> graphique;
	float const pas = 1 / precision;

	for (float x = I.point1.x; x < I.point2.x - pas; x += pas) {
		for (float y = I.point1.y; y < I.point2.y - pas; y += pas) {

			std::array<bool, 4> quadB;
			std::array<Point, 4> quadPos;

			for (int i = 0; i < 4; i++) {
				quadPos[i] = { x + pas * (i % 2), y + pas * ((int)(i / 2) != 0) };
				quadB[i] = fonction(quadPos[i].x, quadPos[i].y) >= valeur;
			}

			for (int i = 0; i < 2; i++) {
				if (quadB[i] != quadB[2 * i + 1]) {
					Point point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos[i];
					point1B = quadB[i];

					point2 = quadPos[i];
					point2B = quadB[i];

					point3 = quadPos[2 * i + 1];
					point3B = quadB[2 * i + 1];

					for (int k = 0; k < 3; k++) {
						if (point2B == point1B) {

							point1 = point2;
							point2 = moyenne(point2, point3);
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point2, point2);
						}
					}

					graphique.push_back(point2);
				}

				if (quadB[2 * i] != quadB[i + 2]) {
					Point point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos[2 * i];
					point1B = quadB[2 * i];

					point2 = quadPos[2 * i];
					point2B = quadB[2 * i];

					point3 = quadPos[i + 2];
					point3B = quadB[i + 2];


					for (int k = 0; k < 3; k++) {
						if (point2B == point1B) {

							point1 = point2;
							point2 = moyenne(point2, point3);
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point2, point2);
						}
					}

					graphique.push_back(point2);
				}
			}
		}
	}

	return graphique;
}

template<typename fonct>
std::vector<sf::Vertex> construit(fonct f, float valeur, float const precision, Intervalle2D I, float zoom1) {
	float const zoom = 1 / zoom1;

	auto fonction = [f, zoom](float x, float y)->float {return f(x * zoom, y * zoom); };

	std::vector<sf::Vertex> graphique;
	float const pas = 1 / precision;

	for (float x = I.point1.x; x < I.point2.x - pas; x += pas) {
		for (float y = I.point1.y; y < I.point2.y - pas; y += pas) {

			std::array<bool, 4> quadB;
			std::array<Point, 4> quadPos;

			for (int i = 0; i < 4; i++) {
				quadPos[i] = { x + pas * (i % 2), y + pas * ((int)(i / 2) != 0) };
				quadB[i] = fonction(x + pas * (i % 2), y + pas * ((int)(i / 2) != 0)) >= valeur;
			}

			for (int i = 0; i < 2; i++) {
				if (quadB[i] != quadB[2 * i + 1]) {
					Point point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos[i];
					point1B = quadB[i];

					point2 = quadPos[i];
					point2B = quadB[i];

					point3 = quadPos[2 * i + 1];
					point3B = quadB[2 * i + 1];

					for (int k = 0; k < 3; k++) {
						if (point2B == point1B) {

							point1 = point2;
							point2 = moyenne(point2, point3);
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point2, point2);
						}
					}

					graphique.push_back(point2);
				}

				if (quadB[2 * i] != quadB[i + 2]) {
					Point point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos[2 * i];
					point1B = quadB[2 * i];

					point2 = quadPos[2 * i];
					point2B = quadB[2 * i];

					point3 = quadPos[i + 2];
					point3B = quadB[i + 2];


					for (int k = 0; k < 3; k++) {
						if (point2B == point1B) {

							point1 = point2;
							point2 = moyenne(point2, point3);
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point2, point2);
						}
					}

					graphique.push_back(point2);
				}
			}
		}
	}

	return graphique;
}
