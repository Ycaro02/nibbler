#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../../include/GraphicLib.hpp"
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"

extern "C" {

	/**
	 * @brief Create a window with SFML
	 * @param width The width of the window
	 * @param height The height of the window
	 * @param title The title of the window
	 * @return Allocate object of sf::RenderWindow
	*/
    sf::RenderWindow* createWindowWrapper(u32 width,u32 height, const char* title) {
        sf::RenderWindow* win = new sf::RenderWindow(sf::VideoMode(width, height), title);
		if (!win) {
			std::cerr << "Failed to initialize SFML window" << std::endl;
			return (nullptr);
		}
		return (win);
	}


	/**
	 * @brief Clear the window with SFML
	 * @param window The window pointers
	*/
    void windowClearWrapper(sf::RenderWindow* window) {
        window->clear(sf::Color::Black);
    }

	/**
	 * @brief Display the window with SFML
	 * @param window The window pointers
	*/
    void windowDisplayWrapper(sf::RenderWindow* window) {
        window->display();
    }

	/**
	 * @brief Check if the window is open with SFML
	 * @param window The window pointers
	 * @return true if the window is open, false otherwise
	*/
    bool windowIsOpenWrapper(sf::RenderWindow* window) {
        return (window->isOpen());
    }

	/**
	 * @brief Close the window with SFML, and delete the window (allocated by new)
	 * @param window The window pointers
	*/
    void windowCloseWrapper(sf::RenderWindow* window) {
        window->close();
		delete window;
    }


	/**
	 * @brief Poll the event with SFML
	 * @param window The window pointers
	 * @return Normalised key value (Defined in include/GraphicLib.hpp)
	*/
    s32 windowPollEventWrapper(sf::RenderWindow* window) {
		sf::Event event = sf::Event();

		s32 keyValue = NKEY_INVALID;
		if (!window) {
			return (keyValue);
		}
		if (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return (NKEY_ESC);
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) { keyValue = NKEY_UP ; }
				else if (event.key.code == sf::Keyboard::Down) { keyValue = NKEY_DOWN ; }
				else if (event.key.code == sf::Keyboard::Left) { keyValue = NKEY_LEFT ; }
				else if (event.key.code == sf::Keyboard::Right) { keyValue = NKEY_RIGHT ; }
				else if (event.key.code == sf::Keyboard::Num1) { keyValue = NKEY_1 ; }
				else if (event.key.code == sf::Keyboard::Num2) { keyValue = NKEY_2 ; }
				else if (event.key.code == sf::Keyboard::Num3) { keyValue = NKEY_3 ; }
				else if (event.key.code == sf::Keyboard::Escape) { keyValue = NKEY_ESC ; }
			}
		}
		return (keyValue);
	}

	/**
	 * @brief Color a tile at x, y with r, g, b, a
	 * @param window The window pointers
	*/
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

	/**
	 * @brief Raylib destructor
	 * @note Nothing to do here to keep compatibility with the SDL2 lib
	*/
	void libDestructorWrapper() {
		/* Nothing todo */
	}
}
