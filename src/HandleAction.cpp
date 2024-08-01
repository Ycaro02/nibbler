#include "../include/HandleAction.hpp"
#include "../include/GraphicLib.hpp"
#include "../include/Nibbler.hpp"
#include "../include/Menu.hpp"

HandleAction::HandleAction() : ctx(*(Nibbler *)nullptr) {
	act.clear();
}

HandleAction::~HandleAction() {
}

HandleAction::HandleAction(const HandleAction& ref) : ctx(ref.ctx) {
	*this = ref;
}

HandleAction& HandleAction::operator=(const HandleAction& ref) {
	if (this != &ref) {
		act = ref.act;
	}
	return (*this);
}

HandleAction::HandleAction(Nibbler &ctxNib) : ctx(ctxNib) {
	setNewAction({&HandleAction::isKeyPause, &HandleAction::actionPause});
	setNewAction({&HandleAction::isKeyChangeLib, &HandleAction::actionChangeLib});
	setNewAction({&HandleAction::isMoveKey, &HandleAction::actionMove});
	setNewAction({&HandleAction::isMenuKey, &HandleAction::actionMenu});
	setNewAction({&HandleAction::isChangeColor, &HandleAction::actionChangeColor});
}

void HandleAction::actionProcess(u32 key) {
    std::list<keyAction>::iterator it = act.begin();

    for (; it != act.end(); it++) {
        if ((this->*(it->condition))(key)) {
            (this->*(it->action))(key);
            break;
        }
    }
}

u32 HandleAction::getActionNb() const {
	return (act.size());
}


void HandleAction::setNewAction(keyAction actNew) {
	act.push_back(actNew);
}

/* Function to handle condition */

bool HandleAction::isKeyPause(u32 key) {
	return (key == NKEY_ESC || key == NKEY_P);
}

bool HandleAction::isKeyChangeLib(u32 key) {
	return (key != ctx.getCurrentLibIdx() && (key == NKEY_1 || key == NKEY_2 || key == NKEY_3));
}

bool HandleAction::isMoveKey(u32 key) {
	return (!ctx.getPause() && (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT));
}

bool HandleAction::isMenuKey(u32 key) {
	return (ctx.getPause() && (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_ENTER));
}

bool HandleAction::isChangeColor(u32 key) {
	return (key == NKEY_A);
}

/* Function to handle action */

void HandleAction::actionPause(u32 key) {
	(void)key;
	ctx.setPause(!ctx.getPause());
}

void HandleAction::actionChangeLib(u32 key) {
	GraphicLib *lib = ctx.getCurrentLib();

	ctx.setCurrentLibIdx(key);
	lib->close();
}

void HandleAction::actionMove(u32 key) {
	ctx.getSnake().handleSnakeDir(key);
}

void HandleAction::actionMenu(u32 key) {
	ctx.getCurrentLib()->getMenu()->handleMenu(ctx, key);
}

void HandleAction::actionChangeColor(u32 key) {
	(void)key;
	ctx.setColorMode(!(ctx.getColorMode()));
}