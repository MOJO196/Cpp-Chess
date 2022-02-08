#include <iostream>
#include <array>
#include <vector>
#include "main.h"
#include "common.h"
#include "moveManager.h"

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

//move class stores moves
//moves will be stored here to undo them
int moveLog[][6];

bool whiteToMove = true;

int main()
{
	//define stuff
	printGameState();

	for (;;) //game
	{
		getUserInput();
		whiteToMove = !whiteToMove;

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
