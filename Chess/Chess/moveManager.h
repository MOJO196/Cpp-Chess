struct position
{
	int row;
	int col;
};

void getUserInput();
bool validateMove(int input[]);
bool pawnMoves(int input[]);
std::vector<position> rookMoves(position startPos);