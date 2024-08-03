#ifndef MENU_HPP
#define MENU_HPP

#include "BasicDefine.hpp"
#include <string>


/* Forward declaration */
class Button;
class Nibbler;
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
	Menu(iVec2 startMenu, iVec2 sizeMenu, iVec2 txtPause, s32 nbBtn);

	void displayMenu(GraphicLib *lib);
	void handleMenu(Nibbler &ctx, u32 key);

	void resetBtnState();

	/* Setter */
	void setTextureBtnPress(void *texture);
	void setTextureBtnUnpress(void *texture);
	void setCurrentBtn(u8 newBtn);

	/* Getter */
	void 	*getTextureBtnPress();
	void 	*getTextureBtnUnpress();
	Button	*getButton();
	u8		getCurrentBtn();

	private:

	iVec2	start;				/* Start x and y position of the menu */
	iVec2	size;				/* Size x and y of the menu */
	iVec2	txtPausePos;		/* Position of the pause text */
	void 	*textBtnPress;		/* Texture of the button pressed */
	void 	*textBtnUnpress;	/* Texture of the button unpressed */
	s32		btnNumber;			/* Number of button in the menu */
	Button	*btn;				/* Array of button */
	u8		currentBtn;			/* Current button selected */
};


#endif /* MENU_HPP */