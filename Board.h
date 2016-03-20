// Board.h
// Problem Set 4a                                 Chris Canal
// Team cansym                                    canal.c@husky.neu.edu
//                                                Sarada Symonds
//                                                symonds.s@husky.neu.edu
//
// Board header file for homework 4a.
// Contains declarations for Board class.
#ifndef Board_Board_h
#define Board_Board_h
typedef int ValueType;
#include <fstream>

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
};

#endif
