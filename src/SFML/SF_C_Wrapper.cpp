#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"

extern "C" {
    sf::RenderWindow* createWindowWrapper(u32 width,u32 height, const char* title) {
        return (new sf::RenderWindow(sf::VideoMode(width, height), title));
    }

    void windowClearWrapper(sf::RenderWindow* window) {
        // window->clear(sf::Color::Green);
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

	void colorTileWrapper(sf::RenderWindow* window, u32 y, u32 x, u8 r, u8 g, u8 b, u8 a) {
        // Convert tile coordinates to pixel coordinates
        s32 pixel_x = x * TILE_SIZE + (x + 1) * TILE_SPACING;
        s32 pixel_y = y * TILE_SIZE + (y + 1) * TILE_SPACING;

        // Create a rectangle shape for the tile
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        tile.setPosition(pixel_y, pixel_x);
        tile.setFillColor(sf::Color(r, g, b, a));

        // Draw the tile
        window->draw(tile);
    }
}
