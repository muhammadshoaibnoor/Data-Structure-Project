#include <iostream>
using namespace std;

class node {
public:
    int data;
    node* prev;
    node* next;

    node(int value) {
        data = value;
        prev = NULL;
        next = NULL;
    }
};

class list {
private:
    node* head;
    node* tail;

public:
    list() {
        head = NULL;
        tail = NULL;
    }

    void insert_begin(int value) {
        node* newnode = new node(value);
        if (head == NULL) {
            head = tail = newnode;
        } else {
            newnode->next = head;
            head->prev = newnode;
            head = newnode;
        }
    }

    void insert_end(int value) {
        node* newnode = new node(value);
        if (tail == NULL) {
            head = tail = newnode;
        } else {
            tail->next = newnode;
            newnode->prev = tail;
            tail = newnode;
        }
    }

    void insert_pos(int value, int pos) {
        if (pos <= 1) {
            insert_begin(value);
            return;
        }

        node* current = head;
        for (int index = 1; index < pos - 1 && current != NULL; index++) {
            current = current->next;
        }

        if (current == NULL || current->next == NULL) {
            insert_end(value);
            return;
        }

        node* newnode = new node(value);
        newnode->next = current->next;
        newnode->prev = current;
        current->next->prev = newnode;
        current->next = newnode;
    }

    void delete_begin() {
        if (head == NULL) return;

        node* temp = head;
        if (head == tail) {
            head = tail = NULL;
        } else {
            head = head->next;
            head->prev = NULL;
        }
        delete temp;
    }

    void delete_end() {
        








        
    }

    void delete_pos(int pos) {







    }

    void display_forward() {







    }

    void display_backward() {







    }

    void search_value(int value) {







    }
};

int main() {
    list linkedlist;
    return 0;
}
