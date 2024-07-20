#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"
#include <iostream>

/* Typedef for construction wrapper function */
typedef void *(*GraphicLibConstuctor)(int, int, std::string , std::string);


/* SDL2 wrapper constructor */
void *SDL2ConstuctorWrapper(int width, int height, std::string title, std::string lib_path) {
	return (new SDL2Lib(width, height, title, lib_path));
}

/* SFML wrapper constructor */
void *SFLibConstuctorWrapper(int width, int height, std::string title, std::string lib_path) {
	return (new SFLib(width, height, title, lib_path));
}

/* Test function */
void graphicLibTest(std::string name, std::string lib_path, GraphicLibConstuctor constructor) {
	GraphicLib *lib = (GraphicLib *)constructor(1000, 800, name, lib_path);
	if (!lib->windowCreate()) {
		std::cerr << "Failed to create " << name << " window" << std::endl;
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
	graphicLibTest("SFML", "rsc/wrapperlib/SFMLWrapper.so", SFLibConstuctorWrapper);
	graphicLibTest("SDL2", "rsc/wrapperlib/SDL2Wrapper.so", SDL2ConstuctorWrapper);
    return (0);
}

