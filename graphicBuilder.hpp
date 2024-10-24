#ifndef GRAPHICBUILDERCPP
#define GRAPHICBUILDERCPP

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>


//change la couleur de tous les vertex de tab et remplace y par -y (car sfml mesure les y positifs vers le bas)
void arrangeSFML(std::vector<sf::Vertex>& tab, sf::Color couleur = sf::Color::White,int firstElem = 0);
void addAxes(const sf::View& view, sf::Color AxesColor, std::vector<sf::Vertex>& graph);

//change la couleur de tous les vertex de tab
void color(std::vector<sf::Vertex>& tab, sf::Color couleur = sf::Color::White,int firstElem = 0);
//renvoi le point entre point1 et point2
sf::Vector2f moyenne(sf::Vector2f point1, sf::Vector2f point2);

/*
* construit un tableau de "lignes" (de paires de points) où chaque point résout l'équation f(x,y)=0
* la precision est la fraction de I qui servira de pas pour calculer les points
* tous ces points sont enregistrés dans graphique qui est avant tout vidé
*/
template<typename fonct>
void construit(fonct f, double const precision, const int pointRecalc, sf::FloatRect I, std::vector<sf::Vertex>& graphique, bool add = false);
// Construit un tableau de paires de point contenu dans le rectangle de view ainsi que les tracés x = 0 et y = 0
template<typename fonct>
void buildGraph(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor = sf::Color::White, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);
// Construit un tableau de paires de point contenu dans le rectangle de view ainsi que les tracés x = 0 et y = 0 mais prend en compte le comportement de SFML et retourne le graphique
template<typename fonct>
void buildGraphSFML(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor = sf::Color::White, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);

template<typename fonct>
inline void addGraphSFML(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);

template<typename fonct>
inline void addGraph(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes = true, sf::Color AxesColor = sf::Color::White);

/****************************** template implementation ******************************/

template<typename fonct>
inline void construit(fonct f, double const precision, const int pointRecalc, sf::FloatRect I, std::vector<sf::Vertex>& graphique, bool add) {
	sf::Vector2<double> pas = sf::Vector2<double>(I.width, I.height) * precision;
	if (!add)
		graphique.clear();

	for (double x = I.left; x <= I.left + I.width - pas.x; x += pas.x) {
		for (double y = I.top; y <= I.top + I.height - pas.y; y += pas.y) {
			//pour chaque point de l'écran on construit un "quad" formé de 4 points
			//le point qu'on étudie, celui à droite, celui au dessus et celui du coin

			std::array<bool, 4> quadB;//enregistre si le point lié à cette position du tableau donne une valeur supérieur(1) ou inférieur(0) à 0 pour f(quadPos)
			std::array<sf::Vector2f, 4> quadPos;//contient la position des points

			for (int i = 0; i < 4; i++) {
				quadPos.at(i) = { (float)(x + pas.x * (i % 2)), (float)(y + pas.y * ((int)(i / 2) != 0)) };
				quadB.at(i) = f(quadPos.at(i).x, quadPos.at(i).y) >= 0;
			}

			for (int i = 0; i < 2; i++) {
				//étude des points dans la même colone
				if (quadB.at(i) != quadB.at(2 * i + 1)) {
					sf::Vector2f point1, point2, point3;
					bool point1B, point2B, point3B;

					point1 = quadPos.at(i);
					point1B = quadB.at(i);

					point2 = quadPos.at(i);
					point2B = quadB.at(i);

					point3 = quadPos.at(2 * i + 1);
					point3B = quadB.at(2 * i + 1);

					for (int k = 0; k < pointRecalc; k++) {
						if (point2B == point1B) {

							point1 = point2;
							point2 = moyenne(point2, point3);
							point2B = f(point2.x, point2.y) >= 0;
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point1, point2);
							point2B = f(point2.x, point2.y) >= 0;
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

					for (int k = 0; k < pointRecalc; k++) {
						if (point2B == point1B) {
							point1 = point2;
							point2 = moyenne(point2, point3);
							point2B = f(point2.x, point2.y) >= 0;
						}
						else
						{
							point3 = point2;
							point2 = moyenne(point1, point2);
							point2B = f(point2.x, point2.y) >= 0;
						}
					}

					graphique.push_back(point2);
				}
			}
		}
	}

	return;
}
template<typename fonct>
inline void buildGraphSFML(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes, sf::Color AxesColor)
{
	sf::FloatRect viewRect = sf::FloatRect(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, -view.getCenter().y - 0.5f * view.getSize().y), view.getSize());

	construit(f, prec, pointRecalc, viewRect, graph);
	arrangeSFML(graph, graphColor);

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
	return;
}
template<typename fonct>
inline void buildGraph(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes, sf::Color AxesColor)
{
	sf::FloatRect viewRect = sf::FloatRect(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, view.getCenter().y - 0.5f * view.getSize().y), view.getSize());//rectangle que view affiche

	construit(f, prec, pointRecalc, viewRect, graph);
	color(graph, graphColor);

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
	return;
}

template<typename fonct>
inline void addGraphSFML(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes, sf::Color AxesColor)
{
	sf::FloatRect viewRect = sf::FloatRect(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, -view.getCenter().y - 0.5f * view.getSize().y), view.getSize());
	
	int sizeCount = graph.size();
	construit(f, prec, pointRecalc, viewRect, graph, true);
	arrangeSFML(graph, graphColor,sizeCount);

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
	return;
}
template<typename fonct>
inline void addGraph(fonct f, double const prec, const int pointRecalc, sf::View const& view, std::vector<sf::Vertex>& graph, sf::Color graphColor, bool drawAxes, sf::Color AxesColor)
{
	sf::FloatRect viewRect = sf::FloatRect(sf::Vector2f(view.getCenter().x - 0.5f * view.getSize().x, view.getCenter().y - 0.5f * view.getSize().y), view.getSize());//rectangle que view affiche

	construit(f, prec, pointRecalc, viewRect, graph, true);
	color(graph, graphColor);

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
	return;
}

#endif // !GRAPHICBUILDERCPP
