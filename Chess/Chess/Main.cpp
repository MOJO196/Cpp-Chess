#include <iostream>
#include <array>
#include <vector>
#include "main.h"
#include "common.h"
#include "moveManager.h"
#include "print.h"

int moveCount;
bool whiteToMove = true;

int main()
{
	printGameState();

	for (;;) //game
	{
		getUserInput();
		whiteToMove = !whiteToMove;

		moveCount++;
		std::cout << "\x1B[2J\x1B[H";
		printGameState();
	}
}