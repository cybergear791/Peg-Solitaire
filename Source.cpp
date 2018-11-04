//lab 7
//created by nick flouty
// Peg-solitaire with or without using a hash table.

#include "SeparateChaining.cpp"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

bool solved(vector<bool>  x) 
{
	int counter = 0;
	for (int i = 0; i < x.size(); i++)
	{
		if (x[i] == true)
			counter++;
	}
	if(counter == 1) 
		return true;
	return false;
}

bool isValidSpot(vector<int> & board, int i) {
	if (board[i] == 0 || board[i] == 1 || board[i] == 5 || board[i] == 6 || board[i] == 7 || board[i] == 8 || board[i] == 12 || board[i] == 13 || board[i] == 35 || board[i] == 36 || board[i] == 40 || board[i] == 41 || board[i] == 42 || board[i] == 43 || board[i] == 47 || board[i] == 48) //works?
		return false;
	return true;
}
bool canMoveUp(vector<int> & board, int i)
{
	if (board[i] == 2 || board[i] == 3 || board[i] == 4 || board[i] == 9 || board[i] == 10 || board[i] == 11)
		return false;
	return true;
}
bool canMoveRight(vector<int> & board, int i)
{
	if (board[i] == 19 || board[i] == 20 || board[i] == 26 || board[i] == 27 || board[i] == 33 || board[i] == 34)
		return false;
	return true;
}
bool canMoveLeft(vector<int> & board, int i)
{
	if (board[i] == 14 || board[i] == 15 || board[i] == 21 || board[i] == 22 || board[i] == 28 || board[i] == 29)
		return false;
	return true;
}
bool canMoveDown(vector<int> board, int i)
{
	if (board[i] == 37 || board[i] == 38 || board[i] == 39 || board[i] == 44 || board[i] == 45 || board[i] == 46)
		return false;
	return true;
}

vector<vector<int> > MoveList( vector<bool> Pins, vector<int> board){
	vector<vector<int> > movelist;
	
	for (int i = 0; i < Pins.size(); i++) {
		if (isValidSpot(board, i) && Pins[i])
		{
			if (canMoveRight(board, i)  && Pins[i + 1] && !Pins[i + 2])	// +1 right neighbor.     -1 left neighbor			+7 down neighbor	-7 up
			{
				vector<int> moveRight;
				moveRight.resize(3);
				moveRight[0] = i;		
				moveRight[1] = i+1;
				moveRight[2] = i+2;
				
				movelist.push_back(moveRight);

			}
			if (canMoveLeft(board, i)  && Pins[i - 1] && !Pins[i - 2])
			{
				vector<int> moveLeft;
				moveLeft.resize(3);
				moveLeft[0] = i;
				moveLeft[1] = i - 1;
				moveLeft[2] = i - 2;

				movelist.push_back(moveLeft);
			}
			if (canMoveDown(board, i) && Pins[i + 7] && !Pins[i + 14])
			{
				vector<int> moveDown;
				moveDown.resize(3);
				moveDown[0] = i;
				moveDown[1] = i + 7;
				moveDown[2] = i + 14;

				movelist.push_back(moveDown);
			}
			if (canMoveUp(board, i) && Pins[i - 7] && !Pins[i - 14])
			{
				vector<int> moveUp;
				moveUp.resize(3);
				moveUp[0] = i;
				moveUp[1] = i - 7;
				moveUp[2] = i - 14;

				movelist.push_back(moveUp);
			}
		}
	}
	return movelist;

};
vector<bool> makeMove(vector<int>  move, vector<bool> Pins) {
			Pins[move[0]] = false;
			Pins[move[1]] = false;
			Pins[move[2]] = true;
	return Pins;
}

long long int convert(vector<bool> board)
{
	long long int c = 0;
	for (int j = 0; j < board.size(); j++)
	{
		if (board[j] == true)
			c |= (long long)1 <<j;

	}
	return c;
}

bool hsolve( vector<vector<int> > &moveList,vector<bool>  pins,vector<int> board, HashTable<long long int> & H) {	//open hashinga
	if (solved(pins)) 
		return true;
	vector<vector<int> > move;	
	move = MoveList(pins,board);	
	
	for (int i = 0; i < move.size(); i++)
	{
		vector<bool> NewPins;
		NewPins = makeMove(move[i],pins);
		long long int c = convert(NewPins);
		if (!H.contains(c)) {
			if (hsolve(moveList,NewPins,board, H))
			{
				moveList.push_back(move[i]);
				return true;
			}
			else	
				H.insert(c);
		}
	}
	return false;
}
//****************************
bool solve(vector<vector<int> > &moveList, vector<bool>  pins, vector<int> board) {		//no hashing
	if (solved(pins))
		return true;
	vector<vector<int> > move;
	move = MoveList(pins, board);	//size is 0
	
	for (int i = 0; i < move.size(); i++)
	{
		vector<bool> NewPins;
		NewPins = makeMove(move[i], pins);
		if (solve(moveList, NewPins, board)) {
			moveList.push_back(move[i]);
			return true;
			}
	}
	return false;
}
//*********************************************************
int main() {
	int input;
	cout<<"What would you like to do? "<<endl<<"1) Run DFS without using Hashing."<<endl<<"2) Run DFS with using Hashing."<<endl<<"3) Exit."<<endl<<">>";
	cin >> input;
	
	while(input != 3)
	{

	string name;
	cout << "Enter the name of the input board: ";
	cin >> name;
	ifstream fname(name.c_str());
	int temp;
	vector<int> pins;
	while (fname >> temp)
		pins.push_back(temp);

	vector<int> board;
	board.resize(49);
	vector<bool> boardPins;
	boardPins.resize(49, false);

	for (int i = 0; i < pins.size(); i++)
	{
		for (int j = 1; j != board.size(); j++)
		{
			board[j] = j;		//make 1D vector board of ints.
			if (board[j] == pins[i])	//marks 1D vector of board bool; true if theres a pin there
				boardPins[j] = true;
		}
	}
//************************************************NO HASH
if(input == 1){
	vector<vector<int> > moveList;
	clock_t cl =clock();
	if (solve(moveList, boardPins, board)) {
		cl = clock() - cl;
		cout << "Non-Hashing Solved in " <<cl/(double)CLOCKS_PER_SEC<<" seconds"<< endl;
		for (int i = 0; i < moveList.size(); i++)
		{
			cout << "( ";
			for (int j = 0; j < moveList[i].size(); j++)
				cout << moveList[i][j]<<" ";
			cout << ") ";
		}
		cout<<endl;
	}
	else
		cout << "No possible scenarios." << endl;
}
//**********************************************************HASH
if (input ==2){
	HashTable<long long int>  H(5000);
	vector<vector<int> > HmoveList;	
	clock_t cl2 = clock();
	if (hsolve(HmoveList, boardPins, board, H)) {
		cl2 = clock() - cl2;
		cout << "Hashing of table size 5000 Solved in " <<cl2/(double)CLOCKS_PER_SEC<<" seconds"<< endl;	
		for (int i = 0; i < HmoveList.size(); i++)
		{
			cout << "( ";
			for (int j = 0; j <HmoveList[i].size(); j++)
				cout << HmoveList[i][j]<<" ";
			cout << ") ";
		}
		cout<<endl;
	}
	else
		cout << "No possible scenarios." << endl;

//**********************************************************HASH 10K

	HashTable<long long int>  H2(10000);
	vector<vector<int> > HmoveList2;	
	clock_t cl3 = clock();
	if (hsolve(HmoveList2, boardPins, board, H2)) {
		cl3 = clock() - cl3;
		cout << "Hashing of table size 10,000 Solved in " <<cl3/(double)CLOCKS_PER_SEC<<" seconds"<< endl;	
		for (int i = 0; i < HmoveList2.size(); i++)
		{
			cout << "( ";
			for (int j = 0; j <HmoveList2[i].size(); j++)
				cout << HmoveList2[i][j]<<" ";
			cout << ") ";
		}
		cout<<endl;
	}
	else
		cout << "No possible scenarios." << endl;

//**********************************************************HASK 100K

	HashTable<long long int>  H3(100000);
	vector<vector<int> > HmoveList3;	
	clock_t cl4 = clock();
	if (hsolve(HmoveList3, boardPins, board, H3)) {
		cl4 = clock() - cl4;
		cout << "Hashing of table size 100,000 Solved in " <<cl4/(double)CLOCKS_PER_SEC<<" seconds"<< endl;	
		for (int i = 0; i < HmoveList3.size(); i++)
		{
			cout << "( ";
			for (int j = 0; j <HmoveList3[i].size(); j++)
				cout << HmoveList3[i][j]<<" ";
			cout << ") ";
		}
		cout<<endl;
	}
	else
		cout << "No possible scenarios." << endl;
}
	int input2;
	cout<<"What would you like to do? "<<endl<<"1) Run DFS without using Hashing."<<endl<<"2) Run DFS with using Hashing. "<<endl<<"3) Exit."<<endl<<">>";
	cin >> input2;
	input = input2;
}
	return 0;
}
