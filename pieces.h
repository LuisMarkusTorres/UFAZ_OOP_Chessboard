#ifndef PIECES_H
#define PIECES_H

#include <string>

enum Color { WHITE, BLACK };

class Piece {
public:
    Piece(Color c) : color(c) {}
    virtual ~Piece() {}

    Color getColor() const { return color; }

    // col/row : indexes 0..7.
    // board : a 8x8 board of Piece*
    virtual bool isLegalMove(int fromCol, int fromRow,
                                   int toCol, int toRow,
                                   Piece* const board[8][8]) const = 0;

    // The unicode symbol of pieces
    virtual std::string symbol() const = 0;

    // Used for finding checks
    virtual bool isKing() const { return false; }

protected:
    Color color;

    // Verify if the path between the two squares is free (excluding the edges)
    // Used for: Rook, Bishop, Queen
    bool isPathClear(int fromCol, int fromRow, int toCol, int toRow,
                     Piece* const board[8][8]) const {
        int dx = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
        int dy = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;

        int x = fromCol + dx;
        int y = fromRow + dy;
        while (x != toCol || y != toRow) {
            if (board[x][y] != nullptr) return false;
            x += dx;
            y += dy;
        }
        return true;
    }
};

class King : public Piece {
public:
    King(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♔" : "♚"; }
    bool isKing() const override { return true; }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♕" : "♛"; }
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♖" : "♜"; }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♗" : "♝"; }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♘" : "♞"; }
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}
    bool isLegalMove(int fromCol, int fromRow, int toCol, int toRow,
                           Piece* const board[8][8]) const override;
    std::string symbol() const override { return color == WHITE ? "♙" : "♟"; }
};

#endif