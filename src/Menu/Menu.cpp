#include "../../include/Menu.hpp"
#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"


/* Default constructor */
Menu::Menu() {
	start = {0, 0};
	size = {0, 0};
	txtPausePos = {0, 0};
	textBtnPress = nullptr;
	textBtnUnpress = nullptr;
	btnNumber = 0;
	btn = nullptr;
	currentBtn = 0;
}

/* Destructor */
Menu::~Menu() {
	/* Button texture are unloaded when lib close a window */
	if (btn) {
		delete[] btn;
		btn = nullptr;
	}
}

/* Assignment operator */
Menu& Menu::operator=(const Menu &ref) {
	if (this != &ref) {
		start = ref.start;
		size = ref.size;
		textBtnPress = ref.textBtnPress;
		textBtnUnpress = ref.textBtnUnpress;
		btnNumber = ref.btnNumber;
		btn = ref.btn;
	}
	return (*this);
}

/* Copy constructor */
Menu::Menu(const Menu &ref) {
	*this = ref;
}

/** 
 * @brief Constructor
 * @startMenu Start position of the menu
 * @sizeMenu Size of the menu
 * @txtPause Position of the pause text
 * @nbBtn Number of button in the menu
*/
Menu::Menu(iVec2 startMenu, iVec2 sizeMenu, iVec2 txtPause, s32 nbBtn)  {
	start = startMenu;
	size = sizeMenu;
	txtPausePos = txtPause;
	btnNumber = nbBtn;

	iVec2 startBtn, sizeBtn;

	sizeBtn.x = size.x >> 1;
	sizeBtn.y = size.y >> 3;
	startBtn.x = start.x + (size.x >> 2);
	startBtn.y = start.y + sizeBtn.y + (sizeBtn.y >> 2);
	btn = new Button[btnNumber];
	for (s32 i = 0; i < btnNumber; i++) {
		btn[i] = Button(startBtn, sizeBtn);
		startBtn.y += sizeBtn.y + (sizeBtn.y >> 1);
		btn[i].setId(i);
	}
	btn[BTN_RESUME].setName("resume");
	btn[BTN_RESTART].setName("restart");
	btn[BTN_MODE].setName("mode");
	btn[BTN_QUIT].setName("quit");

	currentBtn = BTN_RESUME;
	btn[BTN_RESUME].setState(BTN_PRESSED);
}

/**
 * @brief Display the menu
 * @lib GraphicLib pointer
*/
void Menu::displayMenu(GraphicLib *lib) {
	u8 btnState;
	lib->colorTile(start, size, LIGHT_DARK_RGBA);
	lib->writeText("PAUSE", txtPausePos, lib->getFontSize(), WHITE_RGBA);
	for (s32 i = 0; i < btnNumber; i++) {
		btnState = btn[i].getState();
		if (btnState == BTN_PRESSED) {
			btn[i].drawButton(lib, textBtnPress, YELLOW_RGBA);
		}
		else {
			btn[i].drawButton(lib, textBtnUnpress, WHITE_RGBA);
		}
	}
}

/**
 * @brief Handle the menu
 * @ctx Nibbler reference
 * @key Key pressed
*/
void Menu::handleMenu(Nibbler &ctx, u32 key) {
	if (key == NKEY_UP) {
		btn[currentBtn].setState(BTN_UNPRESS);
		currentBtn = (currentBtn == 0) ? btnNumber - 1 : currentBtn - 1;
		btn[currentBtn].setState(BTN_PRESSED);
	}
	else if (key == NKEY_DOWN) {
		btn[currentBtn].setState(BTN_UNPRESS);
		currentBtn = (currentBtn == btnNumber - 1) ? 0 : currentBtn + 1;
		btn[currentBtn].setState(BTN_PRESSED);
	} else if (key == NKEY_ENTER) {
		if (currentBtn == BTN_RESUME) {
			ctx.setPause(false);
		} else if (currentBtn == BTN_RESTART) {
			ctx.resetGame();
			ctx.setPause(false);
		} else if (currentBtn == BTN_MODE) {
			std::cout << "Mode need to be implemented" << std::endl;
		} else if (currentBtn == BTN_QUIT) {
			ctx.setIsRunning(0);
		}
	}
}

void Menu::resetBtnState() {
	for (s32 i = 0; i < btnNumber; i++) {
		btn[i].setState(BTN_UNPRESS);
	}
	btn[BTN_RESUME].setState(BTN_PRESSED);
	currentBtn = BTN_RESUME;
}

/* Getter/Setter */

void Menu::setTextureBtnPress(void *texture) {
	textBtnPress = texture;
}

void Menu::setTextureBtnUnpress(void *texture) {
	textBtnUnpress = texture;
}

u8 Menu::getCurrentBtn() {
	return (currentBtn);
}

void Menu::setCurrentBtn(u8 newBtn) {
	currentBtn = newBtn;
}

void *Menu::getTextureBtnPress() {
	return (textBtnPress);
}

void *Menu::getTextureBtnUnpress() {
	return (textBtnUnpress);
}

Button *Menu::getButton() {
	return (btn);
}