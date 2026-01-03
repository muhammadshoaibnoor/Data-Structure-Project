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

    }
    void shuffledeck()
    {

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

    }
    bool isGameLost()
    {

    }
    void move_tableau_to_foundation(LinkedList<card>& tableau)
    {

    }
    void move_waste_to_foundation()
    {

    }
};    
