#include "jeu.h"
#include <iostream>
#include <cctype>
#include <stdexcept>

using namespace std;

Game::Game() : turn(WHITE), check(false) {
    for (int lig = 0; lig < 8; ++lig)
        for (int col = 0; col < 8; ++col)
            board[col][lig] = nullptr;

    // White pieces (row 0 = rank 1).
    board[0][0] = new Rook(WHITE);
    board[1][0] = new Knight(WHITE);
    board[2][0] = new Bishop(WHITE);
    board[3][0] = new Queen(WHITE);
    board[4][0] = new King(WHITE);
    board[5][0] = new Bishop(WHITE);
    board[6][0] = new Knight(WHITE);
    board[7][0] = new Rook(WHITE);

    // White pawns (row 1 = rank 2).
    for (int col = 0; col < 8; ++col)
        board[col][1] = new Pawn(WHITE);

    // Black pieces (row 7 = rank 8).
    board[0][7] = new Rook(BLACK);
    board[1][7] = new Knight(BLACK);
    board[2][7] = new Bishop(BLACK);
    board[3][7] = new Queen(BLACK);
    board[4][7] = new King(BLACK);
    board[5][7] = new Bishop(BLACK);
    board[6][7] = new Knight(BLACK);
    board[7][7] = new Rook(BLACK);

    // Black pawns (row 6 = rank 7).
    for (int col = 0; col < 8; ++col)
        board[col][6] = new Pawn(BLACK);
}

Game::~Game() {
    clearBoard();
}

Game::Case Game::parseSquare(const string& s) const {
    if (s.size() != 2) throw invalid_argument("Invalid coordinate.");
    char colChar = tolower(s[0]);
    char rowChar = s[1];
    if (colChar < 'a' || colChar > 'h' || rowChar < '1' || rowChar > '8')
        throw invalid_argument("Coordinate off the board.");
    int col = colChar - 'a';
    int lig = rowChar - '1';
    return {col, lig};
}

bool Game::isValidSquare(int col, int lig) const {
    return col >= 0 && col < 8 && lig >= 0 && lig < 8;
}

bool Game::isInCheck(Color roiCouleur) const {
    // Get the king's position.
    int kingCol = -1, kingRow = -1;
    for (int lig = 0; lig < 8; ++lig) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = board[col][lig];
            if (p && p->getColor() == roiCouleur && p->isKing()) {
                kingCol = col;
                kingRow = lig;
                break;
            }
        }
        if (kingCol != -1) break;
    }
    if (kingCol == -1) return false; // Not supposed to happen in a valid game.

    // Check if any opponent piece can move to the king's square.
    Color adversaire = (roiCouleur == WHITE) ? BLACK : WHITE;
    for (int lig = 0; lig < 8; ++lig) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = board[col][lig];
            if (p && p->getColor() == adversaire) {
                if (p->isLegalMove(col, lig, kingCol, kingRow, board))
                    return true;
            }
        }
    }
    return false;
}

bool Game::putsInCheck(int fromCol, int fromRow, int toCol, int toRow) const {
    // Create a copy of the board to simulate the move.
    Piece* copie[8][8];
    for (int lig = 0; lig < 8; ++lig)
        for (int col = 0; col < 8; ++col)
            copie[col][lig] = board[col][lig]; // Attention : shallow copy.

    // Simulate the move on the copy
    copie[toCol][toRow] = copie[fromCol][fromRow];
    copie[fromCol][fromRow] = nullptr;

    // Check if the player's king is in check after this move.
    int kingCol = -1, kingRow = -1;
    for (int lig = 0; lig < 8; ++lig) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = copie[col][lig];
            if (p && p->getColor() == turn && p->isKing()) {
                kingCol = col;
                kingRow = lig;
                break;
            }
        }
    }
    if (kingCol == -1) return false;

    Color adversaire = (turn == WHITE) ? BLACK : WHITE;
    for (int lig = 0; lig < 8; ++lig) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = copie[col][lig];
            if (p && p->getColor() == adversaire) {
                if (p->isLegalMove(col, lig, kingCol, kingRow, copie))
                    return true;
            }
        }
    }
    return false;
}

string Game::squareSymbol(int col, int lig) const {
    Piece* p = board[col][lig];
    if (p) {
        return string("  ") + p->symbol() + "  ";
    } else {
        return "     "; 
    }
}

void Game::separatorLine() const {
    cout << " +-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
}

void Game::clearBoard() {
    for (int lig = 0; lig < 8; ++lig)
        for (int col = 0; col < 8; ++col)
            delete board[col][lig];
}

void Game::display() const {
    cout << "    a     b     c     d     e     f     g     h    " << endl;

    for (int lig = 7; lig >= 0; --lig) { // ranks 8..1
        separatorLine();
        cout << " |";
        for (int col = 0; col < 8; ++col) {
            cout << squareSymbol(col, lig) << "|";
        }
        cout << endl;
    }
    separatorLine();

    // Get the player's turn and check status.
    cout << "[" << (turn == WHITE ? "White" : "Black") << "]"
         << (check ? " Check!" : "") << endl;
}

void Game::move(const string& orig, const string& dest) {
    try {
        auto [fromCol, fromRow] = parseSquare(orig);
        auto [toCol, toRow] = parseSquare(dest);

        if (!isValidSquare(fromCol, fromRow) || !isValidSquare(toCol, toRow)) {
            throw runtime_error("Coordonnées hors échiquier.");
        }

        Piece* piece = board[fromCol][fromRow];
        if (!piece) {
            throw runtime_error("Empty square.");
        }
        if (piece->getColor() != turn) {
            throw runtime_error("That's not your piece.");
        }

        if (!piece->isLegalMove(fromCol, fromRow, toCol, toRow, board)) {
            throw runtime_error("Illegal move for that piece.");
        }

        // Can't put yourself in check.
        if (putsInCheck(fromCol, fromRow, toCol, toRow)) {
            throw runtime_error("This move would leave your king in check.");
        }
        if (board[toCol][toRow]) {
            delete board[toCol][toRow];
            board[toCol][toRow] = nullptr;
        }

        board[toCol][toRow] = piece;
        board[fromCol][fromRow] = nullptr;

        check = false;
        turn = (turn == WHITE) ? BLACK : WHITE;

        if (isInCheck(turn)) {
            check = true;
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }
}