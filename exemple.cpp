#include <SFML/Graphics.hpp>
#include "graphicBuilder.hpp"
#include <vector>
#include <cmath>
#include <iostream>

float f1(float x, float y) {
	return sin(1/x)*sin(1/y)-0.5;
}

int main()
{

	int const hauteur = 720, longueur = 1280;
	sf::RenderWindow window(sf::VideoMode(longueur, hauteur), "SFML works!");
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(window.getSize()));
	sf::CircleShape test(10);
	test.setFillColor(sf::Color(225, 200, 150));

	float zoom = .01f;//zoom actuel
	view.zoom(zoom);

	auto f2 = [](float x, float y)->float {return 2 * sin(11 * x) - y; };
	auto f3 = [f2](float x, float y)->float {return f2(x, y) + f1(x, y) + y; };

	std::vector<sf::Vertex> graphique;
	std::vector<sf::Vertex> points;
	points.push_back(sf::Vertex(sf::Vector2f(10, 10), sf::Color::White));

	bool followMouse = false;
	std::vector<sf::Vector2i> mousePrecPos;//position précédente de la sourie utilisée pour déplacer la zone d'affichage

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				continue;

			case sf::Event::MouseWheelScrolled:
				//***************************** Gestion de zoom *****************************
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
				view.move(posSouris * (1 - k) * zoom);//permet de zoomer sans déplacer le point au niveau de la souris
				zoom *= k; //on garde trace du zoom de la vu
			}
			continue;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
					followMouse = true;
				continue;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left) {
					mousePrecPos.clear();
					followMouse = false;
				}
				continue;
			default:
				break;

			}

			//***************************** Gestion des déplacements *****************************
			if (followMouse && event.type == sf::Event::MouseMoved) {
				if (!mousePrecPos.empty()) {
					view.move(zoom * (sf::Vector2f(*mousePrecPos.rbegin() - sf::Mouse::getPosition())));
					mousePrecPos.clear();
				}

				mousePrecPos.push_back(sf::Mouse::getPosition());
			}
		}

		buildGraphSFML(f1, 0.003, 5, view, graphique, sf::Color::Blue, true, sf::Color::White);
		addGraphSFML(f2, 0.003, 5, view, graphique, sf::Color::Red, false);
		addGraphSFML(f3, 0.003, 5, view, graphique, sf::Color::Green, false);
		window.setView(view);
		window.clear(sf::Color::Black);

		if (!graphique.empty())
			window.draw(&graphique[0], graphique.size(), sf::Lines);
		if (!points.empty()) {
			arrangeSFML(points);
			window.draw(&points[0], points.size(), sf::Points);
		}
		window.display();
	}

	return 0;
}
