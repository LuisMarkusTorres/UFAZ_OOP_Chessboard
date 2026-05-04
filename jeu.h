#ifndef JEU_H
#define JEU_H

#include <string>
#include <vector>
#include "pieces.h"

class Game {
private:
    int enPassantCol = -1;  // column of the pawn that just double-pushed
    int enPassantRow = -1;  // row of that pawn (-1 = no en passant available)

public:
    Game(); // constructor
    ~Game(); // destructor

    // Show the board and status.
    void display() const;

    // Execute a move (or special command).
    // orig, dest : strings of 2 characters (e.g., "b1", "c3").
    void move(const std::string& orig, const std::string& dest);

    Color getTurn() const { return turn; }

    void setEnPassantPiece(){
        
    }

private:
    // Square indexes (0..7 for columns and rows).
    using Case = std::pair<int,int>; // (colonne, ligne) = (column, row)

    // 8x8 board which contains the Piece class
    // nullptr --> empty square.
    Piece* board[8][8];

    // Color of the player to move.
    Color turn;

    // Indicates whether the opponent king is in check.
    bool check;

    // Convert a square string (e.g., "b1") to column and row indexes.
    Case parseSquare(const std::string& s) const;

    // Verify if the given column and row are valid board coordinates.
    bool isValidSquare(int col, int lig) const;

    // Verify if the given color's king is in check.
    bool isInCheck(Color roiCouleur) const;

    // Simulate a move on a copy and check for king in check.
    bool putsInCheck(int fromCol, int fromRow, int toCol, int toRow) const;

    // Helper to get the symbol of a square for display.
    std::string squareSymbol(int col, int lig) const;

    // Helper to print a separator line for the board display.
    void separatorLine() const;

    // Delete all pieces and set the board to empty.
    void clearBoard();
};

#endif