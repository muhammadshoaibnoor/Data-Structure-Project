#include "raylib.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

template <class T>
class stackk {
    int top;
    int sizee;
    T *ptr = nullptr;
public:
    stackk() {
        sizee = 52;
        top = -1;
        ptr = new T[sizee];
    }
    stackk(int s) {
        sizee = s;
        top = -1;
        ptr = new T[sizee];
    }

    stackk(const stackk & other) {
        sizee = other.sizee;
        top = other.top;
        ptr = new T[sizee];
        for (int i = 0; i <= top; i++) {
            ptr[i] = other.ptr[i];
        }
    }

    stackk& operator=(const stackk & other) {
        if (this != &other) {
            if (ptr) delete[] ptr;
            sizee = other.sizee;
            top = other.top;
            ptr = new T[sizee];
            for (int i = 0; i <= top; i++) {
                ptr[i] = other.ptr[i];
            }
        }
        return *this;
    }

    bool push(T n) {
        if (!is_full()) {
            ptr[++top] = n;
            return true;
        }
        return false;
    }

    bool pop(T &v) {
        if (!is_emptyy()) {
            v = ptr[top--];
            return true;
        }
        return false;
    }

    T topp() {
        if (!is_emptyy()) {
            return ptr[top];
        }
        else {
            return T();
        }
    }

    bool is_emptyy() {
        return top == -1;
    }

    bool is_full() {
        return top == sizee - 1;
    }

    int size() {
        return top + 1;
    }

    T getAt(int index) {
        if (index >= 0 && index <= top) {
            return ptr[index];
        }
        return T();
    }
    
    T getAtSafe(int index) {
        if (ptr && index >= 0 && index <= top) {
            return ptr[index];
        }
        return T();
    }

    void clear() {
        top = -1;
    }

    ~stackk() {
        if (ptr) {
            delete[] ptr;
            ptr = nullptr;
        }
    }
};

struct card {
    int suit;
    int rankk;
    bool faceup;
    string color;

    card() : suit(0), rankk(0), faceup(false), color("") {}

    bool isRed() const { return suit == 0 || suit == 1; }

    bool isValid() const { return suit >= 0 && suit <= 3 && rankk >= 1 && rankk <= 13; }
};

enum Difficulty { EASY = 0, MEDIUM = 1, HARD = 2 };

struct GameSnapshot {
    stackk<card> stock, waste;
    stackk<card> tableau1, tableau2, tableau3, tableau4, tableau5, tableau6, tableau7;
    stackk<card> foundation1, foundation2, foundation3, foundation4;
    int moveCount;
    int elapsedTime;
};

struct BestScore {
    int moves;
    int time;
    
    BestScore() : moves(9999), time(9999) {}
};

struct Hint {
    string message;
    card card;
    int sourceType;
    int destType;
    int destIndex;
};

class Deck {
public:
    stackk<card> stock;
    stackk<card> waste;
    stackk<card> tableau1;
    stackk<card> tableau2;
    stackk<card> tableau3;
    stackk<card> tableau4;
    stackk<card> tableau5;
    stackk<card> tableau6;
    stackk<card> tableau7;
    stackk<card> foundation1{13};
    stackk<card> foundation2{13};
    stackk<card> foundation3{13};
    stackk<card> foundation4{13};

    Deck() {}

    void createdeck() {
        stock.clear();
        card c;
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 14; j++) {
                c.suit = i;
                c.rankk = j;
                c.faceup = false;
                if (i == 0 || i == 1) {
                    c.color = "red";
                }
                else {
                    c.color = "black";
                }
                stock.push(c);
            }
        }
    }

    void shuffledeck() {
        srand((unsigned int)time(0));
        stackk<card> temp{52};
        card c;

        while (!stock.is_emptyy()) {
            if (stock.pop(c)) {
                temp.push(c);
            }
        }

        int count = temp.size();
        card* cards = new card[count];
        for (int i = 0; i < count; i++) {
            cards[i] = temp.getAt(i);
        }

        for (int i = count - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            card tempCard = cards[i];
            cards[i] = cards[j];
            cards[j] = tempCard;
        }

        for (int i = 0; i < count; i++) {
            stock.push(cards[i]);
        }
        delete[] cards;
    }

    void dealtotableau(stackk<card>& tableau, int j, int& acesFaceUp, int& acesFaceDown, 
                       int& acesInStock, Difficulty diff) {
        card c;
        for (int i = 1; i <= j; i++) {
            if (stock.pop(c)) {
                if (c.rankk == 1) {
                    if (diff == EASY) {
                        if (acesFaceUp < 2 && i == j) {
                            c.faceup = true;
                            acesFaceUp++;
                        } else if (acesFaceDown < 1 && i < j) {
                            c.faceup = false;
                            acesFaceDown++;
                        } else if (acesInStock < 1) {
                            stock.push(c);
                            acesInStock++;
                            continue;
                        } else {
                            c.faceup = (i == j);
                        }
                    } else if (diff == MEDIUM) {
                        if (acesFaceUp < 1 && i == j) {
                            c.faceup = true;
                            acesFaceUp++;
                        } else if (acesFaceDown < 2 && i < j) {
                            c.faceup = false;
                            acesFaceDown++;
                        } else if (acesInStock < 1) {
                            stock.push(c);
                            acesInStock++;
                            continue;
                        } else {
                            c.faceup = (i == j);
                        }
                    } else if (diff == HARD) {
                        if (acesFaceDown < 3 && i < j) {
                            c.faceup = false;
                            acesFaceDown++;
                        } else if (acesInStock < 1) {
                            stock.push(c);
                            acesInStock++;
                            continue;
                        } else {
                            c.faceup = (i == j);
                        }
                    }
                } else {
                    if (i == j) {
                        c.faceup = true;
                    } else {
                        c.faceup = false;
                    }
                }
                tableau.push(c);
            }
        }
    }

    void dealtableau(Difficulty diff) {
        int acesFaceUp = 0;
        int acesFaceDown = 0;
        int acesInStock = 0;
        
        dealtotableau(tableau1, 1, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau2, 2, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau3, 3, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau4, 4, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau5, 5, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau6, 6, acesFaceUp, acesFaceDown, acesInStock, diff);
        dealtotableau(tableau7, 7, acesFaceUp, acesFaceDown, acesInStock, diff);
    }

    void drawtowastee(int cardsPerDraw = 1) {
        if (stock.is_emptyy()) {
            recyclewastetostock();
            if (stock.is_emptyy()) {
                return;
            }
        }
        
        for (int i = 0; i < cardsPerDraw && !stock.is_emptyy(); i++) {
            card c;
            if (stock.pop(c)) {
                c.faceup = true;
                waste.push(c);
            }
        }
    }

    void recyclewastetostock() {
        if (waste.is_emptyy()) {
            return;
        }
        card c;
        stackk<card> temp;

        while (!waste.is_emptyy()) {
            if (waste.pop(c)) {
                c.faceup = false;
                temp.push(c);
            }
        }
        while (!temp.is_emptyy()) {
            if (temp.pop(c)) {
                stock.push(c);
            }
        }
    }

    bool can_move_to_foundation(stackk<card>& foundation, card c) {
        if (foundation.is_emptyy()) {
            return c.rankk == 1;
        }
        else {
            card top = foundation.topp();
            return (top.suit == c.suit && c.rankk == top.rankk + 1);
        }
    }

    bool can_move_to_tableau(stackk<card>& tableau, card c) {
        if (tableau.is_emptyy()) {
            return c.rankk == 13;
        }
        else {
            card top = tableau.topp();
            return (c.rankk == top.rankk - 1 && c.color != top.color);
        }
    }

    void flip_tableau_top(stackk<card>& tableau) {
        if (!tableau.is_emptyy()) {
            card top = tableau.topp();
            if (!top.faceup) {
                card temp;
                if (tableau.pop(temp)) {
                    temp.faceup = true;
                    tableau.push(temp);
                }
            }
        }
    }

    bool check_win() {
        return (foundation1.is_full() && foundation2.is_full() &&
                foundation3.is_full() && foundation4.is_full());
    }

    bool check_loss() {
        if (!stock.is_emptyy() || !waste.is_emptyy()) {
            return false;
        }
        
        if (!waste.is_emptyy()) {
            card c = waste.topp();
            if (can_move_to_foundation(foundation1, c) || 
                can_move_to_foundation(foundation2, c) ||
                can_move_to_foundation(foundation3, c) || 
                can_move_to_foundation(foundation4, c)) {
                return false;
            }
            // Check waste to tableau
            if (can_move_to_tableau(tableau1, c) || can_move_to_tableau(tableau2, c) ||
                can_move_to_tableau(tableau3, c) || can_move_to_tableau(tableau4, c) ||
                can_move_to_tableau(tableau5, c) || can_move_to_tableau(tableau6, c) ||
                can_move_to_tableau(tableau7, c)) {
                return false;
            }
        }
        
        stackk<card>* tableaus[] = {&tableau1, &tableau2, &tableau3, &tableau4, &tableau5, &tableau6, &tableau7};
        for (int i = 0; i < 7; i++) {
            if (!tableaus[i]->is_emptyy() && tableaus[i]->topp().faceup) {
                card c = tableaus[i]->topp();
                if (can_move_to_foundation(foundation1, c) || 
                    can_move_to_foundation(foundation2, c) ||
                    can_move_to_foundation(foundation3, c) || 
                    can_move_to_foundation(foundation4, c)) {
                    return false;
                }
                for (int j = 0; j < 7; j++) {
                    if (i != j && can_move_to_tableau(*tableaus[j], c)) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }

    stackk<card>* getTableau(int index) {
        switch(index) {
            case 1: return &tableau1;
            case 2: return &tableau2;
            case 3: return &tableau3;
            case 4: return &tableau4;
            case 5: return &tableau5;
            case 6: return &tableau6;
            case 7: return &tableau7;
            default: return nullptr;
        }
    }

    stackk<card>* getFoundation(int index) {
        switch(index) {
            case 1: return &foundation1;
            case 2: return &foundation2;
            case 3: return &foundation3;
            case 4: return &foundation4;
            default: return nullptr;
        }
    }
};

class Game {
private:
    Texture2D cardTextures[4][13];
    Texture2D cardBack;
    Texture2D backgroundTexture;

    Sound cardSlideSound;
    Sound cardFlipSound;
    Sound cardPlaceSound;
    Sound winSound;
    Sound clickSound;
    Sound hintSound;
    Music bgMusic;

    Deck gameDeck;

    bool dragging;
    vector<card> draggedCards;
    int dragSourceType;
    int dragSourceIndex;
    Vector2 dragOffset;
    Vector2 dragStartPos;
    Vector2 dragCurrentPos;

    int moveCount;
    int elapsedTime;
    atomic<bool> timerRunning;
    thread timerThread;
    chrono::steady_clock::time_point timerStart;
    bool gameStarted, gameWon, gameLost;
    Difficulty difficulty;

    int cardsPerDraw;
    int hintsRemaining;
    int maxHints;
    bool hintsEnabled;

    vector<GameSnapshot> undoStack;
    vector<Hint> activeHints;
    bool showingHints;
    BestScore bestScore[3];
    float hintDisplayTime;

    const float CARD_WIDTH = 180.0f;
    const float CARD_HEIGHT = 250.0f;
    const float CARD_SPACING = 35.0f;
    const float TABLEAU_OFFSET = 45.0f;
    const float TOP_BAR_HEIGHT = 120.0f;
    const float MARGIN = 250.0f;

    float stockX, stockY, wasteX, wasteY, foundationStartX, foundationY, tableauStartX, tableauY;
    bool showingMenu;
    bool showingDifficultySelect;
    bool musicMuted;

    float inputCooldown;

public:
    Game() {
        InitWindow(2560, 1440, "Klondike Solitaire - Complete with Stack");
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        InitAudioDevice();
        SetTargetFPS(60);

        dragging = false;
        gameStarted = false;
        showingDifficultySelect = true;
        showingMenu = false;
        timerRunning = false;
        showingHints = false;
        hintsEnabled = true;
        musicMuted = false;

        moveCount = 0;
        elapsedTime = 0;
        inputCooldown = 0;
        hintDisplayTime = 0;

        cardBack = LoadTexture("back_dark.png");
        if (cardBack.id == 0) {
            Image fallback = GenImageColor(242, 340, DARKBLUE);
            cardBack = LoadTextureFromImage(fallback);
            UnloadImage(fallback);
        }

        const char* suitNames[] = {"hearts", "diamonds", "clubs", "spades"};
        const char* rankNames[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

        for (int suit = 0; suit < 4; suit++) {
            for (int rank = 0; rank < 13; rank++) {
                string filename = string(suitNames[suit]) + "_" + string(rankNames[rank]) + ".png";
                cardTextures[suit][rank] = LoadTexture(filename.c_str());

                if (cardTextures[suit][rank].id == 0) {
                    Image img = GenImageColor(242, 340, WHITE);
                    cardTextures[suit][rank] = LoadTextureFromImage(img);
                    UnloadImage(img);
                }
            }
        }

        backgroundTexture = LoadTexture("background.jpg");
        if (backgroundTexture.id == 0) {
            Image bg = GenImageColor(1, 1, Color{0, 100, 50, 255});
            backgroundTexture = LoadTextureFromImage(bg);
            UnloadImage(bg);
        }

        if (IsAudioDeviceReady()) {
            if (FileExists("card_slide.wav")) {
                cardSlideSound = LoadSound("card_slide.wav");
            } else {
                cardSlideSound = CreateFallbackSound(440.0f, 0.1f);
            }

            if (FileExists("card_flip.wav")) {
                cardFlipSound = LoadSound("card_flip.wav");
            } else {
                cardFlipSound = CreateFallbackSound(330.0f, 0.1f);
            }

            if (FileExists("card_place.wav")) {
                cardPlaceSound = LoadSound("card_place.wav");
            } else {
                cardPlaceSound = CreateFallbackSound(550.0f, 0.1f);
            }

            if (FileExists("win.wav")) {
                winSound = LoadSound("win.wav");
            } else {
                winSound = CreateFallbackSound(659.0f, 0.5f);
            }

            if (FileExists("click.wav")) {
                clickSound = LoadSound("click.wav");
            } else {
                clickSound = CreateFallbackSound(220.0f, 0.05f);
            }

            if (FileExists("hint.wav")) {
                hintSound = LoadSound("hint.wav");
            } else {
                hintSound = CreateFallbackSound(523.0f, 0.2f);
            }

            if (FileExists("music.mp3")) {
                bgMusic = LoadMusicStream("music.mp3");
                SetMusicVolume(bgMusic, 0.3f);
            }
            else if (FileExists("music.ogg")) {
                bgMusic = LoadMusicStream("music.ogg");
                SetMusicVolume(bgMusic, 0.3f);
            }
            else if (FileExists("music.wav")) {
                bgMusic = LoadMusicStream("music.wav");
                SetMusicVolume(bgMusic, 0.3f);
            }
        }

        stockX = MARGIN;
        stockY = TOP_BAR_HEIGHT + 40;
        wasteX = stockX + CARD_WIDTH + CARD_SPACING;
        wasteY = stockY;
        foundationStartX = GetScreenWidth() - MARGIN - 4 * (CARD_WIDTH + CARD_SPACING);
        foundationY = stockY;
        tableauStartX = MARGIN;
        tableauY = stockY + CARD_HEIGHT + 120;

        LoadBestScores();
    }

    ~Game() {
        StopTimer();

        UnloadTexture(cardBack);
        UnloadTexture(backgroundTexture);
        for (int s = 0; s < 4; s++) {
            for (int r = 0; r < 13; r++) {
                UnloadTexture(cardTextures[s][r]);
            }
        }

        if (IsAudioDeviceReady()) {
            UnloadSound(cardSlideSound);
            UnloadSound(cardFlipSound);
            UnloadSound(cardPlaceSound);
            UnloadSound(winSound);
            UnloadSound(clickSound);
            UnloadSound(hintSound);
            if (bgMusic.ctxData != nullptr) {
                UnloadMusicStream(bgMusic);
            }
            CloseAudioDevice();
        }

        CloseWindow();
    }

    Sound CreateFallbackSound(float frequency, float duration) {
        unsigned int sampleRate = 44100;
        unsigned int sampleCount = (unsigned int)(sampleRate * duration);
        short *samples = (short*)RL_MALLOC(sampleCount * sizeof(short));

        for (unsigned int i = 0; i < sampleCount; i++) {
            samples[i] = (short)(32000.0f * sinf(2.0f * PI * frequency * ((float)i / sampleRate)));
        }

        Wave wave = LoadWaveFromMemory(".wav", (unsigned char*)samples, sampleCount * sizeof(short));

        if (wave.data == nullptr) {
            wave.frameCount = sampleCount;
            wave.sampleRate = sampleRate;
            wave.sampleSize = 16;
            wave.channels = 1;
            wave.data = (void*)samples;
        }

        Sound sound = LoadSoundFromWave(wave);
        RL_FREE(samples);
        return sound;
    }

    void LoadBestScores() {
        const char* files[] = {"easy.txt", "medium.txt", "hard.txt"};
        for (int i = 0; i < 3; i++) {
            ifstream file(files[i]);
            if (file.is_open()) {
                file >> bestScore[i].moves >> bestScore[i].time;
                file.close();
            }
            else {
                bestScore[i].moves = 9999;
                bestScore[i].time = 9999;
            }
        }
    }

    void SaveBestScore() {
        bool isNewBest = false;
        
        // Better performance = fewer moves AND less time
        if (moveCount < bestScore[difficulty].moves || 
            (moveCount == bestScore[difficulty].moves && elapsedTime < bestScore[difficulty].time)) {
            bestScore[difficulty].moves = moveCount;
            bestScore[difficulty].time = elapsedTime;
            isNewBest = true;
            
            const char* files[] = {"easy.txt", "medium.txt", "hard.txt"};
            ofstream file(files[difficulty]);
            if (file.is_open()) {
                file << moveCount << " " << elapsedTime;
                file.close();
            }
        }
    }

    void SaveGame() {
        ofstream file("savegame.dat", ios::binary);
        if (file.is_open()) {
            file.write((char*)&difficulty, sizeof(difficulty));
            file.write((char*)&moveCount, sizeof(moveCount));
            file.write((char*)&elapsedTime, sizeof(elapsedTime));
            file.close();
        }
    }

    bool LoadGame() {
        ifstream file("savegame.dat", ios::binary);
        if (file.is_open()) {
            file.read((char*)&difficulty, sizeof(difficulty));
            file.read((char*)&moveCount, sizeof(moveCount));
            file.read((char*)&elapsedTime, sizeof(elapsedTime));
            file.close();

            InitializeGame(difficulty, false);
            return true;
        }
        return false;
    }

    void StartTimer() {
        timerRunning = true;
        timerStart = chrono::steady_clock::now();
        timerThread = thread([this]() {
            while (timerRunning) {
                this_thread::sleep_for(chrono::milliseconds(100));

                if (timerRunning && gameStarted && !gameWon && !gameLost &&
                    !showingMenu && !showingDifficultySelect) {

                    auto now = chrono::steady_clock::now();
                    auto elapsed = chrono::duration_cast<chrono::seconds>(now - timerStart);
                    elapsedTime = (int)elapsed.count();
                }
            }
        });
    }

    void StopTimer() {
        timerRunning = false;
        if (timerThread.joinable()) {
            timerThread.join();
        }
    }

    void InitializeGame(Difficulty diff, bool reset = true) {
        difficulty = diff;

        cardsPerDraw = (diff == EASY) ? 1 : (diff == MEDIUM) ? 2 : 3;
        maxHints = (diff == EASY) ? 3 : (diff == MEDIUM) ? 2 : 1;
        hintsRemaining = maxHints;

        if (reset) {
            gameDeck = Deck();
            gameDeck.createdeck();
            gameDeck.shuffledeck();
            gameDeck.dealtableau(diff);
            
            moveCount = 0;
            elapsedTime = 0;
        }

        gameStarted = true;
        gameWon = false;
        gameLost = false;
        dragging = false;

        undoStack.clear();
        activeHints.clear();
        draggedCards.clear();

        StopTimer();
        StartTimer();

        if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
            PlayMusicStream(bgMusic);
        }
    }

    void SaveState() {
        if (undoStack.size() >= 20) {
            undoStack.erase(undoStack.begin());
        }

        GameSnapshot snap;
        snap.stock = gameDeck.stock;
        snap.waste = gameDeck.waste;
        snap.tableau1 = gameDeck.tableau1;
        snap.tableau2 = gameDeck.tableau2;
        snap.tableau3 = gameDeck.tableau3;
        snap.tableau4 = gameDeck.tableau4;
        snap.tableau5 = gameDeck.tableau5;
        snap.tableau6 = gameDeck.tableau6;
        snap.tableau7 = gameDeck.tableau7;
        snap.foundation1 = gameDeck.foundation1;
        snap.foundation2 = gameDeck.foundation2;
        snap.foundation3 = gameDeck.foundation3;
        snap.foundation4 = gameDeck.foundation4;
        snap.moveCount = moveCount;
        snap.elapsedTime = elapsedTime;

        undoStack.push_back(snap);

        if (IsAudioDeviceReady()) {
            PlaySound(clickSound);
        }
    }

    void Undo() {
        if (!undoStack.empty()) {
            GameSnapshot snap = undoStack.back();
            undoStack.pop_back();

            gameDeck.stock = snap.stock;
            gameDeck.waste = snap.waste;
            gameDeck.tableau1 = snap.tableau1;
            gameDeck.tableau2 = snap.tableau2;
            gameDeck.tableau3 = snap.tableau3;
            gameDeck.tableau4 = snap.tableau4;
            gameDeck.tableau5 = snap.tableau5;
            gameDeck.tableau6 = snap.tableau6;
            gameDeck.tableau7 = snap.tableau7;
            gameDeck.foundation1 = snap.foundation1;
            gameDeck.foundation2 = snap.foundation2;
            gameDeck.foundation3 = snap.foundation3;
            gameDeck.foundation4 = snap.foundation4;
            moveCount = snap.moveCount;
            elapsedTime = snap.elapsedTime;

            dragging = false;
            draggedCards.clear();

            if (IsAudioDeviceReady()) {
                PlaySound(cardFlipSound);
            }
        }
    }

    void GenerateHints() {
        activeHints.clear();
        int hintsToShow = (difficulty == EASY) ? 3 : (difficulty == MEDIUM) ? 2 : 1;
        int hintCount = 0;

        // Check waste to foundation
        if (!gameDeck.waste.is_emptyy() && hintCount < hintsToShow) {
            card c = gameDeck.waste.topp();

            for (int i = 0; i < 4; i++) {
                stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                if (gameDeck.can_move_to_foundation(*foundation, c)) {
                    Hint hint;
                    hint.message = "Waste to Foundation";
                    hint.card = c;
                    hint.sourceType = 0;
                    hint.destType = 8 + i;
                    hint.destIndex = i;
                    activeHints.push_back(hint);
                    hintCount++;
                    if (hintCount >= hintsToShow) return;
                }
            }
        }

        // Check tableau to foundation
        for (int i = 0; i < 7 && hintCount < hintsToShow; i++) {
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            if (!tableau->is_emptyy()) {
                card c = tableau->topp();
                if (c.faceup) {
                    for (int j = 0; j < 4; j++) {
                        stackk<card>* foundation = gameDeck.getFoundation(j + 1);
                        if (gameDeck.can_move_to_foundation(*foundation, c)) {
                            Hint hint;
                            hint.message = "Tableau " + to_string(i + 1) + " to Foundation";
                            hint.card = c;
                            hint.sourceType = i + 1;
                            hint.destType = 8 + j;
                            hint.destIndex = j;
                            activeHints.push_back(hint);
                            hintCount++;
                            if (hintCount >= hintsToShow) return;
                        }
                    }
                }
            }
        }

        // Check tableau to tableau
        for (int i = 0; i < 7 && hintCount < hintsToShow; i++) {
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            if (!tableau->is_emptyy()) {
                card c = tableau->topp();
                if (c.faceup) {
                    for (int j = 0; j < 7; j++) {
                        if (i != j) {
                            stackk<card>* destTableau = gameDeck.getTableau(j + 1);
                            if (gameDeck.can_move_to_tableau(*destTableau, c)) {
                                Hint hint;
                                hint.message = "Tableau " + to_string(i + 1) + " to Tableau " + to_string(j + 1);
                                hint.card = c;
                                hint.sourceType = i + 1;
                                hint.destType = j + 1;
                                hint.destIndex = j;
                                activeHints.push_back(hint);
                                hintCount++;
                                if (hintCount >= hintsToShow) return;
                            }
                        }
                    }
                }
            }
        }

        // Check waste to tableau
        if (!gameDeck.waste.is_emptyy() && hintCount < hintsToShow) {
            card c = gameDeck.waste.topp();
            for (int i = 0; i < 7; i++) {
                stackk<card>* tableau = gameDeck.getTableau(i + 1);
                if (gameDeck.can_move_to_tableau(*tableau, c)) {
                    Hint hint;
                    hint.message = "Waste to Tableau " + to_string(i + 1);
                    hint.card = c;
                    hint.sourceType = 0;
                    hint.destType = i + 1;
                    hint.destIndex = i;
                    activeHints.push_back(hint);
                    hintCount++;
                    if (hintCount >= hintsToShow) return;
                }
            }
        }

        if (activeHints.empty()) {
            Hint hint;
            hint.message = "Draw from stock or try different moves";
            activeHints.push_back(hint);
        }
    }

    void ShowHint() {
        if (hintsRemaining > 0 && hintsEnabled) {
            GenerateHints();
            hintsRemaining--;
            hintDisplayTime = 3.0f;
            showingHints = true;

            if (IsAudioDeviceReady()) {
                PlaySound(hintSound);
            }
        }
    }

    void StartDrag(card c, int sourceType, int sourceIndex, Vector2 position) {
        if (inputCooldown > 0) return;

        SaveState();
        dragging = true;
        dragSourceType = sourceType;
        dragSourceIndex = sourceIndex;
        dragStartPos = position;
        dragCurrentPos = position;
        dragOffset = {GetMousePosition().x - position.x, GetMousePosition().y - position.y};

        draggedCards.clear();

        if (sourceType == 0) {
            card temp;
            if (gameDeck.waste.pop(temp)) {
                draggedCards.push_back(temp);
            }
        }
        else if (sourceType >= 1 && sourceType <= 7) {
            stackk<card>* tableau = gameDeck.getTableau(sourceType);
            int tableauSize = tableau->size();
            
            // Collect valid sequence from sourceIndex
            for (int i = sourceIndex; i < tableauSize; i++) {
                card currentCard = tableau->getAtSafe(i);
                if (!currentCard.faceup) break;
                
                // Logic Check: Ensure dragging strictly valid sequences (Logic from provided file)
                if (i > sourceIndex) {
                    card prevCard = draggedCards.back();
                    if (!(currentCard.rankk == prevCard.rankk - 1 && 
                          currentCard.color != prevCard.color)) {
                        break;
                    }
                }
                
                draggedCards.push_back(currentCard);
            }
            
            // Remove dragged cards from tableau
            for (size_t i = 0; i < draggedCards.size(); i++) {
                card temp;
                tableau->pop(temp);
            }
        }
        else if (sourceType >= 8) {
            stackk<card>* foundation = gameDeck.getFoundation(sourceType - 7);
            card temp;
            if (foundation->pop(temp)) {
                draggedCards.push_back(temp);
            }
        }

        if (IsAudioDeviceReady()) {
            PlaySound(cardSlideSound);
        }

        inputCooldown = 0.1f;
    }

    void UpdateDrag() {
        if (!dragging || draggedCards.empty()) return;

        Vector2 mousePos = GetMousePosition();
        dragCurrentPos = {mousePos.x - dragOffset.x, mousePos.y - dragOffset.y};
    }

    void EndDrag() {
        if (!dragging || draggedCards.empty()) return;

        bool placed = false;
        Vector2 mousePos = GetMousePosition();
        card topCard = draggedCards[0];

        // Try foundation (single card only)
        if (draggedCards.size() == 1) {
            for (int i = 0; i < 4; i++) {
                Vector2 slotPos = {foundationStartX + i * (CARD_WIDTH + CARD_SPACING), foundationY};
                Rectangle slotRect = {slotPos.x, slotPos.y, CARD_WIDTH, CARD_HEIGHT};
                
                if (CheckCollisionPointRec(mousePos, slotRect)) {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (gameDeck.can_move_to_foundation(*foundation, topCard)) {
                        foundation->push(topCard);
                        placed = true;

                        if (dragSourceType >= 1 && dragSourceType <= 7) {
                            stackk<card>* sourceTableau = gameDeck.getTableau(dragSourceType);
                            gameDeck.flip_tableau_top(*sourceTableau);
                        }
                        break;
                    }
                }
            }
        }

        // Try tableau
        if (!placed) {
            for (int i = 0; i < 7; i++) {
                stackk<card>* tableau = gameDeck.getTableau(i + 1);
                float y = tableauY;
                if (!tableau->is_emptyy()) {
                    y = tableauY + (tableau->size()) * TABLEAU_OFFSET;
                }

                Vector2 slotPos = {tableauStartX + i * (CARD_WIDTH + CARD_SPACING), y};
                Rectangle slotRect = {slotPos.x - 30, slotPos.y - 30, 
                                     CARD_WIDTH + 60, CARD_HEIGHT + 60};

                if (CheckCollisionPointRec(mousePos, slotRect)) {
                    if (gameDeck.can_move_to_tableau(*tableau, topCard)) {
                        for (const auto& cd : draggedCards) {
                            tableau->push(cd);
                        }
                        placed = true;

                        if (dragSourceType >= 1 && dragSourceType <= 7) {
                            stackk<card>* sourceTableau = gameDeck.getTableau(dragSourceType);
                            gameDeck.flip_tableau_top(*sourceTableau);
                        }
                        break;
                    }
                }
            }
        }

        if (placed) {
            moveCount++;

            if (IsAudioDeviceReady()) {
                PlaySound(cardPlaceSound);
            }

            CheckAutoComplete();
        }
        else {
            ReturnCards();
        }

        dragging = false;
    }

    void CheckAutoComplete() {
        if (gameDeck.check_win()) {
            gameWon = true;
            SaveBestScore();
            if (IsAudioDeviceReady()) {
                PlaySound(winSound);
            }
        }
        
        // Check loss condition
        if (!gameWon && gameDeck.check_loss()) {
            gameLost = true;
        }
    }

    void ReturnCards() {
        if (!draggedCards.empty()) {
            if (dragSourceType == 0) {
                for (int i = draggedCards.size() - 1; i >= 0; i--) {
                    gameDeck.waste.push(draggedCards[i]);
                }
            }
            else if (dragSourceType >= 1 && dragSourceType <= 7) {
                stackk<card>* tableau = gameDeck.getTableau(dragSourceType);
                for (int i = draggedCards.size() - 1; i >= 0; i--) {
                    tableau->push(draggedCards[i]);
                }
            }
            else if (dragSourceType >= 8) {
                stackk<card>* foundation = gameDeck.getFoundation(dragSourceType - 7);
                foundation->push(draggedCards[0]);
            }

            if (IsAudioDeviceReady()) {
                PlaySound(cardFlipSound);
            }
            
            // Remove saved state since move was cancelled
            if (!undoStack.empty()) {
                undoStack.pop_back();
            }
        }
        draggedCards.clear();
    }

    void Update() {
        float deltaTime = GetFrameTime();

        if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
            UpdateMusicStream(bgMusic);
        }

        if (inputCooldown > 0) {
            inputCooldown -= deltaTime;
        }

        if (showingHints) {
            hintDisplayTime -= deltaTime;
            if (hintDisplayTime <= 0) {
                showingHints = false;
            }
        }

        if (showingDifficultySelect || showingMenu || gameWon || gameLost) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (showingDifficultySelect) showingDifficultySelect = false;
                else if (showingMenu) showingMenu = false;
            }
            return;
        }

        // Check win/loss
        if (gameDeck.check_win() && !gameWon) {
            gameWon = true;
            SaveBestScore();
            if (IsAudioDeviceReady()) {
                PlaySound(winSound);
            }
            return;
        }
        
        if (!gameWon && gameDeck.check_loss()) {
            gameLost = true;
            return;
        }

        // Handle stock click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            Rectangle stockRect = {stockX, stockY, CARD_WIDTH, CARD_HEIGHT};
            
            if (CheckCollisionPointRec(mousePos, stockRect)) {
                if (inputCooldown <= 0) {
                    SaveState();
                    
                    if (gameDeck.stock.is_emptyy()) {
                        if (!gameDeck.waste.is_emptyy()) {
                            gameDeck.recyclewastetostock();
                        }
                    } else {
                        gameDeck.drawtowastee(cardsPerDraw);
                        if (!gameDeck.waste.is_emptyy()) {
                            moveCount++;
                        }
                    }
                    
                    if (IsAudioDeviceReady()) {
                        PlaySound(cardFlipSound);
                    }
                    inputCooldown = 0.2f;
                }
                return;
            }
        }

        // Handle drag
        if (dragging) {
            UpdateDrag();

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                EndDrag();
            }
        }
        else {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && inputCooldown <= 0) {
                Vector2 mousePos = GetMousePosition();

                // Check waste
                if (!gameDeck.waste.is_emptyy()) {
                    Rectangle wasteRect = {wasteX, wasteY, CARD_WIDTH, CARD_HEIGHT};
                    if (CheckCollisionPointRec(mousePos, wasteRect)) {
                        card c = gameDeck.waste.topp();
                        StartDrag(c, 0, 0, {wasteX, wasteY});
                        return;
                    }
                }

                // Check foundations
                for (int i = 0; i < 4; i++) {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (!foundation->is_emptyy()) {
                        float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
                        Rectangle foundationRect = {x, foundationY, CARD_WIDTH, CARD_HEIGHT};

                        if (CheckCollisionPointRec(mousePos, foundationRect)) {
                            card c = foundation->topp();
                            StartDrag(c, 8 + i, i, {x, foundationY});
                            return;
                        }
                    }
                }

                // Check tableaus
                for (int i = 0; i < 7; i++) {
                    stackk<card>* tableau = gameDeck.getTableau(i + 1);
                    if (!tableau->is_emptyy()) {
                        float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);

                        for (int j = tableau->size() - 1; j >= 0; j--) {
                            card c = tableau->getAtSafe(j);
                            if (c.faceup) {
                                float y = tableauY + j * TABLEAU_OFFSET;
                                float height = (j == tableau->size() - 1) ? CARD_HEIGHT : TABLEAU_OFFSET + 20;
                                
                                Rectangle cardRect = {x, y, CARD_WIDTH, height};

                                if (CheckCollisionPointRec(mousePos, cardRect)) {
                                    StartDrag(c, i + 1, j, {x, y});
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Keyboard shortcuts
        if (IsKeyPressed(KEY_U)) Undo();
        if (IsKeyPressed(KEY_H)) ShowHint();
        if (IsKeyPressed(KEY_N)) showingMenu = true;
        if (IsKeyPressed(KEY_S)) SaveGame();
        if (IsKeyPressed(KEY_L)) LoadGame();
        if (IsKeyPressed(KEY_M)) {
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
                if (IsMusicStreamPlaying(bgMusic)) {
                    PauseMusicStream(bgMusic);
                } else {
                    ResumeMusicStream(bgMusic);
                }
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) showingMenu = true;
    }

    void Render() {
        BeginDrawing();
        if (backgroundTexture.id != 0) {
            Rectangle src = {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height};
            Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
            DrawTexturePro(backgroundTexture, src, dest, {0, 0}, 0.0f, WHITE);
        } else {
            ClearBackground({0, 100, 50, 255});
        }

        if (showingDifficultySelect) {
            RenderDifficultySelect();
            EndDrawing();
            return;
        }

        if (showingMenu) {
            RenderMenu();
            EndDrawing();
            return;
        }

        DrawTopBar();
        DrawGameBoard();
        DrawHints();

        if (gameWon || gameLost) {
            RenderGameOver();
        }

        EndDrawing();
    }

    bool DrawButton(float x, float y, float w, float h, const char* text) {
        Rectangle rect = {x, y, w, h};
        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);

        float scale = hovered ? 1.05f : 1.0f;
        float scaledW = w * scale;
        float scaledH = h * scale;
        float offsetX = (w - scaledW) / 2;
        float offsetY = (h - scaledH) / 2;
        
        Rectangle scaledRect = {x + offsetX, y + offsetY, scaledW, scaledH};
        
        Color bgColor = hovered ? Color{60, 60, 60, 255} : Color{50, 50, 50, 255};
        DrawRectangleRounded(scaledRect, 0.2f, 10, bgColor);
        
        if (hovered) {
            for (int i = 0; i < 3; i++) {
                Rectangle borderRect = {scaledRect.x - i, scaledRect.y - i, 
                                        scaledRect.width + i*2, scaledRect.height + i*2};
                float borderAlpha = 0.5f - (i * 0.15f);
                DrawRectangleRoundedLines(borderRect, 0.2f, 10, ColorAlpha(GOLD, borderAlpha));
            }
        } else {
            DrawRectangleRoundedLines(scaledRect, 0.2f, 10, ColorAlpha(WHITE, 0.3f));
        }

        int fontSize = 28;
        if (strlen(text) > 20) fontSize = 24;
        if (strlen(text) > 30) fontSize = 20;

        int textWidth = MeasureText(text, fontSize);
        DrawText(text, (int)(x + (w - textWidth)/2), (int)(y + (h - fontSize)/2), fontSize, WHITE);

        return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void RenderGameOver() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

        if (gameWon) {
            float pulse = sinf(GetTime() * 3) * 0.2f + 0.8f;
            Color victoryColor = ColorAlpha(GOLD, pulse);
            
            DrawText("VICTORY!", GetScreenWidth()/2 - 200, 250, 100, victoryColor);
            DrawText(TextFormat("Moves: %d", moveCount), GetScreenWidth()/2 - 180, 380, 50, WHITE);
            DrawText(TextFormat("Time: %02d:%02d", elapsedTime/60, elapsedTime%60),
                     GetScreenWidth()/2 - 180, 450, 50, WHITE);

            if (moveCount <= bestScore[difficulty].moves && elapsedTime <= bestScore[difficulty].time) {
                DrawText("NEW HIGH SCORE!", GetScreenWidth()/2 - 220, 520, 45, GOLD);
            } else {
                DrawText(TextFormat("Best: %d moves in %d:%02d", 
                        bestScore[difficulty].moves,
                        bestScore[difficulty].time / 60,
                        bestScore[difficulty].time % 60), 
                        GetScreenWidth()/2 - 220, 520, 40, SKYBLUE);
            }
        } else {
            DrawText("GAME OVER", GetScreenWidth()/2 - 250, 350, 100, RED);
            DrawText("No moves available", GetScreenWidth()/2 - 200, 480, 40, WHITE);
        }

        if (DrawButton(GetScreenWidth()/2 - 150, 650, 300, 80, "New Game")) {
            showingDifficultySelect = true;
            gameWon = false;
            gameLost = false;
        }
    }

    void RenderMenu() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        
        float centerX = GetScreenWidth()/2.0f;
        float centerY = GetScreenHeight()/2.0f;
        
        DrawRectangle(centerX - 250, centerY - 300, 500, 600, {40, 40, 40, 255});
        Rectangle menuRect = {centerX - 250, centerY - 300, 500, 600};
        DrawRectangleRoundedLines(menuRect, 0.1f, 10, GOLD);
        
        DrawText("GAME MENU", centerX - 120, centerY - 250, 40, GOLD);
        
        float buttonY = centerY - 150;
        float buttonHeight = 60;
        float buttonSpacing = 70;
        
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "CONTINUE GAME")) {
            showingMenu = false;
        }
        
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "NEW GAME")) {
            showingDifficultySelect = true;
            showingMenu = false;
        }
        
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "SAVE GAME")) {
            SaveGame();
            showingMenu = false;
        }
        
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "LOAD GAME")) {
            LoadGame();
            showingMenu = false;
        }
        
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "QUIT TO DESKTOP")) {
            CloseWindow();
        }
        
        DrawText("Press ESC to close menu", centerX - 150, centerY + 220, 25, WHITE);
    }

    void RenderDifficultySelect() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        
        float pulse = sinf(GetTime() * 2) * 0.1f + 0.9f;
        Color titleColor = ColorAlpha(GOLD, pulse);
        
        DrawText("Klondike Solitaire", GetScreenWidth()/2 - 250, 150, 60, WHITE);
        DrawText("Stack-Based Edition", GetScreenWidth()/2 - 210, 220, 40, titleColor);

        DrawText("Select Difficulty:", GetScreenWidth()/2 - 150, 320, 40, WHITE);

        float buttonY = 400;
        float buttonHeight = 80;
        float buttonSpacing = 100;

        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "EASY - 1 Card Draw")) {
            InitializeGame(EASY);
            showingDifficultySelect = false;
        }
        DrawText("2 Aces visible, 1 hidden, 1 in stock", GetScreenWidth()/2 - 200, buttonY + buttonHeight + 5, 18, LIGHTGRAY);
        
        buttonY += buttonSpacing;
        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "MEDIUM - 2 Card Draw")) {
            InitializeGame(MEDIUM);
            showingDifficultySelect = false;
        }
        DrawText("1 Ace visible, 2 hidden, 1 in stock", GetScreenWidth()/2 - 200, buttonY + buttonHeight + 5, 18, LIGHTGRAY);
        
        buttonY += buttonSpacing;
        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "HARD - 3 Card Draw")) {
            InitializeGame(HARD);
            showingDifficultySelect = false;
        }
        DrawText("All Aces hidden or in stock", GetScreenWidth()/2 - 200, buttonY + buttonHeight + 5, 18, LIGHTGRAY);

        if (FileExists("savegame.dat")) {
            buttonY += buttonSpacing;
            if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "LOAD SAVED GAME")) {
                if (LoadGame()) {
                    showingDifficultySelect = false;
                }
            }
        }
    }

    void DrawTopBar() {
        DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, {40, 40, 40, 255});

        if (DrawButton(20, 20, 120, 80, "Menu")) {
            showingMenu = true;
        }

        DrawText(TextFormat("Moves: %d", moveCount), 200, 40, 35, WHITE);

        int mins = elapsedTime / 60;
        int secs = elapsedTime % 60;
        DrawText(TextFormat("Time: %02d:%02d", mins, secs), 520, 40, 35, WHITE);

        // Show best score
        if (bestScore[difficulty].moves < 9999) {
            DrawText(TextFormat("Best: %d moves, %d:%02d", 
                    bestScore[difficulty].moves, 
                    bestScore[difficulty].time / 60, 
                    bestScore[difficulty].time % 60), 850, 40, 35, GOLD);
        }

        DrawText(TextFormat("Hints: %d/%d", hintsRemaining, maxHints), 1350, 40, 35,
                (hintsRemaining > 0) ? SKYBLUE : GRAY);

        if (DrawButton(1650, 20, 120, 80, "Hint")) {
            ShowHint();
        }
        if (DrawButton(1800, 20, 120, 80, "Undo")) {
            Undo();
        }

        const char* muteButtonText = musicMuted ? "Unmute" : "Mute";
        if (DrawButton(1950, 20, 120, 80, muteButtonText)) {
            musicMuted = !musicMuted;
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
                if (musicMuted) {
                    PauseMusicStream(bgMusic);
                } else {
                    ResumeMusicStream(bgMusic);
                }
            }
        }

        string diffText = (difficulty == EASY) ? "Easy" : (difficulty == MEDIUM) ? "Medium" : "Hard";
        DrawText(TextFormat("Difficulty: %s", diffText.c_str()), 2050, 40, 35, WHITE);
    }

    void DrawGameBoard() {
        if (!gameDeck.stock.is_emptyy()) {
            Rectangle dest = {stockX, stockY, CARD_WIDTH, CARD_HEIGHT};
            DrawTexturePro(cardBack, {0, 0, (float)cardBack.width, (float)cardBack.height},
                          dest, {0, 0}, 0, WHITE);
            DrawText(TextFormat("%d", gameDeck.stock.size()), stockX + 10, stockY + 10, 25, YELLOW);
        } else {
            DrawEmptySlot(stockX, stockY);
            DrawText("↻", stockX + CARD_WIDTH/2 - 15, stockY + CARD_HEIGHT/2 - 20, 40, GRAY);
        }

        if (!gameDeck.waste.is_emptyy()) {
            DrawCard(gameDeck.waste.topp(), wasteX, wasteY);
        } else {
            DrawEmptySlot(wasteX, wasteY);
        }

        const char* foundationNames[] = {"♥ Hearts", "♦ Diamonds", "♣ Clubs", "♠ Spades"};
        Color foundationColors[] = {RED, RED, BLACK, BLACK};
        
        for (int i = 0; i < 4; i++) {
            float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* foundation = gameDeck.getFoundation(i + 1);

            if (!foundation->is_emptyy()) {
                DrawCard(foundation->topp(), x, foundationY);
            } else {
                DrawEmptySlot(x, foundationY);
            }
            DrawText(foundationNames[i], (int)x, (int)foundationY - 40, 25, foundationColors[i]);
        }

        for (int i = 0; i < 7; i++) {
            float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* tableau = gameDeck.getTableau(i + 1);

            DrawText(TextFormat("T%d", i + 1), (int)x + 5, (int)tableauY - 40, 25, WHITE);
            
            if (tableau->is_emptyy()) {
                DrawEmptySlot(x, tableauY);
            } else {
                for (int j = 0; j < tableau->size(); j++) {
                    card c = tableau->getAtSafe(j);
                    if (c.isValid()) {
                        float y = tableauY + j * TABLEAU_OFFSET;
                        DrawCard(c, x, y);
                    }
                }
            }
        }

        if (dragging && !draggedCards.empty()) {
            for (size_t i = 0; i < draggedCards.size(); i++) {
                float yOffset = i * TABLEAU_OFFSET;
                DrawCard(draggedCards[i],
                        dragCurrentPos.x,
                        dragCurrentPos.y + yOffset,
                        true);
            }
            
            Rectangle dragBounds = {
                dragCurrentPos.x - 5, 
                dragCurrentPos.y - 5, 
                CARD_WIDTH + 10, 
                CARD_HEIGHT + 10 + (draggedCards.size() - 1) * TABLEAU_OFFSET
            };
            DrawRectangleRoundedLines(dragBounds, 0.05f, 10, YELLOW);
        }
    }

    void DrawCard(card c, float x, float y, bool isDragging = false) {
        if (!c.isValid()) return;

        Rectangle dest = {x, y, CARD_WIDTH, CARD_HEIGHT};

        if (!c.faceup) {
            DrawTexturePro(cardBack, {0, 0, (float)cardBack.width, (float)cardBack.height},
                          dest, {0, 0}, 0, WHITE);
        } else {
            if (c.suit >= 0 && c.suit < 4 && c.rankk >= 1 && c.rankk <= 13) {
                DrawTexturePro(cardTextures[c.suit][c.rankk - 1],
                              {0, 0, (float)cardTextures[c.suit][c.rankk - 1].width,
                               (float)cardTextures[c.suit][c.rankk - 1].height},
                              dest, {0, 0}, 0, WHITE);
            }
        }

        if (isDragging) {
            DrawRectangleRoundedLines(dest, 0.05f, 10, YELLOW);
        }
    }

    void DrawEmptySlot(float x, float y) {
        Rectangle rect = {x, y, CARD_WIDTH, CARD_HEIGHT};
        DrawRectangleRounded(rect, 0.1f, 10, {0, 0, 0, 80});
        DrawRectangleRoundedLines(rect, 0.1f, 10, Fade(WHITE, 0.3f));
    }

    void DrawHints() {
        if (showingHints && !activeHints.empty()) {
            float alpha = min(hintDisplayTime, 1.0f) * 0.9f;
            
            float hintBoxY = 250;
            float hintBoxHeight = 100 + activeHints.size() * 50;
            
            DrawRectangle(GetScreenWidth()/2 - 400, hintBoxY, 800, hintBoxHeight, Fade(BLACK, alpha));
            Rectangle hintRect = {GetScreenWidth()/2 - 400.0f, (float)hintBoxY, 800.0f, (float)hintBoxHeight};
            DrawRectangleRoundedLines(hintRect, 0.1f, 10, Fade(GOLD, alpha));

            DrawText("HINTS", GetScreenWidth()/2 - 60, hintBoxY + 30, 40, Fade(GOLD, alpha));
            
            for (size_t i = 0; i < activeHints.size(); i++) {
                DrawText(TextFormat("%d. %s", (int)i + 1, activeHints[i].message.c_str()),
                        GetScreenWidth()/2 - 370,
                        hintBoxY + 90 + i * 50, 28, Fade(WHITE, alpha));
            }
        }
    }

    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Render();
        }
    }
};

int main()
{
    Game game;
    game.Run();
    return 0;
}
