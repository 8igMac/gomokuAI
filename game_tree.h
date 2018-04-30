#ifndef GAME_TREE_H
#define GAME_TREE_H
#include <iostream>
#include <vector>
using namespace std;

#define DEBUG 0

struct node 
{
	vector<int> board;
	vector<node*> child;
};

class game_tree 
{
	public:
		game_tree();
		~game_tree();
		int next_move(vector<int> board, int depth);

	private:
		void destroy_tree(node* leaf);
		node* root;
		int evaBoard(vector<int> board);
		vector<int> genNextMove(vector<int> board);
		int maxValue(vector<int> board, int depth);
		int minValue(vector<int> board, int depth);
		vector<int> result(vector<int> board, int action, int who);

		void axisLutInit();
		vector<vector<int> > hrzLineTb; /* look up table for axis: - */
		vector<vector<int> > leftLineTb; /* look up table for axis: \ */
		vector<vector<int> > rightLineTb; /* look up table for axis: / */

		int evaLinePattern(vector<int> linePattern, vector<int> board);
		int evaPattern(int numStoneInRow, vector<int> board, vector<int> linePattern, int leftEnd, int rightEnd);
};
#endif
