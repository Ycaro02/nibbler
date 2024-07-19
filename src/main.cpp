#include <iostream>
#include "../include/SFLib.hpp"

int main() {
	std::cout << "Hello, World!" << std::endl;

	GraphicLib *lib = new SFLib(1920, 1080, "SFML Test", "rsc/wrapperlib/SFMLWrapper.so");
	if (!lib->windowCreate()) {
		std::cerr << "Failed to create SFML window" << std::endl;
		return 1;
	}

	while (lib->isOpen()) {
		lib->processEvents();
		lib->clear();
		lib->display();
	}

	lib->close();
	delete lib;

    return 0;
}

