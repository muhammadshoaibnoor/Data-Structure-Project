#include <iostream>
#include <ctime>
#include <string>
#include <filesystem>
#include "raylib.h"

#include <cstring>

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
// ============================================================================
// PART 2: Game Rules and Tableau Management
// ============================================================================

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
int main(){
  return 0;
}