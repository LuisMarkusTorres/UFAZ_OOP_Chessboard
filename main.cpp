// Compile and execute:
//$ make
//$ ./chess
// If there is a move file, run with :
//$ cat [file] | ./chess

#include "jeu.h"
#include <iostream>
using namespace std;

int main() {
    Game mygame;
    string move;
    bool stop(false);

    mygame.display();

    do {
        cout << "Move (eg. e2e4) ? ";
        cin >> move;

        if (move == "/quit") {
            stop = true;
        } else if (move == "/resign") {
            // The current player resigns, so the other player wins.
            cout << "Resignation by "
                 << (mygame.getTurn() == WHITE ? "White" : "Black")
                 << ". " << (mygame.getTurn() == WHITE ? "Black" : "White")
                 << " wins." << endl;
            stop = true;
        } else if (move == "/draw") {
            cout << "Draw." << endl;
            stop = true;
        } else {
            // Regular move
            string orig = move.substr(0, 2);
            string dest = move.substr(2, 2);
            mygame.move(orig, dest);
            mygame.display();
        }
    } while (!stop);

    return 0;
}