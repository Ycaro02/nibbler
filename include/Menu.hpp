#ifndef MENU_HPP
#define MENU

#include "BasicDefine.hpp"

class Button;

/* Menu class to handle menu */
class Menu {
	public:
	/* Canonical form */
	Menu();
	~Menu();
	Menu& operator=(const Menu &ref);
	Menu(const Menu &ref);

	/* Constructor */
	Menu(iVec2 startMenu, iVec2 sizeMenu, s32 nbBtn, char *btnTextDir);

	private:

	iVec2	start;
	iVec2	size;
	void 	*textBtnPress;
	void 	*textBtnUnpress;
	s32		btnNumber;
	Button	*btn;
};


#endif /* MENU_HPP */