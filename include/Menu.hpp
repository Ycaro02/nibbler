#ifndef MENU_HPP
#define MENU_HPP

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
	void handleMenu(u32 key);


	/* Setter */
	void setTextureBtnPress(void *texture);
	void setTextureBtnUnpress(void *texture);

	/* Getter */
	void *getTextureBtnPress();
	void *getTextureBtnUnpress();
	Button *getButton();

	private:

	iVec2	start;
	iVec2	size;
	iVec2	txtPausePos;
	void 	*textBtnPress;
	void 	*textBtnUnpress;
	s32		btnNumber;
	Button	*btn;
	u8		currentBtn;
};


#endif /* MENU_HPP */