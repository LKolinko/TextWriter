#pragma once
#include "Screen.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
class Button {
public:
	sf::RenderWindow* window = Screen::GetInstance()->window;
	Button() = default;
	void setText(std::string btnText, int charSize, sf::Color textColor, sf::Font& font) {
		text.setFont(font);
		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
	}
	void setBackColor(sf::Color color) {
		button.setFillColor(color);
	}
	void setSize(sf::Vector2f size) {
		button.setSize(size);
		btnWidth = size.x;
		btnHeight = size.y;
	}
	void setPosition(sf::Vector2f point) {
		button.setPosition(point);
		float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (point.y + btnHeight / 2.7) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}
	void drawTo() {
		window->draw(button);
		window->draw(text);
	}
	bool isMouseOver() {
		int mouseX = sf::Mouse::getPosition(*(window)).x;
		int mouseY = sf::Mouse::getPosition(*(window)).y;
		int btnPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;
		int btnxPosWidth = button.getPosition().x + btnWidth;
		int btnyPosHeight = button.getPosition().y + btnHeight;
		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;
		}
		return false;
	}
private:
	sf::RectangleShape button;
	sf::Text text;
	int btnWidth;
	int btnHeight;
};

