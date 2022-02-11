#include <iostream>
#include <array>
#include <vector>
#include "moveManager.h"
#include "common.h"

extern int gameState[8][8];
extern int moveLog[][6];
extern bool whiteToMove;
extern enum pieces;

int blackPieces[6] = { pieces::BP, pieces::BR, pieces::BN, pieces::BB, pieces::BQ, pieces::BK };
int whitePieces[6] = { pieces::WP, pieces::WR, pieces::WN, pieces::WB, pieces::WQ, pieces::WK };

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

	if (gameState[input[0]][input[1]] == pieces::BP || gameState[input[0]][input[1]] == pieces::WP && (input[2] == 0 || input[2] == 7))
	{
		pawnPromotion(input);
	}
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
			possibleMoves = getPawnMoves(startPos, possibleMoves);
			break;
		case pieces::WR:
			possibleMoves = getRookMoves(startPos, possibleMoves, false);
			break;
		case pieces::WN:
			possibleMoves = getKnightMoves(startPos, possibleMoves);
			break;
		case pieces::WB:
			possibleMoves = getBishopMoves(startPos, possibleMoves, false);
			break;
		case pieces::WQ:
			break;
		case pieces::WK:
			break;
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!\n";
			return false;
		}
	}
	else
	{
		switch (gameState[input[0]][input[1]])
		{
		case pieces::BP:
			possibleMoves = getPawnMoves(startPos, possibleMoves);
			break;
		case pieces::BR:
			possibleMoves = getRookMoves(startPos, possibleMoves, false);
			break;
		case pieces::BN:
			possibleMoves = getKnightMoves(startPos, possibleMoves);
			break;
		case pieces::BB:
			possibleMoves = getBishopMoves(startPos, possibleMoves, false);
			break;
		case pieces::BQ:
			break;
		case pieces::BK:
			break;
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!\n";
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

void pawnPromotion(int input[])
{
	//pawn Promotion

	for (;;)
	{
		int pieceID = -1;
		std::cout << "Enter the ID of a piece you want to promote!\n";
		std::cin >> pieceID;

		if (whiteToMove && pieceID == pieces::WN || pieceID == pieces::WB || pieceID == pieces::WR || pieceID == pieces::WQ)
		{
			gameState[input[2]][input[3]] = pieceID;
			break;
		}
		else if (!whiteToMove && pieceID == pieces::BN || pieceID == pieces::BB || pieceID == pieces::BR || pieceID == pieces::BQ)
		{
			gameState[input[2]][input[3]] = pieceID;
			break;
		}
	}
}

//moves
std::vector<position> getPawnMoves(position startPos, std::vector<position> possibleMoves)
{
	int direction = 0;
	int startRow = -1;
<<<<<<< HEAD
	int enemyPieces[6];
=======

	std::vector<position> possibleMoves{};
	std::vector<int> enemyPositionInEnum{};
<<<<<<< Updated upstream
=======
>>>>>>> 9364e8dd6286c53990efa63d8e854141d9efd582
>>>>>>> Stashed changes

	if (whiteToMove)
	{
		if (gameState[startPos.row][startPos.col] == pieces::BP) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));

		direction = -1;
		startRow = 6;
<<<<<<< HEAD
=======

		enemyPositionInEnum.insert(enemyPositionInEnum.end(), { 1, 2, 3, 4, 5, 6 });
<<<<<<< Updated upstream
=======
>>>>>>> 9364e8dd6286c53990efa63d8e854141d9efd582
>>>>>>> Stashed changes
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WP) return {};
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));

		direction = 1;
		startRow = 1;
<<<<<<< HEAD
=======

		enemyPositionInEnum.insert(enemyPositionInEnum.end(), { 7, 8, 9, 10, 11, 12 });
<<<<<<< Updated upstream
=======
>>>>>>> 9364e8dd6286c53990efa63d8e854141d9efd582
>>>>>>> Stashed changes
	}

	//capture
	for (int i = -1; i < 2; i += 2)
	{
		for (int j = 0; j < 6; j++)
		{
<<<<<<< Updated upstream
			if (gameState[startPos.row + direction][startPos.col + i] == enemyPositionInEnum[j])
=======
<<<<<<< HEAD
			if (gameState[startPos.row + direction][startPos.col + i] == enemyPieces[j])
=======
			if (gameState[startPos.row + direction][startPos.col + i] == enemyPositionInEnum[j])
>>>>>>> 9364e8dd6286c53990efa63d8e854141d9efd582
>>>>>>> Stashed changes
			{
				possibleMoves.push_back({ startPos.row + direction, startPos.col + i });
			}
		}
	}

	//move forwards
	if (gameState[startPos.row + direction][startPos.col] == pieces::ES)
	{
		possibleMoves.push_back({ startPos.row + direction, startPos.col });

		if (gameState[startPos.row + direction * 2][startPos.col] == pieces::ES && startPos.row == startRow)
		{
			possibleMoves.push_back({ startPos.row + direction * 2, startPos.col });
		}
	}

	return possibleMoves;
}

std::vector<position> getRookMoves(position startPos, std::vector<position> possibleMoves, bool queenMode)
{
	int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	int enemyPieces[6];

	if (whiteToMove)
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::BR) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::WR) return {};
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (directions[i][0] * j + startPos.row <= 7 && directions[i][0] * j + startPos.row >= 0 &&
				directions[i][1] * j + startPos.col <= 7 && directions[i][1] * j + startPos.col >= 0)
			{
				if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == pieces::ES)
				{
					possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
				}
				for (size_t k = 0; k < 6; k++)
				{
					if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == enemyPieces[k])
					{
						possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
					}
				}
			}
		}
	}

	return possibleMoves;
}

std::vector<position> getKnightMoves(position startPos, std::vector<position> possibleMoves)
{

	int directions[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1} };
	int enemyPieces[6];

	if (whiteToMove)
	{
		if (gameState[startPos.row][startPos.col] == pieces::BN) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WN) return {};
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));
	}

	for (int i = 0; i < 8; i++)
	{
		if (directions[i][0] + startPos.row <= 7 && directions[i][0] + startPos.row >= 0 &&
			directions[i][1] + startPos.col <= 7 && directions[i][1] + startPos.col >= 0)
		{
			if (gameState[directions[i][0] + startPos.row][directions[i][1] + startPos.col] == pieces::ES)
			{
				possibleMoves.push_back({ directions[i][0] + startPos.row, directions[i][1] + startPos.col });
			}
			for (size_t k = 0; k < 6; k++)
			{
				if (gameState[directions[i][0] + startPos.row][directions[i][1] + startPos.col] == enemyPieces[k])
				{
					possibleMoves.push_back({ directions[i][0] + startPos.row, directions[i][1] + startPos.col });
				}
			}
		}
	}

	return possibleMoves;
}

std::vector<position> getBishopMoves(position startPos, std::vector<position> possibleMoves, bool queenMode)
{
	int directions[4][2] = { {-1, 1}, {-1, -1}, {1, 1}, {-1, 1} };
	int enemyPieces[6];

	if (whiteToMove)
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::BB) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::WB) return {};
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (directions[i][0] * j + startPos.row <= 7 && directions[i][0] * j + startPos.row >= 0 &&
				directions[i][1] * j + startPos.col <= 7 && directions[i][1] * j + startPos.col >= 0)
			{
				if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == pieces::ES)
				{
					possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
				}
				for (size_t k = 0; k < 6; k++)
				{
					if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == enemyPieces[k])
					{
						possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
					}
				}
			}
		}
	}

	return possibleMoves;
}

std::vector<position> getQueenMoves(position startPos, std::vector<position> possibleMoves)
{
	std::vector<position> bishopMoves;

	possibleMoves = getRookMoves(startPos, possibleMoves, true);
	bishopMoves = getBishopMoves(startPos, possibleMoves, true);

	possibleMoves.insert(std::end(possibleMoves), std::begin(bishopMoves), std::end(bishopMoves));

	return possibleMoves;
}

std::vector<position> getKingMoves(position startPos, std::vector<position> possibleMoves)
{
	return possibleMoves;
}
