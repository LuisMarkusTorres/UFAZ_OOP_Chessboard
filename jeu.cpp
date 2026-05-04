#include "jeu.h"
#include <iostream>
#include <cctype>
#include <stdexcept>

using namespace std;

// Implementation of the constructor
Game::Game() : turn(WHITE), check(false) {

    // Board creation
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
    clearBoard(); // Upon calling the destructor the board is auto-cleared
}


Game::Case Game::parseSquare(const string& s) const {
    if (s.size() != 2) throw invalid_argument("Invalid coordinate.");
    char colChar = tolower(s[0]); // tolower as columns are indicated as letters a-h
    char rowChar = s[1];
    
    // binary char checker
    if (colChar < 'a' || colChar > 'h' || rowChar < '1' || rowChar > '8')
        throw invalid_argument("Coordinate off the board.");
    
    // char binary arithmetic turned to int
    int col = colChar - 'a';
    int lig = rowChar - '1';
    return {col, lig};
}

// Check if the given coordinates are in the board boundary
bool Game::isValidSquare(int col, int lig) const {
    return col >= 0 && col < 8 && lig >= 0 && lig < 8;
}


bool Game::isInCheck(Color roiCouleur) const {
    int kingCol = -1, kingRow = -1; // not possible king position

    // Firstly we get the king's position.
    /* We go through the board and try to find 
    our wanted King piece on each square */
    for (int lig = 0; lig < 8; ++lig) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = board[col][lig];
            /* If the square is not empty and the color of the
            piece is correct and the piece is the King piece 
            Then we write down the King's actual position */
            if (p && p->getColor() == roiCouleur && p->isKing()) {
                kingCol = col;
                kingRow = lig;
                break;
            }
        }
        if (kingCol != -1) break;
    }

    if (kingCol == -1) return false; // Not supposed to happen in a valid game.

    // the opposing color of our King piece
    Color adversaire = (roiCouleur == WHITE) ? BLACK : WHITE; 
    // We check if any opponent piece can move to the king's square
    // If it can --> return true  |  Otherwise --> return false
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

// Checks if a given square is attacked by any piece of byColor.
// Reuses each piece's isLegalMove — same logic as isInCheck but for any square.
bool Game::isSquareAttacked(int col, int row, Color byColor) const {
    for (int lig = 0; lig < 8; ++lig) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board[c][lig];
            if (p && p->getColor() == byColor) {
                if (p->isLegalMove(c, lig, col, row, board))
                    return true;
            }
        }
    }
    return false;
}

bool Game::putsInCheck(int fromCol, int fromRow, int toCol, int toRow,
                       int epCol, int epRow) const {
    // Create a copy of the board to simulate the move.
    Piece* copie[8][8];
    for (int lig = 0; lig < 8; ++lig)
        for (int col = 0; col < 8; ++col)
            copie[col][lig] = board[col][lig]; // shallow copy

    // Simulate the move on the copy
    copie[toCol][toRow] = copie[fromCol][fromRow];
    copie[fromCol][fromRow] = nullptr;

    // If this was an en passant capture, also remove the captured pawn
    // from the simulation (it sits beside the capturing pawn, not on toRow).
    Piece* moved = copie[toCol][toRow];
    if (moved && moved->isPawn() && epCol != -1 &&
        toCol == epCol && toRow == epRow + ((moved->getColor() == WHITE) ? 1 : -1)) {
        copie[epCol][epRow] = nullptr; // remove the sideways pawn
    }

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
        cout << " " << (lig+1);
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

        // Snapshot the current EP state before resetting it.
        // EP is only valid for exactly one turn, so we capture it here
        // to validate this move, then it will be overwritten below.
        int currentEpCol = enPassantCol;
        int currentEpRow = enPassantRow;

        // Check move legality: pawns get the EP-aware overload, others the standard one.
        bool legal = false;
        if (piece->isPawn()) {
            Pawn* pawn = static_cast<Pawn*>(piece);
            legal = pawn->isLegalMove(fromCol, fromRow, toCol, toRow, board,
                                      currentEpCol, currentEpRow);
        } else {
            legal = piece->isLegalMove(fromCol, fromRow, toCol, toRow, board);
        }

        if (!legal) {
            throw runtime_error("Illegal move for that piece.");
        }

        // Detect en passant capture *before* moving anything on the board:
        // it's a pawn, moving diagonally, to an empty square, with a valid EP target.
        bool isEnPassantCapture = false;
        if (piece->isPawn() && currentEpCol != -1 &&
            toCol == currentEpCol && board[toCol][toRow] == nullptr &&
            toCol != fromCol) { // diagonal move
            isEnPassantCapture = true;
        }

        // Can't put yourself in check (pass EP state so the simulation is accurate).
        if (putsInCheck(fromCol, fromRow, toCol, toRow, currentEpCol, currentEpRow)) {
            throw runtime_error("This move would leave your king in check.");
        }

        // --- All checks passed: execute the move ---

        // Reset en passant: by default no EP is available next turn.
        enPassantCol = -1;
        enPassantRow = -1;

        // If this is an en passant capture, delete the captured pawn sideways.
        if (isEnPassantCapture) {
            delete board[currentEpCol][currentEpRow];
            board[currentEpCol][currentEpRow] = nullptr;
        }

        // Standard capture at destination square.
        if (board[toCol][toRow]) {
            delete board[toCol][toRow];
            board[toCol][toRow] = nullptr;
        }

        board[toCol][toRow] = piece;
        board[fromCol][fromRow] = nullptr;

        // Mark king/rook as having moved so castling rights are lost.
        if (piece->isKing())
            static_cast<King*>(piece)->hasMoved = true;
        else if (dynamic_cast<Rook*>(piece))
            static_cast<Rook*>(piece)->hasMoved = true;

        // If a pawn just double-pushed, record it as the EP target for next turn.
        int direction = (turn == WHITE) ? 1 : -1;
        if (piece->isPawn() && toRow - fromRow == 2 * direction) {
            enPassantCol = toCol;
            enPassantRow = toRow; // the pawn's new position (used by EP-aware isLegalMove)
        }

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


void Game::castle(bool kingside) {
    try {
        // if king white --> first row  |  Otherwise --> last row
        int row = (turn == WHITE) ? 0 : 7;
        Color opponent = (turn == WHITE) ? BLACK : WHITE; // opponent color

        // --- 1. Locate the king and verify it hasn't moved ---
        int kingCol = 4; // Both Kings always start on the e-file (col 4)
        Piece* kingPiece = board[kingCol][row];
        
        /* If the starting position is empty or
        The piece is not the King piece or
        The color of the king doesn't match
        Then --> cannot castle */
        if (!kingPiece || !kingPiece->isKing() || kingPiece->getColor() != turn)
            throw runtime_error("King not on its starting square.");

        // If the King piece has moved, also cannot castle
        if (static_cast<King*>(kingPiece)->hasMoved)
            throw runtime_error("Cannot castle: king has already moved.");

        // --- 2. Locate the rook and verify it hasn't moved ---
        // Determine the rook column, depending if short or long castling
        int rookCol = kingside ? 7 : 0; 
        Piece* rookPiece = board[rookCol][row];

        /* If the starting pos is empty or
        the piece is not a rook piece or
        the rook color is incorrect 
        Then --> cannot castle*/
        if (!rookPiece || !dynamic_cast<Rook*>(rookPiece) || rookPiece->getColor() != turn)
            throw runtime_error("Rook not on its starting square.");
        
        // If the rook hasn moved --> cannot castle
        if (static_cast<Rook*>(rookPiece)->hasMoved)
            throw runtime_error("Cannot castle: rook has already moved.");

        // --- 3. All squares between king and rook must be empty ---
        // Kingside: cols 5,6 must be empty.  Queenside: cols 1,2,3 must be empty.
        /* If long short castle --> the step (1) is positive, so goes towards columns 5, 6
        Otherwise --> the step is negative, checking the 1,2,3 columns */
        int step = kingside ? 1 : -1; 
        for (int c = kingCol + step; c != rookCol; c += step) {
            if (board[c][row] != nullptr) // if there is a piece inbetween --> not possible to castle
                throw runtime_error("Cannot castle: pieces between king and rook.");
        }

        // --- 4. King must not currently be in check ---
        if (isInCheck(turn))
            throw runtime_error("Cannot castle while in check.");

        // --- 5. King must not pass through or land on an attacked square ---
        // Kingside: king travels e→f→g (cols 4→5→6).
        // Queenside: king travels e→d→c (cols 4→3→2).
        int kingDest = kingside ? 6 : 2;
        for (int c = kingCol + step; ; c += step) { // step has already been chosen given the type of castling
            if (isSquareAttacked(c, row, opponent))
                throw runtime_error("Cannot castle: king passes through or lands on attacked square.");
            if (c == kingDest) break;
        }

        // --- All checks passed: execute castling ---

        // Move king to its destination square.
        board[kingDest][row] = kingPiece;
        board[kingCol][row]  = nullptr;
        static_cast<King*>(kingPiece)->hasMoved = true;

        // Move rook to the square the king just crossed.
        int rookDest = kingside ? 5 : 3;
        board[rookDest][row] = rookPiece;
        board[rookCol][row]  = nullptr;
        static_cast<Rook*>(rookPiece)->hasMoved = true;

        // En passant is always reset after any move.
        enPassantCol = -1;
        enPassantRow = -1;

        check = false;
        turn = (turn == WHITE) ? BLACK : WHITE;
        if (isInCheck(turn))
            check = true;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Responsible for castling and regular moves
void Game::tryMove(const string& input) {
    /* Castling input: "O-O" (kingside) or "O-O-O" (queenside).
    We check the string contents, to determine
    to castle or make a reqular move */
    if (input == "O-O") {
        castle(true);
        return;
    }
    if (input == "O-O-O") {
        castle(false);
        return;
    }

    // Regular move: must be exactly 4 characters (e.g. "e2e4").
    if (input.size() != 4) {
        cerr << "Error: Unknown command. Use e.g. 'e2e4', 'O-O', or 'O-O-O'." << endl;
        return;
    }

    string orig = input.substr(0, 2);
    string dest = input.substr(2, 2);
    move(orig, dest);
}