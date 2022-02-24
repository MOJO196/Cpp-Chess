#include <iostream>
#include <array>
#include <vector>
#include "main.h"
#include "moveManager.h"
#include "print.h"

int moveCount;
bool gameIsRunning;
bool whiteToMove = true;
char restart;

int main()
{
	testingArea();

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

void testingArea()
{
	char betterInput[4] = { 'A', '1', 'C', '2' };
	int print[4];
	const int betterRows[9] = {-1, 7, 6, 5, 4, 3, 2, 1, 0 };

	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			print[i] = betterInput[i] - 65;
		}
		else
		{
			print[i] = betterRows[betterInput[i] - 48];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		std::cout << print[i] << std::endl;
	}
}
