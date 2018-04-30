#include "game_tree.h"
#include <limits>
#include <algorithm>
#include <iomanip>

#define WIN5 10000
#define ALIVE4 1000
#define DEAD4 1000
#define LOWDEAD4 1000
#define ALIVE3 100
#define JUMPALIVE3 100
#define DEAD3 100
#define ALIVE2 10
#define LOWALIVE2 10
#define DEAD2 10
#define NOTHING 0


// calculate next best move
int game_tree::next_move(vector<int> board, int depth) 
{
	cout << evaBoard( board ) << endl; //debug
	for(int i=0; i<board.size(); i++)
		if( board[i] == 0 )
			return i;
	/*
	int best_move;
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	for(auto action: psbMove) 
		if(max_value < minValue(result(board, action, 1), depth-1))
			best_move = action;

	return best_move;
	*/
	
}

// core of miniMax algorithm
int game_tree::maxValue(vector<int> board, int depth) 
{
	int value = evaBoard(board);
	int max_value = numeric_limits<int>::min();
	vector<int> psbMove(genNextMove(board));

	if(value >= WIN5 || depth == 0)
		return value;

	for(auto action: psbMove)
		max_value = max(max_value, minValue(result(board,action,1), depth-1));
	
	return max_value;
}
int game_tree::minValue(vector<int> board, int depth) 
{
	int value = evaBoard(board);
	int min_value = numeric_limits<int>::max();
	vector<int> psbMove(genNextMove(board));

	if(value >= WIN5 || depth == 0)
		return value;

	for(auto action: psbMove)
		min_value = min(min_value, maxValue(result(board,action,2), depth-1));
	return min_value;
}

// given board status and an action, return a board status
// who: 1(us), 2(enemy)
vector<int> game_tree::result(vector<int> board, int action, int who) 
{
	board[action] = who;
	return board;
}

// evaluation function for the whole game board
int game_tree::evaBoard(vector<int> board) 
{
	int score = 0;
	for(int i=0; i<hrzLineTb.size(); i++)
		score += evaLinePattern( hrzLineTb[i], board );	

	for(int i=0; i<leftLineTb.size(); i++)
		score += evaLinePattern( leftLineTb[i], board );	
	
	for(int i=0; i<rightLineTb.size(); i++)
		score += evaLinePattern( rightLineTb[i], board );	

	return score;
}

int game_tree::evaLinePattern(vector<int> linePattern, vector<int> board)
{
	int pos1, pos2;
	int score = 0;

	pos1 = 0;
	while(1) 
	{
		// find next '1'
		while( pos1 < linePattern.size() && board[ linePattern[pos1] ] != 1 )
			pos1++;
	
		// find next not '1'
		pos2 = pos1;
		while( pos2 < linePattern.size() && board[ linePattern[pos2] ] == 1 )
			pos2++;
	
		if( pos1 >= linePattern.size() )
			// pos1 out of bound
			break;
		else
			score += evaPattern(pos2-pos1, board, linePattern, pos1, pos2-1);
	
		pos1 = pos2;
	}

	return score;
}

int game_tree::evaPattern(int numStoneInRow, vector<int> board, vector<int> linePattern, int leftEnd, int rightEnd)
{

	if( numStoneInRow == 5 )
	{
		// x11111x
		return WIN5; 
	}

	if( numStoneInRow == 4 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? 2 : board[ linePattern[leftEnd-1] ]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+1] ]; 

		if( left1 == 0 && right1 == 0 )
			// 011110
			return ALIVE4;
		else if( ( left1 == 2 && right1 == 0 ) || ( left1 == 0 && right1 == 2 ) )
			// 211110 or 011112
			return DEAD4;
		else
			// 211112
			return NOTHING;
	}

	if( numStoneInRow == 3 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? 2 : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? 2 : board[ linePattern[leftEnd-2] ]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+3] ]; 

		if( left1 != 1 && right1 == 0 && right2 == 1 && right3 != 1 )
			// x11101x
			return LOWDEAD4;
		else if( ( left2 != 1 && left1 == 0 && right1 == 0 && right2 == 0) ||
						 ( right2 != 1 && right1 == 0 && left1 == 0 && left2 == 0 ) )
			// x011100 or 001110x
			return ALIVE3;
		else if( ( left1 == 2 && right1 == 0 && right2 == 0 ) ||
						 ( right1 == 2 && left1 == 0 && left2 == 0 ) )
			// 211100 or 001112
			return DEAD3;
		else if( left2 == 2 && left1 == 0 && right1 == 0 && right2 == 2 )
			// 2011102
			return DEAD3;
	}

	if( numStoneInRow == 2 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? 2 : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? 2 : board[ linePattern[leftEnd-2] ]; 
		int left3 = ( leftEnd-3 < 0 ) ? 2 : board[ linePattern[leftEnd-3] ]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+3] ]; 
		int right4 = ( rightEnd+4 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+4] ]; 

		if( left1 != 1 && right1 == 0 && right2 == 1 && right3 == 1 && right4 != 1 )
			// x11011x
			return LOWDEAD4;
		else if( left1 == 0 && right1 == 0 && right2 == 1 && right3 == 0 )
			// 011010
			return JUMPALIVE3;
		else if( left1 == 2 && right1 == 0 && right2 == 1 && right3 == 0 )
			// 211010
			return DEAD3;
		else if( left1 == 0 && right1 == 0 && right2 == 1 && right3 == 2 )
			// 011012
			return DEAD3;
		else if( left1 != 1 && right1 == 0 && right2 == 0 && right3 == 1 && right4 != 1 )
			// x11001x
			return DEAD3;
		else if( left2 == 0 && left1 == 0 && right1 == 0 && right2 == 0)
			// 001100
			return ALIVE2;
		else if( ( left1 == 2 && right1 == 0 && right2 == 0 && right3 == 0) ||
						 ( right1 == 2 && left1 == 0 && left2 == 0 && left3 == 0) )
			// 211000 or 000112
			return DEAD2;
		else if( ( left3 == 2 && left2 == 0 && left1 == 0 && right1 == 0 && right2 == 2) ||
						 ( right3 == 2 && right2 == 0 && right1 == 0 && left1 == 0 && left2 == 2 ) )
			// 2001102 or 2011002
			return DEAD2;

	}

	if( numStoneInRow == 1 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? 2 : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? 2 : board[ linePattern[leftEnd-2] ]; 
		int left3 = ( leftEnd-3 < 0 ) ? 2 : board[ linePattern[leftEnd-3] ]; 
		int left4 = ( leftEnd-4 < 0 ) ? 2 : board[ linePattern[leftEnd-4] ]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+3] ]; 
		int right4 = ( rightEnd+4 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+4] ]; 
		int right5 = ( rightEnd+5 == linePattern.size() ) ? 2 : board[ linePattern[rightEnd+5] ]; 

		// x1---1x
		if( left1 != 1 && right1 == 0 && right4 == 1 && right5 != 1	)
		{
			if( right2 == 1 && right3 == 1 )
				// x10(11)1x
				return LOWDEAD4;
			else if( right2 == 0 && right3 == 0 )
				// x10(00)1x
				return DEAD2;
			else if( right2 == 1 && right3 == 0 )
				// x10(10)1x
				return DEAD3;
			else if( right2 == 0 && right3 == 1 )
				// x10(01)1x
				return DEAD3;
		}
		// 010-1
		else if( left1 == 0 && right1 == 0 && right3 == 1 )
		{
			if( right4 == 0 )
			{
				if(right2 == 0)
					// 010010
					return LOWALIVE2;
				else 
					// 010110
					return JUMPALIVE3;
			}
			else if( right2 == 1 && right4 == 2 )
				// 010112
				return DEAD3;
			else if( left2 == 2 && right2 == 0 && right4 == 2 )
				// 2010012
				return DEAD2;
		}
		// 21---0
		else if( left1 == 2 && right4 == 0 )
		{
			if( right1 == 0 )
			{
				if( right2 == 1 && right3 == 0 )
					// 210100
					return DEAD2;
				else if( right2 == 0 && right3 == 1 )
					// 210010
					return DEAD2;
				else if( right2 == 1 && right3 == 1 )
					// 210110
					return DEAD3;
			}
			else if( right1 == 1 && right2 == 0 && right3 == 1 )
				// 211010
				return DEAD3;
		}
		// 210---2
		else if( left1 == 2 && right1 == 0 && right5 == 2 )
		{
			if( right2 == 1 && right3 == 0 && right4 == 0 )
				// 2101002
				return DEAD2;
			else if( right2 == 0 && right3 == 1 && right4 == 0 )
				// 2100102
				return DEAD2;
			else if( right2 == 0 && right3 == 0 && right4 == 1 )
				// 2100012
				return DEAD2;
		}
		// 2010--2
		else if( left2 == 2 && left1 == 0 && right1 == 0 && right2 == 1 && right3 == 0 && right4 == 2 )
			// 2010102
			return DEAD2;
		else if( left3 == 2 && left2 == 0 && left1 == 0 && right1 == 0 && right2 == 1 && right3 == 2)
			// 2001012
			return DEAD2;
		else if( ( left2 != 1 && left1 == 0 && right1 == 0 && right2 == 1 && right3 == 0 && right4 == 0 ) ||
				( left2 == 0 && left1 == 0 && right1 == 0 && right2 == 1 && right3 == 0 && right4 != 1 ) )
			// x010100 or 001010x
			return LOWALIVE2;
		else if( left2 == 0 && left1 == 0 && right1 == 0 && right2 == 1 && right3 == 2 )
			// 001012
			return DEAD2;

	}

	return NOTHING;
}

// generate next possible move
vector<int> game_tree::genNextMove(vector<int> board) 
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
