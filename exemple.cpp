#include <SFML/Graphics.hpp>
#include "graphicBuilder.hpp"
#include <vector>
#include <iostream>


int main()
{

	int const hauteur = 720, longueur = 1280;
	sf::RenderWindow window(sf::VideoMode(longueur, hauteur), "SFML works!");
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(window.getSize()));

	float zoom = 1.f;//zoom actuel
	view.zoom(zoom);

	auto f = [](float x, float y)->float {return sin(0.001*sqrt(x*x+y*y)); };//y + x = 0


	srand(time(NULL));
	std::vector<sf::Vertex> graphique;

	std::vector<sf::Vector2i> mousePrecPos;//position précédente de la sourie utilisée pour déplacer la zone d'affichage
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//***************************** Gestion de zoom *****************************
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
				view.move(posSouris * (1 - k) * zoom);//permet de zoomer sans déplacer le point au niveau de la souris
				zoom *= k; //on garde trace du zoom de la vu
			}
		}

		//***************************** Gestion des déplacements *****************************
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!mousePrecPos.empty()) {
				view.move(zoom * (sf::Vector2f(*mousePrecPos.rbegin() - sf::Mouse::getPosition())));
				mousePrecPos.clear();
			}

			mousePrecPos.push_back(sf::Mouse::getPosition());
		}
		else if (!mousePrecPos.empty())
			mousePrecPos.clear();

		buildGraph(f, 0.f, 0.001f, view, graphique, sf::Color::Blue, true, sf::Color::Black);

		window.setView(view);
		window.clear(sf::Color::White);

		if (!graphique.empty())
			window.draw(&graphique[0], graphique.size(), sf::Lines);

		window.display();
	}

	return 0;
}
