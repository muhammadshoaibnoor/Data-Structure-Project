#include "raylib.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

template <class T>
class stackk {
    int top, sizee;
    T* ptr = nullptr;
public:
    stackk(int s = 52) : top(-1), sizee(s) { ptr = new T[s]; }
    stackk(const stackk& other) {
        sizee = other.sizee; top = other.top;
        ptr = new T[sizee];
        for (int i = 0; i <= top; i++) ptr[i] = other.ptr[i];
    }
    stackk& operator=(const stackk& other) {
        if (this != &other) {
            delete[] ptr; sizee = other.sizee; top = other.top;
            ptr = new T[sizee];
            for (int i = 0; i <= top; i++) ptr[i] = other.ptr[i];
        }
        return *this;
    }
    bool push(T n) { if (top < sizee - 1) { ptr[++top] = n; return true; } return false; }
    bool pop(T& v) { if (top >= 0) { v = ptr[top--]; return true; } return false; }
    T topp() const { return (top >= 0) ? ptr[top] : T(); }
    bool is_emptyy() const { return top == -1; }
    bool is_full() const { return top == sizee - 1; }
    int get_size() const { return top + 1; }
    T get_at(int idx) const { return (idx >= 0 && idx <= top) ? ptr[idx] : T(); }
    ~stackk() { delete[] ptr; }
};

struct Card {
    int suit = 0;
    int rank = 0;
    bool faceup = false;
    string get_color() const { return (suit == 0 || suit == 1) ? "red" : "black"; }
};

struct GameState {
    stackk<Card> stock;
    stackk<Card> waste;
    stackk<Card> tableau[7];
    stackk<Card> foundation[4];

    GameState() : stock(52), waste(52) {
        for (int i = 0; i < 4; i++) foundation[i] = stackk<Card>(13);
    }

    GameState(const GameState& other) : stock(other.stock), waste(other.waste) {
        for (int i = 0; i < 7; i++) tableau[i] = other.tableau[i];
        for (int i = 0; i < 4; i++) foundation[i] = other.foundation[i];
    }

    GameState& operator=(const GameState& other) {
        if (this != &other) {
            stock = other.stock;
            waste = other.waste;
            for (int i = 0; i < 7; i++) tableau[i] = other.tableau[i];
            for (int i = 0; i < 4; i++) foundation[i] = other.foundation[i];
        }
        return *this;
    }
};

struct DragData {
    bool isDragging = false;
    vector<Card> cards;
    int sourceType = -1;
    int sourceIndex = -1;
    Vector2 offset = { 0, 0 };
};

class KlondikeSolitaire {
private:
    Texture2D deckAtlas;
    GameState game;
    vector<GameState> undoStack;
    DragData drag;

    int difficulty = 1;
    int cardsPerDraw = 1;
    int moveCount = 0;
    int hintsRemaining = 3;
    int movesPerHint = 3;

    atomic<int> elapsedSeconds;
    thread* timerThread = nullptr;
    bool timerRunning = false;

    bool gameWon = false;
    bool gameLost = false;
    bool showingMenu = true;
    bool showingHint = false;
    string hintText = "";
    float hintTimer = 0;

    const float CARD_SCALE = 1.2f;
    const float CARD_W = 100.0f * CARD_SCALE;
    const float CARD_H = 145.0f * CARD_SCALE;
    const float CARD_SPACING = 20.0f;
    const float TABLEAU_OFFSET = 35.0f;

    float stockX = 50, stockY = 50;
    float wasteX = 50 + CARD_W + CARD_SPACING;
    float foundationX = 50 + (CARD_W + CARD_SPACING) * 3;
    float tableauY = 300;

public:
    KlondikeSolitaire() {
        InitWindow(1400, 950, "Klondike Solitaire - Professional Edition");
        SetTargetFPS(60);

        deckAtlas = LoadTexture("Deck Sprite.jpg");
        if (deckAtlas.id == 0) {
            cout << "ERROR: Failed to load Deck Sprite.jpg" << endl;
        }

        elapsedSeconds = 0;
    }

    ~KlondikeSolitaire() {
        StopTimer();
        UnloadTexture(deckAtlas);
        CloseWindow();
    }

    void StartTimer() {
        timerRunning = true;
        elapsedSeconds = 0;
        timerThread = new thread([this]() {
            while (timerRunning) {
                this_thread::sleep_for(chrono::seconds(1));
                if (timerRunning) elapsedSeconds++;
            }
            });
    }

    void StopTimer() {
        timerRunning = false;
        if (timerThread && timerThread->joinable()) {
            timerThread->join();
            delete timerThread;
            timerThread = nullptr;
        }
    }

    void SaveBestScore() {
        string filename = (difficulty == 1) ? "easy.txt" : (difficulty == 2) ? "medium.txt" : "hard.txt";
        int bestScore = 999999;

        ifstream infile(filename);
        if (infile.is_open()) {
            infile >> bestScore;
            infile.close();
        }

        if (moveCount < bestScore) {
            ofstream outfile(filename);
            if (outfile.is_open()) {
                outfile << moveCount;
                outfile.close();
            }
        }
    }

    void InitializeGame() {
        srand((unsigned int)time(0));

        Card deckArr[52];
        int k = 0;
        for (int s = 0; s < 4; s++) {
            for (int r = 1; r <= 13; r++) {
                deckArr[k].suit = s;
                deckArr[k].rank = r;
                deckArr[k].faceup = false;
                k++;
            }
        }

        for (int i = 0; i < 52; i++) {
            swap(deckArr[i], deckArr[rand() % 52]);
        }

        for (int i = 51; i >= 0; i--) {
            game.stock.push(deckArr[i]);
        }

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j <= i; j++) {
                Card c;
                game.stock.pop(c);
                if (j == i) c.faceup = true;
                game.tableau[i].push(c);
            }
        }

        moveCount = 0;
        gameWon = false;
        gameLost = false;
        undoStack.clear();
        StartTimer();
    }

    void DrawCardSprite(Card c, float x, float y, bool highlight = false) const {
        float sw = (float)deckAtlas.width / 14.0f;
        float sh = (float)deckAtlas.height / 4.0f;

        Rectangle source = (!c.faceup) ?
            Rectangle{ 13.0f * sw, 0.0f, sw, sh } :
            Rectangle{ (float)(c.rank - 1) * sw, (float)c.suit * sh, sw, sh };

        Rectangle dest = { x, y, CARD_W, CARD_H };
        DrawTexturePro(deckAtlas, source, dest, { 0, 0 }, 0.0f, WHITE);

        if (highlight) {
            DrawRectangleLinesEx(dest, 3, YELLOW);
        }
    }

    void DrawEmptySlot(float x, float y, Color tint = Fade(WHITE, 0.3f)) const {
        DrawRectangleLinesEx({ x, y, CARD_W, CARD_H }, 3, tint);
    }

    static bool CanMoveToFoundation(const stackk<Card>& foundation, Card c) {
        if (foundation.is_emptyy()) {
            return c.rank == 1;
        }
        Card top = foundation.topp();
        return (top.suit == c.suit && c.rank == top.rank + 1);
    }

    static bool CanMoveToTableau(const stackk<Card>& tableau, Card c) {
        if (tableau.is_emptyy()) {
            return c.rank == 13;
        }
        Card top = tableau.topp();
        return (c.rank == top.rank - 1 && c.get_color() != top.get_color());
    }

    bool CheckWin() const {
        for (int i = 0; i < 4; i++) {
            if (!game.foundation[i].is_full()) return false;
        }
        return true;
    }

    bool HasLegalMoves() const {
        if (!game.waste.is_emptyy()) {
            Card c = game.waste.topp();
            for (int i = 0; i < 7; i++) {
                if (CanMoveToTableau(game.tableau[i], c)) return true;
            }
            for (int i = 0; i < 4; i++) {
                if (CanMoveToFoundation(game.foundation[i], c)) return true;
            }
        }

        for (int i = 0; i < 7; i++) {
            if (!game.tableau[i].is_emptyy()) {
                Card c = game.tableau[i].topp();
                if (!c.faceup) continue;

                for (int j = 0; j < 7; j++) {
                    if (i != j && CanMoveToTableau(game.tableau[j], c)) return true;
                }
                for (int j = 0; j < 4; j++) {
                    if (CanMoveToFoundation(game.foundation[j], c)) return true;
                }
            }
        }
        return false;
    }

    void SaveState() {
        undoStack.push_back(game);
        if (undoStack.size() > 100) {
            undoStack.erase(undoStack.begin());
        }
    }

    void UndoMove() {
        if (!undoStack.empty()) {
            game = undoStack.back();
            undoStack.pop_back();
            if (moveCount > 0) moveCount--;
        }
    }

    void ShowHint() {
        if (hintsRemaining <= 0) {
            hintText = "No hints remaining!";
            showingHint = true;
            hintTimer = 3.0f;
            return;
        }

        vector<string> hints;

        if (!game.waste.is_emptyy()) {
            Card c = game.waste.topp();
            for (int i = 0; i < 7; i++) {
                if (CanMoveToTableau(game.tableau[i], c)) {
                    hints.push_back("Move " + CardToString(c) + " from waste to tableau " + to_string(i + 1));
                    if (hints.size() >= movesPerHint) break;
                }
            }
            if (hints.size() < movesPerHint) {
                for (int i = 0; i < 4; i++) {
                    if (CanMoveToFoundation(game.foundation[i], c)) {
                        hints.push_back("Move " + CardToString(c) + " from waste to foundation");
                        if (hints.size() >= movesPerHint) break;
                    }
                }
            }
        }

        if (hints.size() < movesPerHint) {
            for (int i = 0; i < 7 && hints.size() < movesPerHint; i++) {
                if (!game.tableau[i].is_emptyy()) {
                    Card c = game.tableau[i].topp();
                    if (!c.faceup) continue;

                    for (int j = 0; j < 7; j++) {
                        if (i != j && CanMoveToTableau(game.tableau[j], c)) {
                            hints.push_back("Move " + CardToString(c) + " from tableau " + to_string(i + 1) + " to " + to_string(j + 1));
                            if (hints.size() >= movesPerHint) break;
                        }
                    }
                }
            }
        }

        if (hints.empty()) {
            hintText = "Try drawing from stock";
        }
        else {
            hintText = "";
            for (const auto& h : hints) {
                hintText += h + "\n";
            }
        }

        hintsRemaining--;
        showingHint = true;
        hintTimer = 5.0f;
    }

    static string CardToString(Card c) {
        string ranks[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
        string suits[] = { "D", "H", "S", "C" };
        return ranks[c.rank] + suits[c.suit];
    }

    void DrawMenu() {
        ClearBackground(DARKGREEN);

        DrawText("KLONDIKE SOLITAIRE", 500, 150, 60, GOLD);
        DrawText("Select Difficulty:", 550, 300, 40, WHITE);

        Rectangle easyBtn = { 500, 400, 400, 80 };
        Rectangle medBtn = { 500, 500, 400, 80 };
        Rectangle hardBtn = { 500, 600, 400, 80 };

        Vector2 mouse = GetMousePosition();

        DrawRectangleRec(easyBtn, CheckCollisionPointRec(mouse, easyBtn) ? DARKBLUE : BLUE);
        DrawRectangleRec(medBtn, CheckCollisionPointRec(mouse, medBtn) ? Fade(ORANGE, 0.6f) : ORANGE);
        DrawRectangleRec(hardBtn, CheckCollisionPointRec(mouse, hardBtn) ? Fade(RED, 0.6f) : RED);

        DrawText("EASY (1 card, 3 hints)", 550, 425, 30, WHITE);
        DrawText("MEDIUM (2 cards, 2 hints)", 525, 525, 30, WHITE);
        DrawText("HARD (3 cards, 1 hint)", 550, 625, 30, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, easyBtn)) {
                difficulty = 1; cardsPerDraw = 1;
                hintsRemaining = 3; movesPerHint = 3;
                showingMenu = false;
                InitializeGame();
            }
            else if (CheckCollisionPointRec(mouse, medBtn)) {
                difficulty = 2; cardsPerDraw = 2;
                hintsRemaining = 2; movesPerHint = 2;
                showingMenu = false;
                InitializeGame();
            }
            else if (CheckCollisionPointRec(mouse, hardBtn)) {
                difficulty = 3; cardsPerDraw = 3;
                hintsRemaining = 1; movesPerHint = 1;
                showingMenu = false;
                InitializeGame();
            }
        }
    }

    void HandleInput() {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, { stockX, stockY, CARD_W, CARD_H })) {
                SaveState();
                if (!game.stock.is_emptyy()) {
                    for (int i = 0; i < cardsPerDraw && !game.stock.is_emptyy(); i++) {
                        Card c;
                        game.stock.pop(c);
                        c.faceup = true;
                        game.waste.push(c);
                    }
                }
                else {
                    while (!game.waste.is_emptyy()) {
                        Card c;
                        game.waste.pop(c);
                        c.faceup = false;
                        game.stock.push(c);
                    }
                }
                moveCount++;
                return;
            }

            if (!game.waste.is_emptyy() && CheckCollisionPointRec(mouse, { wasteX, stockY, CARD_W, CARD_H })) {
                drag.isDragging = true;
                drag.sourceType = 1;
                drag.sourceIndex = -1;
                drag.cards.push_back(game.waste.topp());
                drag.offset = { mouse.x - wasteX, mouse.y - stockY };
                Card c;
                game.waste.pop(c);
                return;
            }

            for (int i = 0; i < 7; i++) {
                float tx = 50.0f + (i * (CARD_W + CARD_SPACING));
                for (int j = game.tableau[i].get_size() - 1; j >= 0; j--) {
                    Card card = game.tableau[i].get_at(j);
                    if (!card.faceup) continue;

                    float ty = tableauY + (j * TABLEAU_OFFSET);
                    if (CheckCollisionPointRec(mouse, { tx, ty, CARD_W, CARD_H })) {
                        drag.isDragging = true;
                        drag.sourceType = 0;
                        drag.sourceIndex = i;
                        drag.offset = { mouse.x - tx, mouse.y - ty };

                        int cardsToMove = game.tableau[i].get_size() - j;
                        vector<Card> temp;
                        for (int p = 0; p < cardsToMove; p++) {
                            Card c;
                            game.tableau[i].pop(c);
                            temp.insert(temp.begin(), c);
                        }
                        drag.cards = temp;
                        return;
                    }
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && drag.isDragging) {
            bool placed = false;

            for (int i = 0; i < 4; i++) {
                float fx = foundationX + (i * (CARD_W + CARD_SPACING));
                if (drag.cards.size() == 1 && CheckCollisionPointRec(mouse, { fx, stockY, CARD_W, CARD_H })) {
                    if (CanMoveToFoundation(game.foundation[i], drag.cards[0])) {
                        SaveState();
                        game.foundation[i].push(drag.cards[0]);
                        placed = true;
                        moveCount++;

                        if (drag.sourceType == 0 && !game.tableau[drag.sourceIndex].is_emptyy()) {
                            Card top = game.tableau[drag.sourceIndex].topp();
                            if (!top.faceup) {
                                Card c;
                                game.tableau[drag.sourceIndex].pop(c);
                                c.faceup = true;
                                game.tableau[drag.sourceIndex].push(c);
                            }
                        }
                        break;
                    }
                }
            }

            if (!placed) {
                for (int i = 0; i < 7; i++) {
                    float tx = 50.0f + (i * (CARD_W + CARD_SPACING));
                    if (CheckCollisionPointRec(mouse, { tx, tableauY, CARD_W, 600.0f })) {
                        if (CanMoveToTableau(game.tableau[i], drag.cards[0])) {
                            SaveState();
                            for (Card c : drag.cards) {
                                game.tableau[i].push(c);
                            }
                            placed = true;
                            moveCount++;

                            if (drag.sourceType == 0 && !game.tableau[drag.sourceIndex].is_emptyy()) {
                                Card top = game.tableau[drag.sourceIndex].topp();
                                if (!top.faceup) {
                                    Card c;
                                    game.tableau[drag.sourceIndex].pop(c);
                                    c.faceup = true;
                                    game.tableau[drag.sourceIndex].push(c);
                                }
                            }
                            break;
                        }
                    }
                }
            }

            if (!placed) {
                if (drag.sourceType == 1) {
                    game.waste.push(drag.cards[0]);
                }
                else if (drag.sourceType == 0) {
                    for (Card c : drag.cards) {
                        game.tableau[drag.sourceIndex].push(c);
                    }
                }
            }

            drag.isDragging = false;
            drag.cards.clear();
        }

        if (IsKeyPressed(KEY_U)) {
            UndoMove();
        }

        if (IsKeyPressed(KEY_H)) {
            ShowHint();
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            showingMenu = true;
            StopTimer();
        }
    }

    void Render() {
        ClearBackground(DARKGREEN);

        DrawText(TextFormat("TIME: %02d:%02d", (int)elapsedSeconds / 60, (int)elapsedSeconds % 60), 1150, 20, 25, GOLD);
        DrawText(TextFormat("MOVES: %d", moveCount), 1150, 55, 25, WHITE);
        DrawText(TextFormat("HINTS: %d", hintsRemaining), 1150, 90, 25, WHITE);
        DrawText("Press U: Undo", 1150, 125, 20, LIGHTGRAY);
        DrawText("Press H: Hint", 1150, 150, 20, LIGHTGRAY);
        DrawText("ESC: Menu", 1150, 175, 20, LIGHTGRAY);

        if (!game.stock.is_emptyy()) {
            DrawCardSprite(game.stock.topp(), stockX, stockY);
        }
        else {
            DrawEmptySlot(stockX, stockY, GREEN);
        }

        if (!game.waste.is_emptyy()) {
            DrawCardSprite(game.waste.topp(), wasteX, stockY);
        }
        else {
            DrawEmptySlot(wasteX, stockY);
        }

        for (int i = 0; i < 4; i++) {
            float fx = foundationX + (i * (CARD_W + CARD_SPACING));
            if (!game.foundation[i].is_emptyy()) {
                DrawCardSprite(game.foundation[i].topp(), fx, stockY);
            }
            else {
                DrawEmptySlot(fx, stockY, GOLD);
            }
        }

        for (int i = 0; i < 7; i++) {
            float tx = 50.0f + (i * (CARD_W + CARD_SPACING));
            if (game.tableau[i].is_emptyy()) {
                DrawEmptySlot(tx, tableauY);
            }
            else {
                for (int j = 0; j < game.tableau[i].get_size(); j++) {
                    DrawCardSprite(game.tableau[i].get_at(j), tx, tableauY + (j * TABLEAU_OFFSET));
                }
            }
        }

        if (drag.isDragging) {
            Vector2 mouse = GetMousePosition();
            for (size_t i = 0; i < drag.cards.size(); i++) {
                DrawCardSprite(drag.cards[i], mouse.x - drag.offset.x, mouse.y - drag.offset.y + (i * TABLEAU_OFFSET));
            }
        }

        if (showingHint && hintTimer > 0) {
            DrawRectangle(350, 400, 700, 150, Fade(BLACK, 0.8f));
            DrawRectangleLines(350, 400, 700, 150, GOLD);
            DrawText(hintText.c_str(), 370, 430, 20, WHITE);
            hintTimer -= GetFrameTime();
            if (hintTimer <= 0) showingHint = false;
        }

        if (gameWon) {
            DrawRectangle(0, 0, 1400, 950, Fade(BLACK, 0.7f));
            DrawText("YOU WON!", 550, 300, 80, GOLD);
            DrawText(TextFormat("Moves: %d", moveCount), 600, 400, 40, WHITE);
            DrawText(TextFormat("Time: %02d:%02d", (int)elapsedSeconds / 60, (int)elapsedSeconds % 60), 580, 450, 40, WHITE);
            DrawText("Press SPACE to return to menu", 450, 550, 30, LIGHTGRAY);

            if (IsKeyPressed(KEY_SPACE)) {
                showingMenu = true;
                gameWon = false;
            }
        }

        if (gameLost) {
            DrawRectangle(0, 0, 1400, 950, Fade(BLACK, 0.7f));
            DrawText("GAME OVER", 500, 300, 80, RED);
            DrawText("No more moves!", 550, 400, 40, WHITE);
            DrawText("Press SPACE to return to menu", 450, 500, 30, LIGHTGRAY);

            if (IsKeyPressed(KEY_SPACE)) {
                showingMenu = true;
                gameLost = false;
            }
        }
    }

    void Run() {
        while (!WindowShouldClose()) {
            BeginDrawing();

            if (showingMenu) {
                DrawMenu();
            }
            else {
                HandleInput();

                if (CheckWin() && !gameWon) {
                    gameWon = true;
                    StopTimer();
                    SaveBestScore();
                }

                if (!gameWon && game.stock.is_emptyy() && !HasLegalMoves()) {
                    gameLost = true;
                    StopTimer();
                }

                Render();
            }

            EndDrawing();
        }
    }
};

int main() {
    KlondikeSolitaire game;
    game.Run();
    return 0;
}
