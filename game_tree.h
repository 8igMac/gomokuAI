#ifndef GAME_TREE_H
#define GAME_TREE_H
#include <vector>
#include <iostream>
using namespace std;

struct node {
	vector<int> board;
	vector<node*> child;
};

class game_tree {
	public:
		game_tree() : root(NULL) {}
		~game_tree() {
			destroy_tree(root);
		}
		int next_move(vector<int> board, int depth);

	private:
		void destroy_tree(node* leaf);
		node* root;
		int evafunc(vector<int> board);
		vector<int> genNextMove(vector<int> board);
		int maxValue(vector<int> board, int depth);
		int minValue(vector<int> board, int depth);
		vector<int> result(vector<int> board, int action, int who);
};
#endif
