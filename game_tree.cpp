#include "game_tree.h"
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <functional>

#define WIN5 0
#define ALIVE4 1
#define DEAD4 2
#define LOWDEAD4 3
#define ALIVE3 4
#define JUMPALIVE3 5
#define DEAD3 6
#define ALIVE2 7
#define LOWALIVE2 8
#define DEAD2 9
#define NOTHING 10

#define WIN5SCORE 10000000
#define TSS_SUCCESS -2

// adjustable parameter
#define NUMGOODMOVES 3
#define TSS_DEPTH 8


// calculate next best move
int game_tree::next_move(vector<int> board, int depth) 
{
	/*
	//debug
	cout << evaBoard(board) << endl;
	return genRandomMove(board);
	*/
	/*
	//debug
	cout << "board[92]: " << board[92] << endl; 
	for(int i=0; i<rightLineTb.size(); i++)
	{
		for(int j=0; j<rightLineTb[i].size(); j++)
			cout << setw(3) << rightLineTb[i][j] << " ";
		cout << endl;
	}
	*/

	int best_move = 108; // center of the game board
	int max_value = numeric_limits<int>::min();
	// ( alpha, beta )
	// alpha: best choise for MAX so far
	// beta: best choise for MIN so far
	pair<int,int> alphaBeta( numeric_limits<int>::min(), numeric_limits<int>::max() );  
	totalCreatedNode++;

	// run tss
	int tss_move = tss(board, TSS_DEPTH);
	if( tss_move != -1)
	{
		cout << "tss success with move: " << tss_move << endl;
		return tss_move;
	}

	vector<int> psbMove(genNextMove(board, 1, false, false));
	for(auto action: psbMove) 
	{
		int minvalue = minValue(result(board, action, 1), depth-1, alphaBeta);
		if( max_value < minvalue )
		{
			max_value = minvalue;
			best_move = action;
		}
		if( max_value >= WIN5SCORE )
			break;
	}

//	cout << "max value: " << max_value << endl; // debug
	cout << "best move: " << best_move << endl; // debug
	cout << "total node created: " << totalCreatedNode << endl; //debug
	return best_move;
}

// core of miniMax algorithm
int game_tree::maxValue(vector<int> board, int depth, pair<int,int> &alphaBeta) 
{
	totalCreatedNode++;
	int value = evaBoard(board);
	int max_value = numeric_limits<int>::min();
	// run tss
	if( tss(board, TSS_DEPTH) != -1)
	{
	//	cout << "tss success" << endl; //debug
		return numeric_limits<int>::max();
	}


	if(value >= WIN5SCORE || depth == 0)
		return value;

	vector<int> psbMove(genNextMove(board, 1, false, false));
	for(auto action: psbMove)
	{
		max_value = max(max_value, minValue(result(board,action,1), depth-1, alphaBeta));

		// alpha-beta prunning
		if( max_value >= alphaBeta.second )
			return max_value;
		alphaBeta.first = max( alphaBeta.first, max_value );

		if( max_value >= WIN5SCORE )
			break;
	}
	
	return max_value;
}
int game_tree::minValue(vector<int> board, int depth, pair<int,int> &alphaBeta) 
{
	totalCreatedNode++;
	int value = evaBoard(board);
	int min_value = numeric_limits<int>::max();

	if(value >= WIN5SCORE || depth == 0)
	{
		return value;
	}

	vector<int> psbMove(genNextMove(board, 2, false, false));
	for(auto action: psbMove)
	{
		min_value = min(min_value, maxValue(result(board,action,2), depth-1, alphaBeta));
		
		// alpha-beta prunning
		if( min_value <= alphaBeta.first )
			return min_value;
		alphaBeta.second = min( alphaBeta.second, min_value );

		if( min_value <= -WIN5SCORE )
			break;
	}
	return min_value;
}

// TSS a.k.a Threat Space Search algorithm
int game_tree::tss(vector<int> board, int tssDepth)
{
	if( tssDepth < 6 )
		cout << "tss depth: " << tssDepth << endl; //debug
	// tss out of quatar
	if(tssDepth == 0)
		return -1;

	// search current game board
	if( hasOppoThreat(board) )
		return -1;
	
	// generate threat move
	vector<int> threatMove( genNextMove(board, 1, true, false) );
	
	// win move found -> tss success
	if( threatMove.size() == 1 )
		return threatMove[0];

	for(auto action: threatMove)
	{
		int tss_move = tss( conDef( result(board, action, 1) ), tssDepth-1 );
		if( tss_move != -1 )
			return tss_move;
	}

	return -1;
}

// return game board with conservative defence
vector<int> game_tree::conDef(vector<int> board)
{
	vector<int> condef( genNextMove(board, 2, false, true) );
	vector<int> boardDef(board);

	for(auto action: condef)
		boardDef = result( boardDef, action, 2 );
		
	return boardDef;
}


// is opponet has threat in current game board?
bool game_tree::hasOppoThreat(vector<int> board)
{
	// clear evaluated result
	clearEvaResult();
	// eva for opponent 
	for(int i=0; i<hrzLineTb.size(); i++)
		evaLinePattern( hrzLineTb[i], board, 2 );	

	for(int i=0; i<leftLineTb.size(); i++)
		evaLinePattern( leftLineTb[i], board, 2 );	
	
	for(int i=0; i<rightLineTb.size(); i++)
		evaLinePattern( rightLineTb[i], board, 2 );	

	if( evaResult[WIN5] == 0 && evaResult[ALIVE4] == 0 && 
			evaResult[DEAD4] == 0 && evaResult[LOWDEAD4] == 0 && 
			evaResult[ALIVE3] == 0 && evaResult[JUMPALIVE3]	== 0)
		return false;
	else 
		return true;
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
	int score;

	// clear evaluated result
	clearEvaResult();
	// eva for us
	for(int i=0; i<hrzLineTb.size(); i++)
		evaLinePattern( hrzLineTb[i], board, 1 );	

	for(int i=0; i<leftLineTb.size(); i++)
		evaLinePattern( leftLineTb[i], board, 1 );	
	
	for(int i=0; i<rightLineTb.size(); i++)
		evaLinePattern( rightLineTb[i], board, 1 );	

	// calculate our game board score
	score = boardScore();

	// clear evaluated result
	clearEvaResult();
	// eva for opponent
	for(int i=0; i<hrzLineTb.size(); i++)
		evaLinePattern( hrzLineTb[i], board, 2 );	

	for(int i=0; i<leftLineTb.size(); i++)
		evaLinePattern( leftLineTb[i], board, 2 );	
	
	for(int i=0; i<rightLineTb.size(); i++)
		evaLinePattern( rightLineTb[i], board, 2 );	

	// calculate opponent game board score
	score -= boardScore();
	
	return score;
}

void game_tree::evaLinePattern(vector<int> linePattern, vector<int> board, int who)
{
	int pos1, pos2;

	pos1 = 0;
	while(1) 
	{
		// find next 'who'
		while( pos1 < linePattern.size() && board.at( linePattern[pos1] ) != who )
			pos1++;
	
		// find next not 'who'
		pos2 = pos1;
		while( pos2 < linePattern.size() && board.at( linePattern[pos2] ) == who)
			pos2++;
	
		if( pos1 >= linePattern.size() )
			// pos1 out of bound
			break;
		else
			evaPattern(pos2-pos1, board, linePattern, pos1, pos2-1, who);
	
		pos1 = pos2;
	}
}

void game_tree::evaPattern(int numStoneInRow, vector<int> board, vector<int> linePattern, int leftEnd, int rightEnd, int target)
{
	int barrier = ( target == 1 ) ? 2 : 1;

	if( numStoneInRow == 5 )
	{
		// x11111x
		evaResult[ WIN5 ]++;
		return;
	}

	if( numStoneInRow == 4 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? barrier : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? barrier : board[ linePattern[leftEnd-2] ]; 
		int right1 = ( rightEnd+1 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+2] ]; 

		if( left1 == 0 && right1 == 0 )
			// 011110
			evaResult[ ALIVE4 ]++;
		else if( ( left1 == barrier && right1 == 0 && right2 != target) || ( left2 != target && left1 == 0 && right1 == barrier ) )
			// 211110x or x011112
			evaResult[ DEAD4 ]++;
		else
			// 211112
			evaResult[ NOTHING ]++;

		evaResult[ NOTHING ]++;
		return;
	}

	if( numStoneInRow == 3 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? barrier : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? barrier : board[ linePattern[leftEnd-2] ]; 
		int right1 = ( rightEnd+1 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+3] ]; 

		if( left1 != target && right1 == 0 && right2 == target && right3 != target )
			// x11101x
			evaResult[ LOWDEAD4 ]++;
		else if( ( left2 != target && left1 == 0 && right1 == 0 && right2 == 0) ||
						 ( right2 != target && right1 == 0 && left1 == 0 && left2 == 0 ) )
			// x011100 or 001110x
			evaResult[ ALIVE3 ]++;
		else if( ( left1 == barrier && right1 == 0 && right2 == 0 ) ||
						 ( right1 == barrier && left1 == 0 && left2 == 0 ) )
			// 211100 or 001112
			evaResult[ DEAD3 ]++;
		else if( left2 == barrier && left1 == 0 && right1 == 0 && right2 == barrier )
			// 2011102
			evaResult[ DEAD3 ]++;

		evaResult[ NOTHING ]++;
		return;
	}

	if( numStoneInRow == 2 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? barrier : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? barrier : board[ linePattern[leftEnd-2] ]; 
		int left3 = ( leftEnd-3 < 0 ) ? barrier : board[ linePattern[leftEnd-3] ]; 
		int right1 = ( rightEnd+1 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+3] ]; 
		int right4 = ( rightEnd+4 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+4] ]; 

		if( left1 != target && right1 == 0 && right2 == target && right3 == target && right4 != target )
			// x11011x
			evaResult[ LOWDEAD4 ]++;
		else if( left1 == 0 && right1 == 0 && right2 == target && right3 == 0 )
			// 011010
			evaResult[ JUMPALIVE3 ]++;
		else if( left1 == barrier && right1 == 0 && right2 == target && right3 == 0 )
			// 211010
			evaResult[ DEAD3 ]++;
		else if( left1 == 0 && right1 == 0 && right2 == target && right3 == barrier )
			// 011012
			evaResult[ DEAD3 ]++;
		else if( left1 != target && right1 == 0 && right2 == 0 && right3 == target && right4 != target )
			// x11001x
			evaResult[ DEAD3 ]++;
		else if( left2 == 0 && left1 == 0 && right1 == 0 && right2 == 0)
			// 001100
			evaResult[ ALIVE2 ]++;
		else if( ( left1 == barrier && right1 == 0 && right2 == 0 && right3 == 0) ||
						 ( right1 == barrier && left1 == 0 && left2 == 0 && left3 == 0) )
			// 211000 or 000112
			evaResult[ DEAD2 ]++;
		else if( ( left3 == barrier && left2 == 0 && left1 == 0 && right1 == 0 && right2 == barrier) ||
						 ( right3 == barrier && right2 == 0 && right1 == 0 && left1 == 0 && left2 == barrier ) )
			// 2001102 or 2011002
			evaResult[ DEAD2 ]++;

		evaResult[ NOTHING ]++;
		return;
	}

	if( numStoneInRow == 1 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? barrier : board[ linePattern[leftEnd-1] ]; 
		int left2 = ( leftEnd-2 < 0 ) ? barrier : board[ linePattern[leftEnd-2] ]; 
		int left3 = ( leftEnd-3 < 0 ) ? barrier : board[ linePattern[leftEnd-3] ]; 
		int left4 = ( leftEnd-4 < 0 ) ? barrier : board[ linePattern[leftEnd-4] ]; 
		int right1 = ( rightEnd+1 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+1] ]; 
		int right2 = ( rightEnd+2 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+2] ]; 
		int right3 = ( rightEnd+3 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+3] ]; 
		int right4 = ( rightEnd+4 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+4] ]; 
		int right5 = ( rightEnd+5 >= linePattern.size() ) ? barrier : board[ linePattern[rightEnd+5] ]; 

		// x1---1x
		if( left1 != target && right1 == 0 && right4 == target && right5 != target	)
		{
			if( right2 == target && right3 == target )
				// x10(11)1x
				evaResult[ LOWDEAD4 ]++;
			else if( right2 == 0 && right3 == 0 )
				// x10(00)1x
				evaResult[ DEAD2 ]++;
			else if( right2 == target && right3 == 0 )
				// x10(10)1x
				evaResult[ DEAD3 ]++;
			else if( right2 == 0 && right3 == target )
				// x10(01)1x
				evaResult[ DEAD3 ]++;
		}
		// 010-1
		else if( left1 == 0 && right1 == 0 && right3 == target )
		{
			if( right4 == 0 )
			{
				if(right2 == 0)
					// 010010
					evaResult[ LOWALIVE2 ]++;
				else if(right2 == target)
					// 010110
					evaResult[ JUMPALIVE3 ]++;
			}
			else if( right2 == target && right4 == barrier )
				// 010112
				evaResult[ DEAD3 ]++;
			else if( left2 == barrier && right2 == 0 && right4 == barrier )
				// 2010012
				evaResult[ DEAD2 ]++;
		}
		// 21---0
		else if( left1 == barrier && right4 == 0 )
		{
			if( right1 == 0 )
			{
				if( right2 == target && right3 == 0 )
					// 210100
					evaResult[ DEAD2 ]++;
				else if( right2 == 0 && right3 == target )
					// 210010
					evaResult[ DEAD2 ]++;
				else if( right2 == target && right3 == target )
					// 210110
					evaResult[ DEAD3 ]++;
			}
			else if( right1 == target && right2 == 0 && right3 == target )
				// 211010
				evaResult[ DEAD3 ]++;
		}
		// 210---2
		else if( left1 == barrier && right1 == 0 && right5 == barrier )
		{
			if( right2 == target && right3 == 0 && right4 == 0 )
				// 2101002
				evaResult[ DEAD2 ]++;
			else if( right2 == 0 && right3 == target && right4 == 0 )
				// 2100102
				evaResult[ DEAD2 ]++;
			else if( right2 == 0 && right3 == 0 && right4 == target )
				// 2100012
				evaResult[ DEAD2 ]++;
		}
		// 2010--2
		else if( left2 == barrier && left1 == 0 && right1 == 0 && right2 == target && right3 == 0 && right4 == barrier )
			// 2010102
			evaResult[ DEAD2 ]++;
		else if( left3 == barrier && left2 == 0 && left1 == 0 && right1 == 0 && right2 == target && right3 == barrier )
			// 2001012
			evaResult[ DEAD2 ]++;
		else if( ( left2 != target && left1 == 0 && right1 == 0 && right2 == target && right3 == 0 && right4 == 0 ) ||
				( left2 == 0 && left1 == 0 && right1 == 0 && right2 == target && right3 == 0 && right4 != target ) )
			// x010100 or 001010x
			evaResult[ LOWALIVE2 ]++;
		else if( left2 == 0 && left1 == 0 && right1 == 0 && right2 == target && right3 == barrier )
			// 001012
			evaResult[ DEAD2 ]++;
	}

	evaResult[ NOTHING ]++;
	return;
}

// generate next possible move
vector<int> game_tree::genNextMove(vector<int> board, int who, bool enTss, bool enConDef) 
{
	int i=0;
	multimap<int,int>::iterator itr;
	vector<int> goodMove;
	multimap<int, int, greater<int>> psbMove;

	for(i=0; i<board.size(); i++)
		if( board[i] == 0 && hasNeighbor(board, i) )
		{
			int pointScore = evaPoint(board, i, who, enTss, enConDef);

			// tss success
			if( enTss && pointScore == TSS_SUCCESS)
			{
				goodMove.push_back(i);
				return goodMove;
			}

			if( pointScore >= numeric_limits<int>::max() )
			{
				// win move found
				goodMove.push_back(i);
				return goodMove;
			}
			else
				psbMove.insert( pair<int,int>( pointScore, i ) );
		}

	/*
	//debug
	for( itr=psbMove.begin(); itr != psbMove.end(); itr++ )
		cout << who << " " << itr->second << ": " << itr->first << endl; 
		*/

	if( enTss )
	{
		for( itr=psbMove.begin() ; itr != psbMove.end(); itr++ )
			if( itr->first == -1 )
				goodMove.push_back( itr->second );
	}
	else if( enConDef )
	{
		for( itr=psbMove.begin() ; itr != psbMove.end(); itr++ )
			if( itr->first == -3 )
				goodMove.push_back( itr->second );
	}
	else
	{
		if( psbMove.size() != 0 )
		{
			int threshold = psbMove.begin()->first;
			if( threshold > 50000000 ) //10^7
			{
				// push all super pattern with point score
				for( itr=psbMove.begin() ; itr != psbMove.end() && itr->first == threshold ; itr++ )
					goodMove.push_back(itr->second);
			}
			else
			{
				// forward prunning: only push #NUMGOODMOVES of actions
				for( itr=psbMove.begin() ; goodMove.size() < NUMGOODMOVES && itr != psbMove.end(); itr++ )
					goodMove.push_back(itr->second);
			}
		}
	}


	return goodMove;
}

// evaluate action
int game_tree::evaPoint(vector<int> board, int action, int who, bool enTss, bool enConDef)
{
	int score=0;
	int opponent = ( who == 1 ) ? 2 : 1;
	vector<int> oldUsTable(11), newUsTable(11), oldOppoTable(11), newOppoTable(11);
	vector<int> usTable(11), oppoTable(11);	
	vector<int> newBoard( result( board, action, who ) );

	// us
	clearEvaResult();
	evaLinePattern( hrzLineTb[ pos2axis[action][0].first ], board, who );	
	evaLinePattern( leftLineTb[ pos2axis[action][1].first ], board, who );	
	evaLinePattern( rightLineTb[ pos2axis[action][2].first ], board, who );	

	oldUsTable = evaResult;

	clearEvaResult();
	evaLinePattern( hrzLineTb[ pos2axis[action][0].first ], newBoard, who );	
	evaLinePattern( leftLineTb[ pos2axis[action][1].first ], newBoard, who );	
	evaLinePattern( rightLineTb[ pos2axis[action][2].first ], newBoard, who );	

	newUsTable = evaResult;

	// opponent
	clearEvaResult();
	evaLinePattern( hrzLineTb[ pos2axis[action][0].first ], board, opponent );	
	evaLinePattern( leftLineTb[ pos2axis[action][1].first ], board, opponent );	
	evaLinePattern( rightLineTb[ pos2axis[action][2].first ], board, opponent );	

	oldOppoTable = evaResult;

	clearEvaResult();
	evaLinePattern( hrzLineTb[ pos2axis[action][0].first ], newBoard, opponent );	
	evaLinePattern( leftLineTb[ pos2axis[action][1].first ], newBoard, opponent );	
	evaLinePattern( rightLineTb[ pos2axis[action][2].first ], newBoard, opponent );	

	newOppoTable = evaResult;

	// calculate result
	int result;
	for(int i=0; i<usTable.size(); i++)
	{
		result = ( newUsTable[i] - oldUsTable[i] );
		usTable[i] = ( result < 0 ) ? 0 : result;
	}
	for(int i=0; i<oppoTable.size(); i++)
	{
		result = ( oldOppoTable[i] - newOppoTable[i] );
		oppoTable[i] = ( result < 0 ) ? 0 : result;
	}


	/*
	//debug
	if( action == 126 )
	{
		cout << "usTable: ";
		for(int i=0; i<usTable.size(); i++)
			cout << usTable[i] << " ";
		cout << endl;
		cout << "oppoTable: ";
		for(int i=0; i<oppoTable.size(); i++)
			cout << oppoTable[i] << " ";
		cout << endl;
		cout << "oldOppoTable: ";
		for(int i=0; i<oldOppoTable.size(); i++)
			cout << oldOppoTable[i] << " ";
		cout << endl;
		cout << "newOppoTable: ";
		for(int i=0; i<newOppoTable.size(); i++)
			cout << newOppoTable[i] << " ";
		cout << endl;

		cout << "right line: ";
		for(int i=0; i<rightLineTb[ pos2axis[93][2].first ].size(); i++)
			cout << board[ rightLineTb[ pos2axis[93][2].first ][i] ] << " ";
		cout << endl;

		cout << opponent << endl;
	}
	*/

	// if enable tss 
	if( enTss )
	{
		// win => tss success
		if( usTable[WIN5] != 0 || 
				( usTable[ALIVE4] +	usTable[DEAD4] + usTable[LOWDEAD4] + usTable[ALIVE3] + usTable[JUMPALIVE3] ) >= 2 )
			return TSS_SUCCESS;
		// have threat
		else if( ( usTable[ALIVE4] +	usTable[DEAD4] + usTable[LOWDEAD4] + usTable[ALIVE3] + usTable[JUMPALIVE3] ) >= 1 )
			return -1;
		// nothing
		else 
			return 0;
	}

	// if enable conDef
	if( enConDef )
	{
		if( ( oppoTable[ALIVE4] +	oppoTable[DEAD4] + oppoTable[LOWDEAD4] + oppoTable[ALIVE3] + oppoTable[JUMPALIVE3] ) == 0)
			return -3;
		else
			return 0;
	}

	// return if WIN5
	if( usTable[WIN5] >= 1)
		return numeric_limits<int>::max();

	// class 0: defend oppo dead4 and alive4
	if( ( oppoTable[DEAD4] + oppoTable[LOWDEAD4] ) >= 1 || oppoTable[ALIVE4] >= 1)
		return 90000000; //10^7

	// class 1: attack us alive4, 2*dead4, dead4alive3
	if( usTable[ALIVE4] >= 1 ||
			( usTable[DEAD4] + usTable[LOWDEAD4] ) >= 2	||
			( ( usTable[DEAD4] + usTable[LOWDEAD4] ) >= 1 && ( usTable[ALIVE3] + usTable[JUMPALIVE3] ) >= 1 ) )
		return 80000000; //10^7

	// class 2: defend oppo alive4, 2*dead4, dead4alive3
	if( ( oppoTable[ALIVE3] + oppoTable[JUMPALIVE3]) >= 1 ||
			oppoTable[DEAD3] >= 2	||
			( oppoTable[DEAD3]>= 1 && ( oppoTable[ALIVE2] + oppoTable[LOWALIVE2] ) >= 1 ) )
		return 70000000; //10^7

	// class 3: attack us 2*alive3
	if( ( usTable[ALIVE3] + usTable[JUMPALIVE3] ) >= 2 )
		return 60000000; //10^7

	// class 4: deffend oppo 2*alive3
	if( ( oppoTable[ALIVE2] + oppoTable[LOWALIVE2] ) >= 2 )
		return 50000000; //10^7

	// scoring 
	// DEAD4 
	score += ( usTable[DEAD4] * 8000 );
	score += ( usTable[LOWDEAD4] * 8000 );
		
	// ALIVE3 
	score += ( usTable[ALIVE3] * 2100 );
	score += ( usTable[JUMPALIVE3] * 2100 );
	score += ( oppoTable[ALIVE3] * 8000 );
	score += ( oppoTable[JUMPALIVE3] * 8000 );
		
	// DEAD3 
	score += ( usTable[DEAD3] * 300 );
	score += ( oppoTable[DEAD3] * 1400 );
		
	// ALIVE2 
	score += ( usTable[ALIVE2] * 250 );
	score += ( usTable[LOWALIVE2] * 250 );
	score += ( oppoTable[ALIVE2] * 3000 );
	score += ( oppoTable[LOWALIVE2] * 3000 );
		
	// DEAD2 
	score += ( usTable[DEAD2] * 50 );
	score += ( oppoTable[DEAD2] * 100 );
		
	return score;
}

// see if baord[index] has neighbor
bool game_tree::hasNeighbor(vector<int> board, int index)
{
	int check_index;

	// neighbor
	check_index = hrzLineTb[ pos2axis[index][0].first ][ pos2axis[index][0].second-1 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = hrzLineTb[ pos2axis[index][0].first ][ pos2axis[index][0].second+1 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	// next neighbor
	check_index = hrzLineTb[ pos2axis[index][0].first ][ pos2axis[index][0].second-2 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = hrzLineTb[ pos2axis[index][0].first ][ pos2axis[index][0].second+2 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;


	check_index = leftLineTb[ pos2axis[index][1].first ][ pos2axis[index][1].second-1 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = leftLineTb[ pos2axis[index][1].first ][ pos2axis[index][1].second+1 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = leftLineTb[ pos2axis[index][1].first ][ pos2axis[index][1].second-2 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = leftLineTb[ pos2axis[index][1].first ][ pos2axis[index][1].second+2 ];
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;

	check_index = rightLineTb[ pos2axis[index][2].first ][ pos2axis[index][2].second-1 ];	
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = rightLineTb[ pos2axis[index][2].first ][ pos2axis[index][2].second+1 ];	
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = rightLineTb[ pos2axis[index][2].first ][ pos2axis[index][2].second-2 ];	
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;
	check_index = rightLineTb[ pos2axis[index][2].first ][ pos2axis[index][2].second+2 ];	
	if(check_index >= 0 && check_index < 217 && board[check_index] != 0)
		return true;

	return false;
}


// constructor
game_tree::game_tree() :  hrzLineTb(17), leftLineTb(17), rightLineTb(17), pos2axis(217, vector<pair<int, int> >(3)), evaResult(11,0), totalCreatedNode(0)
{
	// initialize look up table for each axis
	axisLutInit();

	// initialize look up table for position 2 axis index
	for(int i=0; i<hrzLineTb.size(); i++)
		for(int j=0; j<hrzLineTb[i].size(); j++)
		{
			pos2axis[ hrzLineTb[i][j] ][0].first = i;
			pos2axis[ hrzLineTb[i][j] ][0].second = j;
		}

	for(int i=0; i<leftLineTb.size(); i++)
		for(int j=0; j<leftLineTb[i].size(); j++)
		{
			pos2axis[ leftLineTb[i][j] ][1].first = i;
			pos2axis[ leftLineTb[i][j] ][1].second = j;
		}

	for(int i=0; i<rightLineTb.size(); i++)
		for(int j=0; j<rightLineTb[i].size(); j++)
		{
			pos2axis[ rightLineTb[i][j] ][2].first = i;
			pos2axis[ rightLineTb[i][j] ][2].second = j;
		}
}

// clear evaluation result table
void game_tree::clearEvaResult()
{
	for(int i=0; i<evaResult.size(); i++)
		evaResult[i] = 0;
}

// calculate game board score
int game_tree::boardScore()
{
	int score = 0;
	// evaluation table
	
	if( evaResult[ WIN5 ] >= 1 )
		// win condition, just return
		score +=  WIN5SCORE;
	else if( evaResult[ ALIVE4 ] >= 1 ||
		 			 evaResult[ DEAD4 ] + evaResult[ LOWDEAD4 ] >= 2 ||
					 ( evaResult[ DEAD4 ] + evaResult[ LOWDEAD4 ] >= 1 && evaResult[ ALIVE3 ] + evaResult[ JUMPALIVE3 ] >= 1 ) )
		return 20000;

	while( evaResult[ ALIVE3 ] + evaResult[ JUMPALIVE3 ] >= 2 )
	{
		if( evaResult[ ALIVE3 ] > 0 )
			evaResult[ ALIVE3 ]--;
		else
			evaResult[ JUMPALIVE3 ]--;

		if( evaResult[ ALIVE3 ] > 0 )
			evaResult[ ALIVE3 ]--;
		else
			evaResult[ JUMPALIVE3 ]--;

		score += 5000;
	}


	while( evaResult[ ALIVE3 ] + evaResult[ JUMPALIVE3 ] >= 1 && evaResult[ DEAD3 ] >= 1 )
	{
		evaResult[ DEAD3 ]--;
		if( evaResult[ ALIVE3 ] > 0)
			evaResult[ ALIVE3 ]--;
		else 
			evaResult[ JUMPALIVE3 ]--;

		score += 1000;
	}

	while( evaResult[ LOWALIVE2 ] + evaResult[ ALIVE2 ] >= 2 )
	{
		if( evaResult[ ALIVE2 ] > 0)
			evaResult[ ALIVE2 ]--;
		else 
			evaResult[ LOWALIVE2 ]--;

		if( evaResult[ ALIVE2 ] > 0)
			evaResult[ ALIVE2 ]--;
		else 
			evaResult[ LOWALIVE2 ]--;

		score += 50;
	}

	score += evaResult[ DEAD4 ] * 500;
	score += evaResult[ LOWDEAD4 ] * 400;
	score += evaResult[ ALIVE3 ] * 100;
	score += evaResult[ JUMPALIVE3 ] * 90;
	score += evaResult[ ALIVE2 ] * 10;
	score += evaResult[ LOWALIVE2 ] * 9;
	score += evaResult[ DEAD3 ] * 5;
	score += evaResult[ DEAD2 ] * 2;

	return score;
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
}

// generate random move
int game_tree::genRandomMove(vector<int> board)
{
	for(int i=0; i<board.size(); i++)
		if( board[i] == 0 )
		{
			cout << i;
			return i;
		}
}

// print evaResult for debug
void game_tree::printEvaResult()
{
	for(int i=0; i<evaResult.size(); i++)
		switch( i )
		{
			case WIN5:
				cout << "WIN5: " << evaResult[i] << endl;
				break;
			case ALIVE4:
				cout << "ALIVE4: " << evaResult[i] << endl;
				break;
			case DEAD4:
				cout << "DEAD4: " << evaResult[i] << endl;
				break;
			case LOWDEAD4:
				cout << "LOWDEAD4: " << evaResult[i] << endl;
				break;
			case ALIVE3:
				cout << "ALIVE3: " << evaResult[i] << endl;
				break;
			case JUMPALIVE3:
				cout << "JUMPALIVE3: " << evaResult[i] << endl;
				break;
			case DEAD3:
				cout << "DEAD3: " << evaResult[i] << endl;
				break;
			case ALIVE2:
				cout << "ALIVE2: " << evaResult[i] << endl;
				break;
			case LOWALIVE2:
				cout << "LOWALIVE2: " << evaResult[i] << endl;
				break;
			case DEAD2:
				cout << "DEAD2: " << evaResult[i] << endl;
				break;
			default:
				cout << "NOTHING: " << evaResult[i] << endl;
		}
}
