struct position
{
	int row;
	int col;
};

void getUserInput();
bool validateMove(int input[]);
<<<<<<< Updated upstream
=======
<<<<<<< HEAD

std::vector<position> getPawnMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getRookMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getKnightMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getBishopMoves(position startPos, std::vector<position> possibleMoves, bool queenMode);
std::vector<position> getQueenMoves(position startPos, std::vector<position> possibleMoves);
std::vector<position> getKingMoves(position startPos, std::vector<position> possibleMoves);
=======
>>>>>>> Stashed changes
void pawnPromotion(int input[]);
std::vector<position> pawnMoves(position startPos);
std::vector<position> rookMoves(position startPos);
>>>>>>> 9364e8dd6286c53990efa63d8e854141d9efd582
