#include "state.h"
#include <queue>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class game {
	state initial, goal;
	priority_queue<state, vector<state>, greater<state> > queue;
	vector<state> used;
	private:
		const int iterationLimit = 20000;
		int X, Y, iterationNumber = 0;
		void nextMove();
		bool notUsed(state);
	
	public:
		game(int, int);
		game(int *input, int, int);
};

game::game(int X = 3, int Y = 3) {
	if (DEBUG)
		cout << "construct" << endl;
	if ( X < 1 || Y < 1 ) {
			X = 3;
			Y = 3;
		}

	state start(X, Y);
	
	state end(goalArray, X, Y);
	initial = start;
	goal = end;
	queue.push(start);
	nextMove();
}

game::game(int *input, int X = 3, int Y = 3) {
	if (DEBUG)
		cout << "input construct" << endl;
	bool normal = true;
	if ( X < 1 || Y < 1 ) {
		X = 3;
		Y = 3;
	}

	for ( int i = 0; i < X*Y; i++ ) {
		if (input[i] == -1)
			normal = false;
	}

	if ( normal ) {
		state start(input, X, Y);
		initial = start;
		queue.push(start);
	}
	else {
		state start(X, Y);
		initial = start;
		queue.push(start);
	}
	
	state end(goalArray, X, Y);
	goal = end;
	nextMove();
}

//Gets the next moves and pushes them into the queue
//Checks to see if the state is used before pushing into queue
void game::nextMove() {
	//test if goal
	state test;
	bool end = false;
	do {
		if (DEBUG)
			cout << "nextMove" << endl;
	test = queue.top();
	queue.pop();
	if ( test == goal ) {
		end = true;
		break;
	}
	if ( notUsed(test) ) {
		//Get moves from state
		if (DEBUG)
			cout << "notUsed" << endl;
		vector<state> movement;
		test.moves(movement);
		while (!movement.empty()) {
			queue.push(movement.back());
			movement.pop_back();
		}
		cout << test << endl;
		used.push_back(test);
		iterationNumber++;
	}
	} while ( !queue.empty() && iterationNumber < iterationLimit );
	if ( end ) {
		cout << "Match found " << test << endl;
		cout << "Started with " << initial << endl;
		cout << "Iterations Used: " << used.size();
	}
	else if ( iterationNumber == iterationLimit ) {
		cout << "Match not found within iteration limit" << endl;
		cout << "Started with " << initial << endl;
		cout << "Iterations Used: " << used.size() << endl;
	}
	else {
		cout << "Unsolvable, Queue is empty." << endl;
		cout << "Started with " << initial << endl;
		cout << "Try again with new input" << endl;
	}
}

//Checks if a state is used
bool game::notUsed(state test) {
	for ( int i = 0; i < used.size(); ++i ) {
		if ( used.at(i) == test ) {
			return false;
			cout << "used" << endl;
		}
	}
	return true;
}


int main() {
	srand(time(NULL));
	int X = 3;
	int Y = 3;
	int input[X*Y];
	bool negative = false;
	cout << "Please input numbers from 0-" << X*Y << endl
			<< "Or -1 to continue" << endl;
	for (int i = 0; i < X*Y && !negative; i++) {
		bool used = false;
		bool message = true;
		do {
			if ( message ) {
				cout << X*Y-i << " numbers remaining." << endl;
				message = false;
			}
			else
				cout << "Please insert an unused number between 0 and " << X*Y-1 << endl;
			cin >> input[i];
			if ( input[i] == -1 ) {
				negative = true;
				break;
			}
			for (int j = 0; j < i; j++)
				if ( input[i] == input[j] )
					used = true;
		} while ( used || input[i] < 0 || input[i] > 8 );

	}
	if ( negative )
		game game;
	else
		game game(input);



	return 0;
}

