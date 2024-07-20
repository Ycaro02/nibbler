#include <iostream>
#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"

void test_SFML() {
	std::cout << "SFML TEST!" << std::endl;

	GraphicLib *lib = new SFLib(1920, 1080, "SFML Test", "rsc/wrapperlib/SFMLWrapper.so");
	if (!lib->windowCreate()) {
		std::cerr << "Failed to create SFML window" << std::endl;
		return ;
	}

	while (lib->isOpen()) {
		lib->clear();
		lib->display();
		lib->processEvents();
	}

	delete lib;

}

void test_SDL2() {
	std::cout << "SDL2 TEST!" << std::endl;

	GraphicLib *lib = new SDL2Lib(1920, 1080, "SDL2 Test", "rsc/wrapperlib/SDL2Wrapper.so");
	if (!lib->windowCreate()) {
		std::cerr << "Failed to create SDL2 window" << std::endl;
		return ;
	}

	while (lib->isOpen()) {
		lib->clear();
		lib->display();
		lib->processEvents();
	}

	delete lib;
}

int main() {
	test_SFML();
	test_SDL2();
    return (0);
}

