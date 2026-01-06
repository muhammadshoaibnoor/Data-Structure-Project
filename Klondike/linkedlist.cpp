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

    }
    void dealtableau()
    {

    }
    void drawtowastee()
    {

    }
    void recyclewastetostock()
    {

    }
    bool can_move_to_foundation(LinkedList<card>& foundation, card c)
    {

    }
    bool can_move_to_tableau(LinkedList<card>& tableau, card c)
    {

    }
    void flip_tableau_top(LinkedList<card>& tableau)
    {

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
