#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../../include/short_type.hpp"

extern "C" {
    sf::RenderWindow* createWindowWrapper(u32 width,u32 height, const char* title) {
        return (new sf::RenderWindow(sf::VideoMode(width, height), title));
    }

    void windowClearWrapper(sf::RenderWindow* window) {
        window->clear(sf::Color::Green);
    }

    void windowDisplayWrapper(sf::RenderWindow* window) {
        window->display();
    }

    bool windowIsOpenWrapper(sf::RenderWindow* window) {
        return (window->isOpen());
    }

    void windowCloseWrapper(sf::RenderWindow* window) {
        window->close();
    }

    bool windowPollEventWrapper(sf::RenderWindow* window, sf::Event* event) {
        return (window->pollEvent(*event));
    }
}
