#include "../../include/SFLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Color.hpp"
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
: AGraphicLib(width, height, title, path, SFML_IDX)
{}

// void SFLib::close() {
// 	if (this->window) {
// 		this->winClose(this->window);
// 		delete (sf::Window *)this->window;
// 	}
// }