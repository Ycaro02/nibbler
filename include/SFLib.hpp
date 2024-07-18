#ifndef SFLIB_HPP
#define SFLIB_HPP

#include "../include/GraphicLib.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <dlfcn.h>

class SFLib : public GraphicLib {
public:
    SFLib();
    SFLib(const std::string& path, int width, int height, std::string& title);
    virtual ~SFLib();
    SFLib& operator=(const SFLib& ref);

    virtual bool init();
    virtual void clear();
    virtual void display();
    virtual bool isOpen();
    virtual void processEvents();
    virtual void close();

private:
    typedef sf::RenderWindow* (*createWindow_t)(unsigned int, unsigned int, const char*);
    typedef void (*destroyWindow_t)(sf::RenderWindow*);
    typedef void (*windowClear_t)(sf::RenderWindow*);
    typedef void (*windowDisplay_t)(sf::RenderWindow*);
    typedef bool (*windowIsOpen_t)(sf::RenderWindow*);
    typedef void (*windowClose_t)(sf::RenderWindow*);
    typedef bool (*windowPollEvent_t)(sf::RenderWindow*, sf::Event*);

    void* dlPtr;
    sf::RenderWindow* window;
    int width;
    int height;
    std::string title;

    createWindow_t createWindow;
    destroyWindow_t destroyWindow;
    windowClear_t windowClear;
    windowDisplay_t windowDisplay;
    windowIsOpen_t windowIsOpen;
    windowClose_t windowClose;
    windowPollEvent_t windowPollEvent;
};

#endif // SFLIB_HPP
