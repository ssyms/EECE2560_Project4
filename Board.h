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

class Board
//class declaration for the Board class
{
  public:
    //default constructor
    Board();

    //initializes the board and updates conflicts
    void initializeBoard();

    //print board and update conflicts
    void printBoard();

    //check whether a value causes conflicts if it is placed in cell
    void checkConflicts(int i, int j, int k);

    //adds a value to cell and updates conflicts
    void addValue(int i, int j, int k);

    //clears cell and update conflicts
    void clearCell(int i, int j);

    //checks to see if board is solved
    bool isSolved();

  private:
    //vector to store words in
    char** boardMatrix;
    int boardSize;
    int** conflictsRows;
    int** conflictsCols;
    int** conflictsSqus;
}; //end of Board

#endif
