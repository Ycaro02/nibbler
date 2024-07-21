#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "short_type.hpp"
#include <vector>

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

	void detectSnakeExpension(Nibbler &ctx);
	void brutForceBodySpace(Nibbler &ctx, s32 currX, s32 currY, s32 maxX, s32 maxY);
	void bodyFollowHead(Nibbler &ctx, s32 oldX, s32 oldY);

	/* Snake move, move the snake in the direction (move head and body follow him)*/
	void SnakeMove(Nibbler &ctx, s32 direction);

	s32 &getHeadX();
	s32 &getHeadY();
	void setHeadX(s32 x);
	void setHeadY(s32 y);
	std::vector<s32> &getBodyX();
	std::vector<s32> &getBodyY();

	private:
	s32 headX;
	s32 headY;
	std::vector<s32> bodyX;
	std::vector<s32> bodyY;
};

#endif /* SNAKE_HPP */