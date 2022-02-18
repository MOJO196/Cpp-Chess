struct position
{
	int row;
	int col;
};

struct move
{
	position startPos;
	position endPos;
	int startPiece;
	int endPiece;
};

void getUserInput();
bool validateMove(int input[]);
bool checkForCheck(position endPos, position startPos);

std::vector<position> getPossibleMoves(position startPos, position endPos, std::vector<position> possibleMoves);

bool casteling(position startPos, position endPos);
void pawnPromotion(int input[]);
std::vector<position> getPawnMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getRookMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getKnightMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getBishopMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getQueenMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getKingMoves(position startPos, std::vector<position> possibleMoves);
