#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
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
    stackk (stackk & other)
{
    sizee=other.sizee;
    top=other.top;
    ptr = new T [sizee];
    for (int i=0; i<=top; i++)
    {
       ptr[i]=other.ptr[i];
    }

}
void operator=(stackk & other)
 {
    sizee=other.sizee;
    top=other.top;
    ptr = new T [sizee];
    for (int i=0; i<=top; i++)
    {
       ptr[i]=other.ptr[i];
    }
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
        else
        {return T();
        }
    }
    bool is_emptyy()
    {
        return top == -1;
    }
    bool is_full()
    {
        return top == sizee - 1;
    }
    ~stackk()
    {
        delete[] ptr;
    }
};


class card
{
public:
int suit;
int rankk;
string color;
bool faceup;
card ()
{
}
string intostring ()
{
if (!faceup)
    {
        return "XX";
    }
    string rankstring;
    string suitstring;
    if (rankk==1)
    {
        rankstring="A";
    }
    if (rankk==2)
    {
        rankstring="2";
    }
    if (rankk==3)
    {
        rankstring="3";
    }
    if (rankk==4)
    {
        rankstring="4";
    }
    if (rankk==5)
    {
        rankstring="5";
    }
    if (rankk==6)
    {
        rankstring="6";
    }
    if (rankk==7)
    {
        rankstring="7";
    }
    if (rankk==8)
    {
        rankstring="8";
    }
    if (rankk==9)
    {
        rankstring="9";
    }
    if (rankk==10)
    {
        rankstring="10";
    }
     if (rankk==11)
    {
        rankstring="J";
    }
     else if (rankk==12)
    {
        rankstring="Q";
    }
     else if (rankk==13)
    {
        rankstring="K";
    }
    if (suit==0)
    {
        suitstring="H";
    }
    else if (suit==1)
    {
        suitstring="D";
    }
    else if (suit==2)
    {
        suitstring="C";
    }
     else if (suit==3)
    {
        suitstring="S";
    }
    return rankstring+suitstring;
}
~card ()
{
}
};

class Deck
{
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
    Deck ()
    {
    }
    void createdeck()
{
    card c;
    for (int i=0; i<4; i++)
    {
        for (int j=1; j<14; j++)
        {
            c.suit=i;
            c.rankk=j;
            c.faceup=true;
            if (i==0 || i==1)
            {
                c.color="red";
            }
            else
            {
                c.color="black";
            }
            stock.push(c);
        }
    }
}
void shuffledeck ()
{
srand(time(0));
card temp[52];
int index=0;
while (!stock.is_emptyy() && index < 52)
{
    stock.pop(temp[index]);
    index++;
}
    for (int i=0; i<52; i++)
    {
        int j=rand()%52;
        card tempa=temp[i];
        temp[i]=temp[j];
        temp[j]=tempa;
    }
    for (int i=0; i<52; i++)
    {
        stock.push(temp[i]);
    }
}
 void dealtotableau (stackk<card>& tableau, int j)
    {
        card c;
        for (int i=1; i<=j ; i++)
        {
            stock.pop(c);
            if (i==j)
            {
             c.faceup=true;
            }
            else
            {
                c.faceup=false;
            }
            tableau.push(c);
        }
    }
    void dealtableau ()
    {
        dealtotableau(tableau1,1);

        dealtotableau(tableau2,2);

        dealtotableau(tableau3,3);

        dealtotableau(tableau4,4);

        dealtotableau(tableau5,5);

        dealtotableau(tableau6,6);

        dealtotableau(tableau7,7);
    }
void drawtowastee()
{
    card c;
    if (stock.is_emptyy())
    {
        recyclewastetostock();
        if (stock.is_emptyy())
        {
            cout << "Stock and waste are empty no card to draw." << endl;
            return;
        }
    }
    if (stock.pop(c))
    {
        c.faceup = true;
        waste.push(c);
        cout << "Top of waste: " << waste.topp().intostring() << endl;
    }
}
void recyclewastetostock()
{
    if (waste.is_emptyy())
    {
        return;
    }
    card c;
    stackk<card> temp;

    while (!waste.is_emptyy())
    {
        waste.pop(c);
        c.faceup = false;
        temp.push(c);
    }
    while (!temp.is_emptyy())
    {
        temp.pop(c);
        stock.push(c);
    }
    cout << "Card is recycled from waste to stock" << endl;
}

bool can_move_to_foundation (stackk <card>& foundation, card c )
{
    if (foundation.is_emptyy())
    {
     if (c.rankk==1)
     {
         return true;
     }
     else
     {
         return false;
     }
    }
    else
    {
     card top=foundation.topp();
     if (top.suit==c.suit && c.rankk==top.rankk+1)
     {
         return true;
     }
     else
     {
         return false;
     }
    }
}
bool can_move_to_tableau (stackk <card>& tableau, card c )
{
    if (tableau.is_emptyy())
    {
     if (c.rankk==13)
     {
         return true;
     }
     else
     {
         return false;
     }
    }
    else
    {
     card top=tableau.topp();
     if (c.rankk==top.rankk-1 && c.color!=top.color)
     {
         return true;
     }
     else
     {
         return false;
     }
    }
}
void flip_tableau_top (stackk <card> &tableau)
{
    if (!tableau.is_emptyy())
    {
        card top= tableau.topp();
        if (!top.faceup)
        {
            tableau.pop(top);
            top.faceup=true;
            tableau.push(top);
        }
    }
}
void move_to_tableau(stackk<card>& tableau, stackk<card>& source, bool source_is_tableau)
{
    if (source.is_emptyy())
    {
        cout<<"Source is empty card cannot move to tableau"<<endl;
        return;
    }
    card c = source.topp();
    if (can_move_to_tableau(tableau, c))
    {
        source.pop(c);
        tableau.push(c);
        cout << c.intostring() << " moved to Tableau"<<endl;
        if (source_is_tableau)
        {
            flip_tableau_top(source);
        }
    }
    else
    {
        cout << "Cannot move "<< c.intostring()<<" to Tableau"<<endl;
    }
}

 void move_tableau_to_tableau (stackk<card>& tableau, stackk<card>& source)
{
    if (tableau.is_emptyy())
    {
        cout<<"Tableau is empty"<<endl;
        return;
    }
    move_to_tableau(tableau, source, true);
}

bool can_move_sequence_to_tableau(stackk<card>& tableau, stackk<card> sequence)
{
    if (sequence.is_emptyy())
    {
        return false;
    }
    stackk<card> temp = sequence;
    card prev;
    temp.pop(prev);
    while (!temp.is_emptyy())
    {
        card curr =temp.topp();
        if (prev.rankk != curr.rankk +1 || prev.color ==curr.color)
        {
            return false;
        }
        prev = curr;
        temp.pop(curr);
    }
    card bottom = sequence.topp();
    if (tableau.is_emptyy())
    {
        return bottom.rankk ==13;
    }
    else
    {
        card d_top = tableau.topp();
         if (bottom.rankk ==d_top.rankk - 1 && bottom.color != d_top.color)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void move_sequence_to_tableau(stackk<card>& source, stackk<card>& destination)
{
    if (source.is_emptyy())
    {
        cout << "Source tableau is empty" << endl;
        return;
    }

    stackk<card> temp;
    card c;
    while (!source.is_emptyy() && source.topp().faceup)
    {
        source.pop(c);
        temp.push(c);
    }

    if (temp.is_emptyy())
    {
        cout << "No face-up cards to move" << endl;
        return;
    }

    stackk<card> orig_order;
    while (!temp.is_emptyy())
    {
        temp.pop(c);
        orig_order.push(c);
    }
    stackk<card> legal;
    card prev;
    orig_order.pop(prev);
    legal.push(prev);

    while (!orig_order.is_emptyy())
    {
        orig_order.pop(c);

        if (prev.color != c.color && prev.rankk + 1 == c.rankk)
        {
            legal.push(c);
            prev = c;
        }
        else
        {
            orig_order.push(c);
            break;
        }
    }
    stackk<card> restore;

    while (!orig_order.is_emptyy())
    {
        orig_order.pop(c);
        restore.push(c);
    }
    while (!restore.is_emptyy())
    {
        restore.pop(c);
        source.push(c);
    }


    int moveable_cards= 0;
    stackk<card> countt=legal;
    while (!countt.is_emptyy())
    {
        countt.pop(c);
        moveable_cards++;
    }
    int num = moveable_cards;

    if (moveable_cards>1)
    {
    cout<<moveable_cards<<" cards can be moved"<<endl;
    cout<<" Enter number (1-"<<moveable_cards<<") or 0 for all: "<<endl;
        cin >>num;
        if (num<= 0 || num>moveable_cards)
        {
         num=moveable_cards;
        }
    }

    stackk<card> sequence;
    while (!legal.is_emptyy())
    {
        legal.pop(c);
        sequence.push(c);
    }
    stackk<card> finall;
 for (int i = 0; i<num;i++)
 {
     sequence.pop(c);
     finall.push(c);

 }

    while (!sequence.is_emptyy())
    {
        sequence.pop(c);
        source.push(c);
    }

    if (!can_move_sequence_to_tableau(destination, finall))
    {
        cout << "Illegal move" << endl;
        while (!finall.is_emptyy())
        {
            finall.pop(c);
            source.push(c);
        }
        return;
    }

    while (!finall.is_emptyy())
    {
        finall.pop(c);
        destination.push(c);
        cout << c.intostring() << " moved to tableau" << endl;
    }

    flip_tableau_top(source);
}

void move_tableau_to_foundation (stackk<card>& tableau)
{
    if (tableau.is_emptyy())
    {
cout<<"Tableau is empty"<<endl;
return;

    }
    card c=tableau.topp();
    stackk<card>* foundation=nullptr;
    switch (c.suit)
    {
    case 0:
       foundation=&foundation1;
       break;
        case 1:
       foundation=&foundation2;
       break;
        case 2:
        foundation=&foundation3;
       break;
        case 3:
        foundation=&foundation4;
       break;
    }
    if (foundation && can_move_to_foundation(*foundation,c))
    {
        tableau.pop(c);
        foundation->push(c);
        cout<<c.intostring()<<" moved to foundation "<<endl;
        flip_tableau_top(tableau);
    }
    else
    {
        cout<<"Can not move "<<c.intostring()<<" to foundation"<<endl;
    }

}

void move_waste_to_tableau (stackk<card> & tableau)
{
    if (waste.is_emptyy())
    {
        cout<<"Waste is empty"<<endl;
        return;
    }
    move_to_tableau(tableau, waste, false);
}

void move_waste_to_foundation ()
{
    if (waste.is_emptyy())
    {
        cout<<"Waste is empty"<<endl;
        return;

    }
    card c=waste.topp();
    stackk<card> *foundation =nullptr;
    switch (c.suit)
    {
    case 0:
        foundation=&foundation1;
        break;
    case 1:
        foundation=&foundation2;
        break;
    case 2:
        foundation=&foundation3;
        break;
    case 3:
        foundation=&foundation4;
        break;
    }

    if (foundation && can_move_to_foundation(*foundation,c))

    {
        waste.pop(c);
        foundation->push(c);
        cout<<c.intostring()<<" moved to foundation"<<endl;
    }
    else
    {
        cout<<"Cannot move "<<c.intostring()<<" to foundation"<<endl;
    }
}

void move_to_foundation (stackk<card>& foundation, stackk<card>& source , bool flip_top_if_tableau =false)
   {
       if (source.is_emptyy())
       {
           cout<<"Source is empty card cannot move to foundation"<<endl;
           return;
       }
       card c= source.topp();
       if (can_move_to_foundation(foundation,c))
       {
           source.pop(c);
           foundation.push(c);
           cout<<c.intostring()<<" moved to foundation"<<endl;
            if (flip_top_if_tableau)
            {
                flip_tableau_top(source);
            }
       }
       else
       {
           cout<<"Cannot move "<<c.intostring()<<" to foundation"<<endl;
       }

   }
void move_foundation_to_tableau (stackk<card>& foundation, stackk<card> &tableau)
{
    if (foundation.is_emptyy())
    {
        cout<<"Foundation is empty"<<endl;
        return;
    }
    move_to_tableau(tableau, foundation, false);
}

bool check_win ()
{
    if (foundation1.is_full()&&foundation2.is_full()&&foundation3.is_full()&&foundation4.is_full())
    {
return true;
    }

    return false;
}
void display(stackk<card> &s)
 {
        stackk<card> temp=s;

        while (!temp.is_emptyy())
            {
            card c = temp.topp();
            cout<<c.intostring()<<" ";
            temp.pop(c);
        }
        cout << endl;
    }

void display_game()
{
    cout << "Stock: ";
    if (stock.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    cout<<"XX"<<endl;
    }
    cout << "Waste: ";
    if (waste.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(waste);
    cout<<endl;
    }

    cout<<"Foundations:"<< endl;
    cout<<"Hearts: ";
    if (foundation1.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(foundation1);

    }

    cout<<"Diamonds: ";
    if (foundation2.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(foundation2);

    }
    cout<<"Clubs: ";
    if (foundation3.is_emptyy())
    {
        cout<<"Empty"<<endl;
    }
    else
    {
    display(foundation3);

    }

    cout<<"Spades: ";
    if (foundation4.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(foundation4);

    }
    cout<<"Tableaus:"<<endl;
    cout<<"1: ";
    if (tableau1.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(tableau1);

    }
    cout<<"2: ";
     if (tableau2.is_emptyy())
    {
    cout<<"Empty" << endl;
    }
    else
    {
    display(tableau2);

    }
    cout<<"3: ";
    if (tableau3.is_emptyy())
    {
    cout<<"Empty" << endl;
    }
    else
    {
    display(tableau3);

    }
    cout<<"4: ";
    if (tableau4.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(tableau4);

    }
    cout<<"5: ";
    if (tableau5.is_emptyy())
    {

    }
    else
    {
    display(tableau5);

    }
    cout<<"6: ";
    if (tableau6.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(tableau6);

    }
    cout<<"7: ";
    if (tableau7.is_emptyy())
    {
    cout<<"Empty"<<endl;
    }
    else
    {
    display(tableau7);

    }
}
};
class Game
{
public:
    Deck game;
    Game()
    {
    }

    void initialize()
    {
        game.createdeck();
        game.shuffledeck();
        game.dealtableau();
    }

    void show_menu()
    {
        cout<<"Klondike Solitaire"<<endl;
        cout<<"1 Draw from stock" <<endl;
        cout<<"2 Move waste to tableau"<<endl;
        cout<<"3 Move waste to foundation"<< endl;
        cout<<"4 Move tableau to tableau" <<endl;
        cout<<"5 Move tableau to foundation"<< endl;
        cout<<"6 Move sequence from tableau to tableau"<<endl;
        cout<<"7 Move foundation to tableau"<<endl;
        cout<<"8 Display game" <<endl;
        cout<<"9 Quit"<<endl;
        cout<<"Enter your choice: "<<endl;
    }
    void draw_from_stock()
    {
        game.drawtowastee();
    }
        void waste_to_tableau()
    {
        int t;
        cout<<"Enter tableau number 1-7 to move waste to: "<<endl;
        cin>>t;
        switch (t)
        {
        case 1:
        {
             game.move_waste_to_tableau(game.tableau1);
            break;
        }
        case 2:
        {
             game.move_waste_to_tableau(game.tableau2);
            break;
         }
        case 3:
         {
            game.move_waste_to_tableau(game.tableau3);
        break;
        }
        case 4:
        {
             game.move_waste_to_tableau(game.tableau4);
        break;
        }
        case 5:
        {
                game.move_waste_to_tableau(game.tableau5);
            break;
        }
        case 6:
            {
                game.move_waste_to_tableau(game.tableau6);
            break;
            }
        case 7:
            {
               game.move_waste_to_tableau(game.tableau7);
                break;
            }
        default:
        {
            cout<<"Invalid tableau number"<<endl;
            break;
        }
        }
    }
void tableau_to_tableau()
{
    int s;
    int d;
    cout<<"Enter source tableau 1-7: ";
    cin>>s;
    cout<<"Enter destination tableau 1-7: ";
    cin>>d;
    stackk<card>* source = nullptr;
    stackk<card>* destination = nullptr;
    switch (s)
    {
    case 1:
    {
        source=&game.tableau1;
        break;
        }
    case 2:
        {
            source=&game.tableau2;
            break;
    }
    case 3:
        {
            source=&game.tableau3;
        break;
        }
    case 4:
{
        source=&game.tableau4;
        break;
        }
  case 5:
        {
        source=&game.tableau5;
    break;
        }
    case 6:
        {
        source=&game.tableau6;
        break;
        }
        case 7:
        {
        source=&game.tableau7;
            break;
        }
        default:
        {
        cout <<"Invalid source"<<endl;
            return;
        }
    }
    switch (d)
    {
    case 1:
        {
        destination=&game.tableau1;
        break;
        }
    case 2:
    {
        destination=&game.tableau2;
        break;
        }
    case 3:
        {
        destination=&game.tableau3;
        break;
        }
    case 4:
        {
        destination=&game.tableau4;
    break;
        }
    case 5:
        {
        destination=&game.tableau5;
        break;
        }
    case 6:
        {
        destination=&game.tableau6;
        break;
        }
    case 7:
        {
        destination=&game.tableau7;
        break;
    }
    default:
    {
        cout<<"Invalid destination"<<endl;
        return;
    }
    }
    game.move_tableau_to_tableau(*destination, *source);
}
void sequence_to_tableau()
{

    int s;
    int d;
    cout<<"Enter source tableau 1-7: ";
    cin>>s;
    cout<<"Enter destination tableau 1-7: ";
    cin>>d;

    stackk<card>* source=nullptr;
    stackk<card>* destination=nullptr;

    switch (s)
    {
        case 1:
        {
            source =&game.tableau1;
            break;
        }
        case 2:
        {
            source=&game.tableau2;
            break;
        }
        case 3:
        {
            source=&game.tableau3;
            break;
        }
        case 4:
        {
            source=&game.tableau4;
            break;
        }
        case 5:
        {
            source=&game.tableau5;
            break;
        }
        case 6:
        {
            source=&game.tableau6;
            break;
        }
        case 7:
        {
            source=&game.tableau7;
            break;
        }
        default:
        {
            cout<<"Invalid source"<<endl;
            return;
        }
    }
    switch (d)
    {
        case 1:
        {
        destination=&game.tableau1;
        break;
        }
        case 2:
        {
            destination=&game.tableau2;
            break;
        }
        case 3:
        {
            destination=&game.tableau3;
            break;
        }
        case 4:
        {
            destination=&game.tableau4;
            break;
        }
        case 5:
        {
            destination=&game.tableau5;
            break;
        }
        case 6:
        {
        destination=&game.tableau6;
        break;
        }
        case 7:
        {
        destination=&game.tableau7;
            break;
        }
        default:
        {
        cout<<"Invalid destination"<< endl;
            return;
        }
    }

    game.move_sequence_to_tableau(*source, *destination);
}
void tableau_to_foundation ()
{
    int t;
        cout<<"Enter tableau number 1-7: ";
        cin>> t;

        switch (t)
            {
            case 1:
                {
                game.move_tableau_to_foundation(game.tableau1);
                break;
                }
            case 2:
            {
            game.move_tableau_to_foundation(game.tableau2);
            break;
            }
                case 3:
                {
                    game.move_tableau_to_foundation(game.tableau3);
                break;
                }
                case 4:
                {
                game.move_tableau_to_foundation(game.tableau4);
                break;
                }
                case 5:
                {
                game.move_tableau_to_foundation(game.tableau5);
                break;
            }
                case 6:
                {
                game.move_tableau_to_foundation(game.tableau6);
                break;
                }
                case 7:
                {
                    game.move_tableau_to_foundation(game.tableau7);
                    break;
                }
                default:
                {
                cout<<"Invalid tableau number"<<endl;
                break;
                }
                }

            }
void foundation_to_tableau()
{
int f;
 int t;
    cout<<"Enter foundation number 1-4: ";
    cin>>f;
    cout<<"Enter tableau number 1-7: ";
    cin>>t;
    stackk<card>* foundation = nullptr;
    stackk<card>* tableau = nullptr;
    switch (f)
    {
    case 1:
    {
        foundation = &game.foundation1;
        break;
    }
    case 2:
    {
        foundation = &game.foundation2;
    break;
    }
    case 3:
    {
        foundation = &game.foundation3;
    break;
    }
    case 4:
    {
        foundation = &game.foundation4;
    break;
    }
    default:
    {
        cout << "Invalid foundation number" << endl;
    return;
    }
}
    switch (t)
    {
    case 1:
    {
        tableau= &game.tableau1;
        break;
    }
    case 2:
    {
    tableau =&game.tableau2;
        break;
    }
    case 3:
    {
        tableau =&game.tableau3;
    break;
    }
    case 4:
    {
        tableau=&game.tableau4;
    break;
    }
    case 5:
    {
        tableau=&game.tableau5;
    break;
    }
    case 6:
    {
        tableau=&game.tableau6;
        break;
    }
    case 7:
    {
        tableau=&game.tableau7;
    break;
    }
    default:
    {
    cout<<"Invalid tableau number"<<endl;
    return;
    }
}
    game.move_foundation_to_tableau(*foundation,*tableau);
}


void start()
{
    int choice;

    while (true)
    {
        show_menu();
        cin>>choice;
        switch (choice)
        {
        case 1:
        {
            draw_from_stock();
            break;
        }

        case 2:
        {
        waste_to_tableau();
        break;
        }
        case 3:
        {
        game.move_waste_to_foundation();
        break;
        }
        case 4:
        {
        tableau_to_tableau();
        break;
        }
        case 5:
        {
            tableau_to_foundation();
            break;
        }
            case 6:
            {
                sequence_to_tableau();
                break;
            }
            case 7:
                {

                foundation_to_tableau();
             break;
                }
            case 8:
            {
                game.display_game();
                break;
            }

            case 9:
            {
                cout<<"Game is Quitting"<<endl;
                return;
            }

            default:
            {
                cout << "Invalid choice" << endl;
                break;
            }
        }

        if (game.check_win())
        {
            cout<<"Congratulations You won the game"<<endl;
            return;
        }
    }
}

};
int main()
{
    Game g;
    g.initialize();
    g.start();
    return 0;
}
