#ifndef BOUTON
#define BOUTON

#include <SFML/Graphics.hpp>
#include <vector>

/// <summary>
///
/// </summary>
class Button : public sf::Drawable
{
public:

	enum State {
			idle,
			hoverButton,
			hoverSlide,
			press
	};

	Button() = delete;
	Button(sf::RenderWindow * window, sf::FloatRect box, sf::Text text = sf::Text());
	//Button(sf::RenderWindow* window, sf::Text text);
	~Button();


	/// \brief
	/// Check the button state in the last call of updateState()
	///
	/// \return
	///	Button::State depending on the state of the button
	State getState() const;

	virtual void uptadeState();
	/// \brief
	/// is equivalent to "getState() == Button::press"
	/// \return
	bool isPressed() const noexcept;

	std::unique_ptr<sf::Texture> m_idleTexture;
	std::unique_ptr<sf::Texture> m_hoverTexture;
	std::unique_ptr<sf::Texture> m_pressedTexture;

protected:

	void build(sf::RenderWindow* window, sf::FloatRect box, sf::Text text);
	sf::FloatRect m_box;

	sf::RenderWindow* m_window;
	State m_state;

	sf::Text m_text;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

/// <summary>
///
/// </summary>
class Slider : public sf::Drawable
{
public:
	~Slider();

	Slider() = delete;

	///\brief
	///
	///
	Slider(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, double  radius, double angle = 0, sf::Text text = sf::Text());
	Slider(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, sf::Text text = sf::Text());

	enum State {
		idle,
		hoverBox,
		hoverSlide,
		press
	};


	virtual void uptadeState();
	State getState() const;
	bool isPressed() const noexcept;

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	float getLerp() const;
	float getLerp(float min, float max) const;

	void setSlide();
	void setLerp(double num,double min,double max);
	void setAngle(double newAngle);

	std::unique_ptr<sf::Texture> m_idleTexture;
	std::unique_ptr<sf::Texture> m_sliderTexture;
	std::unique_ptr<sf::Texture> m_hoverTexture;
	std::unique_ptr<sf::Texture> m_hoverSliderTexture;
	std::unique_ptr<sf::Texture> m_pressedSliderTexture;

protected:

	sf::RenderWindow* m_window;

	sf::Text m_text;

	double m_Radius;
	double m_angle;

	sf::Vector2u m_size;
	sf::Vector2f m_origin;
	sf::Vector2f m_pos;

	State m_state;

	void build(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, double angle, double radius, sf::Text text);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

/*
class circleSlider : public sf::Drawable, public Slider {
	

protected:


	void build(sf::RenderWindow* window, sf::Vector2f origin, sf::Vector2u size, double angle, double radius, sf::Text text);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};*/
#endif // !BOUTON
