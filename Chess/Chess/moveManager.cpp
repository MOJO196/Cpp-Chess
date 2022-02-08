#include <iostream>
#include <array>
#include <vector>
#include "moveManager.h"
#include "common.h"

extern int gameState[8][8];
extern int moveLog[][6];
extern bool whiteToMove;
extern enum pieces;

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
	std::vector<position> possibleMoves;
	position startPos = { input[0], input[1] };
	position endPos = { input[2], input[3] };

	if (whiteToMove) {
		switch (gameState[input[0]][input[1]])
		{
		case pieces::WP:
			moveValid = pawnMoves(input);
			break;
		case pieces::WR:
			possibleMoves = rookMoves(startPos);
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
			possibleMoves = rookMoves(startPos);
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

	//doMove
	for (int i = 0; i < possibleMoves.size(); i++)
	{
		if (possibleMoves[i].col == endPos.col && possibleMoves[i].row == endPos.row) return true;
	}

	return false;
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

std::vector<position> rookMoves(position startPos)
{
	if (gameState[startPos.row][startPos.col] == pieces::BR && whiteToMove || (gameState[startPos.row][startPos.col] == pieces::WR && !whiteToMove)) return {};
	int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	std::vector<position> possibleMoves{};


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (directions[i][0] * j + startPos.row < 7 && directions[i][0] * j + startPos.row > 0 &&
				directions[i][1] * j + startPos.col < 7 && directions[i][1] * j + startPos.col > 0)
			{
				//exeptions
				if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == pieces::ES)
				{
					possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
				}
				for (size_t k = 1; k < 7; k++)
				{
					if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == k)
					{
						if (whiteToMove)
						{
							possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
						}
					}
					else
					{
						if (!whiteToMove)
						{
							possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
						}
					}
				}
			}
			break;
		}
	}

	return {};
}