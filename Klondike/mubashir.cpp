#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;
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
};


class CardItem
{
public:
	int rank_no;
	char suit;
	bool is_faceup;

	CardItem()
{
		rank_no=0;
		suit=' ';
		is_faceup=false;
}

	CardItem(int r,char s,bool f)
{
		rank_no=r;
		suit=s;
		is_faceup=f;
}

	bool check_red()
{
		if (suit=='H')
{
			return true;
}
		if (suit=='D')
{
			return true;
}
		return false;
}

	void flip_card()
{
		if (is_faceup==true)
{
			is_faceup=false;
}
		else
{
			is_faceup=true;
}
}

	string color_name()
{
		if (check_red()==true)
{
			return "RED";
}
		else
{
			return "BLACK";
}
}

	string suit_name()
{
		if (suit=='H')
{
			return "H(RED)";
}
		if (suit=='D')
{
			return "D(RED)";
}
		if (suit=='S')
{
			return "S(BLK)";
}
		if (suit=='C')
{
			return "C(BLK)";
}
		return " ";
}

	string f_suit_name()
{
		if (suit=='H')
{
			return "Hearts";
}
		if (suit=='D')
{
			return "Diamonds";
}
		if (suit=='S')
{
			return "Spades";
}
		if (suit=='C')
{
			return "Clubs";
}
	return "Unknown";
}

	string rank_show()
{
		if (rank_no==1)
{
			return "A ";
}
		if (rank_no==10)
{
			return "10";
}
		if (rank_no==11)
{
			return "J ";
}
		if (rank_no==12)
{
			return "Q ";
}
		if (rank_no==13)
{
			return "K ";
}
	string res=to_string(rank_no)+" ";
	return res;
}

	string f_rank_name()
{
		if (rank_no==1)
{
			return "Ace";
}
		if (rank_no==11)
{
			return "Jack";
}
		if (rank_no==12)
{
			return "Queen";
}
		if (rank_no==13)
{
			return "King";
}
	return to_string(rank_no);
}

	string card_detail()
{
		string d=f_rank_name()+" of "+f_suit_name()+" ("+color_name()+")";
		return d;
}
};



class DeckOfCards
{
public:
	Queue<CardItem> all_card;
	DeckOfCards() : all_card(52){}

	void shuffle_cards(Queue<CardItem>& q)
{
		Queue<CardItem> temp1(52);
		Queue<CardItem> temp2(52);

		srand(time(0));
		int passes=5;

		for (int p=0;p<passes;p++)
{
			while (!q.emptyy())
{
				CardItem c=q.dequeue();
				if (rand()%2==0)
{
					temp1.enqueue(c);
}
				else
{
					temp2.enqueue(c);
}
}

			while (!temp1.emptyy())
{
				q.enqueue(temp1.dequeue());
}
			while (!temp2.emptyy())
{
				q.enqueue(temp2.dequeue());
}
}
}

	void full_deck()
{
		Queue<CardItem> temp_deck(52);
		char all_suit[4]={'H','D','S','C'};
		int suit_i=0;

		while (suit_i<4)
{
			int r_value=1;
			while (r_value<=13)
{
				CardItem newCard(r_value,all_suit[suit_i],false);
				temp_deck.enqueue(newCard);
				r_value=r_value+1;
}
			suit_i=suit_i+1;
}

		srand(time(0));
		int shuffle_count=0;
		while (shuffle_count<3)
{
			shuffle_cards(temp_deck);
			shuffle_count=shuffle_count+1;
}

		all_card.clearr();
		while (!temp_deck.emptyy())
{
			all_card.enqueue(temp_deck.dequeue());
}
}

	CardItem draw_card()
{
		if (all_card.emptyy())
{
			CardItem empty_card(0,' ',false);
			return empty_card;
}
		CardItem card_draw=all_card.dequeue();
		return card_draw;
}

	bool check_empty()
{
		return all_card.emptyy();
}
};


class GameRules
{
public:
	bool tab_move(CardItem f_card,CardItem to_card)
{
		bool f_red=f_card.check_red();
		bool to_red=to_card.check_red();
		bool c_oppo=false;

		if (f_red==true && to_red==false)
{
			c_oppo=true;
}
		if (f_red==false && to_red==true)
{
			c_oppo=true;
}

		if (c_oppo==false)
{
			return false;
}

		bool r_descend=false;
		if (f_card.rank_no==to_card.rank_no-1)
{
			r_descend=true;
}

		if (r_descend==true)
{
			return true;
}
		return false;
}

	bool foundation_move(CardItem f_card,CardItem to_card)
{
		bool s_match=false;
		if (f_card.suit==to_card.suit)
{
			s_match=true;
}

		if (s_match==false)
{
			return false;
}

		bool r_ascend=false;
		if (f_card.rank_no==to_card.rank_no+1)
{
			r_ascend=true;
}

		if (r_ascend==true)
{
			return true;
}
		return false;
}

	bool empty_placement(CardItem card_place,bool tab_col)
{
		if (tab_col==true)
{
			if (card_place.rank_no==13)
{
				return true;
}
			return false;
}
		else
{
			if (card_place.rank_no==1)
{
				return true;
}
			return false;
}
}

	bool check_foundation(Queue<CardItem> foundation_pile[4])
{
		for (int i=0;i<4;i++)
{
			if (foundation_pile[i].gsize()!=13)
{
				return false;
}
}
		return true;
}

	string invalid_mover(CardItem f_card,CardItem to_card,bool is_tab)
{
		if (is_tab==true)
{
			bool f_red=f_card.check_red();
			bool to_red=to_card.check_red();
			if (f_red==to_red)
{
				return "Cards should be in opposite colors ";
}
			if (f_card.rank_no!=to_card.rank_no-1)
{
				return "Cards must be in descending rank order";
}
}
		else
{
			if (f_card.suit!=to_card.suit)
{
				return "Foundation cards must be same suit";
}
			if (f_card.rank_no!=to_card.rank_no+1)
{
				return "Foundation cards must be in ascending rank order";
}
}
		return "Unknown reason";
}
};



class TableauColumn
{
public:
	Queue<CardItem> card_col;

	TableauColumn() : card_col(52) {}

	void add_card(CardItem c)
{
		card_col.enqueue(c);
}

	bool empty()
{
		return card_col.emptyy();
}

	int card_count()
{
		return card_col.gsize();
}

	int count_fdcard()
{
		Queue<CardItem> temp(52);
		int count=0;

		while (!card_col.emptyy())
{
			CardItem c=card_col.dequeue();
			if (!c.is_faceup) count++;
			temp.enqueue(c);
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}
		return count;
}

	int count_fucard()
{
		Queue<CardItem> temp(52);
		int count=0;

		while (!card_col.emptyy())
{
			CardItem c=card_col.dequeue();
			if (c.is_faceup) count++;
			temp.enqueue(c);
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}
		return count;
}

	CardItem get_top_card(int& faceup_count)
{
		Queue<CardItem> temp(52);
		CardItem last_card;
		faceup_count=0;

		while (!card_col.emptyy())
{
			last_card=card_col.dequeue();
			if (last_card.is_faceup) faceup_count++;
			temp.enqueue(last_card);
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}
		return last_card;
}

	bool move_faceup_cards_to(TableauColumn& dest_col)
{
		Queue<CardItem> temp(52);
		Queue<CardItem> moving_cards(52);
		bool found_faceup=false;
		CardItem first_faceup;

		while (!card_col.emptyy())
{
			CardItem c=card_col.dequeue();
			if (!found_faceup && c.is_faceup)
{
				found_faceup=true;
				first_faceup=c;
				moving_cards.enqueue(c);
}
			else if (found_faceup)
{
				moving_cards.enqueue(c);
}
			else
{
				temp.enqueue(c);
}
}

		if (!found_faceup)
{
			while (!temp.emptyy())
{
				card_col.enqueue(temp.dequeue());
}
			return false;
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}

		while (!moving_cards.emptyy())
{
			dest_col.add_card(moving_cards.dequeue());
}

		return true;
}


	CardItem remove_lastcard()
{
		Queue<CardItem> temp(52);
		CardItem last_card;
		bool h=false;

		while (!card_col.emptyy())
{
			last_card=card_col.dequeue();
			if (!card_col.emptyy())
{
				temp.enqueue(last_card);
}
			else
{
				h=true;
}
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}

		if (h)
{
			return last_card;
}
		else
{
			return CardItem();
}
}

	void flip_topp()
{
		Queue<CardItem> temp(52);
		CardItem last_card;
		bool has_card=false;

		while (!card_col.emptyy())
{
			last_card=card_col.dequeue();
			if (!card_col.emptyy())
{
				temp.enqueue(last_card);
}
			else
{
				has_card=true;
}
}

		if (has_card && !last_card.is_faceup)
{
			last_card.flip_card();
}

		while (!temp.emptyy())
{
			card_col.enqueue(temp.dequeue());
}
		if (has_card)
{
			card_col.enqueue(last_card);
}
}

	void display_column(int max_height)
{
		Queue<CardItem> temp(52);
		int card_count=0;

		while (!card_col.emptyy())
{
			CardItem c=card_col.dequeue();
			temp.enqueue(c);
			card_count++;
}

		int display_pos=0;
		while (!temp.emptyy())
{
			CardItem c=temp.dequeue();

			if (display_pos>=card_count-max_height)
{
				if (c.is_faceup)
{
					cout<<"[";
					cout<<setw(5)<<left<<(c.rank_show()+c.suit_name());
					cout<<"]";
}
				else
{
					cout<<"[";
					cout<<setw(5)<<left<<"??";
					cout<<"]";
}
}
			else
{
				cout<<"       ";
}

			card_col.enqueue(c);
			display_pos++;
}
}
};


class GameStatistics
{
public:
	int t_moves;
	int stock_draws;
	int waste_recycles;
	int tab_moves;
	int foundation_moves;
	int card_flip;
	int u_count;
	int hint_count;

	GameStatistics()
{
		t_moves=0;
		stock_draws=0;
		waste_recycles=0;
		tab_moves=0;
		foundation_moves=0;
		card_flip=0;
		u_count=0;
		hint_count=0;
}

	void inc_moves()
{
		t_moves=t_moves+1;
}

	void inc_sdraws()
{
		stock_draws=stock_draws+1;
}

	void inc_wrecycle()
{
		waste_recycles=waste_recycles+1;
}

	void inc_tabmove()
{
		tab_moves=tab_moves+1;
}

	void inc_foundation()
{
		foundation_moves=foundation_moves+1;
}

	void inc_cardflip()
{
		card_flip=card_flip+1;
}

	void show_stat()
{
		cout<<"GAME STATISTICS"<<endl;
		cout<<"Total Moves: "<<t_moves<<endl;
		cout<<"Stock Draws: "<<stock_draws<<endl;
		cout<<"Waste Recycles: "<<waste_recycles<<endl;
		cout<<"Tableau Moves: "<<tab_moves<<endl;
		cout<<"Foundation Moves: "<<foundation_moves<<endl;
		cout<<"Cards Flipped: "<<card_flip<<endl;
}
};
