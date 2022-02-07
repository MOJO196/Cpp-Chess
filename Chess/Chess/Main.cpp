#include <iostream>
#include <string>
#include <array>
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

struct position
{
	int xPos;
	int yPos;
};

//move class stores moves
//moves will be stored here to undo them
int moveLog[][6];

bool whiteToMove = true;

int main()
{
	/*
	//define stuff
	printGameState();

	for (;;) //game
	{
		getUserInput();
		whiteToMove = !whiteToMove;

		std::cout << "\x1B[2J\x1B[H";
		printGameState();
	}
	*/
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
		std::cout << "\nEnter your start row!\n";
		std::cin >> input[0];
		std::cout << "Enter your start column!\n";
		std::cin >> input[1];
		std::cout << "Enter your end row!\n";
		std::cin >> input[2];
		std::cout << "Enter your start column!\n";
		std::cin >> input[3];

		if (validateMove(input)) break;

		std::cout << "Invalid move!";
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

	bool moveValid = false;
	if (whiteToMove) {
		switch (gameState[input[0]][input[1]])
		{
		case pieces::WP:
			moveValid = pawnMoves(input);
			break;
		case pieces::WR:
			moveValid = rookMoves(input);
			break;
		case pieces::WN:
			break;
		case pieces::WB:
			break;
		case pieces::WQ:
			break;
		case pieces::WK:
			break;
		default:
			return false;
		}
	}
	else
	{
		switch (gameState[input[0]][input[1]])
		{
		case pieces::BP:
			moveValid = pawnMoves(input);
			break;
		case pieces::BR:
			moveValid = rookMoves(input);
			break;
		case pieces::BN:
			break;
		case pieces::BB:
			break;
		case pieces::BQ:
			break;
		case pieces::BK:
			break;
		default:
			return false;
		}
	}

	//check

	return moveValid;
	//check if move is valid
}

//moves
bool pawnMoves(int input[])
{
	if (whiteToMove)
	{
		if (gameState[input[0] - 1][input[1]] == pieces::ES && input[1] == input[3])
		{
			if (input[0] - 1 == input[2])
			{
				if (input[0] - 1 == 0)
				{
					//promote
				}

				return true;
			}
			else if (gameState[input[0] - 2][input[1]] == pieces::ES && input[0] - 2 == input[2] && input[0] == 6)
			{
				return true;
			}
			return false;
		}
		//capture
		if (input[0] - 1 == input[2] && input[1] - 1 == input[3] || input[0] - 1 == input[2] && input[1] + 1 == input[3])
		{
			if (gameState[input[2]][input[3]] == 0) return false;

			for (size_t i = 7; i < 13; i++)
			{
				if (gameState[input[2]][input[3]] == i) return false;
			}

			return true;
		}
		//casteling
	}
	else
	{
		if (gameState[input[0] + 1][input[1]] == pieces::ES && input[1] == input[3])
		{
			if (input[0] + 1 == input[2])
			{
				if (input[0] - 1 == 7)
				{
					//promote
				}

				return true;
			}
			else if (gameState[input[0] + 2][input[1]] == pieces::ES && input[0] + 2 == input[2] && input[0] == 1)
			{
				return true;
			}
			return false;
		}
		//capture
		if (input[0] + 1 == input[2] && input[1] - 1 == input[3] || input[0] + 1 == input[2] && input[1] + 1 == input[3])
		{
			for (size_t i = 0; i < 7; i++)
			{
				if (gameState[input[2]][input[3]] == i) return false;
			}

			return true;
		}
		//casteling
	}

	return false;
}

bool rookMoves(int input[])
{
	int directions[4][2] = {
		{-1, 0},
		{1, 0},
		{0, -1},
		{0, 1}
	};

	int r = 0;

	std::array<std::array<int, 2>, 4> possibleMoves;

	/*
	possibleMoves[0] = { 1, 2 };
	possibleMoves[1] = { 1, 2 };
	possibleMoves[2] = { 1, 2 };
	possibleMoves[3] = { 1, 2 };

	for (size_t i = 0; i < 4; i++)
	{
		std::cout << possibleMoves[i][0] << " " << possibleMoves[i][1] << std::endl;
	}
	*/


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (directions[i][0] * j + input[0] < 7 && directions[i][0] * j + input[0] > 0 &&
				directions[i][1] * j + input[1] < 7 && directions[i][1] * j + input[1] > 0)
			{
				//exeptions
				if (gameState[directions[i][0] * j + input[0]][directions[i][1] * j + input[1]] == pieces::ES)
				{
					possibleMoves[r] = { directions[i][0] * j + input[0], directions[i][1] * j + input[1] };
					r++;
				}
				for (size_t k = 1; k < 7; k++)
				{
					if (gameState[directions[i][0] * j + input[0]][directions[i][1] * j + input[1]] == k)
					{
						if (whiteToMove)
						{
							possibleMoves[r] = { directions[i][0] * j + input[0], directions[i][1] * j + input[1] };
							r++;
						}
					}
					else
					{
						if (!whiteToMove)
						{
							possibleMoves[r] = { directions[i][0] * j + input[0], directions[i][1] * j + input[1] };
							r++;
						}
					}
				}
			}
			break;
		}
	}

	return false;
}

