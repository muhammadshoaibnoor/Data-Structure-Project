# Klondike Solitaire Game

A complete, fully functional Klondike Solitaire game implemented in C++ using Raylib graphics library and Stack data structure.

## Features

- **Pure Stack-Based Implementation**: All game piles (deck, waste, tableau, foundation) use custom Stack data structure
- **Classic Klondike Rules**: 
  - 7 tableau piles with cascading cards
  - 4 foundation piles (one per suit)
  - Draw pile and waste pile
  - Win by completing all foundation piles (Ace to King)
- **Interactive Gameplay**:
  - Click deck to draw cards
  - Click cards to auto-move to foundation
  - Click and drag to move cards between tableau piles
  - Visual feedback with card graphics
- **Game Controls**:
  - `SPACE`: Draw a card from the deck
  - `ESC`: Start a new game
  - `LEFT CLICK`: Select and move cards
  - `CLOSE WINDOW`: Exit game

## Requirements

- C++ compiler (g++, MinGW, MSVC, etc.)
- Raylib library (version 4.0 or higher)

## Installation

### Windows

1. **Install Raylib**:
   - Download Raylib from: https://www.raylib.com/
   - Or use a package manager like vcpkg:
     ```
     vcpkg install raylib
     ```

2. **Compile the game**:
   ```bash
   g++ klondike.cpp -o klondike.exe -lraylib -lopengl32 -lgdi32 -lwinmm
   ```

3. **Run**:
   ```bash
   klondike.exe
   ```

### Linux

1. **Install Raylib**:
   ```bash
   sudo apt-get install libraylib-dev
   ```

2. **Compile the game**:
   ```bash
   g++ klondike.cpp -o klondike -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```

3. **Run**:
   ```bash
   ./klondike
   ```

### macOS

1. **Install Raylib**:
   ```bash
   brew install raylib
   ```

2. **Compile the game**:
   ```bash
   g++ klondike.cpp -o klondike -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
   ```

3. **Run**:
   ```bash
   ./klondike
   ```

## How to Play

### Objective
Move all cards to the four foundation piles, building from Ace to King in each suit (Hearts, Diamonds, Clubs, Spades).

### Rules
1. **Tableau Piles**: 
   - Cards must be placed in descending order (K, Q, J, 10, ..., 2, A)
   - Cards must alternate colors (red on black, black on red)
   - Only Kings can be placed on empty tableau piles
   - You can move sequences of cards together

2. **Foundation Piles**:
   - Must start with an Ace
   - Build up in the same suit (A, 2, 3, ..., Q, K)
   - Click on a card to auto-move it to foundation if possible

3. **Draw Pile**:
   - Click to draw one card at a time
   - When empty, automatically resets from waste pile

### Game Controls
- **Draw Card**: Click on the deck (top-left) or press `SPACE`
- **Move to Foundation**: Click on any valid card (top of pile) to automatically move it to the foundation
- **Move Between Tableau**: Click on a card, then click on destination pile to move
- **New Game**: Press `ESC` to restart with a shuffled deck

## Technical Implementation

### Stack Data Structure
The game uses a custom template-based Stack implementation with:
- Fixed-size array storage
- Push, Pop, Peek operations
- Index-based access operator
- Size and empty state tracking

### Game Components
- **Deck (52 cards)**: Main draw pile
- **Waste Pile**: Revealed cards from deck
- **Tableau (7 piles)**: Main playing area with cascading cards
- **Foundation (4 piles)**: Winning piles, one per suit

### Card Representation
Each card has:
- Rank (1-13): Ace through King
- Suit (0-3): Hearts, Diamonds, Clubs, Spades
- Face state: Face-up or face-down

## Code Structure

```
klondike.cpp
├── Stack<T> Template Class
│   ├── push(), pop(), peek()
│   ├── size(), isEmpty(), clear()
│   └── operator[] for indexing
│
├── Card Structure
│   ├── rank, suit, faceUp
│   └── isRed(), isBlack()
│
└── KlondikeGame Class
    ├── Game state (deck, waste, tableau, foundation)
    ├── Game logic (move validation, rules)
    ├── Rendering (drawCard, draw UI)
    └── Input handling (mouse clicks, keyboard)
```

## Screenshots

The game features:
- Green felt table background
- Card graphics with rank and suit symbols
- Visual distinction between face-up and face-down cards
- Win screen overlay when game is completed

## License

Free to use for educational purposes.

## Credits

- Graphics Library: Raylib (https://www.raylib.com/)
- Game Design: Classic Klondike Solitaire
- Implementation: Stack-based data structure approach
