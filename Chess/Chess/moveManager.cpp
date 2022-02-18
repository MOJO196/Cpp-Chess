#include <iostream>
#include <array>
#include <vector>
#include <exception>
#include "moveManager.h"
#include "common.h"
#include "print.h"

extern int gameState[8][8];
extern int moveCount;
extern bool whiteToMove;
extern bool gameIsRunning;
extern enum pieces;

int blackPieces[6] = { pieces::BP, pieces::BR, pieces::BN, pieces::BB, pieces::BQ, pieces::BK };
int whitePieces[6] = { pieces::WP, pieces::WR, pieces::WN, pieces::WB, pieces::WQ, pieces::WK };

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

position blackKingPos = { 0, 4 };
position whiteKingPos = { 7, 4 };

std::vector<move> moveLog{};

void getUserInput()
{
	int input[4];
	char action;

	for (;;)
	{
		std::cout << std::endl << "Enter what you want to do undo[u], move[m]" << std::endl;
		std::cin >> action;

		if (action == 'm')
		{
			std::cout << "Enter your start row!" << std::endl;
			std::cin >> input[0];
			std::cout << "Enter your start column!" << std::endl;
			std::cin >> input[1];
			std::cout << "Enter your end row!" << std::endl;
			std::cin >> input[2];
			std::cout << "Enter your start column!" << std::endl;
			std::cin >> input[3];

			if (validateMove(input))
			{
				moveLog.push_back({ {input[0], input[1]}, {input[2], input[3]}, gameState[input[0]][input[1]], gameState[input[2]][input[3]] });

				gameState[input[2]][input[3]] = gameState[input[0]][input[1]];
				gameState[input[0]][input[1]] = pieces::ES;
				break;
			}

			if (!gameIsRunning) break;

			std::cout << "\x1B[2J\x1B[H";
			printGameState();

			std::cout << std::endl << "Invalid move!" << std::endl;
		}
		else if (action == 'u')
		{
			if (moveCount > 0)
			{
				gameState[moveLog.back().startPos.row][moveLog.back().startPos.col] = moveLog.back().startPiece;
				gameState[moveLog.back().endPos.row][moveLog.back().endPos.col] = moveLog.back().endPiece;

				moveLog.pop_back();
				moveCount--;

				whiteToMove = !whiteToMove;

				std::cout << "\x1B[2J\x1B[H";
				printGameState();
			}
			else
			{
				std::cout << "You can not undo your the first move!" << std::endl;
			}
		}
		else
		{
			std::cout << "\x1B[2J\x1B[H";
			printGameState();

			std::cout << std::endl << "Invalid actiion!" << std::endl;
		}
	}

	//pawn Promotion
	if (gameState[input[0]][input[1]] == pieces::BP || gameState[input[0]][input[1]] == pieces::WP && (input[2] == 0 || input[2] == 7))
	{
		pawnPromotion(input);
	}

	if (gameState[input[0]][input[1]] == pieces::WK || gameState[input[0]][input[1]] == pieces::BK)
	{
		position newKingPos = { input[2], input[3] };

		if (whiteToMove)
		{
			whiteKingPos = newKingPos;
		}
		else
		{
			blackKingPos = newKingPos;
		}
	}
}

bool validateMove(int input[])
{
	for (size_t i = 0; i < 4; i++) if (input[i] > 9 && input[i] < 0)
	{
		return false;
	}

	bool moveValid = false;
	std::vector<position> possibleMoves;
	position startPos = { input[0], input[1] };
	position endPos = { input[2], input[3] };

	possibleMoves = getPossibleMoves(startPos, endPos, possibleMoves);

	if (possibleMoves[0].row == -1)
	{
		return false;
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
		std::cout << std::endl;
	}

	//validate

	//is move doable
	for (int i = 0; i < possibleMoves.size(); i++)
	{
		if (possibleMoves[i].col == endPos.col && possibleMoves[i].row == endPos.row)
		{
			moveValid = true;
			break;
		}
	}

	if (!moveValid)
	{
		return false;
	}

	//checks
	moveValid = checkForCheck(endPos, startPos);

	return moveValid;
}

bool checkForCheck(position endPos, position startPos)
{
	int movedPiece = gameState[startPos.row][startPos.col];
	int replacedPiece = gameState[endPos.row][endPos.col];
	int enemyPieces[6];

	position kingPos;
	position piecePos;
	std::vector<position> moves{};

	gameState[endPos.row][endPos.col] = gameState[startPos.row][startPos.col];
	gameState[startPos.row][startPos.col] = pieces::ES;

	if (whiteToMove)
	{
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
		kingPos = whiteKingPos;
	}
	else
	{
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));
		kingPos = blackKingPos;
	}

	whiteToMove = !whiteToMove;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (gameState[i][j] != pieces::ES)
			{
				for (int k = 0; k < 6; k++)
				{
					if (gameState[i][j] == enemyPieces[k])
					{
						piecePos = { i, j };
						moves = getPossibleMoves(piecePos, { -1 , -1 }, moves);

						for (int l = 0; l < moves.size(); l++)
						{
							if (moves[l].row == -1)
							{
								throw std::runtime_error("This should never happen!");
							}

							if (moves[l].row == kingPos.row && moves[l].col == kingPos.col)
							{
								char giveUp;

								gameState[startPos.row][startPos.col] = movedPiece;
								gameState[endPos.row][endPos.col] = replacedPiece;

								whiteToMove = !whiteToMove;

								std::cout << "Your king is in danger!" << std::endl;
								std::cout << "Do you want to give up? [y/n]" << std::endl;
								std::cin >> giveUp;

								if (giveUp == 'y' || giveUp == 'Y')
								{
									gameIsRunning = false;
								}

								return false;
							}
						}
					}
				}
			}
		}
	}

	gameState[startPos.row][startPos.col] = movedPiece;
	gameState[endPos.row][endPos.col] = replacedPiece;

	whiteToMove = !whiteToMove;

	return true;
}

bool casteling(position startPos, position endPos)
{
	std::vector<position> piecesBetween;

	//get positions in between
	if (whiteToMove)
	{
		if (endPos.col = 0)
		{

		}
		else if (endPos.col = 7)
		{

		}
	}
	else
	{

	}

	//for every position

	return true;
}

std::vector<position> getPossibleMoves(position startPos, position endPos, std::vector<position> possibleMoves)
{
	if (whiteToMove) {
		switch (gameState[startPos.row][startPos.col])
		{
		case pieces::WP:
			return getPawnMoves(startPos, possibleMoves);
		case pieces::WR:
			return getRookMoves(startPos, possibleMoves, false);
		case pieces::WN:
			return getKnightMoves(startPos, possibleMoves);
		case pieces::WB:
			return getBishopMoves(startPos, possibleMoves, false);
		case pieces::WQ:
			return getQueenMoves(startPos, possibleMoves);
		case pieces::WK:
			if (gameState[endPos.row][endPos.col] == pieces::BR && endPos.row == 0)
			{
				casteling(startPos, endPos);
				return { { -2, -2 } };
			}

			return getKingMoves(startPos, possibleMoves);
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!" << std::endl;
			return possibleMoves = { { -1, -1 } };
		}
	}
	else
	{
		switch (gameState[startPos.row][startPos.col])
		{
		case pieces::BP:
			return getPawnMoves(startPos, possibleMoves);
		case pieces::BR:
			return getRookMoves(startPos, possibleMoves, false);
		case pieces::BN:
			return getKnightMoves(startPos, possibleMoves);
		case pieces::BB:
			return getBishopMoves(startPos, possibleMoves, false);
		case pieces::BQ:
			return getQueenMoves(startPos, possibleMoves);
		case pieces::BK:
			if (gameState[endPos.row][endPos.col] == pieces::BR && endPos.row == 0)
			{
				casteling(startPos, endPos);
				return { { -2, -2 } };
			}

			return getKingMoves(startPos, possibleMoves);
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!" << std::endl;
			return possibleMoves = { { -1, -1 } };
		}
	}
}

void pawnPromotion(int input[])
{
	//pawn Promotion

	for (;;)
	{
		int pieceID = -1;

		std::cout << "Enter the ID of a piece you want to promote!" << std::endl;
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
		if (gameState[startPos.row][startPos.col] == pieces::BP)
		{
			return {};
		}

		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));

		direction = -1;
		startRow = 6;
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WP)
		{
			return {};
		}

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
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::BR)
		{
			return {};
		}

		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::WR)
		{
			return {};
		}

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

					j = 8; //break out of two for loops
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
		if (gameState[startPos.row][startPos.col] == pieces::BN)
		{
			return {};
		}

		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WN)
		{
			return {};
		}

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
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::BB)
		{
			return {};
		}

		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (!queenMode) if (gameState[startPos.row][startPos.col] == pieces::WB)
		{
			return {};
		}

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

					j = 8; //break out of two for loops
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
		if (gameState[startPos.row][startPos.col] == pieces::BK)
		{
			return {};
		}

		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));
	}
	else
	{
		if (gameState[startPos.row][startPos.col] == pieces::WK)
		{
			return {};
		}

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
