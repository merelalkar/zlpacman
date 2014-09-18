
#ifndef PEGAS_FAKESPIDER_CARD_COLUMN_H
#define PEGAS_FAKESPIDER_CARD_COLUMN_H
#pragma once;

namespace Pegas
{
	class OpenClosedCardCommand: public GameCommand
	{
	 public:
		 OpenClosedCardCommand(CardColumn* column): m_ñolumn(column) {};
		 virtual ~OpenClosedCardCommand() {};
		 virtual void undo();

	private:
		CardColumn* m_ñolumn;
	};

	class CardColumn: public GameObject
	{
	public:
		typedef std::list<PlayingCard*> ClosedCardList;
		typedef std::list<CardStack*> CardStackList;

		enum
		{
			k_closedCardRelativeTopOffset = 10,
			k_openCardRelativeTopOffset = 30
		};
		 

		CardColumn():m_zIndex(0) {};
		virtual ~CardColumn();
		
		virtual void draw(GrafManager& context);
		virtual void setLeft(CURCOORD left);
		virtual void setTop(CURCOORD top);

		void initialize(const CardList& cards);
		void addCard(PlayingCard* card);
		PlayingCard* getTopCard();
		CardStack* getTopStack();
		void removeLastStack();
		void removeCards(const CardList& cards); 
		
		OpenClosedCardCommand* openClosedCard(); 
		void undoOpenClosedCard();
				
		bool isEmpty();
		bool isTopStack(const CardStack* stack);
		bool isIntersects(CURCOORD x, CURCOORD y);
	private:
		
		void alignCards();
		void recalkSize(); 
		void destroyCardStacks();

		CardStack* createNewStack();
		int getNextZIndex() { return ++m_zIndex; };

		ClosedCardList m_closedCards;
		CardStackList m_cardStacks;

		CardColumn(const CardColumn& src);
		CardColumn& operator=(const CardColumn& src);

		int m_zIndex;
	};
}

#endif //PEGAS_FAKESPIDER_CARD_COLUMN_H