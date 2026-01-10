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
#include <random>

using namespace std;

template <class T>
class stackk
{
    int top;
    int sizee;
    T *ptr = nullptr;
public:
    stackk()
    {
        sizee = 52;
        top = -1;
        ptr = new T[sizee];
    }
    stackk(int s)
    {
        sizee = s;
        top = -1;
        ptr = new T[sizee];
    }

    stackk(const stackk & other)
    {
        sizee = other.sizee;
        top = other.top;
        ptr = new T[sizee];
        for (int i = 0; i <= top; i++)
        {
            ptr[i] = other.ptr[i];
        }
    }

    stackk& operator=(const stackk & other)
    {
        if (this != &other)
        {
            if (ptr) delete[] ptr;
            sizee = other.sizee;
            top = other.top;
            ptr = new T[sizee];
            for (int i = 0; i <= top; i++)
            {
                ptr[i] = other.ptr[i];
            }
        }
        return *this;
    }

    bool push(T n)
    {
        if (!is_full())
        {
            ptr[++top] = n;
            return true;
        }
        return false;
    }

    bool pop(T &v)
    {
        if (!is_emptyy())
        {
            v = ptr[top--];
            return true;
        }
        return false;
    }

    T topp()
    {
        if (!is_emptyy())
        {
            return ptr[top];
        }
        return T();
    }

    bool is_emptyy()
    {
        return top == -1;
    }
    bool is_full()
    {
        return top == sizee - 1;
    }
    int size()
    {
        return top + 1;
    }

    T getAtSafe(int index)
    {
        if (ptr && index >= 0 && index <= top)
        {
            return ptr[index];
        }
        return T();
    }

    void clear()
    {
        top = -1;
    }

    ~stackk()
    {
        if (ptr)
        {
            delete[] ptr;
            ptr = nullptr;
        }
    }
};

struct card
{
    int suit;
    int rankk;
    bool faceup;
    string color;

    card() : suit(0), rankk(0), faceup(false), color("") {}

    bool isRed() const
    { return suit == 0 || suit == 1; }
    bool isValid() const { return suit >= 0 && suit <= 3 && rankk >= 1 && rankk <= 13; }

    void restoreColor()
    {
        if (suit == 0 || suit == 1) color = "red";
        else color = "black";
    }
};

enum Difficulty { EASY = 0, MEDIUM = 1, HARD = 2 };
enum PendingAction { ACTION_NONE, ACTION_QUIT, ACTION_TO_MENU, ACTION_JUST_SAVE };

struct GameSnapshot
{
    stackk<card> stock, waste;
    stackk<card> tableau1, tableau2, tableau3, tableau4, tableau5, tableau6, tableau7;
    stackk<card> foundation1, foundation2, foundation3, foundation4;
    int moveCount;
    int elapsedTime;
};

struct BestScore
{
    int moves;
    int time;
    BestScore() : moves(9999), time(9999) {}
};

struct Hint
{
    string message;
    card card;
    int sourceType;
    int destType;
    int destIndex;
};

class Deck
{
public:
    stackk<card> stock;
    stackk<card> waste;
    stackk<card> tableau1, tableau2, tableau3, tableau4, tableau5, tableau6, tableau7;
    stackk<card> foundation1, foundation2, foundation3, foundation4;

    Deck() {}

    void clearAll()
    {
        stock.clear();
        waste.clear();
        tableau1.clear();
        tableau2.clear();
        tableau3.clear();
        tableau4.clear();
        tableau5.clear();
        tableau6.clear();
        tableau7.clear();
        foundation1.clear();
        foundation2.clear();
        foundation3.clear();
        foundation4.clear();
    }

    void createdeck()
    {
        stock.clear();
        card c;
        for (int i = 0; i < 4; i++)
            {
            for (int j = 1; j < 14; j++)
            {
                c.suit = i;
                c.rankk = j;
                c.faceup = false;
                if (i == 0 || i == 1) c.color = "red";
                else c.color = "black";
                stock.push(c);
            }
        }
    }

    void shuffledeck()
    {
        srand((unsigned int)time(0));
        vector<card> temp;
        card c;
        while (!stock.is_emptyy())
            {
            if (stock.pop(c)) temp.push_back(c);
        }
        for (int i = temp.size() - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            swap(temp[i], temp[j]);
        }
        for (const auto& cd : temp) stock.push(cd);
    }

    void dealtableau(Difficulty diff)
    {
        vector<card> allCards;
        card c;
        while(!stock.is_emptyy())
            {
            if(stock.pop(c)) allCards.push_back(c);
        }

        vector<card> aces;
        vector<card> others;
        for(const auto& cd : allCards)
            {
            if(cd.rankk == 1) aces.push_back(cd);
            else others.push_back(cd);
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(others.begin(), others.end(), std::default_random_engine(seed));

        int acesVisible = 0;
        int acesHidden = 0;
        int acesStock = 0;

        if (diff == EASY)
            {
            acesVisible = 2; acesHidden = 1; acesStock = 1;
        }
        else if (diff == MEDIUM)
            {
            acesVisible = 1; acesHidden = 2; acesStock = 1;
        }
        else
            {
            acesVisible = 0; acesHidden = 0; acesStock = 0;
        }

        vector<card> topPool;
        vector<card> hiddenPool;
        vector<card> stockPool;

        int aceIdx = 0;

        for(int i=0; i<acesVisible && aceIdx < 4; i++) topPool.push_back(aces[aceIdx++]);
        for(int i=0; i<acesHidden && aceIdx < 4; i++) hiddenPool.push_back(aces[aceIdx++]);
        for(int i=0; i<acesStock && aceIdx < 4; i++) stockPool.push_back(aces[aceIdx++]);

        while(aceIdx < 4)
            {
            if(rand() % 2 == 0) hiddenPool.push_back(aces[aceIdx++]);
            else stockPool.push_back(aces[aceIdx++]);
        }

        while(topPool.size() < 7 && !others.empty())
            {
            topPool.push_back(others.back());
        others.pop_back();

        }
        while(hiddenPool.size() < 21 && !others.empty())
        {
            hiddenPool.push_back(others.back());
            others.pop_back();
        }
        while(!others.empty())
        {
            stockPool.push_back(others.back());
            others.pop_back();
        }

        std::shuffle(topPool.begin(), topPool.end(), std::default_random_engine(seed));
        std::shuffle(hiddenPool.begin(), hiddenPool.end(), std::default_random_engine(seed));

        int hiddenCounter = 0;

        auto fillCol = [&](stackk<card>& tab, int colIdx)
        {
            int hiddenCount = colIdx;
            for(int i=0; i<hiddenCount; i++)
                {
                card h = hiddenPool[hiddenCounter++];
                h.faceup = false;
                tab.push(h);
            }
            card t = topPool[colIdx];
            t.faceup = true;
            tab.push(t);
        };

        fillCol(tableau1, 0);
        fillCol(tableau2, 1);
        fillCol(tableau3, 2);
        fillCol(tableau4, 3);
        fillCol(tableau5, 4);
        fillCol(tableau6, 5);
        fillCol(tableau7, 6);

        for(const auto& s : stockPool)
            {
            stock.push(s);
        }
    }

    void drawtowastee(int cardsPerDraw = 1)
    {
        if (stock.is_emptyy())
            {
            recyclewastetostock();
            if (stock.is_emptyy())
            return;
        }
        for (int i = 0; i < cardsPerDraw && !stock.is_emptyy(); i++)
        {
            card c;
            if (stock.pop(c))
            {
                c.faceup = true;
                waste.push(c);
            }
        }
    }

    void recyclewastetostock()

    {
        if (waste.is_emptyy())
            return;
        card c;
        stackk<card> temp;
        while (!waste.is_emptyy())
        {
            if (waste.pop(c))
            {
                c.faceup = false;
                temp.push(c);
            }
        }
        while (!temp.is_emptyy())
        {
            if (temp.pop(c)) stock.push(c);
        }
    }

    stackk<card>* getTableau(int index)
    {
        switch(index)
        {
            case 1:
                return &tableau1;
            case 2:
                return &tableau2;
            case 3:
                return &tableau3;
            case 4:
                return &tableau4;
            case 5:
                return &tableau5;
            case 6:
                return &tableau6;
            case 7:
                return &tableau7;
            default:
                return nullptr;
        }
    }

    stackk<card>* getFoundation(int index)
    {
        switch(index)
        {
            case 1:
                return &foundation1;
            case 2:
                return &foundation2;
            case 3:
                return &foundation3;
            case 4:
                return &foundation4;
            default:
                return nullptr;
        }
    }

    bool can_move_to_foundation(stackk<card>& foundation, card c)
    {
        if (foundation.is_emptyy()) return c.rankk == 1;
        card top = foundation.topp();
        return (top.suit == c.suit && c.rankk == top.rankk + 1);
    }

    bool can_move_to_tableau(stackk<card>& tableau, card c)
    {
        if (tableau.is_emptyy())
            return c.rankk == 13;
        card top = tableau.topp();
        return (c.rankk == top.rankk - 1 && c.color != top.color);
    }

    void flip_tableau_top(stackk<card>& tableau)
    {
        if (!tableau.is_emptyy())
            {
            card top = tableau.topp();
            if (!top.faceup)
            {
                card temp;
                if (tableau.pop(temp))
                {
                    temp.faceup = true;
                    tableau.push(temp);
                }
            }
        }
    }

    bool check_win()
    {
        return (foundation1.is_full() && foundation2.is_full() && foundation3.is_full() && foundation4.is_full());
    }

    bool check_loss()
    {
        for(int i=1; i<=7; i++)
            {
            stackk<card>* t = getTableau(i);
            if(!t->is_emptyy() && t->topp().faceup)
            {
                card c = t->topp();
                for(int j=1; j<=4; j++)
                    {
                        if(can_move_to_foundation(*getFoundation(j), c))
                        return false;
                }
                for(int j=1; j<=7; j++)
                    {
                        if(i!=j && can_move_to_tableau(*getTableau(j), c))
                        return false;
                }
            }
        }

        auto check_pile_deeply = [&](stackk<card>& pile) -> bool
        {
            for(int k=0; k < pile.size(); k++)
                {
                card c = pile.getAtSafe(k);
                for(int f=1; f<=4; f++)
                {
                    if(can_move_to_foundation(*getFoundation(f), c))
                    return true;
                }
                for(int t=1; t<=7; t++)
                {
                    if(can_move_to_tableau(*getTableau(t), c))
                    return true;
                }
            }
            return false;
        };

        if (check_pile_deeply(stock))
            return false;
        if (check_pile_deeply(waste))
        return false;

        return true;
    }

    bool has_valid_initial_move()
    {
        return !waste.is_emptyy() || !stock.is_emptyy();
    }
};

class Game
{
private:
    Texture2D cardTextures[4][13];
    Texture2D cardBack;
    Texture2D backgroundTexture;
    Texture2D btnMenu, btnHint, btnUndo, btnMute, btnUnmute;
    Texture2D restockTexture;

    Sound cardSlideSound, cardFlipSound, cardPlaceSound, winSound, clickSound, hintSound;
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

    bool showingWelcome;
    bool showingMenu;
    bool showingDifficultySelect;
    bool showingSavePrompt;
    PendingAction pendingAction;

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

        showingWelcome = true;
        showingDifficultySelect = false;
        showingMenu = false;
        showingSavePrompt = false;
        pendingAction = ACTION_NONE;

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

        for (int suit = 0; suit < 4; suit++)
            {
            for (int rank = 0; rank < 13; rank++)
            {
                string filename = string(suitNames[suit]) + "_" + string(rankNames[rank]) + ".png";
                cardTextures[suit][rank] = LoadTexture(filename.c_str());
                if (cardTextures[suit][rank].id == 0)
                {
                    Image img = GenImageColor(242, 340, WHITE);
                    cardTextures[suit][rank] = LoadTextureFromImage(img);
                    UnloadImage(img);
                }
            }
        }

        backgroundTexture = LoadTexture("background.jpg");
        if (backgroundTexture.id == 0)
            {
            Image bg = GenImageColor(1, 1, Color{0, 100, 50, 255});
            backgroundTexture = LoadTextureFromImage(bg);
            UnloadImage(bg);
        }

        btnMenu = LoadTexture("menu.png");
        btnHint = LoadTexture("hint.png");
        btnUndo = LoadTexture("undo.png");
        btnMute = LoadTexture("mute.png");
        btnUnmute = LoadTexture("unmute.png");
        restockTexture = LoadTexture("restock.png");

        if (IsAudioDeviceReady())
            {
            if (FileExists("card_slide.wav")) cardSlideSound = LoadSound("card_slide.wav");
            else cardSlideSound = CreateFallbackSound(440.0f, 0.1f);

            if (FileExists("card_flip.wav")) cardFlipSound = LoadSound("card_flip.wav");
            else cardFlipSound = CreateFallbackSound(330.0f, 0.1f);

            if (FileExists("card_place.wav")) cardPlaceSound = LoadSound("card_place.wav");
            else cardPlaceSound = CreateFallbackSound(550.0f, 0.1f);

            if (FileExists("win.wav")) winSound = LoadSound("win.wav");
            else winSound = CreateFallbackSound(659.0f, 0.5f);

            if (FileExists("click.wav")) clickSound = LoadSound("click.wav");
            else clickSound = CreateFallbackSound(220.0f, 0.05f);

            if (FileExists("hint.wav")) hintSound = LoadSound("hint.wav");
            else hintSound = CreateFallbackSound(523.0f, 0.2f);

            if (FileExists("music.mp3"))
                {
                    bgMusic = LoadMusicStream("music.mp3");
            SetMusicVolume(bgMusic, 0.3f);
            }
            else if (FileExists("music.ogg"))
                {
                    bgMusic = LoadMusicStream("music.ogg");
            SetMusicVolume(bgMusic, 0.3f);
            }
            else if (FileExists("music.wav"))
                {
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

    ~Game()
    {
        StopTimer();
        UnloadTexture(cardBack);
        UnloadTexture(backgroundTexture);
        UnloadTexture(btnMenu);
        UnloadTexture(btnHint);
        UnloadTexture(btnUndo);
        UnloadTexture(btnMute);
        UnloadTexture(btnUnmute);
        UnloadTexture(restockTexture);
        for (int s = 0; s < 4; s++)
            {
            for (int r = 0; r < 13; r++)
            {
                UnloadTexture(cardTextures[s][r]);
            }
        }
        if (IsAudioDeviceReady())
        {
            UnloadSound(cardSlideSound);
        UnloadSound(cardFlipSound);
         UnloadSound(cardPlaceSound);
            UnloadSound(winSound);
             UnloadSound(clickSound);
              UnloadSound(hintSound);
            if (bgMusic.ctxData != nullptr) UnloadMusicStream(bgMusic);
            CloseAudioDevice();
        }
        CloseWindow();
    }

    Sound CreateFallbackSound(float frequency, float duration)
    {
        unsigned int sampleRate = 44100;
        unsigned int sampleCount = (unsigned int)(sampleRate * duration);
        short *samples = (short*)RL_MALLOC(sampleCount * sizeof(short));
        for (unsigned int i = 0; i < sampleCount; i++)
            {
            samples[i] = (short)(32000.0f * sinf(2.0f * PI * frequency * ((float)i / sampleRate)));
        }
        Wave wave = LoadWaveFromMemory(".wav", (unsigned char*)samples, sampleCount * sizeof(short));
        if (wave.data == nullptr)
        {
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

    void LoadBestScores()
    {
        const char* files[] = {"easy.txt", "medium.txt", "hard.txt"};
        for (int i = 0; i < 3; i++)
            {
            ifstream file(files[i]);
            if (file.is_open())
            {
                file >> bestScore[i].moves >> bestScore[i].time;
                file.close();
            }
        else
            {
                bestScore[i].moves = 9999; bestScore[i].time = 9999;
            }
        }
    }

    void SaveBestScore()
    {
        bool isNewBest = false;
        if (moveCount < bestScore[difficulty].moves || (moveCount == bestScore[difficulty].moves && elapsedTime < bestScore[difficulty].time))
            {
            bestScore[difficulty].moves = moveCount;
            bestScore[difficulty].time = elapsedTime;
            isNewBest = true;
            const char* files[] = {"easy.txt", "medium.txt", "hard.txt"};
            ofstream file(files[difficulty]);
            if (file.is_open())
            {
                file << moveCount << " " << elapsedTime;
                file.close();
            }
        }
    }

    void WriteStack(ofstream& file, stackk<card>& st)
    {
        int size = st.size();
        file.write((char*)&size, sizeof(size));
        for (int i = 0; i < size; i++)
            {
            card c = st.getAtSafe(i);
            file.write((char*)&c.suit, sizeof(c.suit));
            file.write((char*)&c.rankk, sizeof(c.rankk));
            file.write((char*)&c.faceup, sizeof(c.faceup));
        }
    }

    void ReadStack(ifstream& file, stackk<card>& st)
    {
        st.clear();
        int size = 0;
        file.read((char*)&size, sizeof(size));
        for (int i = 0; i < size; i++)
            {
            card c;
            file.read((char*)&c.suit, sizeof(c.suit));
            file.read((char*)&c.rankk, sizeof(c.rankk));
            file.read((char*)&c.faceup, sizeof(c.faceup));
            c.restoreColor();
            st.push(c);
        }
    }

    void SaveGame()
    {
        ofstream file("savegame.dat", ios::binary);
        if (file.is_open())
            {
            file.write((char*)&difficulty, sizeof(difficulty));
            file.write((char*)&moveCount, sizeof(moveCount));
            file.write((char*)&elapsedTime, sizeof(elapsedTime));

            WriteStack(file, gameDeck.stock);
            WriteStack(file, gameDeck.waste);
            WriteStack(file, gameDeck.tableau1);
        WriteStack(file, gameDeck.tableau2);
            WriteStack(file, gameDeck.tableau3);
            WriteStack(file, gameDeck.tableau4);
            WriteStack(file, gameDeck.tableau5);
            WriteStack(file, gameDeck.tableau6);
            WriteStack(file, gameDeck.tableau7);
            WriteStack(file, gameDeck.foundation1);
            WriteStack(file, gameDeck.foundation2);
            WriteStack(file, gameDeck.foundation3);
            WriteStack(file, gameDeck.foundation4);

            file.close();
            cout << "Game Saved." << endl;
        }
    }

    bool LoadGame()
    {
        ifstream file("savegame.dat", ios::binary);
        if (file.is_open())
            {
            file.read((char*)&difficulty, sizeof(difficulty));
            file.read((char*)&moveCount, sizeof(moveCount));
            file.read((char*)&elapsedTime, sizeof(elapsedTime));

            gameDeck.clearAll();
            ReadStack(file, gameDeck.stock);
            ReadStack(file, gameDeck.waste);
            ReadStack(file, gameDeck.tableau1);
        ReadStack(file, gameDeck.tableau2);
            ReadStack(file, gameDeck.tableau3);
            ReadStack(file, gameDeck.tableau4);
            ReadStack(file, gameDeck.tableau5);
            ReadStack(file, gameDeck.tableau6);
            ReadStack(file, gameDeck.tableau7);
            ReadStack(file, gameDeck.foundation1);
            ReadStack(file, gameDeck.foundation2);
            ReadStack(file, gameDeck.foundation3);
            ReadStack(file, gameDeck.foundation4);

            file.close();

            cardsPerDraw = (difficulty == EASY) ? 1 : (difficulty == MEDIUM) ? 2 : 3;
            maxHints = (difficulty == EASY) ? 5 : (difficulty == MEDIUM) ? 2 : 0;
            hintsRemaining = maxHints;

            gameStarted = true;
            gameWon = false;
            gameLost = false;
            dragging = false;
            undoStack.clear();
            activeHints.clear();
            draggedCards.clear();
            StopTimer();
            StartTimer();
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) PlayMusicStream(bgMusic);
            return true;
        }
        return false;
    }

    void StartTimer()
    {
        timerRunning = true;
        timerStart = chrono::steady_clock::now();
        timerThread = thread([this]()
                             {
            while (timerRunning)
                                {
                this_thread::sleep_for(chrono::milliseconds(100));
                if (timerRunning && gameStarted && !gameWon && !gameLost &&
                    !showingMenu && !showingDifficultySelect && !showingSavePrompt)
                    {
                    auto now = chrono::steady_clock::now();
                    auto elapsed = chrono::duration_cast<chrono::seconds>(now - timerStart);
                    elapsedTime = (int)elapsed.count();
                }
            }
        });
    }

    void StopTimer()
    {
        timerRunning = false;
        if (timerThread.joinable()) timerThread.join();
    }

    void InitializeGame(Difficulty diff, bool reset = true)
    {
        difficulty = diff;
        cardsPerDraw = (diff == EASY) ? 1 : (diff == MEDIUM) ? 2 : 3;
        maxHints = (diff == EASY) ? 5 : (diff == MEDIUM) ? 2 : 0;
        hintsRemaining = maxHints;

        if (reset)
            {
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
        StopTimer(); StartTimer();
        if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) PlayMusicStream(bgMusic);
    }

    void SaveState()
    {
        if (undoStack.size() >= 20) undoStack.erase(undoStack.begin());
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
        if (IsAudioDeviceReady()) PlaySound(clickSound);
    }

    void Undo()
    {
        if (!undoStack.empty())
            {
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
            moveCount++;
            dragging = false; draggedCards.clear();
            if (IsAudioDeviceReady()) PlaySound(cardFlipSound);
        }
    }

    void GenerateHints()
    {
        activeHints.clear();
        int hintsToShow = (difficulty == EASY) ? 3 : (difficulty == MEDIUM) ? 2 : 1;
        int hintCount = 0;
        if (!gameDeck.waste.is_emptyy() && hintCount < hintsToShow) {
            card c = gameDeck.waste.topp();
            for (int i = 0; i < 4; i++)
                {
                stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                if (gameDeck.can_move_to_foundation(*foundation, c))
                {
                    Hint hint; hint.message = "Waste to Foundation"; hint.card = c;
                    activeHints.push_back(hint); hintCount++;
                    if (hintCount >= hintsToShow) return;
                }
            }
        }
        for (int i = 0; i < 7 && hintCount < hintsToShow; i++)
            {
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            if (!tableau->is_emptyy())
            {
                card c = tableau->topp();
                if (c.faceup)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        stackk<card>* foundation = gameDeck.getFoundation(j + 1);
                        if (gameDeck.can_move_to_foundation(*foundation, c))
                        {
                            Hint hint; hint.message = "Tableau " + to_string(i + 1) + " to Foundation"; hint.card = c;
                            activeHints.push_back(hint); hintCount++;
                            if (hintCount >= hintsToShow) return;
                        }
                    }
                }
            }
        }
         if (activeHints.empty())
         {
            Hint hint; hint.message = "Draw from stock or try different moves";
            activeHints.push_back(hint);
        }
    }

    void ShowHint()
    {
        if (hintsRemaining > 0 && hintsEnabled)
            {
            GenerateHints();
            hintsRemaining--;
            hintDisplayTime = 3.0f;
            showingHints = true;
            if (IsAudioDeviceReady()) PlaySound(hintSound);
        }
    }

    void StartDrag(card c, int sourceType, int sourceIndex, Vector2 position)
    {
        if (inputCooldown > 0) return;
        SaveState();
        dragging = true; dragSourceType = sourceType;
        dragSourceIndex = sourceIndex;
        dragStartPos = position;
        dragCurrentPos = position;
        dragOffset = {GetMousePosition().x - position.x, GetMousePosition().y - position.y};
        draggedCards.clear();

        if (sourceType == 0)
            {
            card temp; if (gameDeck.waste.pop(temp)) draggedCards.push_back(temp);
        }
        else if (sourceType >= 1 && sourceType <= 7)
            {
            stackk<card>* tableau = gameDeck.getTableau(sourceType);
            int tableauSize = tableau->size();
            for (int i = sourceIndex; i < tableauSize; i++)
            {
                card currentCard = tableau->getAtSafe(i);
                if (!currentCard.faceup) break;
                if (i > sourceIndex) {
                    card prevCard = draggedCards.back();
                    if (!(currentCard.rankk == prevCard.rankk - 1 && currentCard.color != prevCard.color)) break;
                }
                draggedCards.push_back(currentCard);
            }
        }
        else if (sourceType >= 8)
            {
            stackk<card>* foundation = gameDeck.getFoundation(sourceType - 7);
            card temp; if (foundation->pop(temp)) draggedCards.push_back(temp);
        }
        if (IsAudioDeviceReady()) PlaySound(cardSlideSound);
        inputCooldown = 0.1f;
    }

    void UpdateDrag()
    {
        if (!dragging || draggedCards.empty())
            return;
        Vector2 mousePos = GetMousePosition();
        dragCurrentPos = {mousePos.x - dragOffset.x, mousePos.y - dragOffset.y};
    }

    void EndDrag()
    {
        if (!dragging || draggedCards.empty())
            return;
        bool placed = false;
        Vector2 mousePos = GetMousePosition();
        card topCard = draggedCards[0];

        if (draggedCards.size() == 1)
            {
            for (int i = 0; i < 4; i++)
            {
                Vector2 slotPos = {foundationStartX + i * (CARD_WIDTH + CARD_SPACING), foundationY};
                Rectangle slotRect = {slotPos.x, slotPos.y, CARD_WIDTH, CARD_HEIGHT};
                if (CheckCollisionPointRec(mousePos, slotRect))
                {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (gameDeck.can_move_to_foundation(*foundation, topCard))
                    {
                        if (dragSourceType >= 1 && dragSourceType <= 7)
                        {
                            stackk<card>* sourceTableau = gameDeck.getTableau(dragSourceType);
                            for (size_t j = 0; j < draggedCards.size(); j++)
                            {
                                card temp; sourceTableau->pop(temp);
                            }
                            gameDeck.flip_tableau_top(*sourceTableau);
                        }
                        foundation->push(topCard); placed = true; break;
                    }
                }
            }
        }

        if (!placed)
            {
            for (int i = 0; i < 7; i++)
            {
                stackk<card>* tableau = gameDeck.getTableau(i + 1);
                float y = tableauY;
                if (!tableau->is_emptyy())
                {
                     for(int k=0; k<tableau->size(); k++)
                     {
                         card tc = tableau->getAtSafe(k);
                         float off = tc.faceup ? 35.0f : 15.0f;
                         y += off;
                     }
                }

                Vector2 slotPos = {tableauStartX + i * (CARD_WIDTH + CARD_SPACING), y};
                Rectangle slotRect = {slotPos.x - 30, slotPos.y - 100, CARD_WIDTH + 60, CARD_HEIGHT + 150};

                if (CheckCollisionPointRec(mousePos, slotRect))
                    {
                    if (gameDeck.can_move_to_tableau(*tableau, topCard))
                    {
                        if (dragSourceType >= 1 && dragSourceType <= 7)
                        {
                            stackk<card>* sourceTableau = gameDeck.getTableau(dragSourceType);
                            for (size_t j = 0; j < draggedCards.size(); j++)
                            {
                                card temp; sourceTableau->pop(temp);
                            }
                            gameDeck.flip_tableau_top(*sourceTableau);
                        }
                        for (const auto& cd : draggedCards) tableau->push(cd);
                        placed = true; break;
                    }
                }
            }
        }

        if (placed)
            {
            moveCount++;
            if (IsAudioDeviceReady()) PlaySound(cardPlaceSound);
            CheckAutoComplete();
        }
        else Undo();
        dragging = false; draggedCards.clear();
    }

    void CheckAutoComplete()
    {
        if (gameDeck.check_win())
            {
            gameWon = true; SaveBestScore();
            if (IsAudioDeviceReady()) PlaySound(winSound);
        }
        if (!gameWon && gameDeck.check_loss()) gameLost = true;
    }

    void Update()
    {

        float deltaTime = GetFrameTime();

        if (showingWelcome)
            {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showingWelcome = false; showingDifficultySelect = true;
                if (IsAudioDeviceReady()) PlaySound(clickSound);
            }
            return;
        }

        if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) UpdateMusicStream(bgMusic);
        if (inputCooldown > 0) inputCooldown -= deltaTime;

        if (showingSavePrompt) return;

        if (showingHints)
            {
            hintDisplayTime -= deltaTime;
            if (hintDisplayTime <= 0) showingHints = false;
        }

        if (showingDifficultySelect || showingMenu || gameWon || gameLost)
            {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (showingDifficultySelect) showingDifficultySelect = false;
                else if (showingMenu) showingMenu = false;
            }
            return;
        }

        if (gameDeck.check_win() && !gameWon)
            {
            gameWon = true; SaveBestScore();
            if (IsAudioDeviceReady()) PlaySound(winSound);
            return;
        }
        if (!gameWon && gameDeck.check_loss())
        {
            gameLost = true; return;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
            Vector2 mousePos = GetMousePosition();
            Rectangle stockRect = {stockX, stockY, CARD_WIDTH, CARD_HEIGHT};
            if (CheckCollisionPointRec(mousePos, stockRect))
            {
                if (inputCooldown <= 0)
                {
                    SaveState();
                    if (gameDeck.stock.is_emptyy())
                    {
                        if (!gameDeck.waste.is_emptyy()) gameDeck.recyclewastetostock();
                    }
        else
            {
                        gameDeck.drawtowastee(cardsPerDraw);
                        if (!gameDeck.waste.is_emptyy()) moveCount++;
                    }
                    if (IsAudioDeviceReady()) PlaySound(cardFlipSound);
                    inputCooldown = 0.2f;
                }
                return;
            }
        }

        if (dragging)
            {
            UpdateDrag();
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) EndDrag();
        }
        else
            {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && inputCooldown <= 0)
            {
                Vector2 mousePos = GetMousePosition();
                if (!gameDeck.waste.is_emptyy())
                {
                    Rectangle wasteRect = {wasteX, wasteY, CARD_WIDTH, CARD_HEIGHT};
                    if (CheckCollisionPointRec(mousePos, wasteRect))
                    {
                        card c = gameDeck.waste.topp();
                        StartDrag(c, 0, 0, {wasteX, wasteY}); return;
                    }
                }
                for (int i = 0; i < 4; i++)
                {
                    stackk<card>* foundation = gameDeck.getFoundation(i + 1);
                    if (!foundation->is_emptyy())
                    {
                        float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
                        Rectangle foundationRect = {x, foundationY, CARD_WIDTH, CARD_HEIGHT};
                        if (CheckCollisionPointRec(mousePos, foundationRect))
                        {
                            card c = foundation->topp();
                            StartDrag(c, 8 + i, i, {x, foundationY}); return;
                        }
                    }
                }
                for (int i = 0; i < 7; i++)
                {
                    stackk<card>* tableau = gameDeck.getTableau(i + 1);
                    if (!tableau->is_emptyy())
                    {
                        float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);
                        float yBase = tableauY;
                        vector<float> yPositions;
                        for(int k=0; k<tableau->size(); k++)
                        {
                            yPositions.push_back(yBase);
                            float off = tableau->getAtSafe(k).faceup ? 35.0f : 15.0f;
                            yBase += off;
                        }

                        for (int j = tableau->size() - 1; j >= 0; j--)
                            {
                            card c = tableau->getAtSafe(j);
                            if (c.faceup)
                            {
                                float y = yPositions[j];
                                float height = (j == tableau->size() - 1) ? CARD_HEIGHT : 35.0f;
                                Rectangle cardRect = {x, y, CARD_WIDTH, height};
                                if (CheckCollisionPointRec(mousePos, cardRect))
                                {
                                    StartDrag(c, i + 1, j, {x, y}); return;
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
        if (IsKeyPressed(KEY_S)) { pendingAction = ACTION_JUST_SAVE; showingSavePrompt = true; }
        if (IsKeyPressed(KEY_L)) LoadGame();
        if (IsKeyPressed(KEY_M))
            {
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr)
            {
                if (IsMusicStreamPlaying(bgMusic)) PauseMusicStream(bgMusic);
                else ResumeMusicStream(bgMusic);
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) showingMenu = true;
    }

    void Render()
    {
        BeginDrawing();
        if (backgroundTexture.id != 0)
            {
            Rectangle src = {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height};
            Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
            DrawTexturePro(backgroundTexture, src, dest, {0, 0}, 0.0f, WHITE);
        }
        else ClearBackground({0, 100, 50, 255});

        if (showingWelcome)
            {
            RenderWelcome();
            EndDrawing();
        return;
        }
        if (showingDifficultySelect)
            {
            RenderDifficultySelect();
            EndDrawing();
        return;
        }
        if (showingMenu)
            {
            RenderMenu();
            if (showingSavePrompt) RenderSavePrompt();
            EndDrawing(); return;
        }

        DrawTopBar();
        DrawGameBoard();
        DrawHints();
        if (gameWon || gameLost) RenderGameOver();
        if (showingSavePrompt) RenderSavePrompt();

        EndDrawing();
    }

    void RenderWelcome()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
        int centerX = GetScreenWidth() / 2;
        int centerY = GetScreenHeight() / 2;
        DrawRectangleLinesEx({(float)centerX - 475, (float)centerY - 350, 950, 1000}, 5.0f, GOLD);
        const char* title = "KLONDIKE SOLITAIRE";
        int titleSize = 80;
        DrawText(title, centerX - MeasureText(title, titleSize) / 2, centerY - 280, titleSize, GOLD);
        const char* subTitle = "Data Structure Semester Project";
        int subSize = 40;
        DrawText(subTitle, centerX - MeasureText(subTitle, subSize) / 2, centerY - 180, subSize, SKYBLUE);
        DrawLine(centerX - 300, centerY - 120, centerX + 300, centerY - 120, WHITE);
        DrawText("Developed By:", centerX - MeasureText("Developed By:", 30) / 2, centerY - 60, 30, LIGHTGRAY);
        DrawText("Muhammad Naveed", centerX - MeasureText("Muhammad Naveed", 40) / 2, centerY, 40, WHITE);
        DrawText("Muhammad Shoaib", centerX - MeasureText("Muhammad Shoaib", 40) / 2, centerY + 60, 40, WHITE);
        DrawText("Muhammad Wasif", centerX - MeasureText("Muhammad Naveed", 40) / 2, centerY+ 120, 40, WHITE);
        DrawText("Muhammad Jamal", centerX - MeasureText("Muhammad Shoaib", 40) / 2, centerY + 180, 40, WHITE);
        DrawText("Muhammad Bilal", centerX - MeasureText("Muhammad Naveed", 40) / 2, centerY +240, 40, WHITE);
        DrawText("Muhammad Mubashir", centerX - MeasureText("Muhammad Shoaib", 40) / 2, centerY + 300, 40, WHITE);
        DrawText("Instructor:", centerX - MeasureText("Instructor:", 30) / 2, centerY + 380, 30, LIGHTGRAY);
        DrawText("Abdul Rafay", centerX - MeasureText("Abdul Rafay", 40) / 2, centerY + 440, 40, YELLOW);
        float alpha = (sinf(GetTime() * 3.0f) + 1.0f) / 2.0f;
        Color promptColor = ColorAlpha(WHITE, alpha);
        const char* prompt = "- Press ENTER or CLICK to Start -";
        DrawText(prompt, centerX - MeasureText(prompt, 30) / 2, centerY + 500, 30, promptColor);
    }

    void RenderSavePrompt()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        int w = 600; int h = 300;
        int x = (GetScreenWidth() - w) / 2; int y = (GetScreenHeight() - h) / 2;
        DrawRectangle(x, y, w, h, {40, 40, 40, 255});
        DrawRectangleLinesEx({(float)x, (float)y, (float)w, (float)h}, 3.0f, GOLD);
        const char* msg = (pendingAction == ACTION_JUST_SAVE) ? "Do you want to save game?" : "Do you want to save before leaving?";
        DrawText(msg, x + (w - MeasureText(msg, 30))/2, y + 60, 30, WHITE);
        float btnY = y + 180;
        if (DrawButton(x + 50, btnY, 150, 60, "YES", true))
            {
            SaveGame(); showingSavePrompt = false;
            if (pendingAction == ACTION_QUIT) CloseWindow();
            else if (pendingAction == ACTION_TO_MENU)
            {
                showingDifficultySelect = true;
        showingMenu = false;
        }
        }
        if (DrawButton(x + 225, btnY, 150, 60, "NO", true))
            {
            showingSavePrompt = false;
            if (pendingAction == ACTION_QUIT) CloseWindow();
            else if (pendingAction == ACTION_TO_MENU)
            {
                showingDifficultySelect = true;
        showingMenu = false;
        }
        }
        if (DrawButton(x + 400, btnY, 150, 60, "CANCEL", true))
            {
            showingSavePrompt = false; pendingAction = ACTION_NONE;
        }
    }

    bool DrawButton(float x, float y, float w, float h, const char* text, bool isSmallButton = false)
    {
        Rectangle rect = {x, y, w, h};
        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
        float scale = hovered ? 1.08f : 1.0f;
        float scaledW = w * scale; float scaledH = h * scale;
        float offsetX = (w - scaledW) / 2; float offsetY = (h - scaledH) / 2;
        Rectangle scaledRect = {x + offsetX, y + offsetY, scaledW, scaledH};
        if (!isSmallButton) DrawRectangleRounded({scaledRect.x + 3, scaledRect.y + 3, scaledRect.width, scaledRect.height}, 0.2f, 10, ColorAlpha(BLACK, 0.4f));
        Color topColor = hovered ? Color{80, 150, 200, 255} : Color{60, 120, 180, 255};
        Color bottomColor = hovered ? Color{40, 100, 150, 255} : Color{30, 80, 120, 255};
        for (int i = 0; i < (int)scaledRect.height; i++)
            {
            float lerpFactor = (float)i / scaledRect.height;
            Color lineColor = {
                (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * lerpFactor),
                (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * lerpFactor),
                (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * lerpFactor), 255 };
            DrawLine((int)scaledRect.x, (int)(scaledRect.y + i), (int)(scaledRect.x + scaledRect.width), (int)(scaledRect.y + i), lineColor);
        }
        Color borderColor = hovered ? Color{255, 200, 0, 255} : Color{200, 200, 200, 255};
        DrawRectangleRounded(scaledRect, 0.2f, 10, ColorAlpha(borderColor, 0.0f));
        DrawRectangleRoundedLines(scaledRect, 0.2f, 10, borderColor);
        if (hovered) DrawRectangleRoundedLines({scaledRect.x - 2, scaledRect.y - 2, scaledRect.width + 4, scaledRect.height + 4}, 0.2f, 10, ColorAlpha(GOLD, 0.5f));
        int fontSize = isSmallButton ? 22 : 28;
        if (strlen(text) > 20) fontSize = 24; if (strlen(text) > 30) fontSize = 20;
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, (int)(x + (w - textWidth)/2 + 1), (int)(y + (h - fontSize)/2 + 1), fontSize, ColorAlpha(BLACK, 0.5f));
        DrawText(text, (int)(x + (w - textWidth)/2), (int)(y + (h - fontSize)/2), fontSize, WHITE);
        return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    bool DrawImageButton(float x, float y, float w, float h, Texture2D buttonImg)
    {
        Rectangle rect = {x, y, w, h};
        bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
        float scale = hovered ? 1.08f : 1.0f;
        float scaledW = w * scale; float scaledH = h * scale;
        float offsetX = (w - scaledW) / 2; float offsetY = (h - scaledH) / 2;
        Rectangle scaledRect = {x + offsetX, y + offsetY, scaledW, scaledH};
        DrawRectangleRounded({scaledRect.x + 3, scaledRect.y + 3, scaledRect.width, scaledRect.height}, 0.2f, 10, ColorAlpha(BLACK, 0.4f));
        Color topColor = hovered ? Color{80, 150, 200, 255} : Color{60, 120, 180, 255};
        Color bottomColor = hovered ? Color{40, 100, 150, 255} : Color{30, 80, 120, 255};
        for (int i = 0; i < (int)scaledRect.height; i++)
            {
            float lerpFactor = (float)i / scaledRect.height;
            Color lineColor = {
                (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * lerpFactor),
                (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * lerpFactor),
                (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * lerpFactor), 255 };
            DrawLine((int)scaledRect.x, (int)(scaledRect.y + i), (int)(scaledRect.x + scaledRect.width), (int)(scaledRect.y + i), lineColor);
        }
        Color borderColor = hovered ? Color{255, 200, 0, 255} : Color{200, 200, 200, 255};
        DrawRectangleRoundedLines(scaledRect, 0.2f, 10, borderColor);
        if (hovered) DrawRectangleRoundedLines({scaledRect.x - 2, scaledRect.y - 2, scaledRect.width + 4, scaledRect.height + 4}, 0.2f, 10, ColorAlpha(GOLD, 0.5f));
        if (buttonImg.id != 0) {
            Rectangle imgRect = {scaledRect.x + 10, scaledRect.y + 10, scaledRect.width - 20, scaledRect.height - 20};
            DrawTexturePro(buttonImg, {0, 0, (float)buttonImg.width, (float)buttonImg.height}, imgRect, {0, 0}, 0, WHITE);
        }
        return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void RenderGameOver()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        if (gameWon)
            {
            float pulse = sinf(GetTime() * 3) * 0.2f + 0.8f;
            Color victoryColor = ColorAlpha(GOLD, pulse);
            DrawText("VICTORY!", GetScreenWidth()/2 - 200, 250, 100, victoryColor);
            DrawText(TextFormat("Moves: %d", moveCount), GetScreenWidth()/2 - 180, 380, 50, WHITE);
            DrawText(TextFormat("Time: %02d:%02d", elapsedTime/60, elapsedTime%60), GetScreenWidth()/2 - 180, 450, 50, WHITE);
            if (moveCount <= bestScore[difficulty].moves && elapsedTime <= bestScore[difficulty].time) DrawText("NEW HIGH SCORE!", GetScreenWidth()/2 - 220, 520, 45, GOLD);
            else DrawText(TextFormat("Best: %d moves in %d:%02d", bestScore[difficulty].moves, bestScore[difficulty].time / 60, bestScore[difficulty].time % 60), GetScreenWidth()/2 - 220, 520, 40, SKYBLUE);
        }
        else
            {
            DrawText("GAME OVER", GetScreenWidth()/2 - 250, 350, 100, RED);
            DrawText("No moves available", GetScreenWidth()/2 - 200, 480, 40, WHITE);
        }
        if (DrawButton(GetScreenWidth()/2 - 150, 650, 300, 80, "New Game"))
        {
            showingDifficultySelect = true;
        gameWon = false;
        gameLost = false;
        }
    }

    void RenderMenu()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        float centerX = GetScreenWidth()/2.0f; float centerY = GetScreenHeight()/2.0f;
        DrawRectangle(centerX - 250, centerY - 300, 500, 600, {40, 40, 40, 255});
        Rectangle menuRect = {centerX - 250, centerY - 300, 500, 600};
        DrawRectangleRoundedLines(menuRect, 0.1f, 10, GOLD);
        DrawText("GAME MENU", centerX - 120, centerY - 250, 40, GOLD);
        float buttonY = centerY - 150; float buttonHeight = 60; float buttonSpacing = 70;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "CONTINUE GAME")) showingMenu = false;
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "NEW GAME"))
            {
                pendingAction = ACTION_TO_MENU;
        showingSavePrompt = true;
        }
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "SAVE GAME"))
            {
                pendingAction = ACTION_JUST_SAVE;
        showingSavePrompt = true;
        }
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "LOAD GAME"))
            {
                if (LoadGame()) showingMenu = false;
        }
        buttonY += buttonSpacing;
        if (DrawButton(centerX - 200, buttonY, 400, buttonHeight, "QUIT TO DESKTOP"))
            {
                pendingAction = ACTION_QUIT;
        showingSavePrompt = true;
        }
        DrawText("Press ESC to close menu", centerX - 150, centerY + 220, 25, WHITE);
    }

    void RenderDifficultySelect()
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        float pulse = sinf(GetTime() * 2) * 0.1f + 0.9f;
        Color titleColor = ColorAlpha(GOLD, pulse);
        DrawText("Klondike Solitaire", GetScreenWidth()/2 - 250, 150, 60, WHITE);
        DrawText("Stack-Based Edition", GetScreenWidth()/2 - 210, 220, 40, titleColor);
        DrawText("Select Difficulty:", GetScreenWidth()/2 - 150, 320, 40, WHITE);
        float buttonY = 400; float buttonHeight = 80; float buttonSpacing = 100;
        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "EASY"))
            {
                InitializeGame(EASY);
        showingDifficultySelect = false;
        }
        buttonY += buttonSpacing;
        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "MEDIUM"))
            {
                InitializeGame(MEDIUM); showingDifficultySelect = false;
        }
        buttonY += buttonSpacing;
        if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "HARD"))
            {
                InitializeGame(HARD); showingDifficultySelect = false;
        }
        if (FileExists("savegame.dat"))
            {
            buttonY += buttonSpacing;
            if (DrawButton(GetScreenWidth()/2 - 250, buttonY, 500, buttonHeight, "LOAD SAVED GAME"))
            {
                if (LoadGame()) showingDifficultySelect = false;
        }
        }
    }

    void DrawTopBar()
    {
        for (int i = 0; i < (int)TOP_BAR_HEIGHT; i++)
            {
            float lerpFactor = (float)i / TOP_BAR_HEIGHT;
            Color lineColor = {
                (unsigned char)(35 + (45 - 35) * lerpFactor),
                (unsigned char)(35 + (55 - 35) * lerpFactor),
                (unsigned char)(45 + (70 - 45) * lerpFactor), 255 };
            DrawLine(0, i, GetScreenWidth(), i, lineColor);
        }
        DrawLine(0, (int)TOP_BAR_HEIGHT - 2, GetScreenWidth(), (int)TOP_BAR_HEIGHT - 2, Color{0, 150, 200, 255});
        DrawLine(0, (int)TOP_BAR_HEIGHT - 1, GetScreenWidth(), (int)TOP_BAR_HEIGHT - 1, Color{0, 180, 255, 200});
        if (DrawImageButton(15, 15, 90, 90, btnMenu)) showingMenu = true;
        Rectangle statsPanel = {120, 20, 900, 80};
        DrawRectangleRounded(statsPanel, 0.1f, 10, ColorAlpha(Color{0, 0, 0, 200}, 0.4f));
        DrawRectangleRoundedLines(statsPanel, 0.1f, 10, ColorAlpha(Color{0, 180, 255, 255}, 0.6f));
        DrawText("Moves:", 140, 32, 28, Color{100, 200, 255, 255});
        DrawText(TextFormat("%d", moveCount), 280, 32, 32, GOLD);
        DrawText("Time:", 430, 32, 28, Color{100, 200, 255, 255});
        int mins = elapsedTime / 60; int secs = elapsedTime % 60;
        DrawText(TextFormat("%02d:%02d", mins, secs), 540, 32, 32, Color{100, 255, 150, 255});
        DrawText("Best:", 730, 32, 28, Color{100, 200, 255, 255});
        if (bestScore[difficulty].moves < 9999) DrawText(TextFormat("%d/%d:%02d", bestScore[difficulty].moves, bestScore[difficulty].time / 60, bestScore[difficulty].time % 60), 820, 32, 28, GOLD);
        else DrawText("--", 820, 32, 28, GRAY);
        Rectangle controlsPanel = {1050, 20, 1500, 80};
        DrawRectangleRounded(controlsPanel, 0.1f, 10, ColorAlpha(Color{0, 0, 0, 200}, 0.4f));
        DrawRectangleRoundedLines(controlsPanel, 0.1f, 10, ColorAlpha(Color{200, 100, 0, 255}, 0.6f));
        string diffText = (difficulty == EASY) ? "EASY" : (difficulty == MEDIUM) ? "MEDIUM" : "HARD";
        Color diffColor = (difficulty == EASY) ? Color{100, 255, 100, 255} : (difficulty == MEDIUM) ? Color{255, 200, 100, 255} : Color{255, 100, 100, 255};
        DrawText("Difficulty:", 1070, 32, 26, Color{200, 150, 100, 255});
        DrawText(diffText.c_str(), 1280, 32, 30, diffColor);
        Color hintColor = (hintsRemaining > 0) ? Color{100, 200, 255, 255} : Color{150, 150, 150, 255};
        DrawText(TextFormat("Hints: %d/%d", hintsRemaining, maxHints), 1560, 32, 26, hintColor);
        float buttonX = 1850; float buttonW = 85; float buttonH = 85;
        if (DrawImageButton(buttonX, 15, buttonW, buttonH, btnHint)) ShowHint();
        buttonX += buttonW + 12;
        if (DrawImageButton(buttonX, 15, buttonW, buttonH, btnUndo)) Undo();
        buttonX += buttonW + 12;
        Texture2D muteImg = musicMuted ? btnUnmute : btnMute;
        if (DrawImageButton(buttonX, 15, buttonW, buttonH, muteImg)) {
            musicMuted = !musicMuted;
            if (IsAudioDeviceReady() && bgMusic.ctxData != nullptr) {
                if (musicMuted) PauseMusicStream(bgMusic); else ResumeMusicStream(bgMusic);
            }
        }
    }

    void DrawGameBoard()
    {
        if (!gameDeck.stock.is_emptyy())
            {
            Rectangle dest = {stockX, stockY, CARD_WIDTH, CARD_HEIGHT};
            DrawTexturePro(cardBack, {0, 0, (float)cardBack.width, (float)cardBack.height}, dest, {0, 0}, 0, WHITE);
            DrawText(TextFormat("%d", gameDeck.stock.size()), stockX + 10, stockY + 10, 25, YELLOW);
        }
        else
            {
            DrawEmptySlot(stockX, stockY);
            if (restockTexture.id != 0)
            {
                 float iconSize = 85.0f;
                 float ox = stockX + (CARD_WIDTH - iconSize)/2;
                 float oy = stockY + (CARD_HEIGHT - iconSize)/2;
                 Rectangle source = {0, 0, (float)restockTexture.width, (float)restockTexture.height};
                 Rectangle dest = {ox, oy, iconSize, iconSize};
                 DrawTexturePro(restockTexture, source, dest, {0, 0}, 0.0f, WHITE);
            }
        else
            {
                 DrawText("R", stockX + CARD_WIDTH/2 - 15, stockY + CARD_HEIGHT/2 - 20, 40, GRAY);
            }
        }
        if (!gameDeck.waste.is_emptyy()) DrawCard(gameDeck.waste.topp(), wasteX, wasteY);
        else DrawEmptySlot(wasteX, wasteY);
        for (int i = 0; i < 4; i++)
        {
            float x = foundationStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* foundation = gameDeck.getFoundation(i + 1);
            if (!foundation->is_emptyy()) DrawCard(foundation->topp(), x, foundationY);
            else DrawEmptySlot(x, foundationY);
        }
        for (int i = 0; i < 7; i++)
        {
            float x = tableauStartX + i * (CARD_WIDTH + CARD_SPACING);
            stackk<card>* tableau = gameDeck.getTableau(i + 1);
            DrawText(TextFormat("T%d", i + 1), (int)x + 5, (int)tableauY - 40, 25, WHITE);
            if (tableau->is_emptyy()) DrawEmptySlot(x, tableauY);
            else
            {
                float currentY = tableauY;
                for (int j = 0; j < tableau->size(); j++)
                {
                    card c = tableau->getAtSafe(j);
                    if (c.isValid()) {
                        DrawCard(c, x, currentY);
                        float offset = c.faceup ? 35.0f : 15.0f;
                        currentY += offset;
                    }
                }
            }
        }
        if (dragging && !draggedCards.empty())
        {
            for (size_t i = 0; i < draggedCards.size(); i++)
            {
                float yOffset = i * 35.0f;
                DrawCard(draggedCards[i], dragCurrentPos.x, dragCurrentPos.y + yOffset, true);
            }
            Rectangle dragBounds = { dragCurrentPos.x - 5, dragCurrentPos.y - 5, CARD_WIDTH + 10, CARD_HEIGHT + 10 + (draggedCards.size() - 1) * 35.0f };
            DrawRectangleRoundedLines(dragBounds, 0.05f, 10, YELLOW);
        }
    }

    void DrawCard(card c, float x, float y, bool isDragging = false)
    {
        if (!c.isValid()) return;
        Rectangle dest = {x, y, CARD_WIDTH, CARD_HEIGHT};
        if (!c.faceup) DrawTexturePro(cardBack, {0, 0, (float)cardBack.width, (float)cardBack.height}, dest, {0, 0}, 0, WHITE);
        else
            {
            if (c.suit >= 0 && c.suit < 4 && c.rankk >= 1 && c.rankk <= 13)
                DrawTexturePro(cardTextures[c.suit][c.rankk - 1], {0, 0, (float)cardTextures[c.suit][c.rankk - 1].width, (float)cardTextures[c.suit][c.rankk - 1].height}, dest, {0, 0}, 0, WHITE);
        }
        if (isDragging) DrawRectangleRoundedLines(dest, 0.05f, 10, YELLOW);
    }

    void DrawEmptySlot(float x, float y)
    {
        Rectangle rect = {x, y, CARD_WIDTH, CARD_HEIGHT};
        DrawRectangleRounded(rect, 0.1f, 10, {0, 0, 0, 80});
        DrawRectangleRoundedLines(rect, 0.1f, 10, Fade(WHITE, 0.3f));
    }

    void DrawHints()
    {
        if (showingHints && !activeHints.empty())
            {
            float alpha = min(hintDisplayTime, 1.0f) * 0.9f;
            float hintBoxY = 250;
            float hintBoxHeight = 100 + activeHints.size() * 50;
            DrawRectangle(GetScreenWidth()/2 - 400, hintBoxY, 800, hintBoxHeight, Fade(BLACK, alpha));
            Rectangle hintRect = {GetScreenWidth()/2 - 400.0f, (float)hintBoxY, 800.0f, (float)hintBoxHeight};
            DrawRectangleRoundedLines(hintRect, 0.1f, 10, Fade(GOLD, alpha));
            DrawText("HINTS", GetScreenWidth()/2 - 60, hintBoxY + 30, 40, Fade(GOLD, alpha));
            for (size_t i = 0; i < activeHints.size(); i++)
            {
                DrawText(TextFormat("%d. %s", (int)i + 1, activeHints[i].message.c_str()), GetScreenWidth()/2 - 370, hintBoxY + 90 + i * 50, 28, Fade(WHITE, alpha));
            }
        }
    }

    void Run()
    {
        while (!WindowShouldClose())
            {
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
