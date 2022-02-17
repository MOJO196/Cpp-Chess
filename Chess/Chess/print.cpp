#include <iostream>
#include "common.h"

const char* pieceNames[] = { "ES", "BP", "BR", "BN", "BB", "BQ", "BK", "WP", "WR", "WN", "WB", "WQ", "WK" };

extern int gameState[8][8];

void printGameState()
{
	std::cout << "  ";

	for (size_t i = 0; i < 8; i++)
	{
		std::cout << i << "  ";
	}

	std::cout << "\n";

	for (size_t i = 0; i < 8; i++)
	{
		std::cout << i << " ";

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