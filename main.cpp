// main.cpp
// Problem Set 4a                                 Chris Canal
// Team cansym                                    canal.c@husky.neu.edu
//                                                Sarada Symonds
//                                                symonds.s@husky.neu.edu
//
// Main program file for homework 4a.
#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int blank = -1;  // Indicates that a cell is blank

const int squareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int boardSize = squareSize * squareSize;

const int minValue = 1;
const int maxValue = 9;

int numSolutions = 0;

class Board
// Stores the entire Sudoku board
{
public:
//Constructor
Board(int);

//Sets all cells to -1 indicating a blank board
void Clear();

//Sets an individual cell to -1
void ClearCell(int i, int j, ValueType val);

//Reads Sudoku text file and puts it into value matrix
void Initialize(ifstream &fin);

//Prints out the board
void Print();

//Prints out the conflicts
void PrintConflicts();

//Checks if the cell is empty(-1)
bool IsBlank(int i, int j);

//checks conflicts in the row, conlumn, and sqaure
bool CheckConflicts(int i, int j, ValueType val);

//gets the value of a cell
ValueType GetCell(int i, int j);

//sets the value of the cell and updates conflicts
void  SetCell(int i,int j, ValueType val);

//adds value to given cell if there are no conflicts and updates
void AddValue(int i, int j, ValueType val);

//checks whether board is solved
bool IsSolved();

//recursively solves the sudoku puzzle
bool PlaceN(int i, int j, int value);

void setConflictsMatrixToZero();

int ** calculateConflictsMatrix();

bool smartPlaceN();

private:

// The following matrices go from 1 to BoardSize in each
// dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)
matrix<ValueType> value;
bool conflictsSq[9][9];
bool conflictsRow[9][9];
bool conflictsCol[9][9];
int cMatrix[9][9];
};

int SquareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.

	return squareSize * (((i-1))/squareSize) + ((j-1))/squareSize + 1;
}

Board::Board(int sqSize)
	: value(boardSize+1,boardSize+1)
// Board constructor
{
}

ValueType Board::GetCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= boardSize && j >= 1 && j <= boardSize)
		return value[i][j];
	else
		throw rangeError("bad value in GetCell");
}

bool Board::IsBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if ( GetCell( i, j) == -1 )
	{
		return true;
	} else if( GetCell( i,  j) > 0 && GetCell( i,  j) < 10 )
	{
		return false;
	}
	else if (i < 1 || i > boardSize || j < 1 || j > boardSize)
		throw rangeError("bad value in SetCell");
	return false;
}

void Board::ClearCell(int i, int j, ValueType val)
//clears a single cell and updates conflicts
{
	value[i][j] = blank;
	int sq = SquareNumber(i, j);
	conflictsRow[(i - 1)][(val - 1)] = true;
	conflictsCol[(j - 1)][(val - 1)] = true;
	conflictsSq[(sq - 1)][(val - 1)] = true;
}

void Board::Clear()
// Clear the entire board.
{
	for (int i = 1; i <= boardSize; i++)
		for (int j = 1; j <= boardSize; j++)
		{
			value[i][j] = blank;
			int sq = SquareNumber(i, j);
			for(int k = 0; k < 9; k++)
			{
				conflictsRow[(i - 1)][k] = true;
				conflictsCol[(j - 1)][k] = true;
				conflictsSq[(sq - 1)][k] = true;
			}

		}
}

void Board::SetCell(int i,int j, ValueType val)
// set cell i,j to val and update conflicts
{
	value[i][j] = val;
	int sq = SquareNumber(i, j);
	conflictsRow[(i - 1)][(val - 1)] = false;
	conflictsCol[(j - 1)][(val - 1)] = false;
	conflictsSq[(sq - 1)][(val - 1)] = false;
}

void Board::Initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
	char ch;

	Clear();
	for (int i = 1; i <= boardSize; i++)
		for (int j = 1; j <= boardSize; j++)
		{
			fin >> ch;

			// If the read char is not Blank
			if (ch != '.')
			{
				SetCell(i,j,ch-'0'); // Convert char to int
			}
		}
}

void Board::setConflictsMatrixToZero(){
	for (int i = 1; i <=9 ; i++){
		for (int j = 1; j <= 9 ; j++){
			cMatrix[i][j] = 0;
		}
	}
}

int ** Board::calculateConflictsMatrix(){
	int ** dynamiccMatrix;
	dynamiccMatrix = new int*[9]; // dynamic `array (size 4) of pointers to int`
	for (int i = 0; i < 9; ++i) {
  		dynamiccMatrix[i] = new int[9];
  		// each i-th pointer is now pointing to dynamic array (size 10) of actual int values
	}
	int sq;
	bool r, c, s;
	for (int i = 1; i <=9 ; i++){
		for (int j = 1; j <= 9 ; j++){
			for (int k = 1; k <= 9 ; k++){
				if (CheckConflicts(i, j, k)) {
					dynamiccMatrix[(i-1)][(j-1)] += 1;
				};
			}
		}
	}
	return dynamiccMatrix;
}


bool Board::smartPlaceN(){
	int ** thiscMatrix = calculateConflictsMatrix();
	for (int conflicts = 1; conflicts <= 9; conflicts++){
		for (int x = 1; x <= 9 ; x++){

			for (int y = 1; y <= 9 ; y++){

				if (thiscMatrix[(x-1)][(y-1)] <= conflicts){

					if(IsBlank(x, y)){

						for (int k = 1; k <= 9 ; k++){

							if (CheckConflicts(x,y,k)){

								SetCell(x, y, k);
								cout << "\nCongradulations, a " << k << " was placed at (" << x << ", " << y << ")\n";
								if (IsSolved()){
									cout << "SOLVED!!!!";
									Print();
									return true;
								}
								Print();
								if (smartPlaceN()){
									return true;
								} else {
									cout << "\nRemoving a " << k << " that was placed at (" << x << ", " << y << ")\n";
									ClearCell(x,y,k);
									Print();
								}
							}
						}
						return false;
					}
				}
			}
		}
	}
	return false;
}

bool Board::CheckConflicts(int i, int j, ValueType val)
//checks whether a value in a cell will cause conflicts
{
	int sq = SquareNumber(i, j);
	bool r = conflictsRow[(i - 1)][(val - 1)];
	bool c = conflictsCol[(j - 1)][(val - 1)];
	bool s = conflictsSq[(sq - 1)][(val - 1)];
	//returns false if there IS a conflict
	return r && c && s;
}


ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
	for (int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}



void Board::Print()
// Prints the current board.
{
	for (int i = 1; i <= boardSize; i++)
	{
		if (((i-1)) % squareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= boardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= boardSize; j++)
		{
			if (((j-1)) % squareSize == 0)
				cout << "|";
			if (!IsBlank(i,j))
				cout << " " << GetCell(i,j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;

	}

	cout << " -";
	for (int j = 1; j <= boardSize; j++)
		cout << "---";
	cout << "-";
	cout << endl;
	PrintConflicts();
}

void Board::PrintConflicts()
//prints conflict list in the board
{
	cout << "Row Conflicts: \n";
	for(int i = 0; i < 9; i++)
	//prints out all row conflicts
	{
		for(int j = 0; j < 9; j++)
		//prints out all row conflicts
		{
			cout << conflictsRow[i][j];
		}
		cout << "\n";
	}

	cout << "Column Conflicts\n";
	for(int i = 0; i < 9; i++)
	//prints out all row conflicts
	{
		for(int j = 0; j < 9; j++)
		//prints out all row conflicts
		{
			cout << conflictsCol[i][j];
		}
		cout << "\n";
	}

	cout << "Square Conflicts\n";
	for(int i = 0; i < 9; i++)
	//prints out all row conflicts
	{
		for(int j = 0; j < 9; j++)
		//prints out all row conflicts
		{
			cout << conflictsSq[i][j];
		}
		cout << "\n";
	}

}


bool Board::PlaceN(int i, int j, int value){
	if(IsBlank(i, j)){
		SetCell(i, j, value);
		Print();
	}
	if (i == 9 && j == 9){
		return IsSolved();
	}
	j = (i == 9) ? j+1 : j;
	if(IsBlank((i%9)+1, j)){
		for (int k = 1; k <= 9; k++){
			if (CheckConflicts((i%9)+1,j,k)){
				Print();
				if (PlaceN((i%9)+1,j,k)){
					return true;
				} else {
					ClearCell((i%9)+1,j,k);
					Print();
				}
			}
		}
		return false;
	} else {
		j = (i == 8) ? j+1 : j;
		for (int k = 1; k <= 9; k++){
			if (CheckConflicts((i%9)+2,j,k)){
				Print();
				if (PlaceN((i%9)+2,j,k)){
					return true;
				} else {
					ClearCell((i%9)+2,j,k);
					Print();
				}
			}
		}
		return false;
	}
	return false;
}

/*
bool Board::PlaceN(int i, int j)
//recursively solves sudoku puzzle
{
	Print();
	//cout <<"\n________________________________";
	//cout << "\nIN CELL:    " << i << ", " << j << "\n\n";
	if(IsBlank(i, j))
	//if the cell is blank
	{
		//cout << "Blank Cell: " << i << ", " << j << endl;
		for(int k = 1; k <= 9; k++)
		//increments through each possible digit
		{
			//cout << "Testing Digit " << k << " in Cell: " << i << ", " << j << endl;
			if(CheckConflicts(i, j, k))
			//checks for conflicts
			{
				//cout << "No conflict for " << k << " in Cell: " << i << ", " << j << endl;
				SetCell(i, j, k);
				if(i == 9)
				//increments i and j
				{
					if(j == 9)
					{
						return true;
					} else
					{
						i = 1;
						if(PlaceN(i, j+1))
						//recurs to the next cell
						{
							return true;
						}
						else {
							//cout << "Clearing " << k << " from Cell: " << i << ", " << j << endl;
							ClearCell(i, j, k);
							i = 9;
						} //end of recursion
					}
				} else {
					if(PlaceN(i+1, j))
					//recurs to the next cell
					{
						return true;
					}
					else {
						//cout << "Clearing " << k << " from Cell: " << i << ", " << j << endl;
						ClearCell(i, j, k);
					} //end of recursion
				}

			} //end of if statment to check for conflicts

		} // end of for loop to increment through each digit

		//if no digit does not cause conflicts, return false
		return false;

	} else {
		if(i == 9)
		//increments to next cell
		{
			if(j == 9)
			{
				return true;
			} else
			{
				i = 0;
				j++;
			}
		}

		return PlaceN(i + 1,j);
	}
}
*/

void Board::AddValue(int i, int j, ValueType val)
//cheks conflicts, adds value to cell, updates conflicts
{
	if(CheckConflicts(i, j, val))
	//if there are conflicts
	{
		SetCell(i, j, val);
	} else {
		cout << "There is a conflict!\n";
	}
}

bool Board::IsSolved()
//checks whether board is solved
{
	for (int i = 1; i <= boardSize; i++)
	//for each row
	{
		for (int j = 1; j <= boardSize; j++)
		//for each column
		{
			if (value[i][j] == blank)
			//checks if cell is blank
			{
				cout << "Board is not solved.\n";
				cout << "Blank space at: " << i << ", " << j << endl;
				return false;
			}

		} //end of column for loop

	} //end of cell for loop

	return true;

} //end of isSolved

int main()
{
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudoku.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		Board b1(squareSize);
		b1.Clear();
		b1.Print();

		while (fin && fin.peek() != 'Z')
		{
			b1.Initialize(fin);
			b1.Print();
			int i = 1;
			//while (b1.PlaceN(1,1, i) | (i < 9)) {
			//	i++;
			//}
			if (b1.smartPlaceN()){
				cout << "\nGood Job";
			};
			cout << "\n I'm done, going home!";
		}

	}

	catch  (indexRangeError &ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
}
