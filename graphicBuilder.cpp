#include "graphicBuilder.hpp"

void arrangeSFML(std::vector<sf::Vertex>& tab, sf::Color couleur) {
	for (auto& point : tab) {
		point.position.y = -point.position.y;
		point.color = couleur;
	}
}
void color(std::vector<sf::Vertex>& tab, sf::Color couleur) {
	for (auto& point : tab) {
		point.color = couleur;
	}
}

sf::Vector2f moyenne(sf::Vector2f point1, sf::Vector2f point2)
{
	sf::Vector2f moy;

	moy.x = .5f * (point1.x + point2.x);
	moy.y = .5f * (point1.y + point2.y);

	return moy;
}
