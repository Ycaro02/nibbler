#ifndef SFLIB_HPP
#define SFLIB_HPP

#include "../include/GraphicLib.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <dlfcn.h>

/* Function pointers typedef */
typedef sf::RenderWindow*	(*createWindow_t)(unsigned int, unsigned int, const char*);
typedef void				(*destroyWindow_t)(sf::RenderWindow*);
typedef void				(*windowClear_t)(sf::RenderWindow*);
typedef void				(*windowDisplay_t)(sf::RenderWindow*);
typedef bool				(*windowIsOpen_t)(sf::RenderWindow*);
typedef void				(*windowClose_t)(sf::RenderWindow*);
typedef bool				(*windowPollEvent_t)(sf::RenderWindow*, sf::Event*);

class SFLib : public GraphicLib {
public:

	/* Canonical form */
    SFLib();
	SFLib(const SFLib& ref);
    SFLib& operator=(const SFLib& ref);
    virtual ~SFLib();

	/* Real Constructor */
    SFLib(const std::string& path, int width, int height, std::string& title);
    
	virtual bool init();			/* Create window need to rename */
    virtual void clear();			/* Clear the window */
    virtual void display();			/* Display the window */
    virtual bool isOpen();			/* Check if the window is open */
    virtual void processEvents();	/* Process events */
    virtual void close();			/* Close the window */

private:
    void*				dlPtr;		/* Pointer to the dynamic library */
    sf::RenderWindow*	window;		/* Pointer to the window */
    int					width;		/* Width of the window */
    int					height;		/* Height of the window */
    std::string			title;		/* Title of the window */

    createWindow_t		createWindow;		/* Function pointer to createWindow */
    destroyWindow_t		destroyWindow;		/* Function pointer to destroyWindow */
    windowClear_t		windowClear;		/* Function pointer to windowClear */
    windowDisplay_t		windowDisplay;		/* Function pointer to windowDisplay */
    windowIsOpen_t		windowIsOpen;		/* Function pointer to windowIsOpen */
    windowClose_t		windowClose;		/* Function pointer to windowClose */
    windowPollEvent_t	windowPollEvent;	/* Function pointer to windowPollEvent */
};

#endif // SFLIB_HPP
