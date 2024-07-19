#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

extern "C" {
    sf::RenderWindow* createWindowWrapper(unsigned int width, unsigned int height, const char* title) {
        return (new sf::RenderWindow(sf::VideoMode(width, height), title));
    }

    void windowClearWrapper(sf::RenderWindow* window) {
        window->clear(sf::Color::Black);
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
