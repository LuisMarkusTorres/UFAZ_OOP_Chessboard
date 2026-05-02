#include "pieces.h"
#include <cstdlib>

bool King::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                            Piece* const board[8][8]) const {
    int dCol = abs(toCol - fromCol);
    int dRow = abs(toRow - fromRow);
    if (dCol <= 1 && dRow <= 1 && (dCol + dRow != 0)) {
        // Verify that the destination square is not already occupied by a friendly piece
        Piece* dest = board[toCol][toRow];
        return !dest || dest->getColor() != color;
    }
    return false;
}

bool Queen::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                             Piece* const board[8][8]) const {
    int dCol = abs(toCol - fromCol);
    int dRow = abs(toRow - fromRow);
    if (!(fromCol == toCol || fromRow == toRow || dCol == dRow))
        return false;

    if (!isPathClear(fromCol, fromRow, toCol, toRow, board))
        return false;

    Piece* dest = board[toCol][toRow];
    return !dest || dest->getColor() != color;
}

bool Rook::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                             Piece* const board[8][8]) const {
    if (fromCol != toCol && fromRow != toRow)
        return false;
    if (!isPathClear(fromCol, fromRow, toCol, toRow, board))
        return false;
    Piece* dest = board[toCol][toRow];
    return !dest || dest->getColor() != color;
}

bool Bishop::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                            Piece* const board[8][8]) const {
    if (abs(toCol - fromCol) != abs(toRow - fromRow))
        return false;
    if (!isPathClear(fromCol, fromRow, toCol, toRow, board))
        return false;
    Piece* dest = board[toCol][toRow];
    return !dest || dest->getColor() != color;
}

bool Knight::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                                 Piece* const board[8][8]) const {
    int dCol = abs(toCol - fromCol);
    int dRow = abs(toRow - fromRow);
    if (!((dCol == 2 && dRow == 1) || (dCol == 1 && dRow == 2)))
        return false;
    Piece* dest = board[toCol][toRow];
    return !dest || dest->getColor() != color;
}

bool Pawn::isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                             Piece* const board[8][8]) const {
    int direction = (color == WHITE) ? 1 : -1;
    int startingRank = (color == WHITE) ? 1 : 6; // rank 2 (index 1) or 7 (index 6)
    int dCol = toCol - fromCol;
    int dRow = toRow - fromRow;

    Piece* dest = board[toCol][toRow];

    if (dCol == 0 && dRow == direction) {
        return dest == nullptr;
    }
    // First move: two squares forward
    if (dCol == 0 && dRow == 2 * direction && fromRow == startingRank) {
        // The intermediate square must be empty
        int interLig = fromRow + direction;
        if (board[fromCol][interLig] != nullptr) return false;
        return dest == nullptr;
    }
    // Diagonal capture
    if (abs(dCol) == 1 && dRow == direction) {
        return dest && dest->getColor() != color;
    }
    return false;
}