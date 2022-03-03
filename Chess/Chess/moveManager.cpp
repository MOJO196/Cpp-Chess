#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <exception>
#include "moveManager.h"
#include "common.h"
#include "print.h"

extern int gameState[8][8];
extern int moveCount;
extern bool whiteToMove;
extern bool gameIsRunning;
extern enum pieces;

const int betterRows[9] = { -1, 7, 6, 5, 4, 3, 2, 1, 0 };

bool enPassant = false;

int score = 0;
int scoreLookup[5] = { 1, 5, 3, 3, 9 };

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

castleManager casManager = { false, -1 };

castling validCastleDirections[4] =
{
	{{0, 4}, {0, 2}, {0, 0}, {0, 3}},
	{{0, 4}, {0, 6}, {0, 7}, {0, 5}},
	{{7, 4}, {7, 2}, {7, 0}, {7, 3}},
	{{7, 4}, {7, 6}, {7, 7}, {7, 5}}
};

void getUserInput()
{
	int input[4];
	char action;

	for (;;)
	{
		std::cout << "\nEnter what you want to do undo[u], move[m]\n";
		std::cin >> action;

		if (action == 'm')
		{
			std::cout << "Enter your move for exaple (A1A3)!\n";

			std::string stringIn;
			std::cin >> stringIn;

			const char* betterInput = stringIn.c_str();

			if (stringIn.size() != 4)
			{
				std::cout << "Your input is to long!\n";
				continue;
			}

			input[1] = betterInput[0] - 65;
			input[3] = betterInput[2] - 65;
			input[0] = betterRows[betterInput[1] - 48];
			input[2] = betterRows[betterInput[3] - 48];

			if (validateMove(input))
			{
				if (!casManager.castleOnNextMove)
				{
					moveLog.push_back({ {input[0], input[1]}, {input[2], input[3]}, gameState[input[0]][input[1]], gameState[input[2]][input[3]], -1 });

					gameState[input[2]][input[3]] = gameState[input[0]][input[1]];
					gameState[input[0]][input[1]] = pieces::ES;


					//enPassant
					if (enPassant)
					{
						if (whiteToMove)
						{
							gameState[input[2] + 1][input[3]] = pieces::ES;
						}
						else
						{
							gameState[input[2] - 1][input[3]] = pieces::ES;
						}

						moveLog.back().castlingDir = -2;
						enPassant = false;
					}
				}
				else
				{
					const int& dir = casManager.castlingDir;
					moveLog.push_back({ {input[0], input[1]}, {input[2], input[3]}, gameState[input[0]][input[1]], gameState[input[2]][input[3]], dir });

					gameState[input[0]][validCastleDirections[dir].kingEnd.col] = gameState[input[0]][input[1]];
					gameState[input[0]][validCastleDirections[dir].rookEnd.col] = gameState[input[2]][input[3]];

					gameState[input[0]][input[1]] = pieces::ES;
					gameState[input[2]][input[3]] = pieces::ES;

					casManager.castleOnNextMove = false;
				}

				break;
			}

			if (!gameIsRunning) break;

			std::cout << "\x1B[2J\x1B[H";
			printGameState();

			std::cout << "\nInvalid move!\n";
		}
		else if (action == 'u')
		{
			if (moveCount > 0)
			{
				if (moveLog.back().castlingDir == -1)
				{
					gameState[moveLog.back().startPos.row][moveLog.back().startPos.col] = moveLog.back().startPiece;
					gameState[moveLog.back().endPos.row][moveLog.back().endPos.col] = moveLog.back().endPiece;
				}
				else if (moveLog.back().castlingDir == -2)
				{
					gameState[moveLog.back().startPos.row][moveLog.back().startPos.col] = moveLog.back().startPiece;
					gameState[moveLog.back().endPos.row][moveLog.back().endPos.col] = moveLog.back().endPiece;

					if (!whiteToMove)
					{
						gameState[moveLog.back().endPos.row + 1][moveLog.back().endPos.col] = pieces::BP;
					}
					else
					{
						gameState[moveLog.back().endPos.row - 1][moveLog.back().endPos.col] = pieces::WP;
					}
				}
				else
				{
					int allyPieces[2];

					if (!whiteToMove)	//the other player has to move
					{
						allyPieces[0] = pieces::WR;
						allyPieces[1] = pieces::WK;
					}
					else
					{
						allyPieces[0] = pieces::BR;
						allyPieces[1] = pieces::BK;
					}

					gameState[moveLog.back().startPos.row][validCastleDirections[moveLog.back().castlingDir].rookStart.col] = allyPieces[0];
					gameState[moveLog.back().startPos.row][validCastleDirections[moveLog.back().castlingDir].kingStart.col] = allyPieces[1];

					gameState[moveLog.back().startPos.row][validCastleDirections[moveLog.back().castlingDir].rookEnd.col] = pieces::ES;
					gameState[moveLog.back().startPos.row][validCastleDirections[moveLog.back().castlingDir].kingEnd.col] = pieces::ES;
				}

				moveLog.pop_back();
				moveCount--;

				whiteToMove = !whiteToMove;

				std::cout << "\x1B[2J\x1B[H";
				printGameState();
			}
			else
			{
				std::cout << "You can not undo your the first move!\n";
			}
		}
		else
		{
			std::cout << "\x1B[2J\x1B[H"; //clear console
			printGameState();

			std::cout << "\nInvalid actiion!\n";
		}
	}

	if (input != NULL)
	//pawn Promotion
	if (gameState[input[0]][input[1]] == pieces::BP || gameState[input[0]][input[1]] == pieces::WP && (input[2] == 0 || input[2] == 7))
	{
		pawnPromotion(input);
	}

	//update king location
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

int evaluateScore()
{
	score = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				if (gameState[i][j] == whitePieces[k])
				{
					score += scoreLookup[k];
					break;
				}
				else if (gameState[i][j] == blackPieces[k])
				{
					score -= scoreLookup[k];
					break;
				}
			}
		}
	}

	return score;
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

	switch (possibleMoves[0].row)
	{
	case -1:
		return false;
	case -2:
		//sucessfull castle
		return true;
	case -3:
		std::cout << "You cant castle here!\n";
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
		std::cout << "\n";
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
	moveValid = posUnderAttack(endPos, startPos, { -1 , -1 }, true);

	return moveValid;
}

bool posUnderAttack(position endPos, position startPos, position pos, bool check)
{
	int movedPiece = gameState[startPos.row][startPos.col];
	int replacedPiece = gameState[endPos.row][endPos.col];
	int enemyPieces[6];

	position piecePos;
	position posToCheck = pos;
	std::vector<position> moves{};

	gameState[endPos.row][endPos.col] = gameState[startPos.row][startPos.col];
	gameState[startPos.row][startPos.col] = pieces::ES;

	if (whiteToMove)
	{
		std::copy(std::begin(blackPieces), std::end(blackPieces), std::begin(enemyPieces));

		if (check)
		{
			posToCheck = whiteKingPos;
		}
	}
	else
	{
		std::copy(std::begin(whitePieces), std::end(whitePieces), std::begin(enemyPieces));

		if (check)
		{
			posToCheck = blackKingPos;
		}
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

							if (moves[l].row == posToCheck.row && moves[l].col == posToCheck.col)
							{
								char giveUp;

								gameState[startPos.row][startPos.col] = movedPiece;
								gameState[endPos.row][endPos.col] = replacedPiece;

								whiteToMove = !whiteToMove;

								std::cout << "Your king is in danger!\n";
								std::cout << "Do you want to give up? [y/n]\n";
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

bool castleMove(position startPos, position endPos)
{
	std::vector<position> piecesBetween;
	int dir;

	for (auto& i : moveLog)
	{
		if (i.startPos.row == startPos.row && i.endPos.col == startPos.col &&
			i.endPos.row == endPos.row && i.endPos.col == endPos.col)
		{
			return false;
		}
	}

	//get positions in between
	if (whiteToMove)
	{
		if (endPos.col == 0)
		{
			piecesBetween.push_back({ 7, 1 });
			piecesBetween.push_back({ 7, 2 });
			piecesBetween.push_back({ 7, 3 });

			dir = 0;
		}
		else if (endPos.col == 7)
		{
			piecesBetween.push_back({ 7, 5 });
			piecesBetween.push_back({ 7, 6 });

			dir = 1;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (endPos.col == 0)
		{
			piecesBetween.push_back({ 0, 1 });
			piecesBetween.push_back({ 0, 2 });
			piecesBetween.push_back({ 0, 3 });

			dir = 2;
		}
		else if (endPos.col == 7)
		{
			piecesBetween.push_back({ 0, 5 });
			piecesBetween.push_back({ 0, 6 });

			dir = 3;
		}
		else
		{
			return false;
		}
	}

	for (position& i : piecesBetween)
	{
		if (gameState[i.row][i.col] != pieces::ES)
		{
			return false;
		}
	}

	for (position& i : piecesBetween)
	{
		if (!posUnderAttack(endPos, startPos, i, false))
		{
			return false;
		}
	}

	casManager = { true, dir };

	return true;
}

std::vector<position> getPossibleMoves(position startPos, position endPos, std::vector<position> possibleMoves)
{
	if (whiteToMove)
	{
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
			if (gameState[endPos.row][endPos.col] == pieces::WR && endPos.row == 7)
			{
				if (castleMove(startPos, endPos))
				{
					return { { -2, -2 } };
				}

				return { { -3, -3} };
			}

			return getKingMoves(startPos, possibleMoves);
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!\n";
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
				if (castleMove(startPos, endPos))
				{
					return { { -2, -2 } };
				}

				return { { -3, -3} };
			}

			return getKingMoves(startPos, possibleMoves);
		default:
			std::cout << "This is an enemy piece, please select a piece of yours!\n";
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
		else
		{
			std::cout << "Invalid piece ID!\n";
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

			//en passant
			else if (moveLog.size() > 0)
			{
				if (moveLog.back().startPiece == enemyPieces[0] && startPos.row == moveLog.back().endPos.row &&
					moveLog.back().endPos.col + i == startPos.col && abs(moveLog.back().startPos.row - moveLog.back().endPos.row) == 2)
				{
					possibleMoves.push_back({ startPos.row + direction, startPos.col - i });
					enPassant = true;
				}
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
