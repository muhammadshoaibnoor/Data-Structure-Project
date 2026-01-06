#include <iostream>
using namespace std;

template <class T>
struct Node
{
    T data;
    Node* next;
};
template <class T>
class LinkedList
{
    Node<T>* head;
    int count;

public:
    LinkedList()
    {
        head = nullptr;
        count = 0;
    }

    LinkedList(int s)
    {
        head = nullptr;
        count = 0;
    }

    LinkedList(const LinkedList& other)
    {
        head = nullptr;
        count = 0;
        if (other.head == nullptr)
            return;

        Node<T>* tempOther = other.head;
        head = new Node<T>;
        head->data = tempOther->data;
        head->next = nullptr;
        count++;

        Node<T>* current = head;
        tempOther = tempOther->next;

        while (tempOther != nullptr)
        {
            current->next = new Node<T>;
            current = current->next;
            current->data = tempOther->data;
            current->next = nullptr;
            tempOther = tempOther->next;
            count++;
        }
    }

    void operator=(const LinkedList& other)
    {
        if (this == &other)
            return;

        T dummy;
        while (!isEmpty())
        {
            deleteFromHead(dummy);
        }

        if (other.head == nullptr)
        {
            head = nullptr;
            count = 0;
            return;
        }

        Node<T>* tempOther = other.head;
        head = new Node<T>;
        head->data = tempOther->data;
        head->next = nullptr;
        count++;

        Node<T>* current = head;
        tempOther = tempOther->next;

        while (tempOther != nullptr)
        {
            current->next = new Node<T>;
            current = current->next;
            current->data = tempOther->data;
            current->next = nullptr;
            tempOther = tempOther->next;
            count++;
        }
    }

    bool insertAtHead(T n)
    {
        Node<T>* newNode = new Node<T>;
        newNode->data = n;
        newNode->next = head;
        head = newNode;
        count++;
        return true;
    }

    bool deleteFromHead(T& v)
    {
        if (!isEmpty())
        {
            Node<T>* temp = head;
            v = temp->data;
            head = head->next;
            delete temp;
            count--;
            return true;
        }
        return false;
    }

    void clear() {
        T dummy;
        while (!isEmpty()) {
            deleteFromHead(dummy);
        }
    }

    T peek()
    {
        if (!isEmpty())
        {
            return head->data;
        }
        else
        {
            return T();
        }
    }

    bool isEmpty()
    {
        return head == nullptr;
    }

    int getCount() { return count; }

    Node<T>* getHead() { return head; }

    ~LinkedList()
    {
        clear();
    }
};
class card
{
public:
    int suit;
    int rankk;
    string color;
    bool faceup;
    card() {}

    ~card() {}
};
class Deck
{
public:
    LinkedList<card> tableau1;
    LinkedList<card> tableau2;
    LinkedList<card> tableau3;
    LinkedList<card> tableau4;
    LinkedList<card> tableau5;
    LinkedList<card> tableau6;
    LinkedList<card> tableau7;
    LinkedList<card> stock;
    LinkedList<card> waste;
    LinkedList<card> foundation1{ 13 };
    LinkedList<card> foundation2{ 13 };
    LinkedList<card> foundation3{ 13 };
    LinkedList<card> foundation4{ 13 };
    Deck() {}
    void createdeck()
    {
        card c;
for (int i = 0; i < 4; i++)
{
    for (int j = 1; j < 14; j++)
    {
        c.suit = i;
        c.rankk = j;
        c.faceup = true;
        if (i == 0 || i == 1) c.color = "red";
        else c.color = "black";
        stock.insertAtHead(c);
    }
}
    }
    void shuffledeck()
    {
        srand(time(0));
card temp[52];
int index = 0;
while (!stock.isEmpty() && index < 52)
{
    stock.deleteFromHead(temp[index]);
    index++;
}
for (int i = 0; i < 52; i++)
{
    int j = rand() % 52;
    card tempa = temp[i];
    temp[i] = temp[j];
    temp[j] = tempa;
}
for (int i = 0; i < 52; i++)
{
    stock.insertAtHead(temp[i]);
}
    }
    void dealtotableau(LinkedList<card>& tableau, int j)
{
    card c;
    for (int i = 1; i <= j; i++)
    {
        stock.deleteFromHead(c);
        if (i == j) c.faceup = true;
        else c.faceup = false;
        tableau.insertAtHead(c);
    }
}
    void dealtableau()
{
    dealtotableau(tableau1, 1);
    dealtotableau(tableau2, 2);
    dealtotableau(tableau3, 3);
    dealtotableau(tableau4, 4);
    dealtotableau(tableau5, 5);
    dealtotableau(tableau6, 6);
    dealtotableau(tableau7, 7);
}
    void drawtowastee()
{
    card c;
    if (stock.isEmpty())
    {
        recyclewastetostock();
        if (stock.isEmpty()) return;
    }
    if (stock.deleteFromHead(c))
    {
        c.faceup = true;
        waste.insertAtHead(c);
    }
}
    void recyclewastetostock()
{
    if (waste.isEmpty()) return;
    card c;
    LinkedList<card> temp;
    while (!waste.isEmpty())
    {
        waste.deleteFromHead(c);
        c.faceup = false;
        temp.insertAtHead(c);
    }
    while (!temp.isEmpty())
    {
        temp.deleteFromHead(c);
        stock.insertAtHead(c);
    }
}
bool can_move_to_foundation(LinkedList<card>& foundation, card c)
{
    if (foundation.isEmpty()) return c.rankk == 1;
    card top = foundation.peek();
    return (top.suit == c.suit && c.rankk == top.rankk + 1);
}

bool can_move_to_tableau(LinkedList<card>& tableau, card c)
{
    if (tableau.isEmpty()) return c.rankk == 13;
    card top = tableau.peek();
    return (c.rankk == top.rankk - 1 && c.color != top.color);
}
    void flip_tableau_top(LinkedList<card>& tableau)
{
    if (!tableau.isEmpty())
    {
        card top = tableau.peek();
        if (!top.faceup)
        {
            tableau.deleteFromHead(top);
            top.faceup = true;
            tableau.insertAtHead(top);
        }
    }
}
    bool check_win()
    {
        return (foundation1.getCount() == 13 && foundation2.getCount() == 13 &&
    foundation3.getCount() == 13 && foundation4.getCount() == 13);
    }
    bool isGameLost()
    {
        return stock.isEmpty() && waste.isEmpty() && !hasLegalMoves() && !check_win();
    }
    void move_tableau_to_foundation(LinkedList<card>& tableau)
    {
        if (tableau.isEmpty()) return;
card c = tableau.peek();
LinkedList<card>* foundation = nullptr;
if (c.suit == 0) foundation = &foundation1;
else if (c.suit == 1) foundation = &foundation2;
else if (c.suit == 2) foundation = &foundation3;
else if (c.suit == 3) foundation = &foundation4;

if (foundation && can_move_to_foundation(*foundation, c))
{
    tableau.deleteFromHead(c);
    foundation->insertAtHead(c);
    flip_tableau_top(tableau);
}
    }
    void move_waste_to_foundation()
    {
        if (waste.isEmpty()) return;
card c = waste.peek();
LinkedList<card>* foundation = nullptr;
if (c.suit == 0) foundation = &foundation1;
else if (c.suit == 1) foundation = &foundation2;
else if (c.suit == 2) foundation = &foundation3;
else if (c.suit == 3) foundation = &foundation4;

if (foundation && can_move_to_foundation(*foundation, c))
{
    waste.deleteFromHead(c);
    foundation->insertAtHead(c);
}
    }
bool hasLegalMoves()
{
    LinkedList<card>* tableaus[] = { &tableau1, &tableau2, &tableau3, &tableau4, &tableau5, &tableau6, &tableau7 };
    LinkedList<card>* foundations[] = { &foundation1, &foundation2, &foundation3, &foundation4 };

    if (!waste.isEmpty())
    {
        card w = waste.peek();
        for (int i = 0; i < 4; i++) if (can_move_to_foundation(*foundations[i], w)) return true;
        for (int i = 0; i < 7; i++) if (can_move_to_tableau(*tableaus[i], w)) return true;
    }

    for (int i = 0; i < 7; i++)
    {
        if (tableaus[i]->isEmpty()) continue;
        Node<card>* curr = tableaus[i]->getHead();
        while (curr && curr->data.faceup)
        {
            card sequenceLead = curr->data;
            if (curr == tableaus[i]->getHead()) {
                for (int f = 0; f < 4; f++) if (can_move_to_foundation(*foundations[f], sequenceLead)) return true;
            }
            for (int j = 0; j < 7; j++)
            {
                if (i == j) continue;
                if (can_move_to_tableau(*tableaus[j], sequenceLead)) return true;
            }
            curr = curr->next;
        }
    }
    return false;
}

};    
struct DeckState {
    LinkedList<card> stock, waste;
    LinkedList<card> t1, t2, t3, t4, t5, t6, t7;
    LinkedList<card> f1, f2, f3, f4;
    int moves, hints;
    float time;
};

struct Hint {
    int srcType, srcIdx, destType, destIdx;
};

class Game
{
public:
    const int ORIGINAL_WIDTH = 800;
    const int ORIGINAL_HEIGHT = 600;
    int screenWidth = 1000;
    int screenHeight = 700;
    float scaleX, scaleY, minScale;

    const int BASE_CARD_WIDTH = 71;
    const int BASE_CARD_HEIGHT = 96;
    float scaledCardWidth, scaledCardHeight;
    float scaledCardSpacing, scaledTableauOffset;

    Deck game;
    Texture2D cardTextures[53];
    Texture2D cardBack, background;
    Sound cardFlip, cardPlace, cardSlide, click, hintSnd, winSnd, lossSnd;
    Music bgm;

    bool isDragging = false;
    LinkedList<card> draggedCards;
    Vector2 dragOffset;
    int sourcePileType = -1, sourcePileIndex = -1;

    enum GameState { MAIN_MENU, DIFF_SELECT, INSTRUCTIONS, PLAYING, WON, LOST, SETTINGS };
    GameState state = MAIN_MENU;
    int remainingHints = 0, hintsPerUse = 0;
    string difficultyStr = "";

    int moveCount = 0;
    int bestMoveCount = -1;

    LinkedList<Hint> activeHints;
    float hintTimer = 0.0f;

    float remainingSeconds = 0.0f;
    bool timeExpired = false;

    LinkedList<DeckState> undoStack;
    bool musicOn = true;
    bool soundsOn = true;

    Game() {
        scaleX = (float)screenWidth / ORIGINAL_WIDTH;
        scaleY = (float)screenHeight / ORIGINAL_HEIGHT;
        minScale = (scaleX < scaleY) ? scaleX : scaleY;

        scaledCardWidth = BASE_CARD_WIDTH * scaleX;
        scaledCardHeight = BASE_CARD_HEIGHT * scaleY;
        scaledCardSpacing = 100 * scaleX;
        scaledTableauOffset = 25 * scaleY;
        loadSettings();
    }
