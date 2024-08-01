#ifndef HANDLEACTION_HPP
#define HANDLEACTION_HPP

#include "BasicDefine.hpp"

#include <list>

/*  Forward declaration */
class Nibbler;
class HandleAction;

/* Structure to store a key and its action */
typedef struct keyAction_s {
    bool (HandleAction::*condition)(u32 key);	/* Condition to check */
    void (HandleAction::*action)(u32 key);		/* Action to do */
} keyAction;

/*  Class to handle the actions */
class HandleAction {
	public:
		HandleAction();
		~HandleAction();
		HandleAction(const HandleAction& ref);
		HandleAction& operator=(const HandleAction& ref);

		HandleAction(Nibbler &ctx);

		void actionProcess(u32 key);
		void setNewAction(keyAction actNew);
		u32 getActionNb() const;


		/*  List of conditions */
		bool isKeyChangeLib(u32 key);
		bool isKeyPause(u32 key);
		bool isMoveKey(u32 key);
		bool isMenuKey(u32 key);
		bool isChangeColor(u32 key);
		bool isExit(u32 key);


		/*  List of actions */
		void actionChangeLib(u32 key);
		void actionPause(u32 key);
		void actionMove(u32 key);
		void actionMenu(u32 key);
		void actionChangeColor(u32 key);
		void actionExit(u32 key);


	private:

		Nibbler 				&ctx;	/* Nibbler reference */
		std::list<keyAction>	act;	/* List of keyAction */

};

#endif /* HANDLEACTION_HPP */ 
