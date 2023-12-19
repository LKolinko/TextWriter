#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Singleton.h"
#include "Screen.h"
class TextLabel : public Singleton<TextLabel> {
public:
	sf::RenderWindow* window = Screen::GetInstance()->window; 
	TextLabel() {
		strings.resize(1e5);
        strs_numbers_.resize(1e5);
	}
	~TextLabel() = default;
	void Create(sf::Font& font) {
		sf::Vector2u windowSize = window->getSize();
		size_ = sf::Vector2f((float)(windowSize.x) * 0.7, (float)(windowSize.y) * 0.98);
		textlable_.setSize(size_);
		title_.setFont(font);
		title_.setString("TextLabel:");
		title_.setFillColor(sf::Color({ 38, 15, 38 }));
		title_.setCharacterSize(25);
		textlable_.setPosition((float)windowSize.x * 0.01, (float)windowSize.y * 0.01);
		title_.setPosition(textlable_.getPosition().x, textlable_.getPosition().y);
		sf::Vector2f p = textlable_.getPosition();
		for (int i = start_; i < start_ + size_.y / 30 - 2; ++i) {
			p.y += 30;
			strs_numbers_[i].setFont(font);
			strs_numbers_[i].setFillColor(sf::Color({ 38, 15, 38 }));
			strs_numbers_[i].setPosition(p);
			strs_numbers_[i].setCharacterSize(title_.getCharacterSize() * 0.9);
			strs_numbers_[i].setString(std::to_string(i + 1) + ".");
		}
		textlable_.setFillColor(sf::Color({ 134, 203, 146 }));
	}
	sf::Vector2f getSize() {
		return size_;
	}
	void Up() {
		if (start_ != 0) {
			--start_;
		}
	}
	void Down() {
		++start_;
	}
	float getSizeText(int n) {
		return textlable_.getSize().x - strs_numbers_[n].getLocalBounds().width - 3;
	}
	void setStrings(std::vector<std::string>& a) {
        stringsSize_ = a.size();
		for (int i = 0; i < a.size(); ++i) {
			sf::Text text;
			text.setFillColor(sf::Color({ 38, 15, 38 }));
			text.setCharacterSize(25 * 0.9);
			text.setString(a[i]);
			strings[i] = text;
		}
	}
	void drawTo(sf::Font& font) {
		window->draw(textlable_);
		window->draw(title_);
		for (int i = start_; i < start_ + size_.y / 30 - 2; ++i) {
			if (i < stringsSize_) {
				strings[i].setPosition(strs_numbers_[i].getPosition().x + strs_numbers_[i].getLocalBounds().width + 3, strs_numbers_[i].getPosition().y);
				strings[i].setFont(font);
				window->draw(strings[i]);
			}
			window->draw(strs_numbers_[i]);
		}
	}
private:
	int start_ = 0;
    int stringsSize_ = 0;
	std::vector<sf::Text> strs_numbers_;
	std::vector<sf::Text> strings;
	sf::Text title_;
	sf::Vector2f size_;
	sf::RectangleShape textlable_;
};
