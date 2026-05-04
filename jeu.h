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

    // Single entry point for all input: handles "e2e4" moves, "O-O", "O-O-O".
    void tryMove(const std::string& input);

    Color getTurn() const { return turn; }

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

    // Verify if a specific square is attacked by any opponent piece.
    bool isSquareAttacked(int col, int row, Color byColor) const;

    // Simulate a move on a copy and check for king in check.
    // epCol/epRow: current en passant target (-1 if none).
    bool putsInCheck(int fromCol, int fromRow, int toCol, int toRow,
                     int epCol, int epRow) const;

    // Execute a regular piece move (e.g. "e2" -> "e4").
    void move(const std::string& orig, const std::string& dest);

    // Execute castling. kingside=true for O-O, false for O-O-O.
    void castle(bool kingside);

    // Ask the player which piece to promote a pawn to and replace it on the board.
    // col/row: the square the pawn just reached.
    void promote(int col, int row);

    // Helper to get the symbol of a square for display.
    std::string squareSymbol(int col, int lig) const;

    // Helper to print a separator line for the board display.
    void separatorLine() const;

    // Delete all pieces and set the board to empty.
    void clearBoard();
};

#endif