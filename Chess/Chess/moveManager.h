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
	int castlingDir;
};

struct castling
{
	position kingStart;
	position kingEnd;
	position rookStart;
	position rookEnd;
};

struct castleManager
{
	bool castleOnNextMove;
	int castlingDir;
};

void getUserInput();
bool validateMove(int input[]);
bool posUnderAttack(position endPos, position startPos, position pos, bool check);

std::vector<position> getPossibleMoves(position startPos, position endPos, std::vector<position> possibleMoves);

bool castleMove(position startPos, position endPos);
void pawnPromotion(int input[]);
std::vector<position> getPawnMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getRookMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getKnightMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getBishopMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getQueenMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getKingMoves(position startPos, std::vector<position> possibleMoves);

int evaluateScore();
