#ifndef SFLIB_HPP
#define SFLIB_HPP

#include "../include/AGraphicLib.hpp"
#include <SFML/Graphics.hpp>
#include <string>

/* Function pointers typedef */
// typedef sf::RenderWindow*	(*createWindow_sfml)(u32,u32, const char*);
// typedef void				(*windowClear_sfml)(sf::RenderWindow*);
// typedef void				(*windowDisplay_sfml)(sf::RenderWindow*);
// typedef bool				(*windowIsOpen_sfml)(sf::RenderWindow*);
// typedef void				(*windowClose_sfml)(sf::RenderWindow*);
// typedef bool				(*windowPollEvent_sfml)(sf::RenderWindow*, sf::Event*);

class SFLib : public AGraphicLib {
public:

	/* Canonical form */
    SFLib();
	SFLib(const SFLib& ref);
    SFLib& operator=(const SFLib& ref);
    ~SFLib();

	/* Real Constructor */
    SFLib(int width, s32 height, const std::string title, const std::string path);
    
    virtual void processEvents(int *, s32*);	/* Process events */
};

#endif /* SFLIB_HPP */
