#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>
#include "raylib.h"
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
int getCardImageIndex()
{
    return suit * 13 + rankk;
}
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
void loadSettings() {
    ifstream file("settings.txt");
    if (file.is_open()) {
        file >> musicOn >> soundsOn;
        file.close();
    }
}

void saveSettings() {
    ofstream file("settings.txt");
    if (file.is_open()) {
        file << musicOn << " " << soundsOn;
        file.close();
    }
}

void saveStateForUndo() {
    DeckState ds;
    ds.stock = game.stock; ds.waste = game.waste;
    ds.t1 = game.tableau1; ds.t2 = game.tableau2; ds.t3 = game.tableau3;
    ds.t4 = game.tableau4; ds.t5 = game.tableau5; ds.t6 = game.tableau6; ds.t7 = game.tableau7;
    ds.f1 = game.foundation1; ds.f2 = game.foundation2; ds.f3 = game.foundation3; ds.f4 = game.foundation4;
    ds.moves = moveCount; ds.hints = remainingHints; ds.time = remainingSeconds;
    undoStack.insertAtHead(ds);
    if (undoStack.getCount() > 4) {
        DeckState dummy;
        undoStack.deleteFromTail(dummy);
    }
}

void performUndo() {
    if (undoStack.isEmpty()) return;
    DeckState ds;
    undoStack.deleteFromHead(ds);
    game.stock = ds.stock; game.waste = ds.waste;
    game.tableau1 = ds.t1; game.tableau2 = ds.t2; game.tableau3 = ds.t3;
    game.tableau4 = ds.t4; game.tableau5 = ds.t5; game.tableau6 = ds.t6; game.tableau7 = ds.t7;
    game.foundation1 = ds.f1; game.foundation2 = ds.f2; game.foundation3 = ds.f3; game.foundation4 = ds.f4;
    moveCount = ds.moves; remainingHints = ds.hints; remainingSeconds = ds.time;
    if (soundsOn) PlaySound(click);
}

void loadAssets()
{
    Image bgImg = FileExists("bg.png") ? LoadImage("bg.png") : GenImageColor(screenWidth, screenHeight, DARKGREEN);
    ImageResize(&bgImg, screenWidth, screenHeight);
    background = LoadTextureFromImage(bgImg); UnloadImage(bgImg);

    Image backImg = FileExists("back.png") ? LoadImage("back.png") : GenImageColor(BASE_CARD_WIDTH, BASE_CARD_HEIGHT, BLUE);
    ImageResize(&backImg, (int)scaledCardWidth, (int)scaledCardHeight);
    cardBack = LoadTextureFromImage(backImg); UnloadImage(backImg);

    for (int i = 1; i <= 52; i++) {
        string fn = to_string(i) + ".png";
        Image cImg = FileExists(fn.c_str()) ? LoadImage(fn.c_str()) : GenImageColor(BASE_CARD_WIDTH, BASE_CARD_HEIGHT, WHITE);
        ImageResize(&cImg, (int)scaledCardWidth, (int)scaledCardHeight);
        cardTextures[i] = LoadTextureFromImage(cImg); UnloadImage(cImg);
    }
    InitAudioDevice();
    cardFlip = LoadSound("card_flip.wav");
    cardPlace = LoadSound("card_place.wav");
    cardSlide = LoadSound("card_slide.wav");
    click = LoadSound("click.wav");
    hintSnd = LoadSound("hint.wav");
    winSnd = LoadSound("win.wav");
    lossSnd = LoadSound("lose.wav");
    bgm = LoadMusicStream("background_music.mp3");
    if (musicOn) PlayMusicStream(bgm);
    SetMusicVolume(bgm, 0.4f);
}

void unloadAssets()
{
    UnloadTexture(background); UnloadTexture(cardBack);
    for (int i = 1; i <= 52; i++) UnloadTexture(cardTextures[i]);
    UnloadSound(cardFlip); UnloadSound(cardPlace); UnloadSound(cardSlide);
    UnloadSound(click); UnloadSound(hintSnd); UnloadSound(winSnd); UnloadSound(lossSnd);
    UnloadMusicStream(bgm); CloseAudioDevice();
}

void serializePile(ofstream& file, string label, LinkedList<card>& pile) {
    file << label << " " << pile.getCount() << "\n";
    Node<card>* curr = pile.getHead();
    while (curr) {
        file << curr->data.suit << " " << curr->data.rankk << " "
            << curr->data.color << " " << curr->data.faceup << "\n";
        curr = curr->next;
    }
}
 void saveGameState() {
        ofstream file("solitaire_save.txt");
        if (file.is_open()) {
            file << moveCount << " " << difficultyStr << " " << remainingSeconds << "\n";
            serializePile(file, "STOCK", game.stock);
            serializePile(file, "WASTE", game.waste);
            serializePile(file, "T1", game.tableau1);
            serializePile(file, "T2", game.tableau2);
            serializePile(file, "T3", game.tableau3);
            serializePile(file, "T4", game.tableau4);
            serializePile(file, "T5", game.tableau5);
            serializePile(file, "T6", game.tableau6);
            serializePile(file, "T7", game.tableau7);
            serializePile(file, "F1", game.foundation1);
            serializePile(file, "F2", game.foundation2);
            serializePile(file, "F3", game.foundation3);
            serializePile(file, "F4", game.foundation4);
            file.close();
        }
    }
   
    void deserializePile(ifstream& file, LinkedList<card>& pile) {
        string label; int count;
        file >> label >> count;
        pile.clear();
        for (int i = 0; i < count; i++) {
            card c; string color; int face;
            file >> c.suit >> c.rankk >> color >> face;
            c.color = color;
            c.faceup = (face != 0);
            pile.insertAtTail(c);
        }
    }
    
    bool loadGameState() {
        ifstream file("solitaire_save.txt");
        if (file.is_open()) {
            file >> moveCount >> difficultyStr >> remainingSeconds;
            deserializePile(file, game.stock);
            deserializePile(file, game.waste);
            deserializePile(file, game.tableau1);
            deserializePile(file, game.tableau2);
            deserializePile(file, game.tableau3);
            deserializePile(file, game.tableau4);
            deserializePile(file, game.tableau5);
            deserializePile(file, game.tableau6);
            deserializePile(file, game.tableau7);
            deserializePile(file, game.foundation1);
            deserializePile(file, game.foundation2);
            deserializePile(file, game.foundation3);
            deserializePile(file, game.foundation4);
            file.close();

            if (difficultyStr == "Easy") { remainingHints = 3; hintsPerUse = 3; }
            else if (difficultyStr == "Medium") { remainingHints = 2; hintsPerUse = 2; }
            else { remainingHints = 1; hintsPerUse = 1; }
            loadBestScore(difficultyStr);
            timeExpired = (remainingSeconds <= 0);
            undoStack.clear();
            return true;
        }
        return false;
    }

    void loadBestScore(string diff) {
        string filename = (diff == "Easy") ? "easy.txt" : (diff == "Medium" ? "medium.txt" : "hard.txt");
        ifstream file(filename.c_str());
        if (file.is_open()) { file >> bestMoveCount; file.close(); }
        else bestMoveCount = -1;
    }

    void saveBestScore() {
        if (bestMoveCount == -1 || moveCount < bestMoveCount) {
            string filename = (difficultyStr == "Easy") ? "easy.txt" : (difficultyStr == "Medium" ? "medium.txt" : "hard.txt");
            ofstream file(filename.c_str());
            if (file.is_open()) { file << moveCount; file.close(); bestMoveCount = moveCount; }
        }
    }
void setDifficulty(string diff) {
    difficultyStr = diff;
    timeExpired = false;
    if (diff == "Easy") { remainingHints = 3; hintsPerUse = 3; remainingSeconds = 1200.0f; }
    else if (diff == "Medium") { remainingHints = 2; hintsPerUse = 2; remainingSeconds = 900.0f; }
    else { remainingHints = 1; hintsPerUse = 1; remainingSeconds = 480.0f; }
    moveCount = 0;
    loadBestScore(diff);
    game = Deck(); game.createdeck(); game.shuffledeck(); game.dealtableau();
    saveGameState();
    undoStack.clear();
    state = PLAYING;
}
void provideHint() {
    if (remainingHints <= 0 || isHintAnimating) return;

    Hint possible[100];
    int possibleCount = 0;
    LinkedList<card>* t[] = { &game.tableau1, &game.tableau2, &game.tableau3, &game.tableau4, &game.tableau5, &game.tableau6, &game.tableau7 };
    LinkedList<card>* f[] = { &game.foundation1, &game.foundation2, &game.foundation3, &game.foundation4 };

    if (!game.waste.isEmpty()) {
        card w = game.waste.peek();
        for (int i = 0; i < 4; i++) if (game.can_move_to_foundation(*f[i], w)) possible[possibleCount++] = { 0, 0, 2, i, 1 };
        for (int i = 0; i < 7; i++) if (game.can_move_to_tableau(*t[i], w)) possible[possibleCount++] = { 0, 0, 1, i, 1 };
    }

    for (int i = 0; i < 7; i++) {
        if (t[i]->isEmpty()) continue;
        Node<card>* curr = t[i]->getHead();
        int currentDepth = 1;
        while (curr && curr->data.faceup) {
            if (curr == t[i]->getHead()) {
                for (int j = 0; j < 4; j++) if (game.can_move_to_foundation(*f[j], curr->data)) possible[possibleCount++] = { 1, i, 2, j, 1 };
            }
            for (int j = 0; j < 7; j++) if (i != j && game.can_move_to_tableau(*t[j], curr->data)) possible[possibleCount++] = { 1, i, 1, j, currentDepth };
            curr = curr->next;
            currentDepth++;
        }
    }

    if (possibleCount == 0) {
        if (!game.stock.isEmpty() || !game.waste.isEmpty()) {
            stockHighlightTimer = 5.0f;
            remainingHints--;
            if (soundsOn) PlaySound(hintSnd);
        }
        return;
    }

    Hint h = possible[rand() % possibleCount];
    hintAnimatedCards.clear();

    hintStartPos.x = (h.srcType == 0) ? 150 * scaleX : (50 + h.srcIdx * 100) * scaleX;
    hintEndPos.x = (h.destType == 2) ? (350 + h.destIdx * 100) * scaleX : (50 + h.destIdx * 100) * scaleX;

    if (h.srcType == 0) {
        hintStartPos.y = 30 * scaleY;
        hintAnimatedCards.insertAtHead(game.waste.peek());
    }
    else {
        int totalCount = t[h.srcIdx]->getCount();
        hintStartPos.y = (180 * scaleY) + (totalCount - h.cardCount) * scaledTableauOffset;
        Node<card>* node = t[h.srcIdx]->getHead();
        for (int k = 0; k < h.cardCount; k++) {
            hintAnimatedCards.insertAtTail(node->data);
            node = node->next;
        }
    }

    if (h.destType == 2) hintEndPos.y = 30 * scaleY;
    else {
        int destCount = t[h.destIdx]->getCount();
        hintEndPos.y = (180 * scaleY) + (destCount == 0 ? 0 : (destCount - 1) * scaledTableauOffset);
    }

    isHintAnimating = true;
    hintAnimTimer = 2.0f;
    remainingHints--;
    if (soundsOn) PlaySound(hintSnd);
}
void drawCard(card c, float x, float y) {
    if (!c.faceup) DrawTexture(cardBack, (int)x, (int)y, WHITE);
    else DrawTexture(cardTextures[c.getCardImageIndex()], (int)x, (int)y, WHITE);
}

void drawPile(LinkedList<card>& pile, float x, float y, float offset = 0) {
    if (pile.isEmpty()) { DrawRectangleLines((int)x, (int)y, (int)scaledCardWidth, (int)scaledCardHeight, Fade(WHITE, 0.6f)); return; }
    LinkedList<card> temp;
    Node<card>* curr = pile.getHead();
    while (curr) { temp.insertAtHead(curr->data); curr = curr->next; }
    curr = temp.getHead();
    int i = 0;
    while (curr) { drawCard(curr->data, x, y + i * offset); curr = curr->next; i++; }
}

bool drawButton(Rectangle rect, const char* text, Color color) {
    bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    DrawRectangleRec(rect, hovered ? ColorBrightness(color, 0.2f) : color);
    DrawRectangleLinesEx(rect, 2, hovered ? WHITE : GRAY);
    int fontSize = (int)(20 * minScale);
    int textW = MeasureText(text, fontSize);
    DrawText(text, (int)(rect.x + (rect.width - textW) / 2), (int)(rect.y + (rect.height - fontSize) / 2), fontSize, RAYWHITE);
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
void handleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (state == PLAYING) { saveGameState(); state = MAIN_MENU; return; }
        if (state == DIFF_SELECT || state == INSTRUCTIONS || state == SETTINGS) { state = MAIN_MENU; return; }
    }

    if (state != PLAYING) return;

    remainingSeconds -= GetFrameTime();
    if (remainingSeconds <= 0) {
        remainingSeconds = 0;
        timeExpired = true;
        state = LOST;
        if (soundsOn) PlaySound(lossSnd);
        return;
    }

    if (isHintAnimating) return;

    Vector2 mPos = GetMousePosition();
    LinkedList<card>* t[] = { &game.tableau1, &game.tableau2, &game.tableau3, &game.tableau4, &game.tableau5, &game.tableau6, &game.tableau7 };
    LinkedList<card>* f[] = { &game.foundation1, &game.foundation2, &game.foundation3, &game.foundation4 };

    if (IsKeyPressed(KEY_H)) provideHint();
    if (IsKeyPressed(KEY_U)) performUndo();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mPos, { 50 * scaleX, 30 * scaleY, scaledCardWidth, scaledCardHeight })) {
            saveStateForUndo(); game.drawtowastee(); moveCount++; if (soundsOn) PlaySound(click); saveGameState(); return;
        }
        if (!game.waste.isEmpty() && CheckCollisionPointRec(mPos, { 150 * scaleX, 30 * scaleY, scaledCardWidth, scaledCardHeight })) {
            saveStateForUndo();
            card c; game.waste.deleteFromHead(c); draggedCards.insertAtHead(c);
            isDragging = true; sourcePileType = 0; dragOffset = { mPos.x - 150 * scaleX, mPos.y - 30 * scaleY };
            if (soundsOn) PlaySound(cardSlide); return;
        }
        for (int i = 0; i < 4; i++) {
            float x = (350 + i * 100) * scaleX;
            if (!f[i]->isEmpty() && CheckCollisionPointRec(mPos, { x, 30 * scaleY, scaledCardWidth, scaledCardHeight })) {
                saveStateForUndo();
                card c; f[i]->deleteFromHead(c); draggedCards.insertAtHead(c);
                isDragging = true; sourcePileType = 2; sourcePileIndex = i;
                dragOffset = { mPos.x - x, mPos.y - 30 * scaleY }; if (soundsOn) PlaySound(cardSlide); return;
            }
        }
        for (int i = 0; i < 7; i++) {
            float x = (50 + i * 100) * scaleX;
            int count = t[i]->getCount();
            for (int j = 0; j < count; j++) {
                float cardY = (180 * scaleY) + (count - 1 - j) * scaledTableauOffset;
                if (CheckCollisionPointRec(mPos, { x, cardY, scaledCardWidth, (j == 0) ? scaledCardHeight : scaledTableauOffset })) {
                    Node<card>* curr = t[i]->getHead(); for (int k = 0; k < j; k++) curr = curr->next;
                    if (!curr->data.faceup) break;
                    bool validSeq = true; Node<card>* check = t[i]->getHead();
                    for (int k = 0; k < j; k++) {
                        card c1 = check->data; card c2 = check->next->data;
                        if (c1.color == c2.color || c1.rankk != c2.rankk - 1) { validSeq = false; break; }
                        check = check->next;
                    }
                    if (!validSeq) break;
                    saveStateForUndo();
                    for (int k = 0; k <= j; k++) { card c; t[i]->deleteFromHead(c); draggedCards.insertAtHead(c); }
                    isDragging = true; sourcePileType = 1; sourcePileIndex = i;
                    dragOffset = { mPos.x - x, mPos.y - cardY }; if (soundsOn) PlaySound(cardSlide); return;
                }
            }
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isDragging) {
        bool moved = false; card bottomCard = draggedCards.peek();
        if (draggedCards.getCount() == 1) {
            for (int i = 0; i < 4; i++) {
                float fX = (350 + i * 100) * scaleX;
                if (CheckCollisionPointRec(mPos, { fX, 30 * scaleY, scaledCardWidth, scaledCardHeight })) {
                    if (game.can_move_to_foundation(*f[i], bottomCard)) {
                        card c; draggedCards.deleteFromHead(c); f[i]->insertAtHead(c);
                        moved = true; moveCount++; break;
                    }
                }
            }
        }
        if (!moved) {
            for (int i = 0; i < 7; i++) {
                float x = (50 + i * 100) * scaleX;
                float y = (180 * scaleY) + (t[i]->isEmpty() ? 0 : (t[i]->getCount() - 1) * scaledTableauOffset);
                if (CheckCollisionPointRec(mPos, { x, y, scaledCardWidth, scaledCardHeight * 2 })) {
                    if (game.can_move_to_tableau(*t[i], bottomCard)) {
                        card c; while (!draggedCards.isEmpty()) { draggedCards.deleteFromHead(c); t[i]->insertAtHead(c); }
                        moved = true; moveCount++; break;
                    }
                }
            }
        }
        if (!moved) {
            DeckState dummy; undoStack.deleteFromHead(dummy);
            card c;
            while (!draggedCards.isEmpty()) {
                draggedCards.deleteFromHead(c);
                if (sourcePileType == 0) game.waste.insertAtHead(c);
                else if (sourcePileType == 1) t[sourcePileIndex]->insertAtHead(c);
                else f[sourcePileIndex]->insertAtHead(c);
            }
            if (soundsOn) PlaySound(click);
        }
        else {
            if (sourcePileType == 1) {
                int prevCount = t[sourcePileIndex]->getCount();
                game.flip_tableau_top(*t[sourcePileIndex]);
                if (t[sourcePileIndex]->getCount() == prevCount && soundsOn) PlaySound(cardFlip);
            }
            if (soundsOn) PlaySound(cardPlace); saveGameState();
        }
        isDragging = false;
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (CheckCollisionPointRec(mPos, { 150 * scaleX, 30 * scaleY, scaledCardWidth, scaledCardHeight })) {
            saveStateForUndo();
            int before = game.foundation1.getCount() + game.foundation2.getCount() + game.foundation3.getCount() + game.foundation4.getCount();
            game.move_waste_to_foundation();
            if (game.foundation1.getCount() + game.foundation2.getCount() + game.foundation3.getCount() + game.foundation4.getCount() > before) {
                moveCount++; saveGameState(); if (soundsOn) PlaySound(cardPlace);
            }
            else { DeckState dummy; undoStack.deleteFromHead(dummy); }
        }
        for (int i = 0; i < 7; i++) {
            float x = (50 + i * 100) * scaleX;
            float h = t[i]->isEmpty() ? scaledCardHeight : scaledCardHeight + (t[i]->getCount() - 1) * scaledTableauOffset;
            if (CheckCollisionPointRec(mPos, { x, 180 * scaleY, scaledCardWidth, h })) {
                saveStateForUndo();
                int before = game.foundation1.getCount() + game.foundation2.getCount() + game.foundation3.getCount() + game.foundation4.getCount();
                game.move_tableau_to_foundation(*t[i]);
                if (game.foundation1.getCount() + game.foundation2.getCount() + game.foundation3.getCount() + game.foundation4.getCount() > before) {
                    moveCount++; saveGameState(); if (soundsOn) PlaySound(cardPlace);
                }
                else { DeckState dummy; undoStack.deleteFromHead(dummy); }
            }
        }
    }
    if (!isDragging) {
        if (game.check_win()) {
            state = WON;
            saveBestScore();
            if (soundsOn) PlaySound(winSnd);
        }
        else if (game.isGameLost()) {
            state = LOST;
            if (soundsOn) PlaySound(lossSnd);
        }
    }
}
