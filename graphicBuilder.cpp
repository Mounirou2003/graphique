#include "graphicBuilder.hpp"

void arrangeSFML(std::vector<sf::Vertex>& tab, sf::Color couleur, int firstElem) {
	for (int i = firstElem; i < tab.size(); i++) {
		tab[i].position.y = -tab[i].position.y;
		tab[i].color = couleur;
	}
}
void color(std::vector<sf::Vertex>& tab, sf::Color couleur, int firstElem) {
	for (int i = firstElem; i < tab.size(); i++) {
		tab[i].color = couleur;
	}
}

sf::Vector2f moyenne(sf::Vector2f point1, sf::Vector2f point2)
{
	sf::Vector2f moy;

	moy.x = .5f * (point1.x + point2.x);
	moy.y = .5f * (point1.y + point2.y);

	return moy;
}
