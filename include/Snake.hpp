#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "short_type.hpp"
#include <vector>

typedef struct iVec2_s {
	s32 x;
	s32 y;
} iVec2;

typedef enum BoardState_e {
	EMPTY = 0U,
	FOOD = 1U,
	SNAKE_HEAD = 2U,
	SNAKE_BODY = 3U
} BoardState;

typedef enum SnakeDirection_e {
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
} SnakeDirection;

class Nibbler;

class Snake {
	public:
	/* Canonical form */
	Snake();
	~Snake();
	Snake& operator=(const Snake &ref);
	Snake(const Snake &ref);

	/* Constructor */
	Snake(Nibbler &ctx, s32 x, s32 y);

	/* Snake eat (grow), when snake eats food add a new body part */
	void SnakeEat(Nibbler &ctx);

	void brutExpension(Nibbler &ctx);
	void brutForceBodySpace(Nibbler &ctx, s32 currX, s32 currY, s32 maxX, s32 maxY);
	void bodyFollowHead(Nibbler &ctx, s32 oldX, s32 oldY);

	/* Snake move, move the snake in the direction (move head and body follow him)*/
	void SnakeMove(Nibbler &ctx, s32 direction);
	void resetSnake();

	s32 &getHeadX();
	s32 &getHeadY();
	void setHeadX(s32 x);
	void setHeadY(s32 y);
	std::vector<iVec2> &getBody();

	private:
	iVec2 head;					/* Head position */
	iVec2 toAdd;				/* Position to add a new body part */
	std::vector<iVec2> body;	/* Body position */
};

#endif /* SNAKE_HPP */