#include <iostream>
#include <fstream>
#include <ios>
using namespace std;

#define STATE_FILE "state_27.txt"
#define MOVE_FILE "move_27.txt"


int main() {

	fstream f;
	f.open(STATE_FILE);

	/* check file exist */
	while(!f) 
		f.open(STATE_FILE);

	/* check file not empty */
	while(f.peek() == ifstream::traits_type::eof()) {
		f.close();
		f.open(STATE_FILE);
	}
	f.close();


	int get_turn = -1;
	int next_turn = -2;

	while(1) {
		/* read game state file */
		while(get_turn != next_turn) {
			f.open(STATE_FILE);
			f >> get_turn;
			f.close();

			if((next_turn != 4 && get_turn == 2) || (next_turn != 3 && get_turn == 1))
				next_turn = get_turn;
		}

		/* print current step */
		cout << next_turn << endl;

		/* calculate next move */
		for(int i=0;i<1000000;i++);

		/* write my move */
		f.open(MOVE_FILE);
		f << next_turn;
		f.close();

		next_turn+=2;
	}



	

	


	return 0;
}
