#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <queue>

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
    
    void operator=(const stackk & other) {
        if (ptr) delete[] ptr;
        sizee = other.sizee;
        top = other.top;
        ptr = new T[sizee];
        for (int i = 0; i <= top; i++) {
            ptr[i] = other.ptr[i];
        }
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
    
    string intostring() {
        if (!faceup) {
            return "XX";
        }
        string rankstring;
        string suitstring;
        
        if (rankk == 1) rankstring = "A";
        else if (rankk == 2) rankstring = "2";
        else if (rankk == 3) rankstring = "3";
        else if (rankk == 4) rankstring = "4";
        else if (rankk == 5) rankstring = "5";
        else if (rankk == 6) rankstring = "6";
        else if (rankk == 7) rankstring = "7";
        else if (rankk == 8) rankstring = "8";
        else if (rankk == 9) rankstring = "9";
        else if (rankk == 10) rankstring = "10";
        else if (rankk == 11) rankstring = "J";
        else if (rankk == 12) rankstring = "Q";
        else if (rankk == 13) rankstring = "K";
        
        if (suit == 0) suitstring = "H";
        else if (suit == 1) suitstring = "D";
        else if (suit == 2) suitstring = "C";
        else if (suit == 3) suitstring = "S";
        
        return rankstring + suitstring;
    }
    
    bool operator==(const card& other) const {
        return suit == other.suit && rankk == other.rankk && faceup == other.faceup;
    }
    
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
    int score;
};

struct Hint {
    string message;
    card card;
    int sourceType;
    int destType;
    int destIndex;
};

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
    float size;
};

struct Animation {
    card animCard;
    Vector2 startPos;
    Vector2 endPos;
    float progress;
    bool active;
    int type;
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
    
    Deck() {
    }
    
    void createdeck() {
        if (!stock.is_emptyy()) {
            card temp;
            while (!stock.is_emptyy()) stock.pop(temp);
        }
        
        card c;
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 14; j++) {
                c.suit = i;
                c.rankk = j;
                c.faceup = true;
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
        vector<card> temp;
        card c;
        
        while (!stock.is_emptyy()) {
            if (stock.pop(c)) {
                temp.push_back(c);
            }
        }
        
        for (int i = temp.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(temp[i], temp[j]);
        }
        
        for (const auto& card : temp) {
            stock.push(card);
        }
    }
    
    void dealtotableau(stackk<card>& tableau, int j) {
        card c;
        for (int i = 1; i <= j; i++) {
            if (stock.pop(c)) {
                if (i == j) {
                    c.faceup = true;
                }
                else {
                    c.faceup = false;
                }
                tableau.push(c);
            }
        }
    }
    
    void dealtableau() {
        dealtotableau(tableau1, 1);
        dealtotableau(tableau2, 2);
        dealtotableau(tableau3, 3);
        dealtotableau(tableau4, 4);
        dealtotableau(tableau5, 5);
        dealtotableau(tableau6, 6);
        dealtotableau(tableau7, 7);
    }
    
    void drawtowastee() {
        card c;
        if (stock.is_emptyy()) {
            recyclewastetostock();
            if (stock.is_emptyy()) {
                return;
            }
        }
        if (stock.pop(c)) {
            c.faceup = true;
            waste.push(c);
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
    bool snapDragging;
    float snapProgress;
    Vector2 snapTargetPos;
    
    bool animatingReturn;
    Vector2 returnStartPos;
    Vector2 returnCurrentPos;
    float returnProgress;
    vector<Animation> animations;
    vector<Particle> particles;
    float autoCompleteTimer;
    bool autoCompleting;
    int autoCompleteCardIndex;
    vector<card> autoCompleteCards;
    
    int moveCount;
    int score;
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
    int bestScore[3];
    float hintDisplayTime;
    
    bool celebrating;
    float celebrationTimer;
    vector<Particle> fireworks;
    
    const float CARD_WIDTH = 140.0f;
    const float CARD_HEIGHT = 190.0f;
    const float CARD_SPACING = 20.0f;
    const float TABLEAU_OFFSET = 35.0f;
    const float TOP_BAR_HEIGHT = 80.0f;
    const float MARGIN = 150.0f;
    
    float stockX, stockY, wasteX, wasteY, foundationStartX, foundationY, tableauStartX, tableauY;
    bool showingMenu;
    bool showingDifficultySelect;
    
    float lastClickTime;
    int lastClickedCardType;
    int lastClickedCardIndex;
    
    float inputCooldown;
    
public:
    Game() {
        InitWindow(1600, 900, "Klondike Solitaire - Enhanced");
        InitAudioDevice();
        SetTargetFPS(60);
        
        dragging = false;
        animatingReturn = false;
        gameStarted = false;
        showingDifficultySelect = true;
        showingMenu = false;
        timerRunning = false;
        showingHints = false;
        snapDragging = false;
        autoCompleting = false;
        celebrating = false;
        hintsEnabled = true;
        
        moveCount = 0;
        score = 0;
        elapsedTime = 0;
        lastClickTime = 0;
        inputCooldown = 0;
        autoCompleteTimer = 0;
        hintDisplayTime = 0;
        celebrationTimer = 0;
        returnProgress = 0.0f;
        
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
        stockY = TOP_BAR_HEIGHT + 20;
        wasteX = stockX + CARD_WIDTH + CARD_SPACING;
        wasteY = stockY;
        foundationStartX = wasteX + CARD_WIDTH + CARD_SPACING * 3;
        foundationY = stockY;
        tableauStartX = MARGIN;
        tableauY = stockY + CARD_HEIGHT + 50;
        
        LoadBestScores();
    }
    
    ~Game() {
        StopTimer();
        
        UnloadTexture(cardBack);
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
                file >> bestScore[i];
                file.close();
            }
            else {
                bestScore[i] = 9999;
            }
        }
    }
    
    void SaveBestScore() {
        if (score < bestScore[difficulty]) {
            bestScore[difficulty] = score;
            const char* files[] = {"easy.txt", "medium.txt", "hard.txt"};
            ofstream file(files[difficulty]);
            if (file.is_open()) {
                file << score;
                file.close();
            }
        }
    }
    
    void SaveGame() {
        ofstream file("savegame.dat", ios::binary);
        if (file.is_open()) {
            file.write((char*)&difficulty, sizeof(difficulty));
            file.write((char*)&moveCount, sizeof(moveCount));
            file.write((char*)&score, sizeof(score));
            file.write((char*)&elapsedTime, sizeof(elapsedTime));
            file.close();
        }
    }
    
    bool LoadGame() {
        ifstream file("savegame.dat", ios::binary);
        if (file.is_open()) {
            file.read((char*)&difficulty, sizeof(difficulty));
            file.read((char*)&moveCount, sizeof(moveCount));
            file.read((char*)&score, sizeof(score));
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
            gameDeck.dealtableau();
            
            moveCount = 0;
            score = 500;
            elapsedTime = 0;
        }
        
        gameStarted = true;
        gameWon = false;
        gameLost = false;
        dragging = false;
        snapDragging = false;
        autoCompleting = false;
        celebrating = false;
        animatingReturn = false;
        
        undoStack.clear();
        activeHints.clear();
        animations.clear();
        particles.clear();
        fireworks.clear();
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
        snap.score = score;
        
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
            score = snap.score;
            
            dragging = false;
            snapDragging = false;
            animatingReturn = false;
            draggedCards.clear();
            
            if (IsAudioDeviceReady()) {
                PlaySound(cardFlipSound);
            }
        }
    }
    
    void GenerateHints() {
        activeHints.clear();
        
        if (!gameDeck.waste.is_emptyy()) {
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
                    return;
                }
            }
            
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
                    return;
                }
            }
        }
        
        for (int i = 0; i < 7; i++) {
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
                            return;
                        }
                    }
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
    
    void CheckAutoComplete() {
        if (gameDeck.check_win()) {
            StartAutoComplete();
        }
    }
    
    void StartAutoComplete() {
        autoCompleting = true;
        autoCompleteTimer = 0;
        autoCompleteCardIndex = 0;
        
        autoCompleteCards.clear();
        card c;
        
        while (!gameDeck.waste.is_emptyy()) {
            if (gameDeck.waste.pop(c)) {
                autoCompleteCards.push_back(c);
            }
        }
        
        for (int i = 1; i <= 7; i++) {
            stackk<card>* tableau = gameDeck.getTableau(i);
            while (!tableau->is_emptyy()) {
                if (tableau->pop(c)) {
                    autoCompleteCards.push_back(c);
                }
            }
        }
        
        if (IsAudioDeviceReady()) {
            PlaySound(winSound);
        }
    }
    
    void UpdateAutoComplete(float deltaTime) {
        if (!autoCompleting) return;
        
        autoCompleteTimer += deltaTime;
        
        if (autoCompleteTimer >= 0.1f && autoCompleteCardIndex < autoCompleteCards.size()) {
            card c = autoCompleteCards[autoCompleteCardIndex];
            
            for (int i = 0; i < 4; i++) {
                stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                if (gameDeck.can_move_to_foundation(*foundation, c)) {
                    foundation->push(c);
                    break;
                }
            }
            
            autoCompleteCardIndex++;
            autoCompleteTimer = 0;
            
            AddAnimation(c, {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}, 
                        {foundationStartX + (c.suit) * (CARD_WIDTH + CARD_SPACING), foundationY}, 1);
        }
        
        if (autoCompleteCardIndex >= autoCompleteCards.size()) {
            autoCompleting = false;
            celebrating = true;
            celebrationTimer = 5.0f;
            StartFireworks();
        }
    }
    
    void AddAnimation(card c, Vector2 start, Vector2 end, int type = 0) {
        Animation anim;
        anim.animCard = c;
        anim.startPos = start;
        anim.endPos = end;
        anim.progress = 0;
        anim.active = true;
        anim.type = type;
        animations.push_back(anim);
    }
    
    void UpdateAnimations(float deltaTime) {
        if (animatingReturn) {
            returnProgress += 5.0f * deltaTime;
            if (returnProgress >= 1.0f) {
                animatingReturn = false;
                dragging = false;
                ReturnCards();
            } else {
                returnCurrentPos = Vector2Lerp(returnStartPos, dragStartPos, returnProgress);
            }
        }
        
        for (auto& anim : animations) {
            if (anim.active) {
                anim.progress += deltaTime * 2.0f;
                if (anim.progress >= 1.0f) {
                    anim.active = false;
                }
            }
        }
        
        animations.erase(
            remove_if(animations.begin(), animations.end(),
                     [](const Animation& a) { return !a.active; }),
            animations.end()
        );
        
        for (auto& particle : particles) {
            particle.position.x += particle.velocity.x * deltaTime;
            particle.position.y += particle.velocity.y * deltaTime;
            particle.velocity.y += 50.0f * deltaTime;
            particle.life -= deltaTime;
            particle.size *= 0.99f;
        }
        
        particles.erase(
            remove_if(particles.begin(), particles.end(),
                     [](const Particle& p) { return p.life <= 0; }),
            particles.end()
        );
        
        UpdateFireworks(deltaTime);
    }
    
    void StartFireworks() {
        for (int i = 0; i < 100; i++) {
            Particle firework;
            firework.position = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
            float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
            float speed = (float)GetRandomValue(200, 500);
            firework.velocity = {cosf(angle) * speed, sinf(angle) * speed};
            firework.color = ColorFromHSV((float)GetRandomValue(0, 360), 1.0f, 1.0f);
            firework.life = (float)GetRandomValue(10, 30) / 10.0f;
            firework.size = (float)GetRandomValue(2, 6);
            fireworks.push_back(firework);
        }
    }
    
    void UpdateFireworks(float deltaTime) {
        if (!celebrating) return;
        
        celebrationTimer -= deltaTime;
        if (celebrationTimer <= 0) {
            celebrating = false;
            fireworks.clear();
            return;
        }
        
        for (auto& firework : fireworks) {
            firework.position.x += firework.velocity.x * deltaTime;
            firework.position.y += firework.velocity.y * deltaTime;
            firework.velocity.y += 100.0f * deltaTime;
            firework.life -= deltaTime;
        }
        
        fireworks.erase(
            remove_if(fireworks.begin(), fireworks.end(),
                     [](const Particle& p) { return p.life <= 0; }),
            fireworks.end()
        );
        
        if (GetRandomValue(0, 100) < 10) {
            StartFireworks();
        }
    }
    
    void AddScore(int points, string reason = "") {
        score += points;
        if (score < 0) score = 0;
        
        if (!reason.empty()) {
            Particle scoreParticle;
            scoreParticle.position = {GetScreenWidth()/2.0f, 100.0f};
            scoreParticle.velocity = {0, -50};
            scoreParticle.color = (points > 0) ? GREEN : RED;
            scoreParticle.life = 1.5f;
            scoreParticle.size = 20.0f;
            particles.push_back(scoreParticle);
        }
    }
    
    bool IsCardClickable(card c, Vector2 position) {
        if (!c.isValid()) return false;
        if (!c.faceup) return false;
        
        Rectangle cardRect = {position.x, position.y, CARD_WIDTH, CARD_HEIGHT};
        return CheckCollisionPointRec(GetMousePosition(), cardRect);
    }
    
    void HandleDoubleClick(card c, int sourceType, int sourceIndex) {
        float currentTime = GetTime();
        float timeSinceLastClick = currentTime - lastClickTime;
        
        if (timeSinceLastClick < 0.3f &&
            lastClickedCardType == sourceType &&
            lastClickedCardIndex == sourceIndex) {
            
            for (int i = 0; i < 4; i++) {
                stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                if (gameDeck.can_move_to_foundation(*foundation, c)) {
                    SaveState();
                    
                    if (sourceType == 0) {
                        card temp;
                        gameDeck.waste.pop(temp);
                        foundation->push(temp);
                    }
                    else if (sourceType >= 1 && sourceType <= 7) {
                        stackk<card>* tableau = gameDeck.getTableau(sourceType);
                        card temp;
                        tableau->pop(temp);
                        foundation->push(temp);
                        gameDeck.flip_tableau_top(*tableau);
                    }
                    
                    AddScore(10, "Auto-move to foundation");
                    moveCount++;
                    
                    AddAnimation(c, dragStartPos, 
                                {foundationStartX + i * (CARD_WIDTH + CARD_SPACING), foundationY});
                    
                    if (IsAudioDeviceReady()) {
                        PlaySound(cardPlaceSound);
                    }
                    
                    CheckAutoComplete();
                    break;
                }
            }
            
            lastClickTime = 0;
        }
        else {
            lastClickTime = currentTime;
            lastClickedCardType = sourceType;
            lastClickedCardIndex = sourceIndex;
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
        dragOffset = Vector2Subtract(GetMousePosition(), position);
        
        draggedCards.clear();
        
        if (sourceType == 0) {
            card temp;
            if (gameDeck.waste.pop(temp)) {
                draggedCards.push_back(temp);
            }
        }
        else if (sourceType >= 1 && sourceType <= 7) {
            stackk<card>* tableau = gameDeck.getTableau(sourceType);
            for (int i = sourceIndex; i < tableau->size(); i++) {
                draggedCards.push_back(tableau->getAtSafe(i));
            }
            for (int i = tableau->size() - 1; i >= sourceIndex; i--) {
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
        dragCurrentPos = Vector2Subtract(mousePos, dragOffset);
        
        if (!snapDragging) {
            Vector2 nearestSnap = FindNearestSnapPoint(mousePos);
            float distance = Vector2Distance(mousePos, nearestSnap);
            
            if (distance < 50.0f) {
                snapDragging = true;
                snapTargetPos = nearestSnap;
                snapProgress = 0;
            }
        }
        
        if (snapDragging) {
            snapProgress += GetFrameTime() * 5.0f;
            if (snapProgress > 1.0f) snapProgress = 1.0f;
            
            dragCurrentPos = Vector2Lerp(dragCurrentPos, 
                                        Vector2Subtract(snapTargetPos, dragOffset), 
                                        snapProgress);
        }
    }
    
    Vector2 FindNearestSnapPoint(Vector2 position) {
        Vector2 nearest = position;
        float minDistance = 1000000.0f;
        
        for (int i = 0; i < 4; i++) {
            Vector2 slotPos = {foundationStartX + i * (CARD_WIDTH + CARD_SPACING), foundationY};
            float dist = Vector2Distance(position, slotPos);
            if (dist < minDistance) {
                minDistance = dist;
                nearest = slotPos;
            }
        }
        
        for (int i = 0; i < 7; i++) {
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            float y = tableauY;
            if (!tableau->is_emptyy()) {
                y = tableauY + (tableau->size() - 1) * TABLEAU_OFFSET;
            }
            Vector2 slotPos = {tableauStartX + i * (CARD_WIDTH + CARD_SPACING), y};
            float dist = Vector2Distance(position, slotPos);
            if (dist < minDistance) {
                minDistance = dist;
                nearest = slotPos;
            }
        }
        
        return nearest;
    }
    
    void EndDrag() {
        if (!dragging || draggedCards.empty()) return;
        
        bool placed = false;
        Vector2 mousePos = GetMousePosition();
        card topCard = draggedCards[0];
        
        if (draggedCards.size() == 1) {
            for (int i = 0; i < 4; i++) {
                Vector2 slotPos = {foundationStartX + i * (CARD_WIDTH + CARD_SPACING), foundationY};
                Rectangle slotRect = {slotPos.x, slotPos.y, CARD_WIDTH, CARD_HEIGHT};
                
                if (CheckCollisionPointRec(mousePos, slotRect)) {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (gameDeck.can_move_to_foundation(*foundation, topCard)) {
                        foundation->push(topCard);
                        placed = true;
                        AddScore(10, "To foundation");
                        
                        if (dragSourceType >= 1 && dragSourceType <= 7) {
                            stackk<card>* sourceTableau = gameDeck.getTableau(dragSourceType);
                            gameDeck.flip_tableau_top(*sourceTableau);
                        }
                        break;
                    }
                }
            }
        }
        
        if (!placed) {
            for (int i = 0; i < 7; i++) {
                stackk<card>* tableau = gameDeck.getTableau(i + 1);
                float y = tableauY;
                if (!tableau->is_emptyy()) {
                    y = tableauY + (tableau->size() - 1) * TABLEAU_OFFSET;
                }
                
                Vector2 slotPos = {tableauStartX + i * (CARD_WIDTH + CARD_SPACING), y};
                Rectangle slotRect = {slotPos.x, slotPos.y, CARD_WIDTH, CARD_HEIGHT + 20};
                
                if (CheckCollisionPointRec(mousePos, slotRect)) {
                    if (gameDeck.can_move_to_tableau(*tableau, topCard)) {
                        for (const auto& card : draggedCards) {
                            tableau->push(card);
                        }
                        placed = true;
                        AddScore(5, "To tableau");
                        
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
            animatingReturn = true;
            returnProgress = 0.0f;
            returnStartPos = {mousePos.x - dragOffset.x, mousePos.y - dragOffset.y};
            returnCurrentPos = returnStartPos;
            
            if (!undoStack.empty()) {
                undoStack.pop_back();
            }
        }
        
        dragging = false;
        snapDragging = false;
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
        
        UpdateAnimations(deltaTime);
        
        UpdateAutoComplete(deltaTime);
        
        if (showingDifficultySelect || showingMenu || gameWon || gameLost) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (showingDifficultySelect) showingDifficultySelect = false;
                else if (showingMenu) showingMenu = false;
            }
            return;
        }
        
        if (gameDeck.check_win() && !autoCompleting && !celebrating) {
            StartAutoComplete();
            return;
        }
        
        if (CheckLoss()) {
            gameLost = true;
            return;
        }
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            Rectangle stockRect = {stockX, stockY, CARD_WIDTH, CARD_HEIGHT};
            
            if (CheckCollisionPointRec(mousePos, stockRect)) {
                if (inputCooldown <= 0) {
                    SaveState();
                    for (int i = 0; i < cardsPerDraw; i++) {
                        gameDeck.drawtowastee();
                    }
                    if (!gameDeck.waste.is_emptyy()) {
                        AddScore(1, "Draw from stock");
                        moveCount++;
                        
                        if (IsAudioDeviceReady()) {
                            PlaySound(cardFlipSound);
                        }
                    }
                    inputCooldown = 0.2f;
                }
                return;
            }
        }
        
        if (dragging) {
            UpdateDrag();
            
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                EndDrag();
            }
        }
        else if (!animatingReturn) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && inputCooldown <= 0) {
                Vector2 mousePos = GetMousePosition();
                
                if (!gameDeck.waste.is_emptyy()) {
                    Rectangle wasteRect = {wasteX, wasteY, CARD_WIDTH, CARD_HEIGHT};
                    if (CheckCollisionPointRec(mousePos, wasteRect)) {
                        card c = gameDeck.waste.topp();
                        HandleDoubleClick(c, 0, 0);
                        StartDrag(c, 0, 0, {wasteX, wasteY});
                    }
                }
                
                for (int i = 0; i < 4; i++) {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (!foundation->is_emptyy()) {
                        float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
                        Rectangle foundationRect = {x, foundationY, CARD_WIDTH, CARD_HEIGHT};
                        
                        if (CheckCollisionPointRec(mousePos, foundationRect)) {
                            card c = foundation->topp();
                            HandleDoubleClick(c, 8 + i, i);
                            StartDrag(c, 8 + i, i, {x, foundationY});
                            break;
                        }
                    }
                }
                
                for (int i = 0; i < 7; i++) {
                    stackk<card>* tableau = gameDeck.getTableau(i + 1);
                    if (!tableau->is_emptyy()) {
                        float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);
                        
                        for (int j = tableau->size() - 1; j >= 0; j--) {
                            card c = tableau->getAtSafe(j);
                            if (c.faceup) {
                                float y = tableauY + j * TABLEAU_OFFSET;
                                float height = (j == tableau->size() - 1) ? CARD_HEIGHT : TABLEAU_OFFSET;
                                Rectangle cardRect = {x, y, CARD_WIDTH, height};
                                
                                if (CheckCollisionPointRec(mousePos, cardRect)) {
                                    HandleDoubleClick(c, i + 1, j);
                                    StartDrag(c, i + 1, j, {x, y});
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
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
    }
    
    bool CheckLoss() {
        if (gameDeck.stock.is_emptyy() && gameDeck.waste.is_emptyy()) {
            return false;
        }
        return false;
    }
    
    void Render() {
        BeginDrawing();
        ClearBackground({0, 100, 50, 255});
        
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
        DrawAnimations();
        DrawParticles();
        DrawFireworks();
        DrawHints();
        
        if (gameWon || gameLost) {
            RenderGameOver();
        }
        
        EndDrawing();
    }
    
    void RenderDifficultySelect() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        DrawText("Klondike Solitaire", GetScreenWidth()/2 - 150, 100, 50, WHITE);
        DrawText("Enhanced Edition", GetScreenWidth()/2 - 120, 160, 30, GOLD);
        
        DrawText("Select Difficulty:", GetScreenWidth()/2 - 100, 250, 30, WHITE);
        
        if (DrawButton(GetScreenWidth()/2 - 150, 320, 300, 70, "EASY - 1 Card Draw")) {
            InitializeGame(EASY);
            showingDifficultySelect = false;
        }
        if (DrawButton(GetScreenWidth()/2 - 150, 410, 300, 70, "MEDIUM - 2 Card Draw")) {
            InitializeGame(MEDIUM);
            showingDifficultySelect = false;
        }
        if (DrawButton(GetScreenWidth()/2 - 150, 500, 300, 70, "HARD - 3 Card Draw")) {
            InitializeGame(HARD);
            showingDifficultySelect = false;
        }
        
        if (FileExists("savegame.dat")) {
            if (DrawButton(GetScreenWidth()/2 - 150, 590, 300, 70, "LOAD SAVED GAME")) {
                if (LoadGame()) {
                    showingDifficultySelect = false;
                }
            }
        }
    }
    
    void RenderMenu() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        DrawText("Game Menu", GetScreenWidth()/2 - 80, 100, 40, WHITE);
        
        if (DrawButton(GetScreenWidth()/2 - 100, 200, 200, 60, "RESUME")) {
            showingMenu = false;
        }
        if (DrawButton(GetScreenWidth()/2 - 100, 280, 200, 60, "NEW GAME")) {
            showingDifficultySelect = true;
            showingMenu = false;
        }
        if (DrawButton(GetScreenWidth()/2 - 100, 360, 200, 60, "SAVE GAME")) {
            SaveGame();
        }
        if (DrawButton(GetScreenWidth()/2 - 100, 440, 200, 60, "QUIT")) {
            CloseWindow();
            exit(0);
        }
    }
    
    void DrawTopBar() {
        DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, {40, 40, 40, 255});
        
        if (DrawButton(10, 10, 80, 60, "Menu")) {
            showingMenu = true;
        }
        
        DrawText(TextFormat("Moves: %d", moveCount), 120, 30, 25, WHITE);
        DrawText(TextFormat("Score: %d", score), 280, 30, 25, GOLD);
        
        int mins = elapsedTime / 60;
        int secs = elapsedTime % 60;
        DrawText(TextFormat("Time: %02d:%02d", mins, secs), 450, 30, 25, WHITE);
        
        DrawText(TextFormat("Hints: %d/%d", hintsRemaining, maxHints), 650, 30, 25, 
                (hintsRemaining > 0) ? SKYBLUE : GRAY);
        
        if (DrawButton(800, 10, 80, 60, "Hint")) {
            ShowHint();
        }
        if (DrawButton(900, 10, 80, 60, "Undo")) {
            Undo();
        }
        if (DrawButton(1000, 10, 80, 60, "Save")) {
            SaveGame();
        }
        
        if (DrawButton(1100, 10, 80, 60, IsAudioDeviceReady() && bgMusic.ctxData != nullptr && IsMusicStreamPlaying(bgMusic) ? "Music On" : "Music Off")) {
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
                if (IsMusicStreamPlaying(bgMusic)) {
                    PauseMusicStream(bgMusic);
                } else {
                    ResumeMusicStream(bgMusic);
                }
            }
        }
        
        string diffText = (difficulty == EASY) ? "Easy" : (difficulty == MEDIUM) ? "Medium" : "Hard";
        DrawText(TextFormat("Difficulty: %s", diffText.c_str()), 1200, 30, 25, WHITE);
    }
    
    void DrawGameBoard() {
        if (!gameDeck.stock.is_emptyy()) {
            DrawCard(gameDeck.stock.topp(), stockX, stockY);
        } else {
            DrawEmptySlot(stockX, stockY);
        }
        
        if (!gameDeck.waste.is_emptyy()) {
            DrawCard(gameDeck.waste.topp(), wasteX, wasteY);
        } else {
            DrawEmptySlot(wasteX, wasteY);
        }
        
        const char* foundationNames[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        for (int i = 0; i < 4; i++) {
            float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* foundation = gameDeck.getFoundation(i + 1);
            
            if (!foundation->is_emptyy()) {
                DrawCard(foundation->topp(), x, foundationY);
            } else {
                DrawEmptySlot(x, foundationY);
            }
            DrawText(foundationNames[i], (int)x, (int)foundationY - 25, 20, WHITE);
        }
        
        for (int i = 0; i < 7; i++) {
            float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            
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
            DrawText(TextFormat("%d", i + 1), (int)x, (int)tableauY - 25, 20, WHITE);
        }
        
        if (dragging && !draggedCards.empty()) {
            for (size_t i = 0; i < draggedCards.size(); i++) {
                DrawCard(draggedCards[i], 
                        dragCurrentPos.x, 
                        dragCurrentPos.y + i * TABLEAU_OFFSET, 
                        true);
            }
        }
        
        if (animatingReturn) {
            for (size_t i = 0; i < draggedCards.size(); i++) {
                DrawCard(draggedCards[i], 
                        returnCurrentPos.x, 
                        returnCurrentPos.y + i * TABLEAU_OFFSET, 
                        true);
            }
        }
    }
    
    void DrawAnimations() {
        for (const auto& anim : animations) {
            if (anim.active) {
                Vector2 pos = Vector2Lerp(anim.startPos, anim.endPos, anim.progress);
                DrawCard(anim.animCard, pos.x, pos.y);
                
                if (anim.type == 1) {
                    DrawCircle(pos.x, pos.y, 10, Fade(GOLD, 0.5f));
                }
            }
        }
    }
    
    void DrawParticles() {
        for (const auto& particle : particles) {
            if (particle.life > 0) {
                DrawCircle(particle.position.x, particle.position.y, 
                          particle.size, Fade(particle.color, particle.life));
            }
        }
    }
    
    void DrawFireworks() {
        for (const auto& firework : fireworks) {
            if (firework.life > 0) {
                DrawCircle(firework.position.x, firework.position.y, 
                          firework.size, Fade(firework.color, firework.life));
                
                for (int i = 0; i < 5; i++) {
                    float angle = GetTime() * 5.0f + i * 72.0f * DEG2RAD;
                    Vector2 offset = {cosf(angle) * 10.0f, sinf(angle) * 10.0f};
                    DrawCircle(firework.position.x + offset.x, 
                              firework.position.y + offset.y, 
                              firework.size / 2.0f, Fade(firework.color, firework.life * 0.5f));
                }
            }
        }
    }
    
    void DrawHints() {
        if (showingHints && !activeHints.empty()) {
            float alpha = min(hintDisplayTime, 1.0f) * 0.9f;
            DrawRectangle(GetScreenWidth()/2 - 250, 200, 500, 150, Fade(BLACK, alpha));
            DrawRectangleLines(GetScreenWidth()/2 - 250, 200, 500, 150, Fade(GOLD, alpha));
            
            DrawText("HINT", GetScreenWidth()/2 - 40, 220, 30, Fade(GOLD, alpha));
            DrawText(activeHints[0].message.c_str(), 
                    GetScreenWidth()/2 - 230, 
                    280, 24, Fade(WHITE, alpha));
        }
    }
    
    void RenderGameOver() {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        
        if (gameWon) {
            DrawText("VICTORY!", GetScreenWidth()/2 - 150, 200, 80, GOLD);
            DrawText(TextFormat("Final Score: %d", score), GetScreenWidth()/2 - 120, 300, 40, WHITE);
            DrawText(TextFormat("Moves: %d", moveCount), GetScreenWidth()/2 - 120, 350, 40, WHITE);
            DrawText(TextFormat("Time: %02d:%02d", elapsedTime/60, elapsedTime%60), 
                    GetScreenWidth()/2 - 120, 400, 40, WHITE);
            
            if (score == bestScore[difficulty]) {
                DrawText("NEW HIGH SCORE!", GetScreenWidth()/2 - 150, 450, 30, GOLD);
            }
        } else {
            DrawText("GAME OVER", GetScreenWidth()/2 - 180, 300, 80, RED);
            DrawText("No moves available", GetScreenWidth()/2 - 150, 400, 30, WHITE);
        }
        
        if (DrawButton(GetScreenWidth()/2 - 100, 550, 200, 60, "New Game")) {
            showingDifficultySelect = true;
            gameWon = false;
            gameLost = false;
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
            DrawRectangleRoundedLinesEx(dest, 0.05f, 10, 3, YELLOW);
        }
    }
    
    void DrawEmptySlot(float x, float y) {
        Rectangle rect = {x, y, CARD_WIDTH, CARD_HEIGHT};
        DrawRectangleRounded(rect, 0.1f, 10, {0, 0, 0, 50});
        DrawRectangleRoundedLinesEx(rect, 0.1f, 10, 3, Fade(WHITE, 0.3f));
    }
    
    bool DrawButton(float x, float y, float w, float h, const char* text) {
        Rectangle rect = {x, y, w, h};
        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
        
        DrawRectangleRounded(rect, 0.2f, 10, hovered ? DARKGRAY : GRAY);
        
        int fontSize = 20;
        if (strlen(text) > 15) fontSize = 16;
        
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, (int)(x + (w - textWidth)/2), (int)(y + (h - fontSize)/2), fontSize, WHITE);
        
        return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
    
    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Render();
        }
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
