#include "game_tree.h"
#include <limits>
#include <algorithm>
#include <iomanip>

#define fiveInRow 100000 // to-do


// calculate next best move
int game_tree::next_move(string board, int depth) 
{

	int best_move;
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	for(auto action: psbMove) 
		if(max_value < minValue(result(board, action, 1), depth-1))
			best_move = action;

	return best_move;
}

// core of miniMax algorithm
int game_tree::maxValue(string board, int depth) 
{
	int value = evafunc(board);
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	if(value >= fiveInRow || depth == 0)
		return value;

	for(auto action: psbMove)
		max_value = max(max_value, minValue(result(board,action,1), depth-1));
	
	return max_value;
}
int game_tree::minValue(string board, int depth) 
{
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
string game_tree::result(string board, int action, int who) 
{
	board[action] = who;
	return board;
}

// evaluation function for the whole game board
int game_tree::evafunc(string board) 
{
	

	
}

// generate next possible move
vector<int> game_tree::genNextMove(string board) 
{

}


game_tree::game_tree() : root(NULL), hrzLineTb(17), leftLineTb(17), rightLineTb(17)
{
	// initialize look up table for each axis
	axisLutInit();
}

game_tree::~game_tree() 
{
	destroy_tree(root);
}

// private destructor
void game_tree::destroy_tree(node* leaf) 
{
	if(leaf == NULL)
		return;
	else
		for(auto item: leaf->child)
			destroy_tree(item);
}

// initialize look up table for each axis
void game_tree::axisLutInit()
{
	int base, offset;
			
	/* axis: - */
	base = 0;
	offset = 0;
	for(int i=0; i<17; i++) 
	{
		vector<int> temp;
		int index;
		offset = ( i < 9 ) ? offset+1 : offset-1;
		for( index=base; index<base+8+offset; index++ )
			temp.push_back(index);
		base = index;
		hrzLineTb[i] = temp;
	}

	/* axis: \ */
	for(int i=0; i<17; i++)
	{
		vector<int> temp;
		if( i < 9 )
		{
			for( base=0+i; base < 17; base++ )
			{
				if( base-i < 9-i )
					offset = base-i;
				else
					offset = offset;

				temp.push_back( hrzLineTb[base][0] + offset );
			}
			leftLineTb[i] = temp;
		}
		else 
		{
			int num;
			for( int index2=0; index2<leftLineTb[0].size(); index2++ )
			{
				num = leftLineTb[0][index2] + (i-8);
				if( num <= *(hrzLineTb[16-(i-8)].end()-1) )
					temp.push_back( num );
				else
					break;
			}
			leftLineTb[i] = temp;
		}
	}

	/* axis: / */
	for(int i=0; i<17; i++)
	{
		vector<int> temp;
		if( i < 9 )
		{
			for( base=0+i; base < 17; base++ )
			{
				if( base-i < 9-i )
					offset = base-i;
				else
					offset = offset;

				int end = hrzLineTb[base].size()-1;
				temp.push_back( hrzLineTb[base][end] - offset );
			}
			rightLineTb[i] = temp;
		}
		else 
		{
			int num;
			for( int index2=0; index2<rightLineTb[0].size(); index2++ )
			{
				num = rightLineTb[0][index2] - (i-8);
				if( num <= hrzLineTb[16-(i-8)][0] )
				{
					temp.push_back( num );
				}
				else
					break;
			}
			rightLineTb[i] = temp;
		}
	}

	// debug
	if(DEBUG)
	{
		for(int a=0; a<hrzLineTb.size(); a++)
		{
			for(int b=0; b<hrzLineTb[a].size(); b++)
				cout << setw(3) << hrzLineTb[a][b] << " ";
			cout << endl;
		}
		for(int a=0; a<leftLineTb.size(); a++)
		{
			for(int b=0; b<leftLineTb[a].size(); b++)
				cout << setw(3) << leftLineTb[a][b] << " ";
			cout << endl;
		}
		for(int a=0; a<rightLineTb.size(); a++)
		{
			for(int b=0; b<rightLineTb[a].size(); b++)
				cout << setw(3) << rightLineTb[a][b] << " ";
			cout << endl;
		}
	}
}
