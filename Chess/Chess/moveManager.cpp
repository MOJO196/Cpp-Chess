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
std::vector<position> pawnMoves(position startPos)
{
	int direction = 0;
	int startRow = -1;
	int promoRow = -1;

	std::vector<position> possibleMoves{};
	std::vector<int> enumPosition{};

	if (whiteToMove)
	{
		if (gameState[startPos.row][startPos.col] == pieces::BP) return {};
		direction = -1;
		startRow = 6;
		promoRow = 0;

		enumPosition.insert(enumPosition.end(), { 1, 2, 3, 4, 5, 6 });
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WP) return {};
		direction = 1;
		startRow = 1;
		promoRow = 7;

		enumPosition.insert(enumPosition.end(), { 7, 8, 9, 10, 11, 12 });
	}

	//capture
	for (int i = -1; i < 2; i += 2)
	{
		for (int j = 0;  j < 6; j++)
		{
			if (gameState[startPos.row + direction][startPos.col + i] == enumPosition[j])
			{
				possibleMoves.push_back({ startPos.row + direction, startPos.col + i });
			}
		}
	}

	//move forwards
	if (gameState[startPos.row + direction][startPos.col] == pieces::ES)
	{

	}

	//promote
}

std::vector<position> rookMoves(position startPos)
{
	if (gameState[startPos.row][startPos.col] == pieces::BR && whiteToMove || (gameState[startPos.row][startPos.col] == pieces::WR && !whiteToMove)) return {};
	int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	std::vector<position> possibleMoves{};


	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 7; j++)
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

	return possibleMoves;
}
