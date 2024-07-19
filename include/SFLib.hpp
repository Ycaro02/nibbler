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
    ~SFLib();

	/* Real Constructor */
    SFLib(int width, int height, const std::string title, const std::string path);
    
	virtual bool windowCreate();	/* Create window need to rename */
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

    createWindow_t		winCreate;		/* Function pointer to createWindow */
    windowClear_t		winClear;		/* Function pointer to windowClear */
    windowDisplay_t		winDisplay;		/* Function pointer to windowDisplay */
    windowIsOpen_t		winIsOpen;		/* Function pointer to windowIsOpen */
    windowClose_t		winClose;		/* Function pointer to windowClose */
    windowPollEvent_t	winPollEvent;	/* Function pointer to windowPollEvent */
};

#endif /* SFLIB_HPP */
