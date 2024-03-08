#define _USE_MATH_DEFINES

#include "bouton.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <cmath>

Button::State Button::getState() const
{
	return m_state;
}
void Button::uptadeState()
{
	sf::IntRect windowBox(sf::Vector2i(0, 0), sf::Vector2i(m_window->getSize()));
	auto mousePos = sf::Mouse::getPosition(*m_window);

	if (!windowBox.contains(mousePos) || !m_box.contains(m_window->mapPixelToCoords(mousePos))) {
		m_state = idle;
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_state = press;
		return;
	}

	m_state = hoverButton;
	return;
}
void Button::build(sf::RenderWindow* window, sf::FloatRect box, sf::Text text) {
	sf::Image temp;

	m_idleTexture = std::make_unique<sf::Texture>();
	m_hoverTexture = std::make_unique<sf::Texture>();
	m_pressedTexture = std::make_unique<sf::Texture>();


	temp.create(box.width, box.height, sf::Color::White);
	m_idleTexture->loadFromImage(temp);

	temp.create(box.width, box.height, sf::Color(225, 225, 225));
	m_hoverTexture->loadFromImage(temp);

	temp.create(box.width, box.height, sf::Color(200, 200, 200));
	m_pressedTexture->loadFromImage(temp);

	m_box = box;
	m_window = window;
	m_text = text;

	auto center = sf::Vector2f(m_box.left, m_box.top) + 0.5f * sf::Vector2f(m_box.width, m_box.height);

	m_text.setOrigin(0.5f * sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height + 0.5f * m_text.getCharacterSize()));
	m_text.setPosition(center);
	m_state = idle;
}
Button::Button(sf::RenderWindow* window, sf::FloatRect box, sf::Text text)
{
	build(window, box, text);
}
Button::~Button()
{
	m_idleTexture = nullptr;
	m_hoverTexture = nullptr;
	m_pressedTexture = nullptr;
}
bool Button::isPressed() const noexcept
{
	return m_state == press;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rect(sf::Vector2f(m_box.width, m_box.height));
	rect.setPosition(m_box.left, m_box.top);

	switch (getState())
	{
	case Button::idle:

		rect.setTexture(m_idleTexture.get());
		break;
	case Button::hoverButton:

		rect.setTexture(m_hoverTexture.get());
		break;
	case Button::press:

		rect.setTexture(m_pressedTexture.get());
		break;
	default:

		throw std::runtime_error("Unexepted error while rendering button");

		break;
	}

	target.draw(rect, states);
	target.draw(m_text, states);
}


///////////////////////////////////////////
// Slider definition


Slider::~Slider()
{
	m_idleTexture = nullptr;
	m_sliderTexture = nullptr;
	m_hoverTexture = nullptr;
	m_hoverSliderTexture = nullptr;
	m_pressedSliderTexture = nullptr;
}
void rotate(double angle, sf::Vector2<double>& vect) {
	vect = sf::Vector2<double>(cos(angle) * vect.x - sin(angle) * vect.y, sin(angle) * vect.x + cos(angle) * vect.y);
}
void rotate(double angle, sf::Vector2f& vect) {
	vect = sf::Vector2f(cos(angle) * vect.x - sin(angle) * vect.y, sin(angle) * vect.x + cos(angle) * vect.y);
}

void Slider::build(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u const size, double angle, double radius, sf::Text text)
{
	m_window = window;
	m_origin = origin;
	m_text = text;
	m_size = size;
	m_Radius = radius;
	m_angle = angle;
	m_state = idle;

	auto V = sf::Vector2f(size.x,0);
	rotate(angle, V);

	m_pos = origin - 0.5f * V;

	sf::Image temp;

	m_idleTexture = std::make_unique<sf::Texture>();
	m_sliderTexture = std::make_unique<sf::Texture>();

	m_hoverTexture = std::make_unique<sf::Texture>();
	m_hoverSliderTexture = std::make_unique<sf::Texture>();

	m_pressedSliderTexture = std::make_unique<sf::Texture>();


	temp.create(size.x, size.y, sf::Color::White);
	m_pressedSliderTexture->loadFromImage(temp);

	temp.create(size.x, size.y, sf::Color::Red);
	m_hoverTexture->loadFromImage(temp);

	temp.create(size.x, size.y, sf::Color::Green);
	m_sliderTexture->loadFromImage(temp);

	temp.create(size.x, size.y, sf::Color::Blue);
	m_hoverSliderTexture->loadFromImage(temp);

	temp.create(size.x, size.y, sf::Color::Yellow);
	m_idleTexture->loadFromImage(temp);

}
Slider::Slider(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, double radius, double angle, sf::Text text)
{
	build(window, origin, size, angle, radius, text);
}
Slider::Slider(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, sf::Text text)
{
	build(window, origin, size, 0, size.y, text);
}

template<typename T>
double norme2(sf::Vector2<T> vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

template<typename T>
double norme2(T vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

void Slider::uptadeState()
{
	sf::IntRect windowBox(sf::Vector2i(0, 0), sf::Vector2i(m_window->getSize()));
	auto mousePos = sf::Mouse::getPosition(*m_window);

	if (!windowBox.contains(mousePos)) {
		m_state = idle;
		return;
	}

	float n = norme2(m_window->mapPixelToCoords(mousePos) - m_pos);
	if (m_state == press || n < 0.25 * m_Radius * m_Radius)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			m_state = press;
			return;
		}

		if (m_state != press) {
			m_state = hoverSlide;
			return;
		}
	}

	sf::Vector2f mousePosRot = m_window->mapPixelToCoords(mousePos), origin = m_origin;

	rotate(-m_angle, mousePosRot);
	rotate(-m_angle, origin);

	sf::FloatRect box(origin-0.5f* sf::Vector2f(m_size), sf::Vector2f(m_size));

	if (box.contains(mousePosRot)) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			m_state = press;
			return;
		}

		m_state = hoverBox;
		return;
	}

	m_state = idle;
}
Slider::State Slider::getState() const
{
	return m_state;
}
bool Slider::isPressed() const noexcept
{
	return m_state == press;
}

float Slider::getLerp() const
{
	sf::Vector2f dir(1, 0);
	auto pos = (m_pos - m_origin)/float(m_size.x);
	rotate(m_angle, dir);

	return pos.x * dir.x + pos.y * dir.y + 0.5;
}

float Slider::getLerp(float min, float max) const
{
	auto n = getLerp();
	return (1-n)*min + n * max;
}

void Slider::setSlide()
{
	auto pos = (m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)) - m_origin)/float(m_size.x);
	sf::Vector2<double> dir(1, 0);

	rotate(m_angle, dir);

	setLerp(pos.x*dir.x+pos.y*dir.y+0.5,0,1);
}
void Slider::setLerp(double num, double min, double max)
{
	{
		auto c = max; 
		max = std::max(min, max);
		min = std::min(min, c);
	}
	double h = max - min;
	num = std::max(num, min);
	num = std::min(num, max);

	num -= min;
	num /= h;

	sf::Vector2f dir(1, 0);

	rotate(m_angle, dir);

	m_pos = float(m_size.x*(num-0.5)) * dir+m_origin;

}
void Slider::setAngle(double newAngle)
{

	auto V = (m_pos - m_origin);

	rotate(newAngle - m_angle, V);

	m_pos = (V + m_origin);

	m_angle = newAngle;
}


void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto fsize = sf::Vector2f(m_size);
	sf::RectangleShape rect(fsize);
	rect.setPosition(m_origin);
	rect.setOrigin(0.5f*fsize);

	rect.setRotation(180*M_1_PI*m_angle);

	sf::CircleShape circle(0.5*m_Radius);
	circle.setPosition(m_pos - 0.5f*sf::Vector2f(m_Radius,m_Radius));

	switch (getState())
	{
	case Slider::idle:
		rect.setTexture(m_idleTexture.get());
		circle.setTexture(m_sliderTexture.get());

		break;
	case Slider::hoverBox:
		rect.setTexture(m_hoverTexture.get());
		circle.setTexture(m_sliderTexture.get());

		break;
	case Slider::hoverSlide:
		rect.setTexture(m_idleTexture.get());
		circle.setTexture(m_hoverSliderTexture.get());

		break;
	case Slider::press:
		rect.setTexture(m_idleTexture.get());
		circle.setTexture(m_pressedSliderTexture.get());

		break;
	default:

		break;
	}

	target.draw(rect, states);
	target.draw(circle, states);
	target.draw(m_text, states);
}
