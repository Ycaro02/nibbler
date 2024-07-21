#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"
#include <iostream>

/* Typedef for construction wrapper function */
typedef void *(*AGraphicLibConstuctor)(int, s32, std::string , std::string);

/* SDL2 wrapper constructor */
void *SDL2ConstuctorWrapper(int width, s32 height, std::string title, std::string lib_path) {
	return (new SDL2Lib(width, height, title, lib_path));
}

/* SFML wrapper constructor */
void *SFLibConstuctorWrapper(int width, s32 height, std::string title, std::string lib_path) {
	return (new SFLib(width, height, title, lib_path));
}

typedef struct NibblerCtx {
	AGraphicLib	*libs[2];
	int			currentLib;
	int 		isRunning;
} NibblerCtx;


void GlibTest(NibblerCtx *ctx) {
	AGraphicLib *currentLib = ctx->libs[ctx->currentLib];

	if (!currentLib->isOpen()) {
		if (!currentLib->windowCreate()) {
			std::cerr << "Failed to create window" << std::endl;
			return ;
		}
	}
	currentLib->clear();
	currentLib->display();
	currentLib->processEvents(&ctx->currentLib, &ctx->isRunning);
}

void GlibInit(AGraphicLib *libs[2]) {
	libs[SFML_IDX] = new SFLib(1000, 800, "SFML", "rsc/wrapperlib/SFMLWrapper.so");
	libs[SDL2_IDX] = new SDL2Lib(1000, 800, "SDL2", "rsc/wrapperlib/SDL2Wrapper.so");
}


int main() {
	NibblerCtx ctx;

	ctx.isRunning = 1;
	ctx.currentLib = SFML_IDX;
	ctx.libs[SFML_IDX] = nullptr;
	ctx.libs[SDL2_IDX] = nullptr;

	GlibInit(ctx.libs);
	while (ctx.isRunning) {
		GlibTest(&ctx);
	}

	delete ctx.libs[0];
	delete ctx.libs[1];
    
	return (0);
}

