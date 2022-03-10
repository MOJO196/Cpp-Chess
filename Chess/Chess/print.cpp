#include <iostream>

#include "common.h"

const char* pieceNames[] = { "ES", "BP", "BR", "BN", "BB", "BQ", "BK", "WP", "WR", "WN", "WB", "WQ", "WK" };
const int betterRows[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };

extern int gameState[8][8];

void printGameState()
{
	std::cout << "  ";

	for (size_t i = 0; i < 8; i++)
	{
		std::cout << (char)(i + 65) << "  ";
	}

	std::cout << "\n";

	for (size_t i = 0; i < 8; i++)
	{
		std::cout << betterRows[i] << " ";

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
