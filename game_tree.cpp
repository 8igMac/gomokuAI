#include "game_tree.h"
#include <limits>
#include <algorithm>
#define fiveInRow 100000 // to-do

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
int game_tree::maxValue(vector<int> board, int depth) {
	int value = evafunc(board);
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	if(value >= fiveInRow || depth == 0)
		return value;

	for(auto action: psbMove)
		max_value = max(max_value, minValue(result(board,action,1), depth-1));
	
	return max_value;
}
int game_tree::minValue(vector<int> board, int depth) {
	int value = evafunc(board);
	int min_value = numeric_limits<int>::max();
	vector<int> psbMove(genNextMove(board));

	if(value >= fiveInRow || depth == 0)
		return value;

	for(auto action: psbMove)
		min_value = min(min_value, maxValue(result(board,action,2), depth-1));
	return min_value;
}

// given board status and an action, return a board status
// who: 1(us), 2(enemy)
vector<int> game_tree::result(vector<int> board, int action, int who) {
	board[action] = who;
	return board;
}

// calculate next best move
int game_tree::next_move(vector<int> board, int depth) {

	int best_move;
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	for(auto action: psbMove) 
		if(max_value < minValue(result(board, action, 1), depth-1))
			best_move = action;

	return best_move;
}
