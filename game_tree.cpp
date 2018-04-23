#include "game_tree.h"
#include <limits>

void game_tree::destroy_tree(node* leaf) {
	if(leaf == NULL)
		return;
	else
		for(auto item: leaf->child)
			destroy_tree(item);
}

// evaluation function for the whole game board
int game_tree::evafunc(vector<int> board) {

}

// generate next possible move
vector<int> game_tree::genNextMove(vector<int> board) {

}

// core of miniMax algorithm
int game_tree::maxValue(vector<int> board) {

}
int game_tree::minValue(vector<int> board) {

}

// given board status and an action, return a board status
// who: 1(us), 2(enemy)
vector<int> game_tree::result(vector<int> board, int action, int who) {
	board[action] = who;
	return board;
}

// calculate next best move
int game_tree::next_move(vector<int> board) {

	int best_move;
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	for(auto action: psbMove) 
		if(max_value < minValue(result(board, action, 1)))
			best_move = action;

	return best_move;
}
