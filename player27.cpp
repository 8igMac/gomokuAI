#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

#define DEBUG 0
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
	int next_move = -1;
	vector<int> game_state(217,0);

	while(1) {
		/* read game state file */
		while(1) {
			f.open(STATE_FILE);
			f >> get_turn;
			if(get_turn == next_turn) {
				for(int i=0; i<game_state.size(); i++) {
					f >> game_state[i];
				}
				f.close();
				break;
			}
			f.close();

			if((next_turn != 4 && get_turn == 2) || (next_turn != 3 && get_turn == 1))
				next_turn = get_turn;
		}

		// debug: print read message
		if(DEBUG) {
			cout << get_turn << endl;
			for(auto item: game_state)
				cout << item << " ";
			cout << endl;
		}

		/* calculate next move */
		for(int i=0; i<game_state.size(); i++) 
			if(game_state[i] == 0) {
				next_move = i;
				break;
			}
		this_thread::sleep_for(chrono::milliseconds(2000)); // sleep for 2 secs

		// debug: print ai's next move
		if(DEBUG)
			cout << "next move: " << next_move << endl; //debug

		/* write my move */
		f.open(MOVE_FILE,ios_base::out);
		f << next_turn << " " << next_move;
		f.close();

		next_turn+=2;
	}



	

	


	return 0;
}
