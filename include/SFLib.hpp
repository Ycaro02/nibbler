#ifndef SFLIB_HPP
#define SFLIB_HPP

#include "../include/GraphicLib.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <dlfcn.h>

/* Function pointers typedef */
typedef sf::RenderWindow*	(*createWindow_sfml)(unsigned int, unsigned int, const char*);
typedef void				(*windowClear_sfml)(sf::RenderWindow*);
typedef void				(*windowDisplay_sfml)(sf::RenderWindow*);
typedef bool				(*windowIsOpen_sfml)(sf::RenderWindow*);
typedef void				(*windowClose_sfml)(sf::RenderWindow*);
typedef bool				(*windowPollEvent_sfml)(sf::RenderWindow*, sf::Event*);

class SFLib : public GraphicLib {
public:

	/* Canonical form */
    SFLib();
	SFLib(const SFLib& ref);
    SFLib& operator=(const SFLib& ref);
    ~SFLib();

	/* Real Constructor */
    SFLib(int width, int height, const std::string title, const std::string path);
    
	virtual bool windowCreate();				/* Create window */
    virtual void clear();						/* Clear the window */
    virtual void display();						/* Display the window */
    virtual bool isOpen();						/* Check if the window is open */
    virtual void processEvents(int *, int*);	/* Process events */
    virtual void close();						/* Close the window */

private:
    void*				dlPtr;			/* Pointer to the dynamic library */
    sf::RenderWindow*	window;			/* Pointer to the window */
    int					width;			/* Width of the window */
    int					height;			/* Height of the window */
    std::string			title;			/* Title of the window */

    createWindow_sfml		winCreate;		/* Function pointer to createWindow */
    windowClear_sfml		winClear;		/* Function pointer to windowClear */
    windowDisplay_sfml		winDisplay;		/* Function pointer to windowDisplay */
    windowIsOpen_sfml		winIsOpen;		/* Function pointer to windowIsOpen */
    windowClose_sfml		winClose;		/* Function pointer to windowClose */
    windowPollEvent_sfml	winPollEvent;	/* Function pointer to windowPollEvent */
};

#endif /* SFLIB_HPP */
