struct position
{
	int row;
	int col;
};

void getUserInput();
bool validateMove(int input[]);
bool checkForCheck();

void pawnPromotion(int input[]);
std::vector<position> getPawnMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getRookMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getKnightMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getBishopMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getQueenMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getKingMoves(position startPos, std::vector<position> possibleMoves);
