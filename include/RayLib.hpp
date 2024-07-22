#ifndef RayLib_HPP
#define RayLib_HPP

#include "../include/AGraphicLib.hpp"
#include "raylib.h" // to remove
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

class RayLib : public AGraphicLib {
public:
    /* Canonical form */
    RayLib();
    RayLib(const RayLib& ref);
    RayLib& operator=(const RayLib& ref);
    ~RayLib();

    /* Real Constructor */
    RayLib(int width, s32 height, const std::string title, const std::string path);
    
    virtual void processEvents(Nibbler& ctx);	/* Process events */
	virtual void close();						/* Close the graphics library */
};

#endif /* RayLib_HPP */