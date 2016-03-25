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
      Board(int);
      void Clear();
      void Initialize(ifstream &fin);
      void Print();
      bool IsBlank(int, int);
      
      ValueType GetCell(int, int);
      void  SetCell(int i,int j, ValueType val);
   private:

      // The following matrices go from 1 to BoardSize in each
      // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)

      matrix<ValueType> value;
      int** conflictsSquare;
      int** conflictsRow;
      int** conflictsCol;
};

Board::Board(int sqSize)
   : value(boardSize+1,boardSize+1)
// Board constructor
{
}

void Board::Clear()
// Clear the entire board.
{

}

void  Board::SetCell(int i,int j, ValueType val)
// set cell i,j to val and update conflicts
{
  value[i][j] = val;
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
            SetCell(i,j,ch-'0');   // Convert char to int
      	 }
      }
}

int SquareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
   // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
   // coordinates of the square that i,j is in.

   return squareSize * ((i-1)/squareSize) + (j-1)/squareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
   for (int i = 0; i < v.size(); i++)
      ostr << v[i] << " ";
   ostr << endl;
   return ostr;
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
   if (i < 1 || i > boardSize || j < 1 || j > boardSize)
      throw rangeError("bad value in SetCell");
}

void Board::Print()
// Prints the current board.
{
   for (int i = 1; i <= boardSize; i++)
   {
      if ((i-1) % squareSize == 0)
      {
         cout << " -";
	 for (int j = 1; j <= boardSize; j++)
	    cout << "---";
         cout << "-";
	 cout << endl;
      }
      for (int j = 1; j <= boardSize; j++)
      {
        	 if ((j-1) % squareSize == 0)
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
}

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

      while (fin && fin.peek() != 'Z')
      {
        	 b1.Initialize(fin);
        	 b1.Print();
        	 //b1.PrintConflicts();
      }
   }
   catch  (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
}
