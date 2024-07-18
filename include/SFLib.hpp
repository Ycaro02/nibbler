#ifndef SFLIB_HPP
#define SFLIB_HPP

#include "GraphicLib.hpp"
#include <SFML/Graphics.hpp>

class SFLib : public GraphicLib {
	public:
	/* Constructor */
	SFLib();
	SFLib(int width, int height, std::string &title);

	/* Destructor */
	virtual ~SFLib();

	/* Operator = overload */
	SFLib & operator = (const SFLib &ref);

	/* Initialize the graphics library */
	virtual bool init();

	/* Clear the screen */
	virtual void clear();

	/* Display the rendered content */
	virtual void display();

	/* Check if the window is open */
	virtual bool isOpen();

	/* Process events */
	virtual void processEvents();

	/* Close the graphics library */
	virtual void close();

	private:

	/* The window pointer */
	sf::RenderWindow	*window;
	int					width;
	int					height;
	std::string			title;
};

#endif /* SFLIB_HPP */