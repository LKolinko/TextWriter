#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "Screen.h"
#include "ButtonLabel.h"
#include "TextLabel.h"
#include "EnterWindow.h"
#include <iostream>
#include <vector>
int main() {
    Screen::GetInstance()->Init();
    sf::Font font;
    font.loadFromFile("arialmt.ttf");
    ButtonLable::GetInstance()->create(Screen::GetInstance()->window->getSize(), 10, font);
    TextLabel::GetInstance()->Create(font);
    auto window = Screen::GetInstance()->window;
    bool mouse_passed = true;
    while (window->isOpen()) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (mouse_passed) {
                    ButtonLable::GetInstance()->OnClick(font);
                }
                mouse_passed = false;
            } else {
                mouse_passed = true;
                ButtonLable::GetInstance()->noClick();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                TextLabel::GetInstance()->Up();
                TextLabel::GetInstance()->Create(font);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                TextLabel::GetInstance()->Down();
                TextLabel::GetInstance()->Create(font);
            }
            // изменение размера окна
            if (event.type == sf::Event::Resized) {
                sf::Vector2f windowSize = sf::Vector2f(event.size.width, event.size.height);

                window->setView(sf::View(sf::Vector2f(windowSize.x / 2.f,
                                                      windowSize.y / 2.f), sf::Vector2f(windowSize)));
                ButtonLable::GetInstance()->create(Screen::GetInstance()->window->getSize(), 10, font);
                TextLabel::GetInstance()->Create(font);
                ButtonLable::GetInstance()->TransferDown(font);
                ButtonLable::GetInstance()->TransferUp(font);
            }
        }
        if (EnterWindow::GetInstance()->flag) {
            sf::Event enterevent{};
            while (EnterWindow::GetInstance()->window->pollEvent(enterevent)) {
                if (enterevent.type == sf::Event::Closed) {
                    EnterWindow::GetInstance()->Close();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    char c[] = { '\n' };
                    EnterWindow::GetInstance()->addChar(c);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    EnterWindow::GetInstance()->Scroll(-5, 0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    EnterWindow::GetInstance()->Scroll(5, 0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    EnterWindow::GetInstance()->Scroll(0, -5);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    EnterWindow::GetInstance()->Scroll(0, 5);
                }
                if (enterevent.type == sf::Event::TextEntered) {
                    if (enterevent.key.code == 8 || enterevent.key.code == 127) {
                        EnterWindow::GetInstance()->deleteChar();
                    } else {
                        char c[] = { enterevent.key.code };
                        EnterWindow::GetInstance()->addChar(c);
                    }
                }

            }
        }
        window->clear(sf::Color::White);
        ButtonLable::GetInstance()->toDrawOn();
        TextLabel::GetInstance()->drawTo(font);
        if (EnterWindow::GetInstance()->flag) {
            EnterWindow::GetInstance()->window->clear(sf::Color::White);
            EnterWindow::GetInstance()->update(font);
            EnterWindow::GetInstance()->window->display();
        }
        window->display();
    }
    return 0;
}
