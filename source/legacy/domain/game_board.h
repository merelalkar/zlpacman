#ifndef PEGAS_FAKESPIDER_GAME_BOARD_H
#define PEGAS_FAKESPIDER_GAME_BOARD_H
#pragma once

namespace Pegas
{
	class MakeTurnCommand: public GameCommand
	{
	public:
		MakeTurnCommand(CardColumn* currentColumn, CardColumn* previousColumn, 
			const CardList& cards, ScoreBoard* scoreBoard);
		virtual ~MakeTurnCommand() {};
		virtual void undo();
	private:
		CardColumn* m_currentColumn;
		CardColumn* m_previousColumn;
		CardList m_playingCards;
		ScoreBoard* m_scoreBoard;
	};

	class GameBoard: public GameObject
	{
	public:
		
		enum
		{
			k_cardColumnCount = 10,
			k_borderOffset = 10,
			k_minBoardWidth = 800,
			k_minBoardHeight = 600,
			k_playingCardWidth = 71,
			k_playingCardHeight = 96
		};

		GameBoard();
		virtual ~GameBoard() {};

		virtual void draw(GrafManager& context);
		void resize(CURCOORD newWidth, CURCOORD newHeight);

		void initialize(const CardList& cards);
		void passNewSeries(const CardList& series);
		void showValidTurn();

		void addBuildedCardStack(CardStack& stack); 
		MakeTurnCommand* putCardsToColumn(PortableCardStack& portableStack); 

		bool hasEmptyColumns();
	private:
		
		void alignGameObjects(CURCOORD boardWidth, CURCOORD boardHeight); 

		CardColumn			m_cardColumns[k_cardColumnCount];
		CardDeck			m_cardDeck;
		BuildedStackDeck	m_buildedStackDeck;
		ScoreBoard          m_scoreBoard;
		PortableCardStack   m_portableCardStack;

		SeriesPassAnimator		m_seriesPassAnimator;
		ShowValidTurnAnimator	m_showValidTurnAnimator;
		AddBuildedStackAnimator m_addBuildedStackAnimator;

		SpriteParameters m_gameBoardSprite;
	};
	
}

#endif //PEGAS_FAKESPIDER_GAME_BOARD_H