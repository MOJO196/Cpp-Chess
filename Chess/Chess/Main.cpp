#include <iostream>
#include <array>
#include <vector>

#include "moveManager.h"
#include "print.h"

int moveCount;
bool gameIsRunning;
bool whiteToMove = true;
char restart;

int main()
{
	clear();

	gameIsRunning = true;
	printGameState();
	std::cout << "\n" << "Score : " << evaluateScore() << "\n"; //The board could be modified, so I wont be 0 everytime

	for (;;) //game
	{
		getUserInput();

		if (!gameIsRunning) break;

		whiteToMove = !whiteToMove;

		moveCount++;
		clear();
		printGameState();

		std::cout << "\n" << "Score : " << evaluateScore() << "\n";
	}

	clear();
	printGameState();

	if (whiteToMove)
	{
		std::cout << "Black won within " << moveCount << " moves!\n";
	}
	else
	{
		std::cout << "White won within " << moveCount << " moves!\n";
	}

	std::cout << "Do you want to restart the game? [y/n]\n";
	std::cin >> restart;

	if (restart == 'y' || restart == 'Y')
	{
		gameIsRunning = false;
	}
}
