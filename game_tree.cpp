#include "game_tree.h"

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

int game_tree::next_move(vector<int> board) {
	
	// dummy next move
	for(int i=0; i<board.size(); i++) 
		if(board[i] == 0)
			return i;

}
