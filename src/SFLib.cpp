#include "../include/SFLib.hpp"

SFLib::SFLib() : window(NULL), width(800), height(600) {}

SFLib::SFLib(int width, int height, std::string &title) : window(NULL), width(width), height(height), title(title) {}

SFLib::~SFLib() {
    delete window;
}

SFLib& SFLib::operator = (const SFLib &ref) {
	window = ref.window;
	return (*this);
}

bool SFLib::init() {
    window = new sf::RenderWindow(sf::VideoMode(this->width, this->height), this->title);
    return (window != nullptr);
}

void SFLib::clear() {
    window->clear(sf::Color::Black);
}

void SFLib::display() {
    window->display();
}

bool SFLib::isOpen() {
    return (window->isOpen());
}

void SFLib::processEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
}

void SFLib::close() {
    if (window->isOpen()) {
        window->close();
    }
}