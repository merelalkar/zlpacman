#ifndef PEGAS_FAKESPIDER_CARD_STACK_H
#define PEGAS_FAKESPIDER_CARD_STACK_H
#pragma once

namespace Pegas
{
	class CardStackIterator;
	
	class CardStack: public GameObject
	{
	public:
		typedef std::list<PlayingCard*> PlayingCardsStack;
		typedef std::list<PlayingCard*>::reverse_iterator PlayingCardStackIterator;
		
		CardStack(CardColumn& owner);
		virtual ~CardStack();
		
		void takeStackByCard(PlayingCard* pTopCard);
		void addCard(PlayingCard* pCard);

		virtual void draw(GrafManager& context);
		virtual void setLeft(CURCOORD left);
		virtual void setTop(CURCOORD top);

		PlayingCard* getTopCard();
		PlayingCard* getBottomCard();
        CardStackIterator& getCardIterator(); 
		int getNumCard() const { return m_cardStack.size(); }
		CardColumn& getColumn() { return m_owner; };
		//Хотелось этого избежать, да хуй тебе, а не инкапсуляция (((
		PlayingCardsStack& getCards() { return m_cardStack; }; 

		bool hasCard(PlayingCard* card); 
		void removeCardsBeginingOne(PlayingCard* card); 
		
	private:
		void alignCards();
		void recalkSize();

		CardStackIterator* m_cardIterator;

		PlayingCardsStack m_cardStack;
		CardColumn& m_owner;

		CardStack(const CardStack& src);
		CardStack& operator=(const CardStack& src);
	};

	
	class CardStackIterator
	{
	public:
		typedef CardStack::PlayingCardStackIterator InnerIterator;

		void reset(const InnerIterator& fisrt_it, const InnerIterator& last_it);
		void moveNext();
		void moveTo(PlayingCard* card); //НАПИСАТЬ РЕАЛИЗАЦИЮ
		bool isLast();
		PlayingCard* getCurrent();
	private:
		InnerIterator m_first;
		InnerIterator m_last;
		InnerIterator m_current;
	};
}

#endif //PEGAS_FAKESPIDER_CARD_STACK_H