#ifndef PEGAS_FAKESPIDER_CARD_DECK_H
#define PEGAS_FAKESPIDER_CARD_DECK_H
#pragma once;


namespace Pegas
{
	class CardDeck: public ClickableGameObject
	{
	public:
		enum
		{
			k_leftRelativeOffset = 12
		};

		CardDeck(GameBoard& gameBoard): m_deckCounter(0), m_gameBoard(gameBoard) {};
		virtual ~CardDeck() {};

		void initialize(int deckCount);
		void decreaseCounter();

		virtual void draw(GrafManager& context);
		virtual void click(CURCOORD x, CURCOORD y);
		
	private:
        void setPlacement(); 

		GameBoard& m_gameBoard;
		int m_deckCounter;
	};
}

#endif //PEGAS_FAKESPIDER_CARD_DECK_H