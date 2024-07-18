#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
	std::cout << "Hello, World!" << std::endl;

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}


	std::cout << "Goodbye, World!" << std::endl;
    return 0;
}

