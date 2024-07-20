#ifndef SDL2LIB_HPP
#define SDL2LIB_HPP

#include "../include/GraphicLib.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <dlfcn.h>

/* Function pointers typedef */
typedef SDL_Window*	(*createWindow_sdl)(unsigned int, unsigned int, const char*);
typedef void		(*windowClear_sdl)(SDL_Window*);
typedef void		(*windowDisplay_sdl)(SDL_Window*);
typedef bool		(*windowIsOpen_sdl)(SDL_Window*);
typedef void		(*windowClose_sdl)(SDL_Window*);
typedef bool		(*windowPollEvent_sdl)(SDL_Window*, SDL_Event*);

class SDL2Lib : public GraphicLib {
public:
    /* Canonical form */
    SDL2Lib();
    SDL2Lib(const SDL2Lib& ref);
    SDL2Lib& operator=(const SDL2Lib& ref);
    ~SDL2Lib();

    /* Real Constructor */
    SDL2Lib(int width, int height, const std::string title, const std::string path);
    
    virtual bool windowCreate();		/* Create window */
    virtual void clear();				/* Clear the window */
    virtual void display();				/* Display the window */
    virtual bool isOpen();				/* Check if the window is open */
    virtual void processEvents();		/* Process events */
    virtual void close();				/* Close the window */

private:
    void 		*dlPtr;					/* Pointer to the dynamic library */
    SDL_Window	*window;				/* Pointer to the window */
    int			width;					/* Width of the window */
    int			height;					/* Height of the window */
    std::string	title;					/* Title of the window */

    createWindow_sdl	winCreate;		/* Function pointer to createWindow */
    windowClear_sdl		winClear;		/* Function pointer to windowClear */
    windowDisplay_sdl	winDisplay;		/* Function pointer to windowDisplay */
    windowIsOpen_sdl	winIsOpen;		/* Function pointer to windowIsOpen */
    windowClose_sdl		winClose;		/* Function pointer to windowClose */
    windowPollEvent_sdl	winPollEvent;	/* Function pointer to windowPollEvent */
};

#endif /* SDL2LIB_HPP */