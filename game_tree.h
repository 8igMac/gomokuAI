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
		// return next best move
		int next_move(vector<int> board, int depth);

	private:

		/*****************************
		 * 
		 *  minimax algorithm related 	
		 *
		 * ***************************/
		// minimax: max
		int maxValue(vector<int> board, int depth, pair<int,int> &alphaBeta);
		// minimax: min
		int minValue(vector<int> board, int depth, pair<int,int> &alphaBeta);
		// change game board according to action done
		vector<int> result(vector<int> board, int action, int who);


		/*****************************
		 * 
		 *  three axis look up table 	
		 *
		 * ***************************/
		// init axis look up table
		void axisLutInit();
		/* look up table for axis: - */
		vector<vector<int> > hrzLineTb; 
		/* look up table for axis: \ */
		vector<vector<int> > leftLineTb; 
		/* look up table for axis: / */
		vector<vector<int> > rightLineTb; 
		/* look up table for position to axis index:
		 * 	pair(axis idx, position in axis)
 		 *  for second layer vector index -> 0: hrz, 1: left, 2: right 
		 */
		vector<vector<pair<int, int>> > pos2axis; 

		
		/*******************************
		 * 
		 *  evaluation related functions 	
		 *
		 * ******************************/
		// evalate game board and score
		int evaBoard(vector<int> board);
		// evaluate line pattern and store result to evaResult
		void evaLinePattern(vector<int> linePattern, vector<int> board, int who);
		// recognize basic gomoku pattern 
		void evaPattern(int numStoneInRow, vector<int> board, vector<int> linePattern, int leftEnd, int rightEnd, int target);


		/*******************************
		 * 
		 *  		evaluation result 	
		 *
		 * ******************************/
		// store evaluation result in table
		// index:
		//   WIN5 0
		//   ALIVE4 1
		//   DEAD4 2
		//   LOWDEAD4 3
		//   ALIVE3 4
		//   JUMPALIVE3 5
		//   DEAD3 6
		//   ALIVE2 7
		//   LOWALIVE2 8
		//   DEAD2 9
		//   NOTHING 10
		vector<int> evaResult;
		// clear evaluation result table
		void clearEvaResult();
		// calculate game board score
		int boardScore(vector<int> board, int who);


		/*******************************
		 * 
		 *  	move generator related  	
		 *
		 * ******************************/
		// generate next possible move
		vector<int> genNextMove(vector<int> board, int who, bool enTss);
		// tell if the position has neighbor
		bool hasNeighbor(vector<int> board, int index);
		// evaluate action
		int evaPoint(vector<int> board, int action, int who, bool enTss);

		
		/*******************************
		 * 
		 *  Threat Search Space related 	
		 *  
		 *  result: TSS success or not
		 *
		 * *****************************/
		// TSS a.k.a Threat Space Search algorithm
		bool tss(vector<int> board, int tssDepth);
		// is opponet has threat in current game board?
		bool hasOppoThreat(vector<int> board);
		// return game board with conservative defence
		vector<int> conDef(vector<int> board);


		/*************************************************
		 * 
		 *  total program efficiency evalutation related  	
		 *
		 * ***********************************************/
		int totalCreatedNode;


		/*******************************
		 * 
		 *  			debug functions  	
		 *
		 * ******************************/
		// generate random move
		int genRandomMove(vector<int> board);
		void printEvaResult();

};
#endif
