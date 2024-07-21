#ifndef SDL2LIB_HPP
#define SDL2LIB_HPP

#include "../include/AGraphicLib.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <dlfcn.h>

/* Function pointers typedef */
// typedef SDL_Window*	(*createWindow_sdl)(u32,u32, const char*);
// typedef void		(*windowClear_sdl)(SDL_Window*);
// typedef void		(*windowDisplay_sdl)(SDL_Window*);
// typedef bool		(*windowIsOpen_sdl)(SDL_Window*);
// typedef void		(*windowClose_sdl)(SDL_Window*);
// typedef bool		(*windowPollEvent_sdl)(SDL_Window*, SDL_Event*);
// typedef void		(*SDL2LibDestructor_sdl)();

class SDL2Lib : public AGraphicLib {
public:
    /* Canonical form */
    SDL2Lib();
    SDL2Lib(const SDL2Lib& ref);
    SDL2Lib& operator=(const SDL2Lib& ref);
    ~SDL2Lib();

    /* Real Constructor */
    SDL2Lib(int width, s32 height, const std::string title, const std::string path);
    
    virtual void processEvents(int *,int *);	/* Process events */
};

#endif /* SDL2LIB_HPP */