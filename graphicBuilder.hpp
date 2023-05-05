#ifndef GRAPHICBUILDERCPP
#define GRAPHICBUILDERCPP

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

void arrange(std::vector<sf::Vertex>& tab, sf::Color couleur = sf::Color::White);
sf::Vector2f moyenne(sf::Vector2f point1, sf::Vector2f point2);

bool buildGraph(float (*f)(float, float), float value, float prec, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor = sf::Color::White, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);
bool construit(float (*f)(float, float), float valeur, float const precision, sf::FloatRect I, std::vector<sf::Vertex>& graphique);

template<typename fonct>
bool construit(fonct f, float valeur, float const precision, sf::FloatRect I, std::vector<sf::Vertex>& graphique);
template<typename fonct>
bool buildGraph(fonct f, float value, float prec, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor = sf::Color::White, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);


template<typename fonct>
bool construit(fonct f, float valeur, float const precision, sf::FloatRect I, std::vector<sf::Vertex>& graphique) {

	sf::Vector2f pas = sf::Vector2f(I.width, I.height) * precision;
	graphique.clear();

	for (float x = I.left; x <= I.left + I.width - pas.x; x += pas.x) {
		for (float y = I.top; y <= I.top + I.height - pas.y; y += pas.y) {
			std::array<bool, 4> quadB;
			std::array<sf::Vector2f, 4> quadPos;

			for (int i = 0; i < 4; i++) {
				quadPos.at(i) = { x + pas.x * (i % 2), y + pas.y * ((int)(i / 2) != 0) };
				quadB.at(i) = f(quadPos.at(i).x, quadPos.at(i).y) >= valeur;
			}

			for (int i = 0; i < 2; i++) {
				if (quadB.at(i) != quadB.at(2 * i + 1)) {
					sf::Vector2f point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos.at(i);
					point1B = quadB.at(i);

					point2 = quadPos.at(i);
					point2B = quadB.at(i);

					point3 = quadPos.at(2 * i + 1);
					point3B = quadB.at(2 * i + 1);

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

				if (quadB.at(2 * i) != quadB.at(i + 2)) {
					sf::Vector2f point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos.at(2 * i);
					point1B = quadB.at(2 * i);

					point2 = quadPos.at(2 * i);
					point2B = quadB.at(2 * i);

					point3 = quadPos.at(2 + i);
					point3B = quadB.at(2 + i);


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

	return false;
}
template<typename fonct>
inline bool buildGraph(fonct f, float value, float const prec, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes, sf::Color AxesColor)
{
	sf::FloatRect viewRect = sf::FloatRect(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, -view.getCenter().y - 0.5f * view.getSize().y), view.getSize());

	if (construit(f, value, prec, viewRect, graph)) {
		arrange(graph, graphColor);

		if (drawAxes) {
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

			std::vector<sf::Vertex> axeX = axeXBuild(AxesColor);
			std::vector<sf::Vertex> axeY = axeYBuild(AxesColor);

			graph.insert(graph.begin(), axeX.begin(), axeX.end());
			graph.insert(graph.begin(), axeY.begin(), axeY.end());
		}
		return true;
	}
	return false;
}

#endif // !GRAPHICBUILDERCPP
