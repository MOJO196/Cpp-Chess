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

position blackKingPos = {0, 4};
position whiteKingPos = {7, 4};

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

	//pawn Promotion
	if (gameState[input[0]][input[1]] == pieces::BP || gameState[input[0]][input[1]] == pieces::WP && (input[2] == 0 || input[2] == 7))
	{
		pawnPromotion(input);
	}

	if (gameState[input[0]][input[1]] == pieces::WK || gameState[input[0]][input[1]] == pieces::BK)
	{
		position newKingPos = { input[2], input[3] };

		if (whiteToMove) whiteKingPos = newKingPos;
		else blackKingPos = newKingPos;
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

	//getAllPossibleMoves
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
			possibleMoves = getQueenMoves(startPos, possibleMoves);
			break;
		case pieces::WK:
			possibleMoves = getKingMoves(startPos, possibleMoves);
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
			possibleMoves = getQueenMoves(startPos, possibleMoves);
			break;
		case pieces::BK:
			possibleMoves = getKingMoves(startPos, possibleMoves);
			break;
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!\n";
			return false;
		}
	}

	//visualizeMoves
	int moves[8][8] = { 0 };

	for (int i = 0; i < possibleMoves.size(); i++)
	{
		moves[possibleMoves[i].row][possibleMoves[i].col] = 1;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (moves[i][j] == 0) std::cout << "-- ";
			else std::cout << "XX ";
		}
		std::cout << "\n";
	}

	//validate

	//moveDoAble
	for (int i = 0; i < possibleMoves.size(); i++)
	{
		if (possibleMoves[i].col == endPos.col && possibleMoves[i].row == endPos.row) moveValid = true;
	}

	if (!moveValid) return false;
	
	//checks
	return moveValid;
}

bool checkForCheck(int input[])
{
	return false;
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
	int enemyPieces[6];

	if (whiteToMove)
	{
		if (gameState[startPos.row][startPos.col] == pieces::BP) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));

		direction = -1;
		startRow = 6;
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WP) return {};
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));

		direction = 1;
		startRow = 1;
	}

	//capture
	for (int i = -1; i < 2; i += 2)
	{
		for (int j = 0; j < 6; j++)
		{
			if (gameState[startPos.row + direction][startPos.col + i] == enemyPieces[j])
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
		for (int j = 1; j < 8; j++)
		{
			if (directions[i][0] * j + startPos.row <= 7 && directions[i][0] * j + startPos.row >= 0 &&
				directions[i][1] * j + startPos.col <= 7 && directions[i][1] * j + startPos.col >= 0)
			{
				if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == pieces::ES)
				{
					possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
				}
				else
				{
					for (size_t k = 0; k < 6; k++)
					{
						if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == enemyPieces[k])
						{
							possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
							break;
						}
					}

					j = 8;
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
					break;
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
		for (int j = 1; j < 8; j++)
		{
			if (directions[i][0] * j + startPos.row <= 7 && directions[i][0] * j + startPos.row >= 0 &&
				directions[i][1] * j + startPos.col <= 7 && directions[i][1] * j + startPos.col >= 0)
			{
				if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == pieces::ES)
				{
					possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
				}
				else
				{
					for (size_t k = 0; k < 6; k++)
					{
						if (gameState[directions[i][0] * j + startPos.row][directions[i][1] * j + startPos.col] == enemyPieces[k])
						{
							possibleMoves.push_back({ directions[i][0] * j + startPos.row, directions[i][1] * j + startPos.col });
							break;
						}
					}

					j = 8;
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
	int directions[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };
	int enemyPieces[6];

	if (whiteToMove)
	{
		if (gameState[startPos.row][startPos.col] == pieces::BK) return {};
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WK) return {};
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
