//Controls the state of the game's board
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
using namespace std;

const bool DEBUG = false;
constexpr static int goalArray[9] = {1,2,3,4,5,6,7,8,0};

class state {
	private:
		int* board;
		int rating, action, X, Y, size;
		void randomAllocate();
		void manhattanGen();
		int goalMatch(int);
		int posMatch(int);
	public:
		state operator=(const state&);
		bool operator==(state);
		bool operator!=(state);
		friend ostream& operator<<(ostream&, state&);
		friend bool operator>(state, state);
		friend bool operator<(state, state);
		

		state(int, int);
		state(const int *input, int, int, int);
		state(const state&);
		~state();
		int returnPos(int);
		int retDist();
		void setDist(int);
		int manhattan();
		void moves(vector<state>&);
};


//In constructor pass the X, Y and Goal State - Makes class completely flexible
state::state(int x = 3, int y = 3) {
	X = x;
	Y = y;
	rating = 0;
	action = 0;
	size = X*Y;
	board = new int[Y*X];
	randomAllocate();
	manhattanGen();
}

state::state(const int *input, int x = 3, int y = 3, int act = 0) {
	X = x;
	Y = y;
	rating = 0;
	action = act;
	size = X*Y;
	board = new int[Y*X];
	if ( DEBUG ) {
		cout << "Inputting String: ";
		for (int i = 0; i < Y*X; i++) {
			cout << input[i] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < Y*X; i++) {
			board[i] = input[i];
	}
	manhattanGen();
}


//makes a copy of the state passed as a parameter
//memcpy is more efficient than copying by values
state::state(const state &copy) {
	X = copy.X;
	Y = copy.Y;
	rating = copy.rating;
	size = copy.size;
	action = copy.action;
	board = new int[size];
	memcpy(board, copy.board, sizeof(int) * size);
}

state::~state() {
	delete[] board;
}

state state::operator=(const state &copy) {
	//In case its the same thing it doesn't all break
	if ( this == &copy ) {
		return *this;
	}
	//free the memory before making more
	delete[] board;

	X = copy.X;
	Y = copy.Y;
	rating = copy.rating;
	size = copy.size;
	action = copy.action;
	board = new int[size];
	memcpy(board, copy.board, sizeof(int) * size);

	return *this;
}

bool state::operator==(state compare) {
	for (int i = 0; i < X*Y; i++) {
		if ( compare.returnPos(i) != returnPos(i)  ) {
			return false;
		}
	}
	return true;
}

bool state::operator!=(state compare) {
	for (int i = 0; i < X*Y; i++) {
		if ( compare.returnPos(i) != returnPos(i)  ) {
			return true;
		}
	}
	
	return false;
	
}

ostream& operator<<(ostream& os, state& State) {
	for (int i = 0; i < State.X*State.Y; i++) {
		if ( i % State.X == 0 )
			os << endl;
		if ( State.returnPos(i) != 0 )
			os << " " << State.returnPos(i) << " ";
		else
			os << "   ";
	}
	os << endl << "Distance: " << State.rating << endl;
	os << "Moves from Start: " << State.retDist() << endl;

	/*
	for (int y = 0; y < Y; y++) {
		os << endl;
			for (int x = 0; x < X; x++) {
				if ( State.board != 0 )
					os << " " << State.board[y][x] << " ";
				else
					os << "   ";
			}
		}
	os << endl;
	*/
    return os;
}

bool operator>(state comparison, state compare) {
	if ( ( comparison.manhattan() + comparison.retDist() ) > ( compare.manhattan() + compare.retDist() ) ) {
		return true;
	}
	return false;
}

bool operator<(state comparison, state compare) {
	if ( ( comparison.manhattan() + comparison.retDist() ) < ( compare.manhattan() + compare.retDist() ) ) {
		return true;
	}
	return false;
}

//bool state::operator<(state compare) {
	//if ( manhattan() < compare.manhattan() ) {
		//return true;
	//}
	//return false;
//}

//Return the value at a position
int state::returnPos(int pos) {
	return board[pos];
}

//Create a random state
void state::randomAllocate() {
	vector<int> tempVect;
	for (int i = 0; i < X*Y; i++) {
		tempVect.push_back(i);
	}
	for (int i = 0; i < Y*X; i++) {
			int select = rand() % tempVect.size();
			board[i] = tempVect.at(select);
			tempVect.erase(tempVect.begin() + select);
	}
}

//Return number of turns passed
int state::retDist() {
	return action;
}

//Set number of turns passed
void state::setDist(int dist) {
	action = dist;
}

int state::manhattan() {
	return rating;
}


//Works out manhattan number
// x = i % X
// y = i / X
void state::manhattanGen() {
	for (int i = 0; i < Y*X; i++) {
			int matchVal = goalMatch(board[i]);
			int temp = 0;
			//Error Checking
			if (matchVal == -1) {
				cout << "No Goal Match Found" << endl;
				rating = -1;
				return;
			}
			temp = ( (i % X) - (matchVal % X) ) + ( ( i / X ) - ( matchVal / X) );
			if ( temp < 0 )
				temp = temp * -1;
			rating = rating + temp;
	}
}

//Finding the same value in goal and returning X and Y Coords
int state::goalMatch(int val) {
	for (int i = 0; i < Y*X; i++) {
			if ( val == goalArray[i] ) {
				return i;
			}
	}
	return -1;
}

int state::posMatch(int val) {
	for (int i = 0; i < Y*X; i++) {
				if ( val == board[i] ) {
					return i;
				}
		}
		return -1;

}

//Pass a vector down the level - More efficient then retrieving size
// x = val % X
// y = val / X
void state::moves( vector<state> &array ) {
	if ( DEBUG ) {
		cout << "Testing Moves: ";
		for ( int i = 0; i < X*Y; i++)
			cout << board[i] << " ";
		cout << endl;
	}
	int val = posMatch(0);
	//Error checking
	if ( val == -1 ) {
		cout << "No Match Found" << endl;
		return;
	}
	//Left side move
	if ( val % X > 0 ) {
		if ( DEBUG )
				cout << "left: ";
		int leftMove[X*Y];
		for (int i = 0; i < Y*X; i++) {
			if ( i == val ) {
					leftMove[i-1] = returnPos(i);
					if (DEBUG)
						cout << "val " << i-1 << " " << returnPos(i) << endl;
			}
				else if ( i == val - 1 ) {
					leftMove[i+1] = returnPos(i);
					if (DEBUG)
						cout << "switch " << i+1 << " " << returnPos(i) << endl;
				}
				else
					leftMove[i] = returnPos(i);
			}
		state left(leftMove, X, Y, action + 1);
		if ( DEBUG )
			cout << endl;
		array.push_back(left);
	}
	//Right side move

	if ( val % X < X - 1 ) {
		if ( DEBUG )
				cout << "right: " << endl;
		int rightMove[X*Y];
		for (int i = 0; i < Y*X; i++) {
				if ( i == val ) {
					rightMove[i+1] = returnPos(i);
					if (DEBUG)
						cout << "val " << i+1 << " " << returnPos(i) << endl;
				}
				else if ( i == val + 1 ) {
					rightMove[i-1] = returnPos(i);
					if (DEBUG)
						cout << "switch " << i-1 << " " << returnPos(i) << endl;
				}
				else
					rightMove[i] = returnPos(i);
			}
		
		state right(rightMove, X, Y, action + 1);
		if ( DEBUG )
				cout << endl;
		array.push_back(right);
	}
	//Upwards move
	// val - X = down one Y level

	if ( val / X > 0 ) {
		if (DEBUG)
				cout << "up: ";
		int upMove[X*Y];
		for (int i = 0; i < Y*X; i++) {
				if ( i == val ) {
					upMove[i-X] = returnPos(i);
				if (DEBUG)
						cout << "val " << i-X << " " << returnPos(i) << endl;
				}
				else if ( i == val - X ) {
					upMove[i+X] = returnPos(i);
				if (DEBUG)
						cout << "Switch " << i+X << " " << returnPos(i) << endl;
				}
				else
					upMove[i] = returnPos(i);
		}
		
		state up(upMove, X, Y, action + 1);
		if ( DEBUG )
			cout << endl;
		array.push_back(up);
	}
	//Downwards move
	// val + X = up one Y level

	if ( val / X < Y - 1 ) {
		if (DEBUG)
				cout << "down: ";
		int downMove[X*Y];
		for (int i = 0; i < Y*X; i++) {
				if ( i == val ) {
					downMove[i+X] = returnPos(i);
				if (DEBUG)
						cout << "val " << i+X << " " << returnPos(i) << endl;
				}
				else if ( i == val + X ) {
					downMove[i-X] = returnPos(i);
				if (DEBUG)
					cout << "Switch " << i-X << " " << returnPos(i) << endl;
				}
				else
					downMove[i] = returnPos(i);
		}
		state down(downMove, X, Y, action + 1);
		if ( DEBUG )
			cout << endl;
		array.push_back(down);
	}
}

