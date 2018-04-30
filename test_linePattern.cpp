#include <iostream>
#include <string>
using namespace std;

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

void evaPattern(int numStoneInRow, string linePattern, int leftEnd, int rightEnd)
{
	
	if( numStoneInRow == 5 )
	{
		// x11111x
		cout << "win5 "; 
		return;
	}

	if( numStoneInRow == 4 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? '2' : linePattern[leftEnd-1]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? '2' : linePattern[rightEnd+1]; 

		if( left1 == '0' && right1 == '0' )
			// 011110
			cout << "alive4 ";
		else if( ( left1 == '2' && right1 == '0' ) || ( left1 == '0' && right1 == '2' ) )
			// 211110 or 011112
			cout << "dead4 ";
		else
			// 211112
			cout << "nothing ";

		return;
	}

	if( numStoneInRow == 3 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? '2' : linePattern[leftEnd-1]; 
		int left2 = ( leftEnd-2 < 0 ) ? '2' : linePattern[leftEnd-2]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? '2' : linePattern[rightEnd+1]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? '2' : linePattern[rightEnd+2]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? '2' : linePattern[rightEnd+3]; 

		if( left1 != '1' && right1 == '0' && right2 == '1' && right3 != '1' )
			// x11101x
			cout << "lowDead4 ";
		else if( ( left2 != '1' && left1 == '0' && right1 == '0' && right2 == '0') ||
						 ( right2 != '1' && right1 == '0' && left1 == '0' && left2 == '0' ) )
			// x011100 or 001110x
			cout << "alive3 ";
		else if( ( left1 == '2' && right1 == '0' && right2 == '0' ) ||
						 ( right1 == '2' && left1 == '0' && left2 == '0' ) )
			// 211100 or 001112
			cout << "dead3 ";
		else if( left2 == '2' && left1 == '0' && right1 == '0' && right2 == '2' )
			// 2011102
			cout << "dead3 ";

		return;
	}

	if( numStoneInRow == 2 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? '2' : linePattern[leftEnd-1]; 
		int left2 = ( leftEnd-2 < 0 ) ? '2' : linePattern[leftEnd-2]; 
		int left3 = ( leftEnd-3 < 0 ) ? '2' : linePattern[leftEnd-3]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? '2' : linePattern[rightEnd+1]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? '2' : linePattern[rightEnd+2]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? '2' : linePattern[rightEnd+3]; 
		int right4 = ( rightEnd+4 == linePattern.size() ) ? '2' : linePattern[rightEnd+4]; 

		if( left1 != '1' && right1 == '0' && right2 == '1' && right3 == '1' && right4 != '1' )
			// x11011x
			cout << "lowDead4 ";
		else if( left1 == '0' && right1 == '0' && right2 == '1' && right3 == '0' )
			// 011010
			cout << "jumpAlive3 ";
		else if( left1 == '2' && right1 == '0' && right2 == '1' && right3 == '0' )
			// 211010
			cout << "dead3 ";
		else if( left1 == '0' && right1 == '0' && right2 == '1' && right3 == '2' )
			// 011012
			cout << "dead3 ";
		else if( left1 != '1' && right1 == '0' && right2 == '0' && right3 == '1' && right4 != '1' )
			// x11001x
			cout << "dead3 ";
		else if( left2 == '0' && left1 == '0' && right1 == '0' && right2 == '0')
			// 001100
			cout << "alive2 ";
		else if( ( left1 == '2' && right1 == '0' && right2 == '0' && right3 == '0') ||
						 ( right1 == '2' && left1 == '0' && left2 == '0' && left3 == '0') )
			// 211000 or 000112
			cout << "dead2 ";
		else if( ( left3 == '2' && left2 == '0' && left1 == '0' && right1 == '0' && right2 == '2') ||
						 ( right3 == '2' && right2 == '0' && right1 == '0' && left1 == '0' && left2 == '2' ) )
			// 2001102 or 2011002
			cout << "dead2 ";

		return;
	}

	if( numStoneInRow == 1 )
	{
		int left1 = ( leftEnd-1 < 0 ) ? '2' : linePattern[leftEnd-1]; 
		int left2 = ( leftEnd-2 < 0 ) ? '2' : linePattern[leftEnd-2]; 
		int left3 = ( leftEnd-3 < 0 ) ? '2' : linePattern[leftEnd-3]; 
		int left4 = ( leftEnd-4 < 0 ) ? '2' : linePattern[leftEnd-4]; 
		int right1 = ( rightEnd+1 == linePattern.size() ) ? '2' : linePattern[rightEnd+1]; 
		int right2 = ( rightEnd+2 == linePattern.size() ) ? '2' : linePattern[rightEnd+2]; 
		int right3 = ( rightEnd+3 == linePattern.size() ) ? '2' : linePattern[rightEnd+3]; 
		int right4 = ( rightEnd+4 == linePattern.size() ) ? '2' : linePattern[rightEnd+4]; 
		int right5 = ( rightEnd+5 == linePattern.size() ) ? '2' : linePattern[rightEnd+5]; 

		// x1---1x
		if( left1 != '1' && right1 == '0' && right4 == '1' && right5 != '1'	)
		{
			if( right2 == '1' && right3 == '1' )
				// x10(11)1x
				cout << "lowdead4 ";
			else if( right2 == '0' && right3 == '0' )
				// x10(00)1x
				cout << "dead2 ";
			else if( right2 == '1' && right3 == '0' )
				// x10(10)1x
				cout << "dead3 ";
			else if( right2 == '0' && right3 == '1' )
				// x10(01)1x
				cout << "dead3 ";
		}
		// 010-1
		else if( left1 == '0' && right1 == '0' && right3 == '1' )
		{
			if( right4 == '0' )
			{
				if(right2 == '0')
					// 010010
					cout << "lowAlive2 ";
				else 
					// 010110
					cout << "jumpAlive3 ";
			}
			else if( right2 == '1' && right4 == '2' )
				// 010112
				cout << "dead3 ";
			else if( left2 == '2' && right2 == '0' && right4 == '2' )
				// 2010012
				cout << "dead2 ";
		}
		// 21---0
		else if( left1 == '2' && right4 == '0' )
		{
			if( right1 == '0' )
			{
				if( right2 == '1' && right3 == '0' )
					// 210100
					cout << "dead2 ";
				else if( right2 == '0' && right3 == '1' )
					// 210010
					cout << "dead2 ";
				else if( right2 == '1' && right3 == '1' )
					// 210110
					cout << "dead3 ";
			}
			else if( right1 == '1' && right2 == '0' && right3 == '1' )
				// 211010
				cout << "dead3 ";
		}
		// 210---2
		else if( left1 == '2' && right1 == '0' && right5 == '2' )
		{
			if( right2 == '1' && right3 == '0' && right4 == '0' )
				// 2101002
				cout << "dead2 ";
			else if( right2 == '0' && right3 == '1' && right4 == '0' )
				// 2100102
				cout << "dead2 ";
			else if( right2 == '0' && right3 == '0' && right4 == '1' )
				// 2100012
				cout << "dead2 ";
		}
		// 2010--2
		else if( left2 == '2' && left1 == '0' && right1 == '0' && right2 == '1' && right3 == '0' && right4 == '2' )
			// 2010102
			cout << "dead2 ";
		else if( left3 == '2' && left2 == '0' && left1 == '0' && right1 == '0' && right2 == '1' && right3 == '2')
			// 2001012
			cout << "dead2 ";
		else if( ( left2 != '1' && left1 == '0' && right1 == '0' && right2 == '1' && right3 == '0' && right4 == '0' ) ||
				( left2 == '0' && left1 == '0' && right1 == '0' && right2 == '1' && right3 == '0' && right4 != '1' ) )
			// x010100 or 001010x
			cout << "lowAlive2 ";
		else if( left2 == '0' && left1 == '0' && right1 == '0' && right2 == '1' && right3 == '2' )
			// 001012
			cout << "dead2 ";

		return;
	}


	cout << "nothing ";
}

void score(string linePattern) 
{
	size_t pos_1, pos_2;

	pos_1 = linePattern.find_first_of("1");
	if (pos_1 != string::npos)
		pos_2 = linePattern.find_first_not_of("1", pos_1+1);
	else
		// didn't find our stone
		return;

	while(1)
	{
		if( pos_2 == string::npos ) 
		{
			// reach end of line pattern
			evaPattern( linePattern.size() - pos_1, linePattern, pos_1, linePattern.size()-1);
			return;
		}
		else
			evaPattern(pos_2 - pos_1, linePattern, pos_1, pos_2-1);

		cout << endl;

		pos_1 = linePattern.find_first_of("1", pos_2+1);
		if (pos_1 != string::npos)
			pos_2 = linePattern.find_first_not_of("1", pos_1+1);
		else
			// didn't find our stone
			break;
	}

	return;
}

int main() {
	string linePattern;

	while(cin >> linePattern) 
		score( linePattern );

	return 0;
}
