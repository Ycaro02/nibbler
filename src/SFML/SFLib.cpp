#include "../../include/SFLib.hpp"
#include "../../include/Nibbler.hpp"
#include <iostream>


/* Canonical form */

SFLib::SFLib() 
: AGraphicLib()
{}

SFLib::~SFLib() {
    if (this->window) {
		delete (sf::Window *)this->window;
    }
    if (this->dlPtr) {
        dlclose(this->dlPtr);
    }
}

SFLib& SFLib::operator=(const SFLib& ref) {
	AGraphicLib::operator=(ref);
	return (*this);
}

SFLib::SFLib(const SFLib& ref)
: AGraphicLib(ref)
{}

/* Classic constructor */

SFLib::SFLib(int width, s32 height, const std::string title, const std::string path) 
: AGraphicLib(width, height, title, path)
{}

/* Wrapper function */
void SFLib::processEvents(Nibbler &ctx) {
    sf::Event event = sf::Event();

    while (this->window && this->winPollEvent(this->window, &event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
			// *isRunning = 0;
			ctx.setIsRunning(0);
        } else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Num1) {
				// std::cout << "SFML Key 1 pressed" << std::endl;
				ctx.setCurrentLibIdx(SFML_IDX);
			} else if (event.key.code == sf::Keyboard::Num2) {
				// std::cout << "SFML Key 2 pressed, close SFML window" << std::endl;
				ctx.setCurrentLibIdx(SDL2_IDX);
				this->close();
				this->window = nullptr;
			}
		}
    }
}

void SFLib::close() {
	if (this->window) {
		this->winClose(this->window);
		delete (sf::Window *)this->window;
	}
}