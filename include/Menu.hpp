#ifndef MENU_HPP
#define MENU

#include "BasicDefine.hpp"
#include <string>

/* Forward declaration */
class Button;
class GraphicLib;

/* Menu class to handle menu */
class Menu {
	public:
	/* Canonical form */
	Menu();
	~Menu();
	Menu& operator=(const Menu &ref);
	Menu(const Menu &ref);

	/* Constructor */
	Menu(iVec2 startMenu, iVec2 sizeMenu, iVec2 txtPause, s32 nbBtn, std::string btnTextDir);
	void displayMenu(GraphicLib *lib);

	private:

	iVec2	start;
	iVec2	size;
	iVec2	txtPausePos;
	void 	*textBtnPress;
	void 	*textBtnUnpress;
	s32		btnNumber;
	Button	*btn;
};


#endif /* MENU_HPP */