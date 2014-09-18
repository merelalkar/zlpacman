#ifndef PEGAS_FAKESPIDER_PORTABLE_CARD_STACK_H
#define PEGAS_FAKESPIDER_PORTABLE_CARD_STACK_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	class PortableCardStack: public GameObject, public IMovable, public Singleton<PortableCardStack>
	{
	public:
		
		PortableCardStack(GameBoard& gameBoard);
		virtual ~PortableCardStack() {};

		void reset(CardColumn* previousColumn);
		void addCard(PlayingCard* card);
		
		virtual void draw(GrafManager& context);
		
		virtual void capture();
		virtual void move(CURCOORD cx, CURCOORD cy);
		virtual void release(CURCOORD x, CURCOORD y);
		CardList& getCards() { return m_cardList; };
		CardColumn* getPreviousColumn() { return m_previousColumn; };

	protected:
		CURCOORD prevLeft;
		CURCOORD prevTop;

	private:
		void alignCards();

		CardList m_cardList;
		CardColumn* m_previousColumn;
		GameBoard& m_gameBoard;
		
		PortableCardStack(const PortableCardStack& src);
		PortableCardStack& operator=(const PortableCardStack& src);
	};
}

#endif //PEGAS_FAKESPIDER_PORTABLE_CARD_STACK