#pragma once
#include "Singleton.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/VideoMode.hpp>
class EnterWindow : public Singleton<EnterWindow> {
public:
	bool flag = false;
	sf::RenderWindow* window;
	void open() {
		textinput = "";
		flag = true;
		window = new sf::RenderWindow(sf::VideoMode(500, 200), "InputWindow");
	}
	void Close() {
		flag = false;
        window->close();
	}
	std::string getString() {
		return textinput;
	}
	void Clear() {
		textinput.clear();
	}
	void addChar(char c[]) {
		textinput += c[0];
        if (textinput.size() >= 2 && textinput.back() == '\n' && textinput[textinput.size() - 2] == '\n') {
            textinput.pop_back();
        }
	}
	void deleteChar() {
		if (textinput.size() > 0) {
			textinput.pop_back();
		}
	}
	void update(sf::Font& font) {
		text.setString(textinput);
		text.setCharacterSize(25);
		text.setPosition(p_);
		text.setFillColor(sf::Color::Black);
		text.setFont(font);
		window->draw(text);
	}
    void Scroll(int i, int j) {
        p_.x += i;
        p_.y += j;
    }
private:
    sf::Vector2f p_ = { 0, 0 };
	sf::Text text;
	std::string textinput;
};
