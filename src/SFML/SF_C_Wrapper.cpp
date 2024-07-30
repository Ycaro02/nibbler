/**
 * @file SF_C_Wrapper.cpp
 * @brief Implementation of the SFML wrapper in extern C to avoid name mangling
*/

/* Basic SFML include */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/* C++ include */
#include "../../include/BasicDefine.hpp"
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
		win->setFramerateLimit(60);
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
				else if (event.key.code == sf::Keyboard::A) { keyValue = NKEY_A ; }
				else if (event.key.code == sf::Keyboard::P) { keyValue = NKEY_P ; }
			}
		}
		return (keyValue);
	}

	/**
	 * @brief Draw a color tile with SFML
	 * @param window The window pointers
	 * @param tilePos, The position of the tile
	 * @param scale The scale of the tile
	 * @note If scale.x/y are equal to TILE_SIZE, we use TILE_SPACING to space the tiles
	*/
	void colorTileWrapper(sf::RenderWindow* window, iVec2 tilePos, iVec2 scale, u32 color) {
		s32 pixel_x, pixel_y; 
		u8 r, g, b, a;
		
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			TILE_POSITION_TO_PIXEL(tilePos, pixel_x, pixel_y);
		} else {
			pixel_x = tilePos.x;
			pixel_y = tilePos.y;
		}

		UINT32_TO_RGBA(color, r, g, b, a);

        // Create a rectangle shape for the tile
        sf::RectangleShape tile(sf::Vector2f(scale.x, scale.y));
        tile.setPosition(pixel_x, pixel_y);
        tile.setFillColor(sf::Color(r, g, b, a));

        // Draw the tile
        window->draw(tile);
    }


	/**
	 * @brief Draw a texture tile with SFML
	 * @param window The window pointers
	 * @param texture The texture to draw
	 * @param tilePos, The position of the tile
	 * @param scale The scale of the tile
	 * @note If scale.x/y are equal to TILE_SIZE, we use TILE_SPACING to space the tiles
	*/
	void drawTextureTileWrapper(sf::RenderWindow* window, sf::Texture *texture, iVec2 tilePos, iVec2 scale) {
		if (!texture) {
			return;
		}
		s32 pixel_x, pixel_y;

		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			TILE_POSITION_TO_PIXEL(tilePos, pixel_x, pixel_y);
		} else {
			pixel_x = tilePos.x;
			pixel_y = tilePos.y;
		}

		sf::Sprite sprite(*texture);

		// Calculate the scale factors
		f32 scaleX = static_cast<f32>(scale.x) / texture->getSize().x;
		f32 scaleY = static_cast<f32>(scale.y) / texture->getSize().y;

		// Set the sprite's scale to fill TILE_SIZE * TILE_SIZE area
		sprite.setScale(scaleX, scaleY);

		// Set the sprite's position
		sprite.setPosition(pixel_x, pixel_y); // Corrected the order of pixel_x and pixel_y

		// Draw the sprite
		window->draw(sprite);
	}

	/**
	 * @brief Load a texture with SFML
	 * @param window The window pointers
	 * @param path The path to the texture
	 * @return The texture pointer
	*/
	void *loadTextureWrapper(sf::RenderWindow* window, const char* path) {
		sf::Texture *texture = new sf::Texture();
	
		if (!texture->loadFromFile(path)) {
			// std::cerr << "Failed to load texture" << std::endl;
			delete texture;
			return (nullptr);
		}
		return (texture);
	}

	/**
	 * @brief Unload a texture with SFML
	 * @param texture The texture pointer
	*/
	void unloadTextureWrapper(sf::Texture *texture) {
		if (texture) {
			delete texture;
		}
	}

	/**
	 * @brief Raylib destructor
	 * @note Nothing to do here to keep compatibility with the SDL2 lib
	*/
	void libDestructorWrapper() {
		/* Nothing todo */
	}


	/**
	 * @brief Load a font with SFML
	 * @param path The path to the font
	 * @return The font pointer
	*/
	void *loadFontWrapper(const char* path) {
		sf::Font *font = new sf::Font();
		if (!font->loadFromFile(path)) {
			delete font;
			return (nullptr);
		}
		return (font);
	}


	/**
	 * @brief Unload a font with SFML
	 * @param font The font pointer
	*/
	void unloadFontWrapper(sf::Font *font) {
		if (font) {
			delete font;
		}
	}
	/**
	 * @brief Write a text with SFML
	 * @param window The window pointers
	 * @param text The text to write
	 * @return The text pointer
	*/
	void writeTextWrapper(sf::RenderWindow* window, sf::Font *font,char* text, iVec2 pos, u32 fontSize, u32 color) {
		sf::Text textObj;
		u8 r, g, b, a;

		UINT32_TO_RGBA(color, r, g, b, a);

		textObj.setFont(*font);
		textObj.setString(text);
		textObj.setCharacterSize(fontSize);
		textObj.setFillColor(sf::Color(r, g, b, a));
		textObj.setPosition(pos.x, pos.y);
		window->draw(textObj);
	}
}
