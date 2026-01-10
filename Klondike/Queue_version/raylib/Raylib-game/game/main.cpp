#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <filesystem>
#include "raylib.h"

using namespace std;
namespace fs = filesystem;

// Particle System for Celebrations
struct Particle {
    Vector2 pos;
    Vector2 vel;
    Color color;
    float life;
    float size;
};

// Animation State
struct CardAnimation {
    bool active;
    Vector2 start;
    Vector2 end;
    float progress;
    int cardRank;
    char cardSuit;
    float duration;
};

// Game States
enum GameScreen {
    MENU,
    PLAYING,
    HELP,
    STATS,
    SETTINGS
};

// Button Structure
struct Button {
    Rectangle rect;
    string text;
    Color color;
    Color hoverColor;
    bool hovered;
};

// Queue Template
template <class T>
class Queue {
private:
    T* arr;
    int frontt, rear, cap, sizee;
    
    // Helper function for recursive reverse
    void reverseRecursive() {
        if (sizee <= 1) return;
        
        // Remove front element
        T frontElement = dequeue();
        
        // Reverse the remaining queue
        reverseRecursive();
        
        // Add the front element to the back
        enqueue(frontElement);
    }
    
public:
    Queue(int s = 52) : cap(s), frontt(0), rear(-1), sizee(0) {
        arr = new T[cap];
    }
    
    ~Queue() { delete[] arr; }
    
    Queue(const Queue& o) : cap(o.cap), frontt(o.frontt), rear(o.rear), sizee(o.sizee) {
        arr = new T[cap];
        for (int i = 0; i < cap; i++) arr[i] = o.arr[i];
    }
    
    Queue& operator=(const Queue& o) {
        if (this != &o) {
            delete[] arr;
            cap = o.cap; frontt = o.frontt; rear = o.rear; sizee = o.sizee;
            arr = new T[cap];
            for (int i = 0; i < cap; i++) arr[i] = o.arr[i];
        }
        return *this;
    }
    
    void enqueue(T v) {
        if (sizee >= cap) return;
        rear = (rear + 1) % cap;
        arr[rear] = v;
        sizee++;
    }
    
    T dequeue() {
        if (sizee <= 0) return T();
        T v = arr[frontt];
        frontt = (frontt + 1) % cap;
        sizee--;
        return v;
    }
    
    T grear() { return sizee > 0 ? arr[rear] : T(); }
    bool emptyy() { return sizee == 0; }
    int gsize() { return sizee; }
    void clearr() { frontt = 0; rear = -1; sizee = 0; }
    T valueat(int i) {
        if (i < 0 || i >= sizee) return T();
        return arr[(frontt + i) % cap];
    }
    
    // Public reverse function
    void reverse() {
        reverseRecursive();
    }
    
    // Helper function to get a copy without removing
    T peekFront() {
        if (sizee <= 0) return T();
        return arr[frontt];
    }
    
    // Helper function to peek at rear without removing
    T peekRear() {
        if (sizee <= 0) return T();
        return arr[rear];
    }
};

// Card Class
class CardItem {
public:
    int rank_no;
    char suit;
    bool is_faceup;
    
    CardItem() : rank_no(0), suit(' '), is_faceup(false) {}
    CardItem(int r, char s, bool f) : rank_no(r), suit(s), is_faceup(f) {}
    
    bool check_red() { return suit == 'H' || suit == 'D'; }
    void flip_card() { is_faceup = !is_faceup; }
    bool king() { return rank_no == 13; }
    bool ace() { return rank_no == 1; }
    
    // Get suit symbol as string
    string getSuitSymbol() {
        switch(suit) {
            case 'H': return "\u2665"; // Hearts symbol
            case 'D': return "\u2666"; // Diamonds symbol
            case 'S': return "\u2660"; // Spades symbol
            case 'C': return "\u2663"; // Clubs symbol
            default: return "?";
        }
    }
    
    // Get suit name
    string getSuitName() {
        switch(suit) {
            case 'H': return "Hearts";
            case 'D': return "Diamonds";
            case 'S': return "Spades";
            case 'C': return "Clubs";
            default: return "Unknown";
        }
    }
    
    // Get rank symbol
    string getRankSymbol() {
        switch(rank_no) {
            case 1: return "A";
            case 11: return "J";
            case 12: return "Q";
            case 13: return "K";
            default: return to_string(rank_no);
        }
    }
    
    // Get rank name
    string getRankName() {
        switch(rank_no) {
            case 1: return "Ace";
            case 2: return "Two";
            case 3: return "Three";
            case 4: return "Four";
            case 5: return "Five";
            case 6: return "Six";
            case 7: return "Seven";
            case 8: return "Eight";
            case 9: return "Nine";
            case 10: return "Ten";
            case 11: return "Jack";
            case 12: return "Queen";
            case 13: return "King";
            default: return "Unknown";
        }
    }
    
    // Get image filename for this card
    string getImageFilename() {
        string rank;
        switch(rank_no) {
            case 1: rank = "ace"; break;
            case 11: rank = "jack"; break;
            case 12: rank = "queen"; break;
            case 13: rank = "king"; break;
            default: rank = to_string(rank_no); break;
        }
        
        string suitName;
        switch(suit) {
            case 'H': suitName = "hearts"; break;
            case 'D': suitName = "diamonds"; break;
            case 'S': suitName = "spades"; break;
            case 'C': suitName = "clubs"; break;
            default: suitName = "unknown"; break;
        }
        
        return rank + "_of_" + suitName + ".png";
    }
};

// Deck of Cards
class DeckOfCards {
public:
    Queue<CardItem> all_card;
    
    DeckOfCards() : all_card(52) {}
    
    void shuffle_cards(Queue<CardItem>& cards) {
        int s = cards.gsize();
        CardItem arr[52];
        for (int i = 0; i < s; i++) arr[i] = cards.dequeue();
        
        for (int i = s - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            CardItem temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        
        for (int i = 0; i < s; i++) cards.enqueue(arr[i]);
    }
    
    void full_deck() {
        Queue<CardItem> temp(52);
        char suits[] = {'H', 'D', 'S', 'C'};
        
        for (int s = 0; s < 4; s++) {
            for (int r = 1; r <= 13; r++) {
                temp.enqueue(CardItem(r, suits[s], false));
            }
        }
        
        srand(time(0));
        for (int i = 0; i < 3; i++) shuffle_cards(temp);
        
        all_card.clearr();
        while (!temp.emptyy()) all_card.enqueue(temp.dequeue());
    }
    
    CardItem draw_card() {
        return all_card.emptyy() ? CardItem() : all_card.dequeue();
    }
    
    bool check_empty() { return all_card.emptyy(); }
};

// Game Rules
class GameRules {
public:
    bool tab_move(CardItem from, CardItem to) {
        if (to.rank_no == 0) return from.king(); // Empty pile accepts only kings
        return from.check_red() != to.check_red() && from.rank_no == to.rank_no - 1;
    }
    
    bool foundation_move(CardItem from, CardItem to) {
        return from.suit == to.suit && from.rank_no == to.rank_no + 1;
    }
    
    bool foundation_place(Queue<CardItem> pile, CardItem card) {
        if (pile.emptyy()) return card.ace();
        if (pile.grear().rank_no == 0) return false; // Should not happen but safety check
        return foundation_move(card, pile.grear());
    }
    
    bool check_win(Queue<CardItem> piles[4]) {
        for (int i = 0; i < 4; i++) {
            if (piles[i].gsize() != 13) return false;
        }
        return true;
    }
    
    // Get suit color for rules explanation
    string getSuitColor(char suit) {
        if (suit == 'H' || suit == 'D') return "Red";
        return "Black";
    }
};

// Tableau Column
class TableauColumn {
public:
    Queue<CardItem> card_col;
    
    TableauColumn() : card_col(52) {}
    
    void add_card(CardItem c) { card_col.enqueue(c); }
    
    CardItem rem_tcard() {
        if (card_col.emptyy()) return CardItem();
        Queue<CardItem> temp(52);
        CardItem last;
        while (!card_col.emptyy()) {
            last = card_col.dequeue();
            if (!card_col.emptyy()) temp.enqueue(last);
        }
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
        return last;
    }
    
    CardItem get_tcard() {
        return card_col.emptyy() ? CardItem() : card_col.peekRear();
    }
    
    bool check_emp() { return card_col.emptyy(); }
    int card_count() { return card_col.gsize(); }
    CardItem card_at(int i) { return card_col.valueat(i); }
    
    Queue<CardItem> get_cards_from(int idx) {
        Queue<CardItem> result(52);
        for (int i = idx; i < card_col.gsize(); i++) {
            result.enqueue(card_col.valueat(i));
        }
        return result;
    }
    
    void remove_from(int idx) {
        Queue<CardItem> temp(52);
        for (int i = 0; i < idx; i++) temp.enqueue(card_col.valueat(i));
        card_col.clearr();
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
    }
    
    void flip_top() {
        if (card_col.emptyy()) return;
        Queue<CardItem> temp(52);
        CardItem last;
        while (!card_col.emptyy()) {
            last = card_col.dequeue();
            if (!card_col.emptyy()) temp.enqueue(last);
        }
        last.flip_card();
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
        card_col.enqueue(last);
    }
};

// Game Snapshot for Undo (renamed from GameState)
struct GameSnapshot {
    Queue<CardItem> stock, waste, found[4], tabs[7];
    int score, moves;
    
    GameSnapshot() : stock(24), waste(24) {
        for (int i = 0; i < 4; i++) found[i] = Queue<CardItem>(13);
        for (int i = 0; i < 7; i++) tabs[i] = Queue<CardItem>(52);
        score = moves = 0;
    }
};

// Statistics
struct GameStatistics {
    int gamesPlayed;
    int gamesWon;
    int highestScore;
    int totalMoves;
    int totalScore;
    
    GameStatistics() : gamesPlayed(0), gamesWon(0), highestScore(0), totalMoves(0), totalScore(0) {}
    
    void addGame(int score, int moves, bool won) {
        gamesPlayed++;
        if (won) gamesWon++;
        if (score > highestScore) highestScore = score;
        totalMoves += moves;
        totalScore += score;
    }
};

// Main Game Class
class SolitaireGame {
public:
    DeckOfCards deck;
    Queue<CardItem> stock, waste, found[4];
    TableauColumn tabs[7];
    GameRules rules;
    int score, moves, cw, ch;
    bool won, dragging, paused;
    
    // Graphics
    Texture2D cardTextures[53];
    Texture2D backTexture;
    Font gameFont;
    bool cardsLoaded;
    
    // Game State
    GameScreen currentScreen;
    vector<GameSnapshot> undos;
    
    // Drag state
    int dragType, dragIdx, dragCard;
    Vector2 dragOff;
    vector<CardItem> dragCards;
    
    // Animation
    vector<CardAnimation> anims;
    vector<Particle> particles;
    float celebTime;
    
    // UI
    double lastClick;
    int lastCol;
    Rectangle stockRect, wasteRect, foundRects[4], tabRects[7];
    bool showHelp;
    float hoverGlow;
    int hoverCol;
    
    // Menu
    vector<Button> menuButtons;
    vector<Button> helpButtons;
    vector<Button> statsButtons;
    vector<Button> settingsButtons;
    
    // Statistics
    GameStatistics statistics;
    
    SolitaireGame() : stock(24), waste(24) {
        for (int i = 0; i < 4; i++) found[i] = Queue<CardItem>(13);
        score = moves = 0;
        won = dragging = paused = showHelp = false;
        cw = 100; ch = 140; // Card width and height
        lastClick = 0; lastCol = hoverCol = -1;
        celebTime = 0;
        hoverGlow = 0;
        dragType = dragIdx = dragCard = 0;
        dragOff = {0, 0};
        cardsLoaded = false;
        currentScreen = MENU;
        
        // Initialize menu
        initMenu();
    }
    
    void initMenu() {
        int centerX = GetScreenWidth() / 2;
        int buttonWidth = 300;
        int buttonHeight = 60;
        int startY = 300;
        int buttonSpacing = 20;
        
        // Clear existing buttons
        menuButtons.clear();
        helpButtons.clear();
        statsButtons.clear();
        settingsButtons.clear();
        
        // Main Menu Buttons
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)startY, (float)buttonWidth, (float)buttonHeight}, 
                              "NEW GAME", Color{100, 149, 237, 255}, Color{65, 105, 225, 255}, false});
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)(startY + buttonHeight + buttonSpacing), (float)buttonWidth, (float)buttonHeight}, 
                              "LOAD GAME", Color{60, 179, 113, 255}, Color{46, 139, 87, 255}, false});
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)(startY + 2*(buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight}, 
                              "STATISTICS", Color{255, 140, 0, 255}, Color{255, 165, 0, 255}, false});
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)(startY + 3*(buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight}, 
                              "SETTINGS", Color{147, 112, 219, 255}, Color{138, 43, 226, 255}, false});
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)(startY + 4*(buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight}, 
                              "RULES & HELP", Color{220, 20, 60, 255}, Color{178, 34, 34, 255}, false});
        menuButtons.push_back({Rectangle{(float)centerX - buttonWidth/2, (float)(startY + 5*(buttonHeight + buttonSpacing)), (float)buttonWidth, (float)buttonHeight}, 
                              "EXIT", Color{105, 105, 105, 255}, Color{169, 169, 169, 255}, false});
        
        // Help Screen Back Button
        helpButtons.push_back({Rectangle{50, (float)GetScreenHeight() - 100, 200, 50}, 
                              "BACK TO MENU", Color{105, 105, 105, 255}, Color{169, 169, 169, 255}, false});
        
        // Statistics Screen Back Button
        statsButtons.push_back({Rectangle{50, (float)GetScreenHeight() - 100, 200, 50}, 
                              "BACK TO MENU", Color{105, 105, 105, 255}, Color{169, 169, 169, 255}, false});
        
        // Settings Screen Back Button
        settingsButtons.push_back({Rectangle{50, (float)GetScreenHeight() - 100, 200, 50}, 
                                 "BACK TO MENU", Color{105, 105, 105, 255}, Color{169, 169, 169, 255}, false});
    }
    
    void loadTextures() {
        // Load font
        gameFont = GetFontDefault();
        
        // Load card back texture
        loadCardBackTexture();
        
        // Try to load all 52 card faces from cards folder
        bool allCardsLoaded = true;
        int idx = 0;
        
        // Map suits to folder names
        vector<pair<char, string>> suits = {
            {'H', "hearts"},
            {'D', "diamonds"},
            {'S', "spades"},
            {'C', "clubs"}
        };
        
        // Map ranks to file names
        vector<pair<int, string>> ranks = {
            {1, "ace"},
            {2, "2"},
            {3, "3"},
            {4, "4"},
            {5, "5"},
            {6, "6"},
            {7, "7"},
            {8, "8"},
            {9, "9"},
            {10, "10"},
            {11, "jack"},
            {12, "queen"},
            {13, "king"}
        };
        
        // Try to load cards from "cards" folder
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                string filename = "cards/" + rank.second + "_of_" + suit.second + ".png";
                
                if (fs::exists(filename)) {
                    Image img = LoadImage(filename.c_str());
                    if (img.data) {
                        // Resize to match current card dimensions
                        ImageResize(&img, cw, ch);
                        cardTextures[idx] = LoadTextureFromImage(img);
                        UnloadImage(img);
                    } else {
                        allCardsLoaded = false;
                        createFallbackCardTexture(idx, rank.first, suit.first);
                    }
                } else {
                    allCardsLoaded = false;
                    createFallbackCardTexture(idx, rank.first, suit.first);
                }
                idx++;
            }
        }
        
        cardsLoaded = allCardsLoaded;
        
        if (!cardsLoaded) {
            cout << "Note: Some card images not found. Using fallback textures." << endl;
        } else {
            cout << "Successfully loaded all card images from cards folder!" << endl;
        }
    }
    
    void loadCardBackTexture() {
        // First try to load card back from cards folder
        string backPath = "cards/card_back.png";
        
        if (fs::exists(backPath)) {
            Image img = LoadImage(backPath.c_str());
            if (img.data) {
                // Resize to match the current card dimensions
                ImageResize(&img, cw, ch);
                backTexture = LoadTextureFromImage(img);
                UnloadImage(img);
                cout << "Loaded card back texture from: " << backPath << endl;
            } else {
                cout << "Failed to load card back image. Creating fallback." << endl;
                createFallbackBackTexture();
            }
        } else {
            cout << "Card back image not found at: " << backPath << ". Creating fallback." << endl;
            createFallbackBackTexture();
        }
    }
    
    void createFallbackBackTexture() {
        // Create a simple fallback card back texture
        Image img = GenImageColor(cw, ch, Color{139, 69, 19, 255});
        
        // Add border
        Rectangle border = {0, 0, (float)cw, (float)ch};
        ImageDrawRectangleLines(&img, border, 2, Color{120, 40, 0, 255});
        
        // Add inner border
        Rectangle innerBorder = {5, 5, (float)(cw - 10), (float)(ch - 10)};
        ImageDrawRectangleLines(&img, innerBorder, 1, Color{120, 40, 0, 255});
        
        // Add simple pattern
        for (int i = 10; i < cw - 10; i += 15) {
            for (int j = 10; j < ch - 10; j += 15) {
                ImageDrawCircle(&img, i, j, 3, Color{120, 40, 0, 100});
            }
        }
        
        backTexture = LoadTextureFromImage(img);
        UnloadImage(img);
    }
    
    void createFallbackCardTexture(int idx, int rank, char suit) {
        // Create fallback card with proper markings and consistent size
        Image img = GenImageColor(cw, ch, WHITE);
        
        // Card border
        Rectangle border = {2.0f, 2.0f, (float)(cw - 4), (float)(ch - 4)};
        Color borderColor = (suit == 'H' || suit == 'D') ? RED : BLACK;
        ImageDrawRectangleLines(&img, border, 2, borderColor);
        
        // Card background with subtle pattern
        for (int i = 0; i < 20; i++) {
            int x = rand() % (cw - 30) + 15;
            int y = rand() % (ch - 30) + 15;
            Color patternColor = (suit == 'H' || suit == 'D') ? 
                Color{255, 200, 200, 50} : Color{200, 200, 255, 50};
            ImageDrawPixel(&img, x, y, patternColor);
        }
        
        // Draw card value and suit in the corners
        string rankSymbol;
        switch(rank) {
            case 1: rankSymbol = "A"; break;
            case 11: rankSymbol = "J"; break;
            case 12: rankSymbol = "Q"; break;
            case 13: rankSymbol = "K"; break;
            default: rankSymbol = to_string(rank); break;
        }
        
        string suitSymbol;
        switch(suit) {
            case 'H': suitSymbol = "♥"; break;
            case 'D': suitSymbol = "♦"; break;
            case 'S': suitSymbol = "♠"; break;
            case 'C': suitSymbol = "♣"; break;
            default: suitSymbol = "?"; break;
        }
        
        Color textColor = (suit == 'H' || suit == 'D') ? RED : BLACK;
        
        // Draw top-left corner
        int textX = 10;
        int textY = 10;
        DrawTextPro(gameFont, rankSymbol.c_str(), 
                   (Vector2){(float)textX, (float)textY}, 
                   (Vector2){0, 0}, 0, 20, 1.0f, textColor);
        
        DrawTextPro(gameFont, suitSymbol.c_str(), 
                   (Vector2){(float)textX, (float)textY + 25}, 
                   (Vector2){0, 0}, 0, 25, 1.0f, textColor);
        
        // Draw bottom-right corner (rotated)
        DrawTextPro(gameFont, rankSymbol.c_str(), 
                   (Vector2){(float)(cw - 30), (float)(ch - 40)}, 
                   (Vector2){0, 0}, 180, 20, 1.0f, textColor);
        
        DrawTextPro(gameFont, suitSymbol.c_str(), 
                   (Vector2){(float)(cw - 30), (float)(ch - 15)}, 
                   (Vector2){0, 0}, 180, 25, 1.0f, textColor);
        
        cardTextures[idx] = LoadTextureFromImage(img);
        UnloadImage(img);
    }
    
    void unloadTextures() {
        UnloadTexture(backTexture);
        for (int i = 0; i < 52; i++) {
            if (cardTextures[i].id > 0) {
                UnloadTexture(cardTextures[i]);
            }
        }
    }
    
    Texture2D getTexture(CardItem c) {
        if (!c.is_faceup || c.rank_no == 0) return backTexture;
        
        // Calculate texture index based on suit and rank
        int suitOffset = 0;
        switch(c.suit) {
            case 'H': suitOffset = 0; break;
            case 'D': suitOffset = 13; break;
            case 'S': suitOffset = 26; break;
            case 'C': suitOffset = 39; break;
            default: suitOffset = 0;
        }
        
        int textureIndex = suitOffset + c.rank_no - 1;
        
        if (textureIndex >= 0 && textureIndex < 52 && cardTextures[textureIndex].id > 0) {
            return cardTextures[textureIndex];
        }
        
        // Fallback to back texture if not found
        return backTexture;
    }
    
    void drawCard(CardItem c, float x, float y, bool highlight = false, float alpha = 1.0f) {
        // Draw card background
        if (highlight) {
            DrawRectangle((int)(x - 4), (int)(y - 4), cw + 8, ch + 8, Fade(GOLD, 0.6f));
        }
        
        // Draw card texture
        if (c.is_faceup && c.rank_no > 0) {
            Texture2D tex = getTexture(c);
            DrawTexture(tex, (int)x, (int)y, Fade(WHITE, alpha));
            
            // Only draw text markings if we're using fallback textures
            if (!cardsLoaded) {
                Color col = c.check_red() ? RED : BLACK;
                
                // Top-left rank and suit
                string rankSymbol = c.getRankSymbol();
                string suitSymbol = c.getSuitSymbol();
                
                // Draw top-left corner
                DrawText(rankSymbol.c_str(), (int)x + 8, (int)y + 8, 20, col);
                DrawText(suitSymbol.c_str(), (int)x + 8, (int)y + 30, 24, col);
                
                // Draw bottom-right corner (rotated)
                DrawText(rankSymbol.c_str(), (int)x + cw - 30, (int)y + ch - 35, 20, col);
                DrawText(suitSymbol.c_str(), (int)x + cw - 30, (int)y + ch - 55, 24, col);
            }
        } else {
            // Draw card back
            DrawTexture(backTexture, (int)x, (int)y, Fade(WHITE, alpha));
        }
        
        if (highlight) {
            DrawRectangle((int)x, (int)y, cw, ch / 3, Fade(WHITE, 0.2f));
        }
    }
    
    void save() {
        if (undos.size() >= 50) undos.erase(undos.begin());
        GameSnapshot s;
        s.stock = stock; s.waste = waste;
        for (int i = 0; i < 4; i++) s.found[i] = found[i];
        for (int i = 0; i < 7; i++) s.tabs[i] = tabs[i].card_col;
        s.score = score; s.moves = moves;
        undos.push_back(s);
    }
    
    void undo() {
        if (undos.empty()) return;
        GameSnapshot s = undos.back();
        undos.pop_back();
        stock = s.stock; waste = s.waste;
        for (int i = 0; i < 4; i++) found[i] = s.found[i];
        for (int i = 0; i < 7; i++) tabs[i].card_col = s.tabs[i];
        score = s.score; moves = s.moves;
    }
    
    void init() {
        deck.full_deck();
        
        // Reset all game state
        stock.clearr();
        waste.clearr();
        for (int i = 0; i < 4; i++) found[i].clearr();
        for (int i = 0; i < 7; i++) tabs[i].card_col.clearr();
        
        // Deal tableau
        for (int c = 0; c < 7; c++) {
            for (int r = 0; r <= c; r++) {
                CardItem card = deck.draw_card();
                if (r == c) card.is_faceup = true;
                tabs[c].add_card(card);
            }
        }
        
        // Stock
        while (!deck.check_empty()) stock.enqueue(deck.draw_card());
        
        // Reset game stats
        score = 0;
        moves = 0;
        won = false;
        undos.clear();
        save();
        updateRects();
        
        // Add to statistics
        statistics.gamesPlayed++;
    }
    
    void updateRects() {
        int y = 100;
        stockRect = {30, (float)y, (float)cw, (float)ch};
        wasteRect = {30.0f + cw + 20, (float)y, (float)cw, (float)ch};
        
        int fx = 30 + (cw + 20) * 3;
        for (int i = 0; i < 4; i++) {
            foundRects[i] = {(float)(fx + i * (cw + 10)), (float)y, (float)cw, (float)ch};
        }
        
        int ty = y + ch + 50;
        for (int i = 0; i < 7; i++) {
            tabRects[i] = {(float)(30 + i * (cw + 20)), (float)ty, (float)cw, (float)(ch + 400)};
        }
    }
    
    void drawStock() {
        if (!stock.emptyy()) {
            save();  // Save state before making the move
            CardItem c = stock.dequeue();
            c.is_faceup = true;
            waste.enqueue(c);
            moves++; 
            score += 2; // Increased from 1 to 2
            addAnimation(stockRect.x, stockRect.y, wasteRect.x, wasteRect.y, c);
        } else if (!waste.emptyy()) {
            recycleWaste();
        }
    }
    
    void recycleWaste() {
        save();
        // Create a temporary queue to reverse the waste
        Queue<CardItem> tempWaste(24);
        
        // Move all cards from waste to tempWaste (reversing the order)
        while (!waste.emptyy()) {
            CardItem c = waste.dequeue();
            c.is_faceup = false;
            tempWaste.enqueue(c);
        }
        
        // Now reverse tempWaste using the recursive reverse method
        tempWaste.reverse();
        
        // Move all cards back to stock (now in correct order)
        while (!tempWaste.emptyy()) {
            CardItem c = tempWaste.dequeue();
            stock.enqueue(c);
        }
        
        moves++;
        score += 3; // Reduced from 5 to 3
        addAnimation(wasteRect.x, wasteRect.y, stockRect.x, stockRect.y, CardItem(0, ' ', false));
    }
    
    void addAnimation(float sx, float sy, float ex, float ey, CardItem c) {
        CardAnimation a;
        a.active = true;
        a.start = {sx, sy};
        a.end = {ex, ey};
        a.progress = 0;
        a.cardRank = c.rank_no;
        a.cardSuit = c.suit;
        a.duration = 0.3f;
        anims.push_back(a);
    }
    
    void addParticles(float x, float y, Color col) {
        for (int i = 0; i < 20; i++) {
            Particle p;
            p.pos = {x + cw/2, y + ch/2};
            float angle = (float)rand() / RAND_MAX * 6.28f;
            float speed = 50 + rand() % 100;
            p.vel = {std::cos(angle) * speed, std::sin(angle) * speed};
            p.color = col;
            p.life = 1.0f;
            p.size = 3.0f + rand() % 5;
            particles.push_back(p);
        }
    }
    
    void updateAnimations(float dt) {
        // Update card animations
        for (auto it = anims.begin(); it != anims.end();) {
            it->progress += dt / it->duration;
            if (it->progress >= 1.0f) {
                it = anims.erase(it);
            } else {
                ++it;
            }
        }
        
        // Update particles
        for (auto it = particles.begin(); it != particles.end();) {
            it->pos.x += it->vel.x * dt;
            it->pos.y += it->vel.y * dt;
            it->vel.y += 200 * dt;
            it->life -= dt;
            if (it->life <= 0) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
        
        hoverGlow += dt * 3;
        if (hoverGlow > 6.28f) hoverGlow -= 6.28f;
        
        if (won) {
            celebTime += dt;
            if ((int)(celebTime * 10) % 3 == 0) {
                float x = 200 + rand() % 800;
                float y = 100 + rand() % 400;
                Color cols[] = {RED, GOLD, BLUE, GREEN, PURPLE, YELLOW};
                addParticles(x, y, cols[rand() % 6]);
            }
        }
    }
    
    bool moveToFoundation(int source, int idx) {
        CardItem card;
        if (source == 0) {
            if (waste.emptyy()) return false;
            card = waste.grear();
        } else if (source <= 7) {
            if (tabs[source - 1].check_emp()) return false;
            card = tabs[source - 1].get_tcard();
            if (!card.is_faceup) return false;
        } else return false;
        
        int fi = card.suit == 'H' ? 0 : card.suit == 'D' ? 1 : card.suit == 'S' ? 2 : 3;
        
        if (!rules.foundation_place(found[fi], card)) return false;
        
        save();
        float sx, sy;
        CardItem movedCard;
        
        if (source == 0) {
            // Get the actual card from waste
            movedCard = waste.grear();
            if (movedCard.rank_no == 0) return false; // Safety check
            
            // Remove from waste
            // We need to get all cards except the last one
            Queue<CardItem> temp(24);
            int wasteSize = waste.gsize();
            
            if (wasteSize == 0) return false;
            
            // Move all cards except the last one to temp
            for (int i = 0; i < wasteSize - 1; i++) {
                temp.enqueue(waste.valueat(i));
            }
            
            // Get the last card (the one we're moving)
            movedCard = waste.valueat(wasteSize - 1);
            
            // Clear waste and restore the other cards
            waste.clearr();
            while (!temp.emptyy()) {
                waste.enqueue(temp.dequeue());
            }
            
            sx = wasteRect.x; sy = wasteRect.y;
        } else {
            movedCard = tabs[source - 1].rem_tcard();
            
            if (!tabs[source - 1].check_emp() && !tabs[source - 1].get_tcard().is_faceup) {
                tabs[source - 1].flip_top();
                score += 5;
            }
            
            sx = tabRects[source - 1].x;
            sy = tabRects[source - 1].y + tabs[source - 1].card_count() * 25;
        }
        
        found[fi].enqueue(movedCard);
        moves++; 
        score += 15; // Increased from 10 to 15
        
        addAnimation(sx, sy, foundRects[fi].x, foundRects[fi].y, movedCard);
        addParticles(foundRects[fi].x, foundRects[fi].y, movedCard.check_red() ? RED : DARKBLUE);
        
        if (rules.check_win(found)) {
            won = true;
            statistics.gamesWon++;
            statistics.addGame(score, moves, true);
        }
        
        return true;
    }
    
    bool moveCards(int from, int to, int startIdx) {
        if (from == to || from < 1 || from > 7 || to < 1 || to > 7) return false;
        if (tabs[from - 1].check_emp() || startIdx >= tabs[from - 1].card_count()) return false;
        
        CardItem moving = tabs[from - 1].card_at(startIdx);
        if (!moving.is_faceup) return false;
        
        bool valid = false;
        if (tabs[to - 1].check_emp()) {
            valid = (moving.king());
        } else {
            CardItem dest = tabs[to - 1].get_tcard();
            valid = rules.tab_move(moving, dest);
        }
        
        if (!valid) return false;
        
        save();
        
        // Get all cards from the starting index
        Queue<CardItem> cardsToMove = tabs[from - 1].get_cards_from(startIdx);
        
        // Add them to the destination
        while (!cardsToMove.emptyy()) {
            tabs[to - 1].add_card(cardsToMove.dequeue());
        }
        
        // Remove them from source
        tabs[from - 1].remove_from(startIdx);
        
        if (!tabs[from - 1].check_emp() && !tabs[from - 1].get_tcard().is_faceup) {
            tabs[from - 1].flip_top();
            score += 10; // Increased from 5 to 10
        }
        
        moves++; 
        score += 8; // Increased from 5 to 8
        return true;
    }
    
    bool moveWasteToTableau(int to) {
        if (to < 1 || to > 7 || waste.emptyy()) return false;
        
        CardItem moving = waste.grear();
        
        bool valid = false;
        if (tabs[to - 1].check_emp()) {
            valid = (moving.king());
        } else {
            CardItem dest = tabs[to - 1].get_tcard();
            valid = rules.tab_move(moving, dest);
        }
        
        if (!valid) return false;
        
        save();
        
        // Get the actual card from waste
        CardItem movedCard;
        Queue<CardItem> temp(24);
        int wasteSize = waste.gsize();
        
        if (wasteSize == 0) return false;
        
        // Move all cards except the last one to temp
        for (int i = 0; i < wasteSize - 1; i++) {
            temp.enqueue(waste.valueat(i));
        }
        
        // Get the last card (the one we're moving)
        movedCard = waste.valueat(wasteSize - 1);
        
        // Clear waste and restore the other cards
        waste.clearr();
        while (!temp.emptyy()) {
            waste.enqueue(temp.dequeue());
        }
        
        tabs[to - 1].add_card(movedCard);
        
        moves++; 
        score += 5;
        return true;
    }
    
    // Button drawing function
    void drawButton(Button& button, float fontSize = 24.0f) {
        Color btnColor = button.hovered ? button.hoverColor : button.color;
        
        // Draw button background with rounded corners
        DrawRectangleRounded(button.rect, 0.3f, 10, btnColor);
        
        // Draw button border - FIXED: using DrawRectangleLinesEx instead
        DrawRectangleLinesEx(button.rect, 2.0f, WHITE);
        
        // Draw button text
        int textWidth = MeasureText(button.text.c_str(), fontSize);
        DrawText(button.text.c_str(), 
                button.rect.x + button.rect.width/2 - textWidth/2,
                button.rect.y + button.rect.height/2 - fontSize/2,
                fontSize, WHITE);
    }
    
    // Update button hover state
    void updateButtonHover(vector<Button>& buttons, Vector2 mousePos) {
        for (auto& button : buttons) {
            button.hovered = CheckCollisionPointRec(mousePos, button.rect);
        }
    }
    
    // Render menu screen
    void renderMenu() {
        // Dark blue gradient background
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{20, 40, 80, 255}, Color{10, 20, 40, 255});
        
        // Draw title with shadow effect
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        
        // Main title
        DrawText("SOLITAIRE", screenWidth/2 - MeasureText("SOLITAIRE", 60)/2, 80, 60, GOLD);
        DrawText("KLONDIKE", screenWidth/2 - MeasureText("KLONDIKE", 48)/2, 150, 48, SKYBLUE);
        DrawText("MINIMAL", screenWidth/2 - MeasureText("MINIMAL", 36)/2, 210, 36, LIME);
        
        // Draw decorative line under title
        DrawLine(screenWidth/2 - 150, 250, screenWidth/2 + 150, 250, Fade(GOLD, 0.5f));
        
        // Update button hover states
        Vector2 mousePos = GetMousePosition();
        updateButtonHover(menuButtons, mousePos);
        
        // Draw all menu buttons
        for (auto& button : menuButtons) {
            drawButton(button);
        }
        
        // Draw version info
    
    }
    
    // Render help screen
    void renderHelp() {
        // Dark blue gradient background
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{20, 40, 80, 255}, Color{10, 20, 40, 255});
        
        // Title
        DrawText("RULES & HELP", GetScreenWidth()/2 - MeasureText("RULES & HELP", 48)/2, 50, 48, GOLD);
        
        // Content area
        int contentWidth = 900;
        int contentHeight = 500;
        int contentX = GetScreenWidth()/2 - contentWidth/2;
        int contentY = 150;
        
        DrawRectangle(contentX, contentY, contentWidth, contentHeight, Fade(Color{30, 30, 30, 255}, 0.8f));
        DrawRectangleLines(contentX, contentY, contentWidth, contentHeight, GOLD);
        
        // Rules content
        int textX = contentX + 40;
        int textY = contentY + 40;
        int lineHeight = 28;
        
        DrawText("KLONDIKE SOLITAIRE RULES", textX, textY, 24, YELLOW); textY += lineHeight + 20;
        
        DrawText("OBJECTIVE:", textX, textY, 20, LIME); textY += lineHeight;
        DrawText("Move all 52 cards to the four foundation piles", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("(one for each suit: Hearts, Diamonds, Spades, Clubs)", textX + 20, textY, 18, WHITE); textY += lineHeight + 10;
        
        DrawText("TABLEAU:", textX, textY, 20, LIME); textY += lineHeight;
        DrawText("• Build down in alternating colors", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Red on Black or Black on Red", textX + 40, textY, 16, LIGHTGRAY); textY += lineHeight;
        DrawText("• Only Kings can start empty columns", textX + 20, textY, 18, WHITE); textY += lineHeight + 10;
        
        DrawText("FOUNDATIONS:", textX, textY, 20, LIME); textY += lineHeight;
        DrawText("• Build up in same suit only", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Start with Aces, end with Kings", textX + 20, textY, 18, WHITE); textY += lineHeight + 10;
        
        DrawText("STOCK & WASTE:", textX, textY, 20, LIME); textY += lineHeight;
        DrawText("• Click stock to draw cards", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Only top waste card can be played", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Recycle waste when stock is empty", textX + 20, textY, 18, WHITE); textY += lineHeight + 10;
        
        DrawText("CONTROLS:", textX, textY, 20, LIME); textY += lineHeight;
        DrawText("• Drag and drop cards to move them", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Double-click cards to move to foundation", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Press H for in-game help", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Press U to undo moves", textX + 20, textY, 18, WHITE); textY += lineHeight;
        DrawText("• Press ESC to return to menu", textX + 20, textY, 18, WHITE); textY += lineHeight;
        
        // Update and draw back button
        updateButtonHover(helpButtons, GetMousePosition());
        for (auto& button : helpButtons) {
            drawButton(button);
        }
    }
    
    // Render statistics screen
    void renderStats() {
        // Dark blue gradient background
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{20, 40, 80, 255}, Color{10, 20, 40, 255});
        
        // Title
        DrawText("STATISTICS", GetScreenWidth()/2 - MeasureText("STATISTICS", 48)/2, 50, 48, GOLD);
        
        // Content area
        int contentWidth = 800;
        int contentHeight = 400;
        int contentX = GetScreenWidth()/2 - contentWidth/2;
        int contentY = 150;
        
        DrawRectangle(contentX, contentY, contentWidth, contentHeight, Fade(Color{30, 30, 30, 255}, 0.8f));
        DrawRectangleLines(contentX, contentY, contentWidth, contentHeight, GOLD);
        
        // Statistics content
        int textX = contentX + 40;
        int textY = contentY + 60;
        int lineHeight = 35;
        
        // Calculate win percentage
        float winPercentage = statistics.gamesPlayed > 0 ? 
            (float)statistics.gamesWon / statistics.gamesPlayed * 100.0f : 0.0f;
        float avgScore = statistics.gamesPlayed > 0 ? 
            (float)statistics.totalScore / statistics.gamesPlayed : 0.0f;
        float avgMoves = statistics.gamesPlayed > 0 ? 
            (float)statistics.totalMoves / statistics.gamesPlayed : 0.0f;
        
        // Draw statistics
        DrawText(TextFormat("Games Played: %d", statistics.gamesPlayed), textX, textY, 24, WHITE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Games Won: %d", statistics.gamesWon), textX, textY, 24, WHITE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Win Percentage: %.1f%%", winPercentage), textX, textY, 24, 
                winPercentage >= 50.0f ? LIME : ORANGE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Highest Score: %d", statistics.highestScore), textX, textY, 24, 
                statistics.highestScore > 0 ? GOLD : WHITE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Average Score: %.1f", avgScore), textX, textY, 24, WHITE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Average Moves: %.1f", avgMoves), textX, textY, 24, WHITE); 
        textY += lineHeight;
        
        DrawText(TextFormat("Total Moves: %d", statistics.totalMoves), textX, textY, 24, WHITE); 
        textY += lineHeight;
        
        // Update and draw back button
        updateButtonHover(statsButtons, GetMousePosition());
        for (auto& button : statsButtons) {
            drawButton(button);
        }
    }
    
    // Render settings screen
    void renderSettings() {
        // Dark blue gradient background
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{20, 40, 80, 255}, Color{10, 20, 40, 255});
        
        // Title
        DrawText("GAME SETTINGS", GetScreenWidth()/2 - MeasureText("GAME SETTINGS", 48)/2, 50, 48, GOLD);
        
        // Content area
        int contentWidth = 800;
        int contentHeight = 400;
        int contentX = GetScreenWidth()/2 - contentWidth/2;
        int contentY = 150;
        
        DrawRectangle(contentX, contentY, contentWidth, contentHeight, Fade(Color{30, 30, 30, 255}, 0.8f));
        DrawRectangleLines(contentX, contentY, contentWidth, contentHeight, GOLD);
        
        // Settings content
        int textX = contentX + 40;
        int textY = contentY + 80;
        int lineHeight = 40;
        
        DrawText("COMING SOON!", textX + 250, textY, 32, YELLOW); textY += lineHeight + 20;
        
        DrawText("Future Features:", textX, textY, 24, LIME); textY += lineHeight;
        DrawText("• 1-card vs 3-card draw option", textX + 20, textY, 20, WHITE); textY += lineHeight;
        DrawText("• Difficulty levels (Easy, Medium, Hard)", textX + 20, textY, 20, WHITE); textY += lineHeight;
        DrawText("• Card designs and themes", textX + 20, textY, 20, WHITE); textY += lineHeight;
        DrawText("• Sound and music settings", textX + 20, textY, 20, WHITE); textY += lineHeight;
        DrawText("• Animation speed controls", textX + 20, textY, 20, WHITE); textY += lineHeight;
        
        // Update and draw back button
        updateButtonHover(settingsButtons, GetMousePosition());
        for (auto& button : settingsButtons) {
            drawButton(button);
        }
    }
    
    // Render game screen
    void renderGame() {
        // Blue background gradient
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{25, 50, 100, 255}, Color{15, 30, 60, 255});
        
        // Title with shadow effect
        DrawText("KLONDIKE SOLITAIRE", 22, 12, 36, BLACK);
        DrawText("KLONDIKE SOLITAIRE", 20, 10, 36, GOLD);
        
        // Score info (clean text without backgrounds)
        DrawText(TextFormat("Score: %d", score), 520, 25, 24, YELLOW);
        DrawText(TextFormat("Moves: %d", moves), 720, 25, 24, LIME);
        
        // Completion percentage - color coded
        int pct = (found[0].gsize() + found[1].gsize() + found[2].gsize() + found[3].gsize()) * 100 / 52;
        
        // Color code completion percentage
        Color pctColor;
        if (pct < 25) pctColor = RED;
        else if (pct < 50) pctColor = ORANGE;
        else if (pct < 75) pctColor = YELLOW;
        else if (pct < 100) pctColor = LIME;
        else pctColor = GREEN;
        
        DrawText(TextFormat("Complete: %d%%", pct), 920, 25, 24, pctColor);
        
        int y = 100;
        
        // Stock
        if (hoverCol == -10) {
            float glow = 0.3f + 0.2f * std::sin(hoverGlow);
            DrawRectangle((int)(stockRect.x - 5), (int)(stockRect.y - 5), cw + 10, ch + 10, 
                         Fade(YELLOW, glow));
        }
        
        if (!stock.emptyy()) {
            drawCard(CardItem(), stockRect.x, stockRect.y);
            DrawText(TextFormat("%d", stock.gsize()), (int)stockRect.x + 10, 
                    (int)stockRect.y + ch + 8, 20, WHITE);
        } else {
            DrawRectangleRounded(stockRect, 0.1f, 10, Fade(DARKGRAY, 0.5f));
            DrawText("EMPTY", (int)stockRect.x + 25, (int)stockRect.y + 60, 18, LIGHTGRAY);
        }
        
        // Stock label
        DrawText("STOCK", (int)stockRect.x + 25, (int)stockRect.y - 25, 18, WHITE);
        
        // Waste
        if (hoverCol == -11) {
            float glow = 0.3f + 0.2f * std::sin(hoverGlow);
            DrawRectangle((int)(wasteRect.x - 5), (int)(wasteRect.y - 5), cw + 10, ch + 10, 
                         Fade(YELLOW, glow));
        }
        
        if (!waste.emptyy()) {
            drawCard(waste.grear(), wasteRect.x, wasteRect.y, dragging && dragType == 0);
        } else {
            DrawRectangleRounded(wasteRect, 0.1f, 10, Fade(DARKGRAY, 0.5f));
        }
        
        // Waste label
        DrawText("WASTE", (int)wasteRect.x + 25, (int)wasteRect.y - 25, 18, WHITE);
        
        // Foundations - NO BORDER AND NO TEXT when empty
        int fx = 30 + (cw + 20) * 3;
        for (int i = 0; i < 4; i++) {
            if (hoverCol == 100 + i) {
                float glow = 0.3f + 0.2f * std::sin(hoverGlow);
                DrawRectangle((int)(foundRects[i].x - 5), (int)(foundRects[i].y - 5), 
                             cw + 10, ch + 10, Fade(YELLOW, glow));
            }
            
            if (!found[i].emptyy()) {
                drawCard(found[i].grear(), foundRects[i].x, foundRects[i].y);
                // Show count inside foundation card
                DrawText(TextFormat("%d", found[i].gsize()), 
                        (int)foundRects[i].x + 40, (int)foundRects[i].y + 50, 
                        24, Fade(WHITE, 0.7f));
            } else {
                // Draw empty foundation - NO BORDER, NO TEXT, just a simple background
                DrawRectangleRounded(foundRects[i], 0.1f, 10, Fade(DARKGRAY, 0.3f));
            }
        }
        
        // Draw suit labels ABOVE the foundation piles
        const char* suitLabels[] = {"Hearts", "Diamonds", "Spades", "Clubs"};
        Color labelColors[] = {RED, RED, WHITE, WHITE};
        
        for (int i = 0; i < 4; i++) {
            int labelX = fx + i * (cw + 10);
            DrawText(suitLabels[i], labelX + 15, y - 25, 18, labelColors[i]);
        }
        
        // Tableau columns
        int ty = y + ch + 50;
        for (int c = 0; c < 7; c++) {
            if (hoverCol == c) {
                float glow = 0.3f + 0.2f * std::sin(hoverGlow);
                int h = tabs[c].check_emp() ? ch : std::min(ch + tabs[c].card_count() * 25, 500);
                DrawRectangle((int)(tabRects[c].x - 5), (int)(tabRects[c].y - 5), 
                             cw + 10, h + 10, Fade(YELLOW, glow));
            }
            
            if (tabs[c].check_emp()) {
                Rectangle r = {tabRects[c].x, (float)ty, (float)cw, (float)ch};
                DrawRectangleRounded(r, 0.1f, 10, Fade(DARKGRAY, 0.5f));
                DrawText("K", (int)tabRects[c].x + 40, ty + 60, 35, LIGHTGRAY);
                DrawText("Only", (int)tabRects[c].x + 25, ty + 100, 15, YELLOW);
                DrawText("Kings", (int)tabRects[c].x + 22, ty + 115, 15, YELLOW);
            } else {
                for (int i = 0; i < tabs[c].card_count(); i++) {
                    bool hl = dragging && dragType == c + 1 && i >= dragCard;
                    drawCard(tabs[c].card_at(i), tabRects[c].x, ty + i * 25, hl);
                }
                
                // Show face-down card count at bottom of tableau
                int faceDownCount = 0;
                for (int i = 0; i < tabs[c].card_count(); i++) {
                    if (!tabs[c].card_at(i).is_faceup) faceDownCount++;
                }
                if (faceDownCount > 0) {
                    DrawText(TextFormat("%d↓", faceDownCount), 
                            (int)tabRects[c].x + 5, ty + tabs[c].card_count() * 25 + 5, 
                            16, Fade(YELLOW, 0.7f));
                }
            }
        }
        
        // Animations
        for (auto& a : anims) {
            float t = a.progress;
            float ease = t < 0.5f ? 2 * t * t : 1 - pow(-2 * t + 2, 2) / 2;
            float x = a.start.x + (a.end.x - a.start.x) * ease;
            float y = a.start.y + (a.end.y - a.start.y) * ease - std::sin(t * 3.14f) * 20;
            drawCard(CardItem(a.cardRank, a.cardSuit, true), x, y, true, 1.0f - t * 0.3f);
        }
        
        // Particles
        for (auto& p : particles) {
            DrawCircle((int)p.pos.x, (int)p.pos.y, p.size, Fade(p.color, p.life));
        }
        
        // Dragging
        if (dragging && !dragCards.empty()) {
            Vector2 m = GetMousePosition();
            for (size_t i = 0; i < dragCards.size(); i++) {
                drawCard(dragCards[i], m.x + dragOff.x, m.y + dragOff.y + i * 25, true, 0.9f);
            }
        }
        
        // Help overlay
        if (showHelp) {
            // Simplified in-game help
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
            
            int hw = 600, hh = 400;
            int hx = GetScreenWidth() / 2 - hw / 2;
            int hy = GetScreenHeight() / 2 - hh / 2;
            
            DrawRectangle(hx, hy, hw, hh, Color{30, 30, 30, 255});
            DrawRectangleLines(hx, hy, hw, hh, GOLD);
            
            DrawText("IN-GAME HELP", hx + 200, hy + 20, 28, GOLD);
            DrawRectangle(hx + 20, hy + 55, hw - 40, 2, GOLD);
            
            int tx = hx + 40, tyy = hy + 75, ls = 24;
            
            DrawText("• Drag cards to move them", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Double-click cards to move to foundation", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Click stock to draw cards", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Empty stock recycles waste", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Press U to undo moves", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Press ESC to return to menu", tx, tyy, 20, WHITE); tyy += ls;
            DrawText("• Press H to close help", tx, tyy, 20, WHITE); tyy += ls;
            
            DrawRectangle(hx + 20, hy + hh - 55, hw - 40, 2, GOLD);
            DrawText("Press H or ESC to close", hx + 180, hy + hh - 40, 20, YELLOW);
        }
        
        // Win screen
        if (won) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
            
            int wx = GetScreenWidth() / 2 - 200;
            int wy = GetScreenHeight() / 2 - 150;
            
            DrawRectangle(wx, wy, 400, 300, Color{40, 40, 40, 255});
            DrawRectangleLines(wx, wy, 400, 300, GOLD);
            
            DrawText("CONGRATULATIONS!", wx + 50, wy + 40, 28, GOLD);
            DrawText("You Won!", wx + 130, wy + 90, 32, LIME);
            
            DrawText(TextFormat("Final Score: %d", score), wx + 100, wy + 140, 24, WHITE);
            DrawText(TextFormat("Total Moves: %d", moves), wx + 100, wy + 180, 24, WHITE);
            
            // Show completed suits
            DrawText("Completed Suits:", wx + 100, wy + 220, 20, YELLOW);
            int suitX = wx + 120;
            for (int i = 0; i < 4; i++) {
                if (found[i].gsize() == 13) {
                    const char* symbols[] = {"\u2665", "\u2666", "\u2660", "\u2663"};
                    Color colors[] = {RED, RED, WHITE, WHITE};
                    DrawText(symbols[i], suitX, wy + 250, 30, colors[i]);
                    suitX += 40;
                }
            }
            
            DrawText("Press ESC for Main Menu", wx + 80, wy + 290, 20, YELLOW);
        }
        
        // Button hints - clean text
        if (!showHelp && !won) {
            // Clean text directly on background
            DrawText("Press H for In-game Help", 20, GetScreenHeight() - 60, 20, YELLOW);
            DrawText("Press U to Undo Move", 20, GetScreenHeight() - 35, 20, LIME);
            DrawText("ESC for Main Menu", GetScreenWidth() - MeasureText("ESC for Main Menu", 20) - 20, 
                    GetScreenHeight() - 60, 20, ORANGE);
            DrawText("Double-click cards for Foundation", GetScreenWidth()/2 - MeasureText("Double-click cards for Foundation", 20)/2, 
                    GetScreenHeight() - 30, 20, Fade(YELLOW, 0.9f));
        }
    }
    
    // Handle menu input
    void handleMenuInput() {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check menu buttons
            for (int i = 0; i < menuButtons.size(); i++) {
                if (CheckCollisionPointRec(mousePos, menuButtons[i].rect)) {
                    switch(i) {
                        case 0: // NEW GAME
                            init();
                            currentScreen = PLAYING;
                            break;
                        case 1: // LOAD GAME
                            // Load game functionality (placeholder)
                            // For now, just start a new game
                            init();
                            currentScreen = PLAYING;
                            break;
                        case 2: // STATISTICS
                            currentScreen = STATS;
                            break;
                        case 3: // SETTINGS
                            currentScreen = SETTINGS;
                            break;
                        case 4: // RULES & HELP
                            currentScreen = HELP;
                            break;
                        case 5: // EXIT
                            // Exit game - will be handled by main loop
                            break;
                    }
                    return;
                }
            }
        }
    }
    
    // Handle help screen input
    void handleHelpInput() {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check back button
            for (auto& button : helpButtons) {
                if (CheckCollisionPointRec(mousePos, button.rect)) {
                    currentScreen = MENU;
                    return;
                }
            }
        }
        
        // ESC to go back
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentScreen = MENU;
        }
    }
    
    // Handle statistics screen input
    void handleStatsInput() {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check back button
            for (auto& button : statsButtons) {
                if (CheckCollisionPointRec(mousePos, button.rect)) {
                    currentScreen = MENU;
                    return;
                }
            }
        }
        
        // ESC to go back
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentScreen = MENU;
        }
    }
    
    // Handle settings screen input
    void handleSettingsInput() {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check back button
            for (auto& button : settingsButtons) {
                if (CheckCollisionPointRec(mousePos, button.rect)) {
                    currentScreen = MENU;
                    return;
                }
            }
        }
        
        // ESC to go back
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentScreen = MENU;
        }
    }
    
    void handleGameInput() {
        Vector2 mousePos = GetMousePosition();
        
        // Keyboard shortcuts
        if (IsKeyPressed(KEY_H)) showHelp = !showHelp;
        if (IsKeyPressed(KEY_U)) undo();
        if (IsKeyPressed(KEY_ESCAPE)) {
            // Save current game state before returning to menu
            save();
            currentScreen = MENU;
            return;
        }
        
        if (showHelp || won) return;
        
        // Update hover
        hoverCol = -1;
        if (CheckCollisionPointRec(mousePos, stockRect)) hoverCol = -10;
        else if (CheckCollisionPointRec(mousePos, wasteRect)) hoverCol = -11;
        else {
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mousePos, foundRects[i])) {
                    hoverCol = 100 + i;
                    break;
                }
            }
            if (hoverCol == -1) {
                for (int i = 0; i < 7; i++) {
                    if (CheckCollisionPointRec(mousePos, tabRects[i])) {
                        hoverCol = i;
                        break;
                    }
                }
            }
        }
        
        // Mouse input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            double now = GetTime();
            
            // Stock pile click
            if (CheckCollisionPointRec(mousePos, stockRect) || 
                (stock.emptyy() && CheckCollisionPointRec(mousePos, wasteRect))) {
                drawStock();
                return;
            }
            
            // Waste pile
            if (CheckCollisionPointRec(mousePos, wasteRect) && !waste.emptyy()) {
                if (now - lastClick < 0.5 && lastCol == 0) {
                    moveToFoundation(0, 0);
                    lastCol = -1;
                } else {
                    dragType = 0;
                    dragCard = 0;
                    dragging = true;
                    dragCards.clear();
                    dragCards.push_back(waste.grear());
                    dragOff = {wasteRect.x - mousePos.x, wasteRect.y - mousePos.y};
                    lastClick = now;
                    lastCol = 0;
                }
                return;
            }
            
            // Foundation piles - double click
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mousePos, foundRects[i])) {
                    lastClick = now;
                    lastCol = -1;
                    return;
                }
            }
            
            // Tableau
            int ty = 100 + ch + 50;
            for (int c = 0; c < 7; c++) {
                if (!CheckCollisionPointRec(mousePos, tabRects[c])) continue;
                
                if (tabs[c].check_emp()) {
                    lastCol = -1;
                    return;
                }
                
                // Find clicked card
                int clickedCard = -1;
                for (int i = 0; i < tabs[c].card_count(); i++) {
                    float cy = ty + i * 25;
                    if (mousePos.y >= cy && mousePos.y < cy + ch) {
                        clickedCard = i;
                    }
                }
                
                if (clickedCard >= 0 && tabs[c].card_at(clickedCard).is_faceup) {
                    if (now - lastClick < 0.5 && lastCol == c + 1 && 
                        clickedCard == tabs[c].card_count() - 1) {
                        moveToFoundation(c + 1, clickedCard);
                        lastCol = -1;
                    } else {
                        dragType = c + 1;
                        dragCard = clickedCard;
                        dragging = true;
                        dragCards.clear();
                        for (int i = clickedCard; i < tabs[c].card_count(); i++) {
                            dragCards.push_back(tabs[c].card_at(i));
                        }
                        dragOff = {tabRects[c].x - mousePos.x, ty + clickedCard * 25 - mousePos.y};
                        lastClick = now;
                        lastCol = c + 1;
                    }
                }
                return;
            }
        }
        
        // Mouse release - drop cards
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && dragging) {
            bool moved = false;
            
            // Try foundations
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mousePos, foundRects[i]) && dragCards.size() == 1) {
                    if (dragType == 0) {
                        moved = moveToFoundation(0, 0);
                    } else if (dragType >= 1 && dragType <= 7) {
                        moved = moveToFoundation(dragType, dragCard);
                    }
                    break;
                }
            }
            
            // Try tableau
            if (!moved) {
                if (dragType == 0) {
                    // Moving from waste to tableau
                    for (int c = 0; c < 7; c++) {
                        if (CheckCollisionPointRec(mousePos, tabRects[c])) {
                            moved = moveWasteToTableau(c + 1);
                            break;
                        }
                    }
                } else if (dragType >= 1 && dragType <= 7) {
                    // Moving between tableau columns
                    for (int c = 0; c < 7; c++) {
                        if (CheckCollisionPointRec(mousePos, tabRects[c])) {
                            moved = moveCards(dragType, c + 1, dragCard);
                            break;
                        }
                    }
                }
            }
            
            dragging = false;
            dragCards.clear();
        }
    }
};

// Main function
int main() {
    InitWindow(1200, 800, "Klondike ");
    SetTargetFPS(60);
    
    SolitaireGame game;
    game.loadTextures();
    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Update animations if in game
        if (game.currentScreen == PLAYING) {
            game.updateAnimations(dt);
        }
        
        // Handle input based on current state
        switch(game.currentScreen) {
            case MENU:
                game.handleMenuInput();
                break;
            case PLAYING:
                game.handleGameInput();
                break;
            case HELP:
                game.handleHelpInput();
                break;
            case STATS:
                game.handleStatsInput();
                break;
            case SETTINGS:
                game.handleSettingsInput();
                break;
        }
        
        BeginDrawing();
        ClearBackground(Color{15, 30, 60, 255});
        
        // Render based on current state
        switch(game.currentScreen) {
            case MENU:
                game.renderMenu();
                break;
            case PLAYING:
                game.renderGame();
                break;
            case HELP:
                game.renderHelp();
                break;
            case STATS:
                game.renderStats();
                break;
            case SETTINGS:
                game.renderSettings();
                break;
        }
        
        EndDrawing();
    }
    
    game.unloadTextures();
    CloseWindow();
    
    return 0;
}