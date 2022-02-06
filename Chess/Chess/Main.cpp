#include <iostream>
#include <string>
#include "main.h"


enum pieces { ES, BP, BR, BN, BB, BQ, BK, WP, WR, WN, WB, WQ, WK };
const char* pieceNames[] = { "ES", "BP", "BR", "BN", "BB", "BQ", "BK", "WP", "WR", "WN", "WB", "WQ", "WK" };

int gameState[8][8] =
{
	{pieces::BR, pieces::BN, pieces::BB, pieces::BQ, pieces::BK, pieces::BB, pieces::BN, pieces::BR},
	{pieces::BP, pieces::BP, pieces::BP, pieces::BP, pieces::BP, pieces::BP, pieces::BP, pieces::BP},
	{pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES},
	{pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES},
	{pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES},
	{pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES, pieces::ES},
	{pieces::WP, pieces::WP, pieces::WP, pieces::WP, pieces::WP, pieces::WP, pieces::WP, pieces::WP},
	{pieces::WR, pieces::WN, pieces::WB, pieces::WQ, pieces::WK, pieces::WB, pieces::WN, pieces::WR}
};

int main()
{
	//define Stuff
	printGameState();

	for (;;) //Game
	{
		getUserInput();

		std::cout << "\x1B[2J\x1B[H";
		printGameState();
	}
}

void printGameState()
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			switch (gameState[i][j])
			{
			case pieces::ES:
				std::cout << "-- ";
				break;
			default:
				std::cout << pieceNames[gameState[i][j]] << " ";
				break;
			}
		}
		std::cout << "\n";
	}
}

void getUserInput()
{
	int input[4];

	for (;;)
	{
		std::cout << "Enter your start row!\n";
		std::cin >> input[0];
		std::cout << "Enter your start column!\n";
		std::cin >> input[1];
		std::cout << "Enter your end row!\n";
		std::cin >> input[2];
		std::cout << "Enter your start column!\n";
		std::cin >> input[3];

		if (validateMove(input)) break;
	}

	gameState[input[2]][input[3]] = gameState[input[0]][input[1]];
	gameState[input[0]][input[1]] = pieces::ES;
}

bool validateMove(int input[])
{
	for (size_t i = 0; i < 4; i++)
	{
		if (input[i] > 9 && input[i] < 0) return false;
	}

	return true;
	//check if move is valid
}
