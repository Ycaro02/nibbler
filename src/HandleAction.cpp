#include "../include/HandleAction.hpp"
#include "../include/GraphicLib.hpp"
#include "../include/Nibbler.hpp"
#include "../include/Menu.hpp"

/* Default Constructor */
HandleAction::HandleAction() : ctx(*(Nibbler *)nullptr) {
	act.clear();
}

/* Destructor */
HandleAction::~HandleAction() {
}

/* Copy Constructor */
HandleAction::HandleAction(const HandleAction& ref) : ctx(ref.ctx) {
	*this = ref;
}

/* Assignment operator */
HandleAction& HandleAction::operator=(const HandleAction& ref) {
	if (this != &ref) {
		act = ref.act;
	}
	return (*this);
}

/** @briefReal Constructor 
 *	@param ctxNib Nibbler reference
 *	@note This constructor set the action list
*/
HandleAction::HandleAction(Nibbler &ctxNib) : ctx(ctxNib) {
	setNewAction({&HandleAction::isExit, &HandleAction::actionExit});
	setNewAction({&HandleAction::isKeyPause, &HandleAction::actionPause});
	setNewAction({&HandleAction::isKeyChangeLib, &HandleAction::actionChangeLib});
	setNewAction({&HandleAction::isMoveKey, &HandleAction::actionMove});
	setNewAction({&HandleAction::isMenuKey, &HandleAction::actionMenu});
	setNewAction({&HandleAction::isChangeColor, &HandleAction::actionChangeColor});
}

/** @brief Process the action list
 *	@param key The key to process
 *	@note This function will process the action list and call the corresponding action
*/
void HandleAction::actionProcess(u32 key) {
    std::list<keyAction>::iterator it = act.begin();

    for (; it != act.end(); it++) {
        if ((this->*(it->condition))(key)) {
            (this->*(it->action))(key);
            break;
        }
    }
}

/* Getter for the action number */
u32 HandleAction::getActionNb() const {
	return (act.size());
}

/* Setter for a new action */
void HandleAction::setNewAction(keyAction actNew) {
	act.push_back(actNew);
}

/* Function to handle condition */

bool HandleAction::isExit(u32 key) {
	return (key == NKEY_ESC);
}

bool HandleAction::isKeyPause(u32 key) {
	return (key == NKEY_P);
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

void HandleAction::actionExit(u32 key) {
	(void)key;
	std::cout << "Act Exit\n";
	ctx.setIsRunning(0);
}