#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include <cstring>
#include <filesystem>
#include "raylib.h"

using namespace std;
namespace fs=filesystem;



struct Particle 
{
    Vector2 pos;
    Vector2 vel;
    Color color;
    float life;
    float size;
};

struct CardAnimation 
{
    bool active;
    Vector2 start;
    Vector2 end;
    float progress;
    int cardRank;
    char cardSuit;
    float duration;
};

enum GameScreen 
{
    LEVEL_SELECT,
    PLAYING,
    HELP,
    STATS,
    SETTINGS
};

struct Button 
{
    Rectangle rect;
    string text;
    Color color;
    Color hoverColor;
    bool hovered;
};

enum DifficultyLevel 
{
    EASY,     
    MEDIUM,   
    HARD     
};

template <class T>
class Queue
{
private:
    T* arr;
    int frontt;
    int rear;
    int cap;
    int sizee;

public:
    Queue()
    {
        cap=52;
        arr=new T[cap];
        frontt=0;
        rear=-1;
        sizee=0;
    }

    Queue(int s)
    {
        cap=s;
        arr=new T[cap];
        frontt=0;
        rear=-1;
        sizee=0;
    }

    ~Queue()
    {
        delete[] arr;
    }

    Queue(Queue& o) : cap(o.cap), frontt(o.frontt), rear(o.rear), sizee(o.sizee) {
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

    void enqueue(T v)
    {
        if (sizee>=cap)
        {
            cout<<"Queue is full"<<endl;
            return;
        }
        rear=(rear+1)%cap;
        arr[rear]=v;
        sizee=sizee+1;
    }

    T dequeue()
    {
        if (sizee<=0)
        {
            T e;
            return e;
        }
        T v=arr[frontt];
        frontt=(frontt+1)%cap;
        sizee=sizee-1;
        return v;
    }

    bool emptyy()
    {
        return sizee==0;
    }

    int gsize()
    {
        return sizee;
    }

    void clearr()
    {
        frontt=0;
        rear=-1;
        sizee=0;
    }

    T valueat(int i) 
    {
        if (i < 0 || i>=sizee) 
        {
            T e;
            return e;
        }
        return arr[(frontt + i) % cap];
    }

    T grear() 
    {
        if (sizee <= 0) 
        {
            T e;
            return e;
        }
        return arr[rear];
    }
    
    T peekFront() 
    {
        if (sizee <= 0) 
        {
            T e;
            return e;
        }
        return arr[frontt];
    }
    
    T peekRear() 
    {
        if (sizee<=0) 
        {
            T e;
            return e;
        }
        return arr[rear];
    }
};

class CardItem 
{
public:
    int rank_no;
    char suit;
    bool is_faceup;
    
    CardItem() : rank_no(0), suit(' '), is_faceup(false) {}
    CardItem(int r, char s, bool f) : rank_no(r), suit(s), is_faceup(f) {}
    
    bool check_red() { return suit=='H' || suit=='D'; }
    void flip_card() { is_faceup=!is_faceup; }
    bool king() { return rank_no ==13; }
    bool ace() { return rank_no == 1; }
    
    string getSuitSymbol() 
    {
        switch(suit) {
            case 'H': return "\u2665"; 
            case 'D': return "\u2666"; 
            case 'S': return "\u2660"; 
            case 'C': return "\u2663"; 
            default: return "?";
        }
    }
    
    string getSuitName() 
    {
        switch(suit) 
        {
            case 'H': return "Hearts";
            case 'D': return "Diamonds";
            case 'S': return "Spades";
            case 'C': return "Clubs";
            default: return "Unknown";
        }
    }

    string getRankSymbol() 
    {
        switch(rank_no) 
        {
            case 1: return "A";
            case 11: return "J";
            case 12: return "Q";
            case 13: return "K";
            default: return to_string(rank_no);
        }
    }
    
    string getRankName() 
    {
        switch(rank_no) 
        {
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
    
    string getImageFilename() 
    {
        string rank;
        switch(rank_no) 
        {
            case 1: rank = "ace"; break;
            case 11: rank = "jack"; break;
            case 12: rank = "queen"; break;
            case 13: rank = "king"; break;
            default: rank = to_string(rank_no); break;
        }
        
        string suitName;
        switch(suit) 
        {
            case 'H': suitName = "hearts"; break;
            case 'D': suitName = "diamonds"; break;
            case 'S': suitName = "spades"; break;
            case 'C': suitName = "clubs"; break;
            default: suitName = "unknown"; break;
        }
        
        return rank + "_of_" + suitName + ".png";
    }
};

class DeckOfCards 
{
public:
    Queue<CardItem> all_card;
    
    DeckOfCards() : all_card(52) {}
    
    void shuffle_cards(Queue<CardItem>& cards) 
    {
        int s = cards.gsize();
        CardItem arr[52];
        for (int i = 0; i < s; i++) arr[i] = cards.dequeue();
        
        for (int i = s - 1; i > 0; i--) 
        {
            int j = rand() % (i + 1);
            CardItem temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        
        for (int i = 0; i < s; i++) cards.enqueue(arr[i]);
    }
    
    void full_deck() 
    {
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
    
    CardItem draw_card() 
    {
        return all_card.emptyy() ? CardItem() : all_card.dequeue();
    }

    bool check_empty() { return all_card.emptyy(); }
};

class GameRules 
{
public:
    bool tab_move(CardItem from, CardItem to) 
    {
        if (to.rank_no == 0) return from.king(); 
        return from.check_red() != to.check_red() && from.rank_no == to.rank_no - 1;
    }
    
    bool foundation_move(CardItem from, CardItem to) 
    {
        return from.suit == to.suit && from.rank_no == to.rank_no + 1;
    }
    
    bool foundation_place(Queue<CardItem> pile, CardItem card) 
    {
        if (pile.emptyy()) return card.ace();
        if (pile.peekRear().rank_no == 0) return false; 
        return foundation_move(card, pile.peekRear());
    }
    
    bool check_win(Queue<CardItem> piles[4]) 
    {
        for (int i = 0; i < 4; i++) 
        {
            if (piles[i].gsize() != 13) return false;
        }
        return true;
    }
    
    string getSuitColor(char suit) 
    {
        if (suit == 'H' || suit == 'D') return "Red";
        return "Black";
    }
};

class TableauColumn 
{
public:
    Queue<CardItem> card_col;
    
    TableauColumn() : card_col(52) {}
    
    void add_card(CardItem c) { card_col.enqueue(c); }
    
    CardItem rem_tcard() 
    {
        if (card_col.emptyy()) return CardItem();
        Queue<CardItem> temp(52);
        CardItem last;
        while (!card_col.emptyy()) 
        {
            last = card_col.dequeue();
            if (!card_col.emptyy()) temp.enqueue(last);
        }
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
        return last;
    }
    
    CardItem get_tcard() 
    {
        return card_col.emptyy() ? CardItem() : card_col.peekRear();
    }
    
    bool check_emp() { return card_col.emptyy(); }
    int card_count() { return card_col.gsize();}
    
    CardItem card_at(int i) 
    {
         return card_col.valueat(i); 
    }
    
    Queue<CardItem> get_cards_from(int idx) 
    {
        Queue<CardItem> result(52);
        for (int i = idx; i < card_col.gsize(); i++) 
        {
            result.enqueue(card_col.valueat(i));
        }
        return result;
    }
    
    void remove_from(int idx) 
    {
        Queue<CardItem> temp(52);
        for (int i = 0; i < idx; i++) temp.enqueue(card_col.valueat(i));
        card_col.clearr();
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
    }
    
    void flip_top() 
    {
        if (card_col.emptyy()) return;
        Queue<CardItem> temp(52);
        CardItem last;
        while (!card_col.emptyy()) 
        {
            last = card_col.dequeue();
            if (!card_col.emptyy()) temp.enqueue(last);
        }
        last.flip_card();
        while (!temp.emptyy()) card_col.enqueue(temp.dequeue());
        card_col.enqueue(last);
    }
};

struct GameSnapshot 
{
    Queue<CardItem> stock, waste, found[4], tabs[7];
    int score, moves;
    
    GameSnapshot() : stock(24), waste(24) 
    {
        for (int i = 0; i < 4; i++) found[i]=Queue<CardItem>(13);
        for (int i = 0; i < 7; i++) tabs[i] = Queue<CardItem>(52);
        score = moves = 0;
    }
};

struct GameStatistics 
{
    int gamesPlayed;
    int gamesWon;
    int highestScore;
    int totalMoves;
    int totalScore;
    
    GameStatistics() : gamesPlayed(0), gamesWon(0), highestScore(0), totalMoves(0), totalScore(0) {}
    
    void addGame(int score, int moves, bool won) 
    {
        gamesPlayed++;
        if (won) gamesWon++;
        if (score > highestScore) highestScore=score;
        totalMoves+=moves;
        totalScore+=score;
    }
};


class SolitaireGame 
{
public:
    // ... (member variables declared here - see part 4)
    
    void loadTextures() 
    {
        gameFont=GetFontDefault();
        
        loadCardBackTexture();
        
        bool allCardsLoaded = true;
        int idx = 0;
        
        // Map suits to folder names
        pair<char, string> suits[] = {
            {'H', "hearts"},
            {'D', "diamonds"},
            {'S', "spades"},
            {'C', "clubs"}
        };
        
        // Map ranks to file names
        pair<int, string> ranks[] = {
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
        for (int s = 0; s < 4; s++) {
            for (int r = 0; r < 13; r++) {
                string filename = "cards/" + ranks[r].second + "_of_" + suits[s].second + ".png";
                
                if (fs::exists(filename)) {
                    Image img = LoadImage(filename.c_str());
                    if (img.data) {
                        // Resize to match current card dimensions
                        ImageResize(&img, cw, ch);
                        cardTextures[idx] = LoadTextureFromImage(img);
                        UnloadImage(img);
                    } else {
                        allCardsLoaded = false;
                        createFallbackCardTexture(idx, ranks[r].first, suits[s].first);
                    }
                } else {
                    allCardsLoaded = false;
                    createFallbackCardTexture(idx, ranks[r].first, suits[s].first);
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
    
    void drawButton(Button& button, float fontSize = 24.0f) {
        Color btnColor = button.hovered ? button.hoverColor : button.color;
        
        // Draw button background with rounded corners
        DrawRectangleRounded(button.rect, 0.3f, 10, btnColor);
        
        // Draw button border
        DrawRectangleLinesEx(button.rect, 2.0f, WHITE);
        
        // Draw button text
        int textWidth = MeasureText(button.text.c_str(), fontSize);
        DrawText(button.text.c_str(), 
                button.rect.x + button.rect.width/2 - textWidth/2,
                button.rect.y + button.rect.height/2 - fontSize/2,
                fontSize, WHITE);
    }
    
    void updateButtonHover(Queue<Button>& buttons, Vector2 mousePos) {
        Queue<Button> tempButtons(buttons.gsize());
        int buttonSize = buttons.gsize();
        for (int i = 0; i < buttonSize; i++) {
            Button btn = buttons.valueat(i);
            btn.hovered = CheckCollisionPointRec(mousePos, btn.rect);
            tempButtons.enqueue(btn);
        }
        buttons = tempButtons;
    }
    
    void renderLevelSelect() {
        // Dark blue gradient background
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                               Color{20, 40, 80, 255}, Color{10, 20, 40, 255});
        
        // Draw title with shadow effect
        int screenWidth = GetScreenWidth();
        
        // Main title
        DrawText("KLONDIKE SOLITAIRE", screenWidth/2 - MeasureText("KLONDIKE SOLITAIRE", 60)/2, 80, 60, GOLD);
        DrawText("SELECT DIFFICULTY", screenWidth/2 - MeasureText("SELECT DIFFICULTY", 48)/2, 160, 48, SKYBLUE);
        
        // Draw decorative line under title
        DrawLine(screenWidth/2 - 150, 230, screenWidth/2 + 150, 230, Fade(GOLD, 0.5f));
        
        // Update button hover states
        Vector2 mousePos = GetMousePosition();
        updateButtonHover(levelButtons, mousePos);
        
        // Draw all level buttons
        int levelButtonSize = levelButtons.gsize();
        for (int i = 0; i < levelButtonSize; i++) {
            Button btn = levelButtons.valueat(i);
            drawButton(btn);
        }
        
        // Draw difficulty descriptions
        int descY = 550;
        DrawText("EASY: Draw 1 card at a time, unlimited recycles", screenWidth/2 - 250, descY, 18, LIGHTGRAY);
        DrawText("MEDIUM: Draw 3 cards at a time, unlimited recycles", screenWidth/2 - 260, descY + 30, 18, LIGHTGRAY);
        DrawText("HARD: Draw 3 cards, only 3 recycles allowed", screenWidth/2 - 220, descY + 60, 18, LIGHTGRAY);
    }
};
int main(){
  return 0;
}