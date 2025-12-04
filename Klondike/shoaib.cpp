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
        else return T();

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
    stackk<card> foundationHearts{13};
    stackk<card> foundationDiamonds{13};
    stackk<card> foundationClubs{13};
    stackk<card> foundationSpades{13};
    Deck ()
    {
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
void move_to_tableau (stackk<card>& tableau, stackk<card>& source)
   {
       if (source.is_emptyy())
       {
           cout<<"Source is empty. Cannot move to tableau"<<endl;
           return;
       }
       card c= source.topp();
       if (can_move_to_tableau(tableau,c))
       {
           source.pop(c);
           tableau.push(c);
           cout<<c.intostring()<<" moved to Tableau"<<endl;
           flip_tableau_top(source);
       }
       else
       {
           cout<<"Cannot move "<<c.intostring()<<" to Tableau"<<endl;
       }
   }
   void move_to_foundation (stackk<card>& foundation, stackk<card>& source)
   {
       if (source.is_emptyy())
       {
           cout<<"Source is empty. Cannot move to foundation"<<endl;
           return;
       }
       card c= source.topp();
       if (can_move_to_foundation(foundation,c))
       {
           source.pop(c);
           foundation.push(c);
           cout<<c.intostring()<<" moved to foundation"<<endl;
            flip_tableau_top(source);
       }
       else
       {
           cout<<"Cannot move "<<c.intostring()<<" to foundation"<<endl;
       }
   }
   void move_tableau_to_tableau (stackk<card>& tableau, stackk<card>& source)
   {
       if (source.is_emptyy())
       {
           cout<<"Source is empty. Cannot move to tableau"<<endl;
           return;
       }
       card c= source.topp();
       if (can_move_to_tableau(tableau,c))
       {
           source.pop(c);
           tableau.push(c);
           cout<<c.intostring()<<" moved to Tableau"<<endl;
       }
       else
       {
           cout<<"Cannot move "<<c.intostring()<<" to Tableau"<<endl;
       }
   }

    void recyclewastetostock()
    {
        card c;
    while (!waste.is_emptyy())
    {
        waste.pop(c);
        c.faceup=false;
        stock.push(c);

    }

    }
    void drawtowastee ()
    {
        int draw=3;
        card c;
        if (stock.is_emptyy())
        {
            recyclewastetostock();
        }
        for (int i=0; i<draw && !stock.is_emptyy(); i++)
        {
            stock.pop(c);
            c.faceup=true;
            waste.push(c);
        }
        if (!waste.is_emptyy())
        {
            cout<<"Top of waste "<<waste.topp().intostring()<<endl;
        }
        else
        {
            cout<<"Waste is empty"<<endl;
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
    while (!stock.is_emptyy())
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
void move_sequence_to_tableau (stackk<card> &source,stackk<card> &destination, int start)
{
    if (source.is_emptyy())
    {
        cout<<"Source is empty"<<endl;
        return;
    }
    stackk<card> temp;
    card c;

    int countt=source.topp().rankk;
    int total=0;
    stackk<card> reverse_stack;

    while (!source.is_emptyy())
    {
        source.pop(c);
        temp.push(c);
    }
    int i=0;
    if (!temp.is_emptyy())
    {
        temp.pop(c);
        if (i<start)
        {
            reverse_stack.push(c);
        }
        else
        {
            source.push(c);
            i++;
        }
    }

    card bottom=source.topp();
    if (!can_move_to_tableau(destination,bottom))
    { cout<<"Cannot move sequence to tableau move is illegal"<<endl;

        while (!source.is_emptyy())
        {
            source.pop(c);
            temp.push(c);
        }
       while (!reverse_stack.is_emptyy())
        {
            reverse_stack.pop(c);
            temp.push(c);
        }
        while (!temp.is_emptyy())
        {
            temp.pop(c);
            source.push(c);
        }
        return;

    }
    stackk<card> finalstack;
    while (!source.is_emptyy())
    {
        source.pop(c);
        finalstack.push(c);
    }
    while (!finalstack.is_emptyy())
    {
        finalstack.pop(c);
        destination.push(c);
    }
    while (!reverse_stack.is_emptyy())
    {
        reverse_stack.pop(c);
        source.push(c);
    }
    flip_tableau_top(source);
    cout<<"Sequence moved successfully"<<endl;
}
void move_waste_to_tableau (stackk<card> & tableau)
{
    if (waste.is_emptyy())
    {
        cout<<"Waste is empty"<<endl;
        return;
    }
    move_to_tableau(tableau,waste);
}

void move_waste_to_foundation (stackk<card> & foundation)
{
    if (waste.is_emptyy())
    {
        cout<<"Waste is empty"<<endl;
        return;
    }
    move_to_foundation(foundation,waste);
}

void move_tableau_to_foundation (stackk<card>& tableau , stackk <card>& foundation)
{
    if (tableau.is_emptyy())
    {
cout<<"Tableau is empty"<<endl;
return;
    }
    move_to_foundation(foundation,tableau);
}
bool check_win ()
{
    if (foundationHearts.is_full()&&foundationDiamonds.is_full()&&foundationClubs.is_full()&&foundationSpades.is_full())
    {
return true;
    }
    return false;
}
};


int main()
{

}
