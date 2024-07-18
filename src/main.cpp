#include <iostream>

#include "../include/SFLib.hpp"

int main() {
	std::cout << "Hello, World!" << std::endl;

	std::string title = "SFML Test";

	GraphicLib *lib = new SFLib(1920, 1080, title);
	if (!lib->init()) {
		std::cerr << "Failed to initialize the graphics library" << std::endl;
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

