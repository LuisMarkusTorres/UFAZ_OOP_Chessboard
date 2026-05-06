#include "jeu.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    Game mygame;
    string move;
    bool stop(false);

    mygame.display();

    // Input source
    istream* input = &cin;

    // If a filename was provided
    if (argc > 1) {
        ifstream* file = new ifstream(argv[1]);

        if (!file->is_open()) {
            cerr << "Failed to open file: " << argv[1] << endl;
            return 1;
        }

        input = file;
    }

    do {
        // Only show prompt for terminal input
        if (input == &cin)
            cout << "Move (eg. e2e4) ? ";

        if (!getline(*input, move))
            break;

        if (move == "/quit") {
            stop = true;

        } else if (move == "/resign") {

            cout << "Resignation by "
                 << (mygame.getTurn() == WHITE ? "White" : "Black")
                 << ". "
                 << (mygame.getTurn() == WHITE ? "Black" : "White")
                 << " wins." << endl;

            stop = true;

        } else if (move == "/draw") {

            cout << "Draw." << endl;
            stop = true;

        } else {

            mygame.tryMove(move);
            mygame.display();

            if (mygame.isGameOver())
                stop = true;
        }

    } while (!stop);

    return 0;
}