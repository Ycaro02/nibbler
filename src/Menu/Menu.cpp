#include "../../include/Menu.hpp"
#include "../../include/Button.hpp"
#include "../../include/GraphicLib.hpp"

Menu::Menu() {
	start = {0, 0};
	size = {0, 0};
	txtPausePos = {0, 0};
	textBtnPress = nullptr;
	textBtnUnpress = nullptr;
	btnNumber = 0;
	btn = nullptr;
	
}

Menu::~Menu() {
}

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

Menu::Menu(const Menu &ref) {
	*this = ref;
}

Menu::Menu(iVec2 startMenu, iVec2 sizeMenu, iVec2 txtPause, s32 nbBtn, std::string btnTextDir)  {
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
		btn[i] = Button(startBtn, sizeBtn, "test", i);
		startBtn.y += sizeBtn.y + (sizeBtn.y >> 1);
	
	}
		
	// std::cout << "Btn start text Y: " << btn[0].nameStart.y << " x: "  << btn[0].nameStart.x << std::endl;
	(void)btnTextDir;
}

void Menu::displayMenu(GraphicLib *lib) {
	lib->colorTile(start, size, LIGHT_DARK_RGBA);
	lib->writeText("PAUSE", txtPausePos, FONT_SIZE, WHITE_RGBA);

	for (s32 i = 0; i < btnNumber; i++) {
		btn[i].drawButton(lib, textBtnUnpress);
	}
}

void Menu::setTextureBtnPress(void *texture) {
	textBtnPress = texture;
}

void Menu::setTextureBtnUnpress(void *texture) {
	textBtnUnpress = texture;
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