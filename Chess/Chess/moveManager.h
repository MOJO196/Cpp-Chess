struct position
{
	int row;
	int col;
};

void getUserInput();
bool validateMove(int input[]);
std::vector<position> pawnMoves(position startPos);
std::vector<position> rookMoves(position startPos);