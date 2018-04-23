#include "game_tree.h"
#include <limits>

void game_tree::destroy_tree(node* leaf) {
	if(leaf == NULL)
		return;
	else
		for(auto item: leaf->child)
			destroy_tree(item);
}

int game_tree::evafunc(vector<int> board) {

}

vector<int> game_tree::genNextMove(vector<int> board) {

}

int game_tree::maxValue(vector<int> board) {

}

int game_tree::minValue(vector<int> board) {

}

vector<int> game_tree::result(vector<int> board, int action) {

}

int game_tree::next_move(vector<int> board) {

	int best_move;
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	for(auto action: psbMove) 
		if(max_value < minValue(result(board, action)))
			best_move = action;

	return best_move;
}
