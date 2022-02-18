#include <iostream>
#include <array>
#include <vector>
#include "main.h"
#include "common.h"
#include "moveManager.h"
#include "print.h"

int moveCount;
bool gameIsRunning;
bool whiteToMove = true;
char restart;

int main()
{
	gameIsRunning = true;
	printGameState();

	for (;;) //game
	{
		getUserInput();

		if (!gameIsRunning) break;

		whiteToMove = !whiteToMove;

		moveCount++;
		std::cout << "\x1B[2J\x1B[H";
		printGameState();
	}

	std::cout << "\x1B[2J\x1B[H";
	printGameState();

	if (whiteToMove)
	{
		std::cout << "Black won within " << moveCount << " moves!" << std::endl;
	}
	else
	{
		std::cout << "White won within " << moveCount << " moves!" << std::endl;
	}

	std::cout << "Do you want to restart the game? [y/n]" << std::endl;
	std::cin >> restart;

	if (restart == 'y' || restart == 'Y')
	{
		gameIsRunning = false;
	}
}
