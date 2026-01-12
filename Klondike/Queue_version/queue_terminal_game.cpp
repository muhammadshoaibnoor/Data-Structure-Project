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













class SolitaireGame
{
public:
	DeckOfCards gameDeck;
	Queue<CardItem> stock_pile;
	Queue<CardItem> waste_pile;
	Queue<CardItem> foundation_piles[4];
	TableauColumn tab_col[7];
	GameRules rules;
	GameStatistics stat;
	int score;
	int m_count;
	bool game_won;
	bool game_start;

	SolitaireGame() : stock_pile(24),waste_pile(24)
{
		for(int i=0;i<4;i++)
{
			foundation_piles[i]=Queue<CardItem>(13);
}
		score=0;
		m_count=0;
		game_won=false;
		game_start=false;
}

	void set_game()
{
		gameDeck.full_deck();
		deal_tab_card();
		while (!gameDeck.check_empty())
{
			CardItem card_fdeck=gameDeck.draw_card();
			stock_pile.enqueue(card_fdeck);
}
		game_start=true;
		game_won=false;
}

	void deal_tab_card()
{
		for (int col_no=0;col_no<7;col_no++)
{
			for (int card_no=0;card_no<=col_no;card_no++)
{
				CardItem drawn_card=gameDeck.draw_card();
				if (card_no==col_no)
{
					drawn_card.is_faceup=true;
}
				tab_col[col_no].add_card(drawn_card);
}
}
}

	void draw_card_fstock()
{
		if (!stock_pile.emptyy())
{
			CardItem drawn_card=stock_pile.dequeue();
			drawn_card.is_faceup=true;
			waste_pile.enqueue(drawn_card);
			m_count=m_count+1;
			stat.inc_moves();
			stat.inc_sdraws();
			cout<<endl<<"Drew: "<<drawn_card.card_detail()<<endl;
}
		else if (!waste_pile.emptyy())
{
			recycle_waste_tostock();
}
		else
{
			cout<<endl<<"Stock pile is completely empty"<<endl;
}
}

	void recycle_waste_tostock()
{
		int c=0;
		while (!waste_pile.emptyy())
{
			CardItem recycle_card=waste_pile.dequeue();
			recycle_card.is_faceup=false;
			stock_pile.enqueue(recycle_card);
			c=c+1;
}
		m_count=m_count+1;
		stat.inc_moves();
		stat.inc_wrecycle();
		cout<<endl<<"Recycled "<<c<<" cards from waste back to stock"<<endl;
}

	bool tab_to_tab(int fcol,int tocol)
{
		if (fcol<0 || fcol>=7)
{
			cout<<endl<<"Invalid source column number"<<endl;
			return false;
}
		if (tocol<0 || tocol>=7)
{
			cout<<endl<<"Invalid destination column number"<<endl;
			return false;
}
		if (fcol==tocol)
{
			cout<<endl<<"Cannot move to same column"<<endl;
			return false;
}

		TableauColumn* src=&tab_col[fcol];
		TableauColumn* dst=&tab_col[tocol];

		if (src->empty())
{
			cout<<endl<<"Source column is empty"<<endl;
			return false;
}

		int src_faceup_count=0;
		CardItem src_top=src->get_top_card(src_faceup_count);

		if (src_faceup_count==0)
{
			cout<<endl<<"No face-up cards in source column"<<endl;
			return false;
}

		if (dst->empty())
{
			if (rules.empty_placement(src_top,true))
{
				src->move_faceup_cards_to(*dst);
				src->flip_topp();
				m_count=m_count+1;
				score=score+5;
				stat.inc_moves();
				stat.inc_tabmove();
				cout<<endl<<"Moved cards to empty column"<<endl;
				return true;
}
			else
{
				cout<<endl<<"Only Kings can be placed on empty columns"<<endl;
				return false;
}
}
		else
{
			int dst_faceup_count=0;
			CardItem dst_top=dst->get_top_card(dst_faceup_count);

			if (rules.tab_move(src_top,dst_top))
{
				src->move_faceup_cards_to(*dst);
				src->flip_topp();
				m_count=m_count+1;
				score=score+5;
				stat.inc_moves();
				stat.inc_tabmove();
				cout<<endl<<"Moved cards successfully"<<endl;
				return true;
}
			else
{
				string r=rules.invalid_mover(src_top,dst_top,true);
				cout<<endl<<"Invalid move: "<<r<<endl;
				return false;
}
}
}

	bool tab_to_foundation(int n)
{
		if (n<0 || n>=7)
{
			cout<<endl<<"Invalid column number"<<endl;
			return false;
}

		TableauColumn* col=&tab_col[n];

		if (col->empty())
{
			cout<<endl<<"Column is empty"<<endl;
			return false;
}

		int faceup_count=0;
		CardItem top_card=col->get_top_card(faceup_count);

		if (faceup_count==0)
{
			cout<<endl<<"Top card is face down"<<endl;
			return false;
}

		int foundation_i=-1;
		if (top_card.suit=='H') foundation_i=0;
		if (top_card.suit=='D') foundation_i=1;
		if (top_card.suit=='S') foundation_i=2;
		if (top_card.suit=='C') foundation_i=3;

		if (foundation_i==-1)
{
			cout<<endl<<"Invalid card suit"<<endl;
			return false;
}

		bool cp=false;
		if (foundation_piles[foundation_i].emptyy())
{
			if (rules.empty_placement(top_card,false))
{
				cp=true;
}
			else
{
				cout<<endl<<"Foundation must start with Ace"<<endl;
				return false;
}
}
		else
{
			Queue<CardItem> temp(52);
			CardItem foundation_top;
			while (!foundation_piles[foundation_i].emptyy())
{
				foundation_top=foundation_piles[foundation_i].dequeue();
				temp.enqueue(foundation_top);
}
			while (!temp.emptyy())
{
				foundation_piles[foundation_i].enqueue(temp.dequeue());
}

			if (rules.foundation_move(top_card,foundation_top))
{
				cp=true;
}
			else
{
				string a=rules.invalid_mover(top_card,foundation_top,false);
				cout<<endl<<"Invalid move: "<<a<<endl;
				return false;
}
}

		if (cp)
{
			CardItem removed_card=col->remove_lastcard();
			foundation_piles[foundation_i].enqueue(removed_card);
			col->flip_topp();
			m_count=m_count+1;
			score=score+10;
			stat.inc_moves();
			stat.inc_foundation();
			cout<<endl<<"Moved "<<top_card.card_detail()<<" to foundation"<<endl;
			return true;
}
		return false;
}

	bool waste_to_tab(int n)
{
		if (waste_pile.emptyy())
{
			cout<<endl<<"Waste pile is empty"<<endl;
			return false;
}

		if (n<0 || n>=7)
{
			cout<<endl<<"Invalid column number"<<endl;
			return false;
}

		Queue<CardItem> temp(52);
		CardItem waste_card;
		while (!waste_pile.emptyy())
{
			waste_card=waste_pile.dequeue();
			if (!waste_pile.emptyy())
{
				temp.enqueue(waste_card);
}
}

		while (!temp.emptyy())
{
			waste_pile.enqueue(temp.dequeue());
}

		TableauColumn* des_col=&tab_col[n];

		if (des_col->empty())
{
			if (rules.empty_placement(waste_card,true))
{
				des_col->add_card(waste_card);
				m_count=m_count+1;
				score=score+5;
				stat.inc_moves();
				stat.inc_tabmove();
				cout<<endl<<"Moved "<<waste_card.card_detail()<<" to column"<<endl;
				return true;
}
			else
{
				waste_pile.enqueue(waste_card);
				cout<<endl<<"Only Kings can be placed on empty columns"<<endl;
				return false;
}
}
		else
{
			int dst_faceup_count=0;
			CardItem dst_top=des_col->get_top_card(dst_faceup_count);

			if (rules.tab_move(waste_card,dst_top))
{
				des_col->add_card(waste_card);
				m_count=m_count+1;
				score=score+5;
				stat.inc_moves();
				stat.inc_tabmove();
				cout<<endl<<"Moved "<<waste_card.card_detail()<<" to column"<<endl;
				return true;
}
			else
{
				waste_pile.enqueue(waste_card);
				string e=rules.invalid_mover(waste_card,dst_top,true);
				cout<<endl<<"Invalid move: "<<e<<endl;
				return false;
}
}
}

	bool waste_to_foundation()
{
		if (waste_pile.emptyy())
{
			cout<<endl<<"Waste pile is empty"<<endl;
			return false;
}

		Queue<CardItem> temp(52);
		CardItem waste_card;
		while (!waste_pile.emptyy())
{
			waste_card=waste_pile.dequeue();
			if (!waste_pile.emptyy())
{
				temp.enqueue(waste_card);
}
}

		while (!temp.emptyy())
{
			waste_pile.enqueue(temp.dequeue());
}

		int foundation_i=-1;
		if (waste_card.suit=='H') foundation_i=0;
		if (waste_card.suit=='D') foundation_i=1;
		if (waste_card.suit=='S') foundation_i=2;
		if (waste_card.suit=='C') foundation_i=3;

		if (foundation_i==-1)
{
			waste_pile.enqueue(waste_card);
			cout<<endl<<"Invalid card suit"<<endl;
			return false;
}

		bool p=false;
		if (foundation_piles[foundation_i].emptyy())
{
			if (rules.empty_placement(waste_card,false))
{
				p=true;
}
			else
{
				waste_pile.enqueue(waste_card);
				cout<<endl<<"Foundation must start with Ace"<<endl;
				return false;
}
}
		else
{
			Queue<CardItem> temp2(52);
			CardItem foundation_top;
			while (!foundation_piles[foundation_i].emptyy())
{
				foundation_top=foundation_piles[foundation_i].dequeue();
				temp2.enqueue(foundation_top);
}
			while (!temp2.emptyy())
{
				foundation_piles[foundation_i].enqueue(temp2.dequeue());
}

			if (rules.foundation_move(waste_card,foundation_top))
{
				p=true;
}
			else
{
				waste_pile.enqueue(waste_card);
				string r=rules.invalid_mover(waste_card,foundation_top,false);
				cout<<endl<<"Invalid move: "<<r<<endl;
				return false;
}
}

		if (p)
{
			foundation_piles[foundation_i].enqueue(waste_card);
			m_count=m_count+1;
			score=score+10;
			stat.inc_moves();
			stat.inc_foundation();
			cout<<endl<<"Moved "<<waste_card.card_detail()<<" to foundation"<<endl;
			return true;
}
		return false;
}

	bool check_win()
{
		bool c=rules.check_foundation(foundation_piles);
		if (c==true)
{
			game_won=true;
}
		return game_won;
}

	int percent()
{
		int card_foundation=0;
		for (int i=0;i<4;i++)
{
			card_foundation=card_foundation+foundation_piles[i].gsize();
}
		int p=(card_foundation*100)/52;
		return p;
}

	void game_board()
{
		system("clear");
		cout<<endl;
		cout<<"================================================================================="<<endl;
		cout<<"                          KLONDIKE SOLITAIRE\n";
		cout<<"================================================================================="<<endl;
		cout<<endl;
		cout<<"  Score: "<<setw(6)<<right<<score;
		cout<<"     Moves: "<<setw(6)<<right<<m_count;
		cout<<"     Completion: "<<setw(3)<<right<<percent()<<"%"<<endl;
		cout<<endl;
		cout<<"================================================================================="<<endl;

		cout<<"  STOCK                WASTE              FOUNDATIONS"<<endl;
		cout<<"  -----                -----              -----------"<<endl;
		cout<<"  ";

		cout<<"+----------+";
		cout<<"     ";

		cout<<"+----------+";
		cout<<"     ";

		cout<<"  H     D     S     C"<<endl;
		cout<<"  ";

		if (!stock_pile.emptyy())
{
			cout<<"|";
			cout<<setw(8)<<left<<(to_string(stock_pile.gsize())+" cards");
			cout<<"|";
}
		else
{
			cout<<"|";
			cout<<setw(8)<<left<<" EMPTY";
			cout<<"|";
}
		cout<<"     ";

		if (!waste_pile.emptyy())
{
			Queue<CardItem> temp(52);
			CardItem wc;
			while (!waste_pile.emptyy())
{
				wc=waste_pile.dequeue();
				temp.enqueue(wc);
}
			while (!temp.emptyy())
{
				waste_pile.enqueue(temp.dequeue());
}
			cout<<"|";
			cout<<setw(8)<<left<<(wc.rank_show()+wc.suit_name());
			cout<<"|";
}
		else
{
			cout<<"|";
			cout<<setw(8)<<left<<" EMPTY";
			cout<<"|";
}
		cout<<"     ";

		for (int f=0;f<4;f++)
{
			if (!foundation_piles[f].emptyy())
{
				Queue<CardItem> temp(52);
				CardItem fc;
				while (!foundation_piles[f].emptyy())
{
					fc=foundation_piles[f].dequeue();
					temp.enqueue(fc);
}
				while (!temp.emptyy())
{
					foundation_piles[f].enqueue(temp.dequeue());
}
				cout<<"[";
				cout<<setw(3)<<left<<fc.rank_show();
				cout<<"]";
}
			else
{
				cout<<"[";
				cout<<setw(3)<<left<<" -";
				cout<<"]";
}
			if (f<3) cout<<"   ";
}
		cout<<endl;
		cout<<"  +----------+";
		cout<<"     ";
		cout<<"+----------+";
		cout<<"     ";

		for (int f=0;f<4;f++)
{
			if (!foundation_piles[f].emptyy())
{
				Queue<CardItem> temp(52);
				CardItem fc;
				while (!foundation_piles[f].emptyy())
{
					fc=foundation_piles[f].dequeue();
					temp.enqueue(fc);
}
				while (!temp.emptyy())
{
					foundation_piles[f].enqueue(temp.dequeue());
}
				cout<<"[";
				cout<<setw(3)<<left<<fc.suit_name()[0];
				cout<<"]";
}
			else
{
				cout<<"[";
				cout<<setw(3)<<left<<" ";
				cout<<"]";
}
			if (f<3) cout<<"   ";
}
		cout<<endl;

		cout<<"   Count: ";
		cout<<setw(2)<<right<<stock_pile.gsize();
		cout<<"       Count: ";
		cout<<setw(2)<<right<<waste_pile.gsize();
		cout<<"       Counts: ";

		for (int g=0;g<4;g++)
{
			cout<<setw(2)<<right<<foundation_piles[g].gsize();
			if (g<3) cout<<" ";
}
		cout<<endl;

		cout<<"================================================================================="<<endl;
		cout<<"  TABLEAU COLUMNS "<<endl;
		cout<<"  ";
		for (int c=0;c<7;c++)
{
			cout<<"--------";
}
		cout<<endl;
		cout<<"  ";
		for (int c=0;c<7;c++)
{
			int totcard=tab_col[c].card_count();
			int fd=tab_col[c].count_fdcard();
			cout<<"  (";
			if (totcard<10) cout<<" ";
			cout<<totcard<<":";
			cout<<fd<<"D)";
			cout<<"   ";
}
		cout<<endl<<endl;

		int mhei=0;
		for (int i=0;i<7;i++)
{
			if (tab_col[i].card_count()>mhei)
{
				mhei=tab_col[i].card_count();
}
}

		for (int row=0;row<mhei;row++)
{
			cout<<"  ";
			for (int col=0;col<7;col++)
{
				tab_col[col].display_column(mhei);
				cout<<"   "<<endl;
}
			cout<<endl;
			break;
}

		cout<<"================================================================================="<<endl;

}

	void detail_state()
{
		cout<<endl<<"========== DETAILED GAME STATE =========="<<endl;
		cout<<"Stock: "<<stock_pile.gsize()<<" cards"<<endl;
		cout<<"Waste: "<<waste_pile.gsize()<<" cards"<<endl;
		cout<<"Foundations: "<<endl;
		string suitn[4]={"Hearts","Diamonds","Spades","Clubs"};
		for (int i=0;i<4;i++)
{
			cout<<" "<<suitn[i]<<": "<<foundation_piles[i].gsize()<<" cards"<<endl;
}
		cout<<endl<<"Tableau Columns: "<<endl;
		for (int col=0;col<7;col++)
{
			cout<<" Column "<<(col+1)<<": "<<tab_col[col].card_count()<<" cards"<<endl;
			cout<<" ("<<tab_col[col].count_fdcard()<<" face-down, "<<endl;
			cout<<tab_col[col].count_fucard()<<" face-up)"<<endl;
}
		cout<<"========================================="<<endl;
}
};



void display_help()
{
	cout<<endl<<"================================================================"<<endl;
	cout<<" Game commands"<<endl;
	cout<<" D - Draw card from stock"<<endl;
	cout<<" WT <col> - Move waste to tableau col"<<endl;
	cout<<" WF - Move waste to foundation"<<endl;
	cout<<" TF <col> Mo ve tableau column to foundation "<<endl;
	cout<<" TT <from> <to>  Move face-up cards  tableau to tableau"<<endl;
	cout<<" S  Show detailed game state "<<endl;
	cout<<" ST Show statistics "<<endl;
	cout<<" R  Restart new game"<<endl;
	cout<<" H  Show help "<<endl;
	cout<<" Q  Quit game"<<endl;
	cout<<"================================================================"<<endl;

}

void display_gameover(SolitaireGame game)
{
	cout<<endl<<"================================================================"<<endl;
	cout<<" GAME OVER"<<endl;
	if (game.game_won)
{
		cout<<endl<<"YOU WON"<<endl;
}
	else
{
		cout<<endl<<" GAME ENDED ";
}
	cout<<endl<<"================================================================"<<endl;
	cout<<" FINAL RESULTS "<<endl;
	cout<<"================================================================"<<endl;
	cout<<" Final Score: "<<game.score<<" points\n";
	cout<<" Total Moves: "<<game.m_count<<" moves\n";
	cout<<" Completion: "<<game.percent()<<"%"<<endl;
	cout<<"================================================================"<<endl;
	game.stat.show_stat();
}

int main()
{
	SolitaireGame game;
	game.set_game();

	string u;
	bool k=true;

	while (k)
{
		game.game_board();
		if (game.check_win())
{
			display_gameover(game);
			break;
}

		cout<<"Enter command (H for help): ";
		cin>>u;

		for (int i=0;i<u.length();i++)
{
			u[i]=toupper(u[i]);
}

		if (u=="Q")
{
			display_gameover(game);
			k=false;
}
		else if (u=="H")
{
			display_help();
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="D")
{
			game.draw_card_fstock();
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="S")
{
			game.detail_state();
			cout<<"ENTER to continue";
			cin.ignore();
			cin.get();
}
		else if (u=="ST")
{
			game.stat.show_stat();
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="R")
{
			game=SolitaireGame();
			game.set_game();
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="WT")
{
			int c;
			cin>>c;
			game.waste_to_tab(c-1);
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="WF")
{
			game.waste_to_foundation();
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="TF")
{
			int c;
			cin>>c;
			game.tab_to_foundation(c-1);
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else if (u=="TT")
{
			int fcol,tocol;
			cin>>fcol>>tocol;
			game.tab_to_tab(fcol-1,tocol-1);
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
		else
{
			cout<<endl<<"invalid type H"<<endl;
			cout<<"enter continue";
			cin.ignore();
			cin.get();
}
}

	return 0;
}


