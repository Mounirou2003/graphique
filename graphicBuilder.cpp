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

void addAxes(std::vector<sf::Vertex>& graph, sf::View const& view, sf::Color axesColor) {
	auto axeXBuild = [view](sf::Color color = sf::Color::White) ->std::vector<sf::Vertex> {
		std::vector<sf::Vertex> X;
		auto vert = sf::Vertex(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, 0));
		vert.color = color;

		X.push_back(vert);
		vert = sf::Vertex(sf::Vector2f(view.getCenter().x + 0.5f * view.getSize().x, 0));
		vert.color = color;

		X.push_back(vert);
		return X;
		};
	auto axeYBuild = [view](sf::Color color = sf::Color::White) ->std::vector<sf::Vertex> {
		std::vector<sf::Vertex> X;
		auto vert = sf::Vertex(sf::Vector2f(0, view.getCenter().y - 0.5f * view.getSize().y));
		vert.color = color;

		X.push_back(vert);
		vert = sf::Vertex(sf::Vector2f(0, view.getCenter().y + 0.5f * view.getSize().y));
		vert.color = color;

		X.push_back(vert);
		return X;
		};

	std::vector<sf::Vertex> axeX = axeXBuild(axesColor);
	std::vector<sf::Vertex> axeY = axeYBuild(axesColor);

	graph.insert(graph.begin(), axeX.begin(), axeX.end());
	graph.insert(graph.begin(), axeY.begin(), axeY.end());
}