#ifndef GAME_TREE_H
#define GAME_TREE_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define DEBUG 0

struct node 
{
	string board;
	vector<node*> child;
};

class game_tree 
{
	public:
		game_tree();
		~game_tree();
		int next_move(string board, int depth);

	private:
		void destroy_tree(node* leaf);
		node* root;
		int evaBoard(string board);
		vector<int> genNextMove(string board);
		int maxValue(string board, int depth);
		int minValue(string board, int depth);
		string result(string board, int action, int who);

		void axisLutInit();
		vector<vector<int> > hrzLineTb; /* look up table for axis: - */
		vector<vector<int> > leftLineTb; /* look up table for axis: \ */
		vector<vector<int> > rightLineTb; /* look up table for axis: / */

		void evaLinePattern(vector<int> linePattern);
		void evaPattern(int numStoneInRow, vector<int> linePattern, int leftEnd, int rightEnd);
};
#endif
