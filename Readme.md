# Chess Mini-Project

**Project by:** Dmitriy Kuramshin, Kamal Yalchin, Toghrul Mardiyev, Luis Markus Torres, Farid Veliyev

## Implemented Functionalities

The project implements all four required levels of functionality.

**Base level** The board is displayed in the terminal after every move using Unicode chess symbols, with column and row labels. Move input follows coordinate notation (e.g. `e2e4`), and the game enforces movement geometry, obstacle detection, and capture rules for every piece type. Pawns may advance two squares from their starting rank, with the intermediate square verified to be empty. Turn alternation between White and Black is enforced throughout. The commands `/quit`, `/resign`, and `/draw` are recognised at any point during the game.

**Check detection** After every move, the game verifies whether the active player's king is in check. Any move that would leave or place the player's own king in check is rejected before it is applied. This is implemented by simulating the proposed move on a temporary board copy and calling `isInCheck` on the result, ensuring that the live board state is never corrupted by an illegal attempt.

**Castling, en passant, promotion** All three special moves are fully implemented. Kingside (`O-O`) and queenside (`O-O-O`) castling are available provided neither the king nor the relevant rook has previously moved, the squares between them are unoccupied, the king is not currently in check, and the king does not pass through or land on an attacked square. En passant capture is available for exactly one half-move following an opponent's double pawn push. Pawn promotion triggers an interactive prompt when a pawn reaches the back rank, allowing the player to choose between Queen, Rook, Bishop, and Knight.

**Checkmate and stalemate** The game correctly terminates when the current player has no legal move. If their king is in check, the result is checkmate, otherwise it is stalemate. Three additional draw conditions are also detected: the fifty-move rule (no pawn move or capture in the last fifty full moves), threefold repetition, and insufficient mating material.

The program also accepts a move file as a command-line argument, reading moves line by line in the same format as interactive input. A `canonical_position()` method serialises the final board state in a format compatible with the project's automated test framework.

## Challenges Faced

**En passant** was a complicated rule to implement correctly. Because the capture is only legal on the half-move immediately following an opponent double pawn push, the game must remember which pawn pushed and where. This is handled by storing the column and row of the just-pushed pawn in `enPassantCol` and `enPassantRow` within the `Game` object. Those fields are reset to `-1` at the start of every move and only set again when a double push actually occurs. The `Pawn` class provides an overloaded version of `isLegalMove` that accepts these coordinates, so the en passant condition is checked cleanly without polluting the base move logic.

**Castling** required tracking move history at the piece level rather than the board level. Both `King` and `Rook` carry a `hasMoved` boolean flag that is set to `true` the first time each piece moves. This flag is checked before any castling attempt. The legality check also verifies that every square the king passes through during the manoeuvre is not under attack, which required iterating those intermediate squares through `isSquareAttacked`.

**Threefold repetition** required a compact representation of game state that could be compared across turns. The `boardSnapshot` function serialises the full board position, the active colour and the en passant file into a single string. That string is appended to a `positionHistory` vector after every move, and a repetition count of three triggers a draw.

Finally, **Pawn promotion** required integrating an interactive prompt cleanly into the turn flow. Once a pawn reaches the back rank the `promote` function is called immediately within `move`, before `checkGameOver` runs. The chosen piece replaces the pawn on the board in place, so the rest of the pipeline sees the promoted piece correctly when evaluating check and game-over conditions.

## Technical Choices

The piece hierarchy is built around an abstract base class, `Piece`, which declares two pure virtual methods: `isLegalMove` and `symbol`. Every concrete piece type: `King`, `Queen`, `Rook`, `Bishop`, `Knight`, and `Pawn` inherits from `Piece` and provides its own implementation of these methods. This design means that the board and game logic never need to branch on piece type directly, they simply call `isLegalMove` through the base class pointer and let virtual dispatch handle the rest. Two additional virtual methods, `isKing` and `isPawn`, return `false` by default and are overridden only where needed, allowing the game logic to identify these particular pieces for check detection and en passant without breaking encapsulation through casts.

The board is stored as a flat two-dimensional array of `Piece*` with dimensions `[8][8]`, indexed by column and row in the range `0..7`. Empty squares are represented by `nullptr`. This representation is simple and direct: iterating the board, copying it for simulation, and indexing by coordinate are all constant-time operations, and the pointer type allows polymorphic dispatch without any additional indirection layer.

Coordinate handling is centralised through a private type alias, `Case`, defined as `std::pair<int,int>` in `jeu.h`. The `parseSquare` method converts algebraic square names such as `"e4"` into `(column, row)` index pairs. By funnelling all coordinate conversions through this one method, the rest of the codebase works entirely in integer indices, and bugs caused by repeated manual character arithmetic are avoided.

The project is split into two main classes. `Game`, defined in `jeu.h` and `jeu.cpp`, owns the board array, the game state (active colour, en passant target, half-move clock, position history), and all methods that operate on them, including move execution, castling, promotion, check detection, and game-over evaluation. Input parsing and the main loop are handled in `main.cpp`, it also provides the three regex-based validation functions for standard moves, kingside castling and queenside castling. This separation means that the `Game` class is not responsible for reading from standard input or files, and `main.cpp` is not responsible for any chess logic.